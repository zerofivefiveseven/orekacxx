#pragma warning(disable : 4786)

#define _WINSOCKAPI_  // prevents the inclusion of winsock.h

#include <RecorderSender.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>

#include "ConfigManager.h"
#include "RecorderSender.h"
#include "LogManager.h"
#include "messages/Message.h"
#include "messages/TapeMsg.h"
#include "OrkClient.h"
#include "Daemon.h"
#include "CapturePluginProxy.h"
#include "EventStreaming.h"
#include "messages/InitMsg.h"
#include "OrkTrack.h"
#include <vector>
#include <memory>
#include <atomic>

//не проведена раюота с сокетами
class RecorderSubThread {
public:
    RecorderSubThread::RecorderSubThread() = default;

    void Run();

    bool InitializeSocket();

    bool SendChunk(const CStdString &tapeId, const std::byte *data, size_t size);

    bool RegisterTape(const AudioTapeRef &audioTape);

    CStdString m_threadId;
    std::mutex m_mutex;
    int m_socketFd = -1;
    sockaddr_un m_socketAddr{};

    AudioTapeRef m_currentTape;
    std::atomic<bool> m_isBusy{false};
};

TapeProcessorRef RecorderSender::m_singleton;
static std::vector<std::shared_ptr<RecorderSubThread> > s_RecorderSenderThreads;
static std::unordered_map<std::string, std::shared_ptr<RecorderSubThread> > s_TapeToThreadMap;
static std::mutex s_mapMutex;

bool RecorderSubThread::InitializeSocket() {
    m_socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    CStdString logMsg;
    if (m_socketFd < 0) {
        FLOG_ERROR(LOG.recordingSenderlog, "Failed to create UNIX socket for thread %s", m_threadId);
        return false;
    }

    memset(&m_socketAddr, 0, sizeof(m_socketAddr));
    m_socketAddr.sun_family = AF_UNIX;
    snprintf(m_socketAddr.sun_path, sizeof(m_socketAddr.sun_path),
             "/tmp/orka_recorder_%s.sock", m_threadId);

    if (connect(m_socketFd, (struct sockaddr *) &m_socketAddr, sizeof(m_socketAddr)) < 0) {
        FLOG_ERROR(LOG.recordingSenderlog, "Failed to connect UNIX socket for thread %s", m_threadId);
        close(m_socketFd);
        m_socketFd = -1;
        return false;
    }

    return true;
}

bool RecorderSubThread::RegisterTape(const AudioTapeRef &audioTape) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_isBusy) {
        return false;
    }

    m_currentTape = audioTape;
    m_isBusy = true;

    // Отправляем начальный маркер или метаданные
    CStdString header;
    header.Format("START_TAPE|%s|%lld", audioTape->m_beginDate, audioTape->m_endDate);

    if (send(m_socketFd, header, header.GetLength(), 0) < 0) {
        CStdString logMsg;
        FLOG_ERROR(LOG.recordingSenderlog, "[%s] Failed to send header for tape %s",
                   m_threadId, audioTape->m_localIp);
        m_isBusy = false;
        return false;
    }

    return true;
}

bool RecorderSubThread::SendChunk(const CStdString &m_localIp, const std::byte *data, size_t size) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_isBusy || m_currentTape->m_localIp != m_localIp) {
        return false;
    }

    // Отправляем чанк данных
    if (send(m_socketFd, data, size, 0) < 0) {
        CStdString logMsg;
        FLOG_ERROR(LOG.recordingSenderlog, "[%s] Failed to send chunk for tape %s",
                   m_threadId, m_localIp);
        return false;
    }

    return true;
}

void RecorderSubThread::Run() {
    SetThreadName("orka:recorder");
    CStdString logMsg;
    FLOG_INFO(LOG.recordingSenderlog, "[%s] RecorderSender thread started", m_threadId);

    if (!InitializeSocket()) {
        FLOG_ERROR(LOG.recordingSenderlog, "[%s] Failed to initialize socket, thread exiting", m_threadId);
        return;
    }

    while (!Daemon::Singleton()->IsStopping()) {
        OrkSleepSec(1); // Основная работа теперь через SendChunk
    }

    if (m_socketFd >= 0) {
        close(m_socketFd);
        m_socketFd = -1;
    }
    FLOG_INFO(LOG.recordingSenderlog, "[%s] RecorderSender thread exiting", m_threadId);
}

void RecorderSender::Initialize() {
    if (m_singleton.get() == nullptr) {
        m_singleton.reset(new RecorderSender());

        uint threadCount = CONFIG.m_numBatchThreads;
        if (threadCount == 0) { threadCount = std::thread::hardware_concurrency(); }

        for (int i = 0; i < threadCount; i++) {
            auto thread = std::make_shared<RecorderSubThread>();
            thread->m_threadId.Format("RecorderThread_%d", i);

            try {
                std::thread workerThread(&RecorderSubThread::Run, thread.get());
                workerThread.detach();
                s_RecorderSenderThreads.push_back(thread);
            } catch (const std::exception &ex) {
                CStdString logMsg;
                FLOG_ERROR(LOG.recordingSenderlog, "Failed to start thread %s: %s",
                           thread->m_threadId, ex.what());
            }
        }
    }
}

bool RecorderSender::RegisterAudioTape(AudioTapeRef &audioTape) {
    // Ищем свободный тред
    for (auto &thread: s_RecorderSenderThreads) {
        if (!thread->m_isBusy && thread->RegisterTape(audioTape)) {
            std::lock_guard<std::mutex> lock(s_mapMutex);
            s_TapeToThreadMap[static_cast<std::string>(audioTape->m_localIp)] = thread;

            // Отправляем сообщение о начале записи
            TapeMsg msgRef;
            audioTape->GetDetails(&msgRef);

            return true;
        }
    }

    CStdString logMsg;
    FLOG_ERROR(LOG.recordingSenderlog, "No available threads to register audio tape");
    return false;
}

bool RecorderSender::SendAudioChunk(const CStdString &localIp, const std::byte *data, size_t size) {
    std::lock_guard<std::mutex> lock(s_mapMutex);
    auto it = s_TapeToThreadMap.find(static_cast<std::string>(localIp));
    if (it != s_TapeToThreadMap.end()) {
        return it->second->SendChunk(localIp, data, size);
    }
    return false;
}

void RecorderSender::FinalizeAudioTape(const CStdString &localIp) {
    std::lock_guard<std::mutex> lock(s_mapMutex);
    auto it = s_TapeToThreadMap.find(static_cast<std::string>(localIp));
    if (it != s_TapeToThreadMap.end()) {
        // Отправляем маркер конца записи
        CStdString footer = "END_TAPE";
        send(it->second->m_socketFd, footer, footer.GetLength(), 0);

        // Освобождаем поток
        it->second->m_isBusy = false;
        it->second->m_currentTape.reset();

        s_TapeToThreadMap.erase(it);
    }
}
