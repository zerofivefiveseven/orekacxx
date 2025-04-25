//
// Created by revyakin on 10.04.25.
//

#ifndef RECORDERSENDER_H
#define RECORDERSENDER_H

#include <Filter.h>

#include "ThreadSafeQueue.h"
#include "TapeProcessor.h"
#include "AudioTape.h"

class RecorderSender;
typedef oreka::shared_ptr<RecorderSender> RecorderSenderRef;

class RecorderSender : public OrkSingleton<RecorderSender>  { //смысл этого наследования остаётся под большим вопросом
public:
    RecorderSender() = default;

    static void Initialize();

    static void Shutdown();

    static bool RegisterAudioTape(const AudioTapeRef &audioTape);

    static bool SendAudioChunk(const CStdString &tapeId, const std::byte *data, size_t size);

    static void FinalizeAudioTape(const CStdString &tapeId);

    void __CDECL__ Open(TapeMsg &msg);

    void __CDECL__ Close();

    void Process();

    void SetQueueSize(int size);

private:
    static TapeProcessorRef m_singleton;
    ThreadSafeQueue<AudioTapeRef> m_audioTapeQueue;
    size_t m_threadCount{};
    std::mutex m_mutex;
};


#endif //RECORDERSENDER_H
