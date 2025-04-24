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

class RecorderSender : public TapeProcessor {
public:
    TapeProcessorRef  Instanciate() override;
    void  AddAudioTape(AudioTapeRef& audioTapeRef) override;
    CStdString __CDECL__ GetName() override;
    static void Initialize();

    static bool RegisterAudioTape(AudioTapeRef &audioTape);

    bool SendAudioChunk(const CStdString &tapeId, const std::byte *data, size_t size);

    void FinalizeAudioTape(const CStdString &tapeId);

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



    void __CDECL__ Open(TapeMsg& msg);
    void __CDECL__ Close();
    void TransferAudio (AudioTapeDescription);
    static void ThreadHandler();

    RecorderSender *Instance();

    RecorderSender();

    void Run();

    void SetQueueSize(int size);
private:

    static TapeProcessorRef m_singleton;
    ThreadSafeQueue<AudioTapeRef> m_audioTapeQueue;
    size_t m_threadCount{};
    std::mutex m_mutex;
};


#endif //RECORDERSENDER_H
