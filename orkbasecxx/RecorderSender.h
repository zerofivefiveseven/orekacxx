//
// Created by revyakin on 10.04.25.
//

#ifndef RECORDERSENDER_H
#define RECORDERSENDER_H

#include <Filter.h>

#include "ThreadSafeQueue.h"
#include "TapeProcessor.h"
#include "AudioTape.h"
#include <map>

class RecorderSender;
typedef oreka::shared_ptr<RecorderSender> RecorderSenderRef;

class RecorderSender : public OrkSingleton<RecorderSender> {
public:
    CStdString __CDECL__ GetName() override;
    static void Initialize();
    TapeProcessorRef __CDECL__ Instanciate() override;
    // FilterRef __CDECL__ Instanciate() override;
    // //кусок
    // void AudioChunkIn(AudioChunkRef &chunk) override;
    //
    // void AudioChunkOut(AudioChunkRef &chunk) override;
    //
    // AudioEncodingEnum GetInputAudioEncoding() override;
    //
    // AudioEncodingEnum GetOutputAudioEncoding() override;
    //
    // void CaptureEventIn(CaptureEventRef &event) override;
    //
    // void CaptureEventOut(CaptureEventRef &event) override;
    //



    void __CDECL__ AddAudioTape(AudioTapeRef& audioTapeRef) override;
    // void __CDECL__ AddAudioChunk(AudioTapeRef& audioTapeRef);

    static void ThreadHandler();

    void SetQueueSize(int size);
private:
    RecorderSender();

    static TapeProcessorRef m_singleton;
    ThreadSafeQueue<AudioTapeRef> m_audioTapeQueue;
    size_t m_threadCount;
    std::mutex m_mutex;
};


#endif //RECORDERSENDER_H
