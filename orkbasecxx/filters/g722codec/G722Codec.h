/*
 * Oreka -- A media capture and retrieval platform
 *
 * Copyright (C) 2005, orecx LLC
 *
 * http://www.orecx.com
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License.
 * Please refer to http://www.gnu.org/copyleft/gpl.html
 *
 */
#ifndef __G722DECODER_H__
#define __G722DECODER_H__ 1

#include "Filter.h"
#include "G722.h"

class G722ToPcmFilter : public Filter
{
public:
	G722ToPcmFilter();
	~G722ToPcmFilter() override;

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
	g722_decode_state_t m_ctx;
};

#endif
