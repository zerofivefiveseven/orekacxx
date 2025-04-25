#ifndef __SPEEXCODEC_H__
#define __SPEEXCODEC_H__

#include "LogManager.h"
#include "Filter.h"
#include "speex/speex.h"

class SpeexDecoder final : public Filter
{
public:
	SpeexDecoder();
	~SpeexDecoder() override;

	FilterRef __CDECL__ Instanciate() override;
	void __CDECL__ AudioChunkIn(AudioChunkRef& chunk) override;
	void __CDECL__ AudioChunkOut(AudioChunkRef& chunk) override;
	AudioEncodingEnum __CDECL__ GetInputAudioEncoding() override;
	AudioEncodingEnum __CDECL__ GetOutputAudioEncoding() override;
	CStdString __CDECL__ GetName() override;
	bool __CDECL__ SupportsInputRtpPayloadType(int rtpPayloadType ) override;
	void __CDECL__ CaptureEventIn(CaptureEventRef& event) override;
	void __CDECL__ CaptureEventOut(CaptureEventRef& event) override;

private:
	AudioChunkRef m_outputAudioChunk;
	SpeexBits m_bits;
	void * m_state;
	bool m_initialized;
	int m_frameSize;
	int m_enh;
};

#endif
