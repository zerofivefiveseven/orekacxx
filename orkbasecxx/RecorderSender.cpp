//
// Created by revyakin on 10.04.25.
//

#include "RecorderSender.h"
#include <vector>
#include <bitset>

#include "ConfigManager.h"
#include "CommandProcessing.h"
#include "audiofile/LibSndFileFile.h"
#include "Daemon.h"
#include "Filter.h"
#include "Reporting.h"
#include "Utils.h"

#pragma warning( disable: 4786 )

CStdString processorName("RecorderSender");
TapeProcessorRef RecorderSender::m_singleton;

void RecorderSender::Initialize() {
    if (m_singleton.get() == nullptr) {
        m_singleton.reset(new RecorderSender());
        TapeProcessorRegistry::instance()->RegisterTapeProcessor(m_singleton);
    }
    TapeProcessorRegistry::instance()->RegisterTapeProcessor(m_singleton);
}

TapeProcessorRef RecorderSender::Instanciate() {
    return m_singleton;
}

//
// void RecorderSender::AudioChunkIn(AudioChunkRef &chunk) {}
// void RecorderSender::AudioChunkOut(AudioChunkRef &chunk){}
// AudioEncodingEnum RecorderSender::GetInputAudioEncoding() { return {}; }
// AudioEncodingEnum RecorderSender::GetOutputAudioEncoding() {return {};}
// void RecorderSender::CaptureEventIn(CaptureEventRef &event) {}
// void RecorderSender::CaptureEventOut(CaptureEventRef &event) {}
void RecorderSender::AddAudioTape(AudioTapeRef &audioTapeRef) {
    // делать это только после успешной отправки audioTapeRef->m_isDoneProcessed = true; 	//to notify API caller the importing is good so far
    if (!m_audioTapeQueue.push(audioTapeRef)) {
        // Log error
        LOG4CXX_ERROR(LOG.recordingSenderlog, CStdString("queue full"));
    }
    CStdString logMsg;
    FLOG_INFO(LOG.recordingSenderlog, "[%s] queuesize:%d", m_threadCount,
              m_audioTapeQueue.numElements());
}

void RecorderSender::ThreadHandler() {
    SetThreadName("orka:batch");

    CStdString debug;
    CStdString logMsg;

    CStdString processorName("RecorderSender");
    const TapeProcessorRef recorderSenderFromPipeline = TapeProcessorRegistry::instance()->
            GetNewTapeProcessor(processorName);
    if (recorderSenderFromPipeline.get() == nullptr) {
        LOG4CXX_ERROR(LOG.recordingSenderlog, "Could not instanciate BatchProcessing");
        return;
    }
    auto pRecorderSender = dynamic_cast<RecorderSender *>(recorderSenderFromPipeline->Instanciate().get());

    pRecorderSender->SetQueueSize(CONFIG.m_batchProcessingQueueSize);

    size_t threadId = 0; {
        MutexSentinel sentinel(pRecorderSender->m_mutex);
        threadId = pRecorderSender->m_threadCount++;
    }
    const CStdString threadIdString = IntToString(threadId);
    debug.Format("thread Th%s starting - queue size:%d", threadIdString, CONFIG.m_batchProcessingQueueSize);
    LOG4CXX_INFO(LOG.recordingSenderlog, debug);

    for (bool stop = false; stop == false;) {
        AudioTapeRef audioTapeRef;

        CStdString trackingId = "[no-trk]";
        try {
            audioTapeRef = pRecorderSender->m_audioTapeQueue.pop();
            //это последнее звено пайплайна
            if (audioTapeRef.get() == nullptr) {
                if (Daemon::Singleton()->IsStopping()) {
                    stop = true;
                }
                if (Daemon::Singleton()->GetShortLived()) {
                    Daemon::Singleton()->Stop();
                }
            } else {
                //LOG4CXX_INFO(LOG.immediateProcessingLog, "Previous:" + lastHandled + " Current:" + audioTapeRef->m_portId);
                // lastHandled = audioTapeRef->m_portId;

                if (audioTapeRef->IsStoppedAndValid()) {
                    MessageRef sessionData;
                    audioTapeRef->GetMessage(sessionData);
                    //это дата о сессии, а потом надо отправлять аудио
                    // sessionData = std::dynamic_pointer_cast<TapeMsg>(sessionData);
                    // auto serialized = sessionData->SerializeSingleLine();
		            AudioFileRef fileRef = audioTapeRef->GetAudioFileRef();
                    fileRef->Open(AudioFile::READ);

                } else {
                    continue;
                }
            }
        } catch (CStdString &e) {
            LOG4CXX_ERROR(LOG.immediateProcessingLog, CStdString("ImmediateProcessing: ") + e);
        }
        LOG4CXX_INFO(LOG.immediateProcessingLog, CStdString("Exiting thread"));
    }
}


RecorderSender::RecorderSender() {
    //тут надо открывать коннекцию
    m_threadCount = 0;
}
