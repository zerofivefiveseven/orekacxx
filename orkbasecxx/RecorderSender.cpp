#pragma warning(disable : 4786)


#include <RecorderSender.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <memory>
#include <unordered_map>
#include <condition_variable>
#include <ConfigManager.h>
#include <LogManager.h>
#include <queue>

class RecorderSubThread {
public:
    RecorderSubThread() : m_stopFlag(false) {
    }

    void Run();

    void Stop();

    bool InitializeSocket();

    bool EnqueueChunk(const CStdString &localIp, const std::byte *data, size_t size);

    bool RegisterTape(const AudioTapeRef &audioTape);

    void FinalizeCurrentTape();

    CStdString m_threadId;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    struct ChunkData {
        CStdString m_localIp;
        std::vector<std::byte> data;
    };

    std::queue<ChunkData> m_chunkQueue;
    int m_socketFd = -1;
    sockaddr_un m_socketAddr{};
    AudioTapeRef m_currentTape;
    std::atomic<bool> m_stopFlag;
};

TapeProcessorRef RecorderSender::m_singleton;
static std::vector<std::shared_ptr<RecorderSubThread> > s_RecorderSenderThreads;
static std::unordered_map<std::string, std::shared_ptr<RecorderSubThread> > s_TapeToThreadMap;
static std::mutex s_mapMutex;

bool RecorderSubThread::InitializeSocket() {
    CStdString logMsg;
    m_socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_socketFd < 0) {
        FLOG_ERROR(LOG.recordingSenderlog, "Failed to create UNIX socket for thread %s", m_threadId);
        return false;
    }

    memset(&m_socketAddr, 0, sizeof(m_socketAddr));
    m_socketAddr.sun_family = AF_UNIX;
    snprintf(m_socketAddr.sun_path, sizeof(m_socketAddr.sun_path),
             "/tmp/orka_recorder_%s.sock", m_threadId.data());

    if (connect(m_socketFd, reinterpret_cast<struct sockaddr *>(&m_socketAddr), sizeof(m_socketAddr)) < 0) {
        FLOG_ERROR(LOG.recordingSenderlog, "Failed to connect UNIX socket for thread %s", m_threadId);
        close(m_socketFd);
        m_socketFd = -1;
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

    std::unique_lock lock(m_mutex);
    while (!m_stopFlag) {
        m_cv.wait(lock, [this]() {
            return !m_chunkQueue.empty() || m_stopFlag;
        });

        if (m_stopFlag) break;

        while (!m_chunkQueue.empty()) {
            auto [localIp, data] = std::move(m_chunkQueue.front());
            m_chunkQueue.pop();

            if (send(m_socketFd, data.data(), data.size(), 0) < 0) {
                FLOG_ERROR(LOG.recordingSenderlog, "[%s] Failed to send chunk for tape %s",
                           m_threadId, localIp);
            }
        }
    }

    if (m_currentTape) {
        FinalizeCurrentTape();
    }

    if (m_socketFd >= 0) {
        close(m_socketFd);
        m_socketFd = -1;
    }
    FLOG_INFO(LOG.recordingSenderlog, "[%s] RecorderSender thread exiting", m_threadId);
}

void RecorderSubThread::Stop() {
    m_stopFlag = true;
    m_cv.notify_one();
}

bool RecorderSubThread::RegisterTape(const AudioTapeRef &audioTape) {
    std::lock_guard lock(m_mutex);

    if (m_currentTape) {
        return false;
    }

    m_currentTape = audioTape;

    CStdString header;
    header.Format("START_TAPE|%s|%lld", audioTape->m_localIp, audioTape->m_beginDate);

    ChunkData startMarker;
    startMarker.m_localIp = audioTape->m_localIp;
    startMarker.data.assign(reinterpret_cast<const std::byte *>(header.GetBuffer()),
                            reinterpret_cast<const std::byte *>(header.GetBuffer() + header.GetLength()));

    m_chunkQueue.push(std::move(startMarker));
    m_cv.notify_one();

    return true;
}

bool RecorderSubThread::EnqueueChunk(const CStdString &localIp, const std::byte *data, size_t size) {
    std::lock_guard lock(m_mutex);

    if (!m_currentTape || m_currentTape->m_localIp != localIp) {
        return false;
    }

    ChunkData chunk;
    chunk.m_localIp = localIp;
    chunk.data.assign(data, data + size);

    m_chunkQueue.push(std::move(chunk));
    m_cv.notify_one();

    return true;
}

void RecorderSubThread::FinalizeCurrentTape() {
    if (!m_currentTape) return;

    const CStdString footer = "END_TAPE";
    send(m_socketFd, footer, footer.GetLength(), 0);

    m_currentTape.reset();
}

bool RecorderSender::RegisterAudioTape(const AudioTapeRef &audioTape) {
    std::lock_guard lock(s_mapMutex);

    // Ищем свободный тред
    for (const auto &thread: s_RecorderSenderThreads) {
        if (thread->RegisterTape(audioTape)) {
            s_TapeToThreadMap[static_cast<std::string>(audioTape->m_localIp)] = thread;
            return true;
        }
    }

    CStdString logMsg;
    FLOG_ERROR(LOG.recordingSenderlog, "No available threads to register audio tape for IP: %s",
               audioTape->m_localIp);
    return false;
}

bool RecorderSender::SendAudioChunk(const CStdString &localIp, const std::byte *data, size_t size) {
    std::lock_guard lock(s_mapMutex);
    if (auto it = s_TapeToThreadMap.find(localIp); it != s_TapeToThreadMap.end()) {
        return it->second->EnqueueChunk(localIp, data, size);
    }
    return false;
}

void RecorderSender::FinalizeAudioTape(const CStdString &localIp) {
    std::lock_guard lock(s_mapMutex);
    if (auto it = s_TapeToThreadMap.find(localIp); it != s_TapeToThreadMap.end()) {
        it->second->FinalizeCurrentTape();
        s_TapeToThreadMap.erase(it);
    }
}

void RecorderSender::Initialize() {

        uint threadCount = CONFIG.m_numRecorderSenderThreads;
        if (threadCount == 0) {
            threadCount = std::thread::hardware_concurrency();
        }

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


void RecorderSender::Shutdown() {
    for (const auto &thread: s_RecorderSenderThreads) {
        thread->Stop();
    }
    s_RecorderSenderThreads.clear();
    s_TapeToThreadMap.clear();
}
