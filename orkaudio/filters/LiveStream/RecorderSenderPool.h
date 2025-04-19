//
// Created by revyakin on 17.04.25.
//

#ifndef RECORDERSENDERPOOL_H
#define RECORDERSENDERPOOL_H

#include "StdString.h"
#include "AudioCapture.h"
#include "sndfile.h"
#include "AudioFile.h"

#include <set>
#include <audiocaptureplugins/voip/VoIpSession.h>

#include "AudioCapturePlugin.h"
#include "Utils.h"
#include "AudioCapture.h"

class RecorderSenderPool : public OrkSingleton<RecorderSenderPool> {
public:
    LiveStreamSessions();
    bool StartStreamNativeCallId(CStdString &nativecallid);
    bool StopStreamNativeCallId(CStdString &nativecallid);
    std::set<std::string> GetLiveCallList();
    std::set<std::string> GetStreamCallList();
    void AddToStreamCallList(CStdString &nativecallid);
    void RemoveFromStreamCallList(CStdString &nativecallid);

private:
    VoIpSessions *voIpSessions;
    bool SessionFoundForNativeCallId(CStdString &nativecallid, VoIpSessionRef &session);
    bool NativeCallIdInStreamCallList(CStdString &nativecallid);
};

#define LiveStreamSessionsSingleton LiveStreamSessions

#endif //RECORDERSENDERPOOL_H
