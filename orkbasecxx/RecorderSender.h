//
// Created by revyakin on 10.04.25.
//

#ifndef RECORDERSENDER_H
#define RECORDERSENDER_H

#include "ThreadSafeQueue.h"
#include "TapeProcessor.h"
#include "AudioTape.h"
#include <map>

class RecorderSender;
typedef oreka::shared_ptr<RecorderSender> RecorderSenderRef;

class RecorderSender final : public TapeProcessor {
public:
    static void Initialize();

    CStdString __CDECL__ GetName() override;
    TapeProcessorRef __CDECL__ Instanciate() override;
    void __CDECL__ AddAudioTape(AudioTapeRef& audioTapeRef) override;

    static void ThreadHandler();

    void SetQueueSize(int size);
private:
    RecorderSender();
    static TapeProcessorRef m_singleton;

    ThreadSafeQueue<AudioTapeRef> m_audioTapeQueue;

    size_t m_threadCount;
    int m_currentDay;
};


#endif //RECORDERSENDER_H
