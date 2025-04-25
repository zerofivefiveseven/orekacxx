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
#ifndef __FILTER_H__
#define __FILTER_H__

#include <list>
#include "AudioCapture.h"
#include "dll.h"

class FilterConfigurationParameters
{
public:
	void* param1;
	void* param2;

};
typedef oreka::shared_ptr<FilterConfigurationParameters> FilterConfigurationParametersRef;

class Filter;

typedef oreka::shared_ptr<Filter> FilterRef;

/** Filter Interface
 *  a filter is a black box that takes media chunks as an input and produces media chunks as an output
 *  it can be translating between two encodings (codec) or just processing the signal
 */
class Filter
{
public:
	virtual ~Filter() = default;

	Filter();
	virtual FilterRef __CDECL__ Instanciate() = 0;
	virtual void __CDECL__ AudioChunkIn(AudioChunkRef& chunk) = 0;
	virtual void __CDECL__ AudioChunkOut(AudioChunkRef& chunk) = 0;
	virtual AudioEncodingEnum __CDECL__ GetInputAudioEncoding() = 0;
	virtual AudioEncodingEnum __CDECL__ GetOutputAudioEncoding() = 0;
	virtual CStdString __CDECL__ GetName() = 0;
	/** Input RTP payload time - this is overridden if the filter is a codec that accepts a certain
	    RTP payload type such as GSM. if not, returns -1 by default */
	virtual bool __CDECL__ SupportsInputRtpPayloadType(int rtpPayloadType );
	virtual void __CDECL__ CaptureEventIn(CaptureEventRef& event) = 0;
	virtual void __CDECL__ CaptureEventOut(CaptureEventRef& event) = 0;
	virtual void __CDECL__ SetSessionInfo(CStdString& trackingId);
	virtual void __CDECL__ Configure(FilterConfigurationParametersRef configParams);
	virtual void __CDECL__ SetNumOutputChannels(int numChan);
protected:
	CStdString m_trackingId;
	int m_numOutputChannels;

};
//===================================================================

class AlawToPcmFilter final : public Filter
{
public:
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
};

//===================================================================

class UlawToPcmFilter final : public Filter
{
public:
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
};

//===================================================================
/** Filter Registry
*/
class FilterRegistry
{
public:
	static FilterRegistry* instance();
	void RegisterFilter(FilterRef& Filter);
	FilterRef GetNewFilter(AudioEncodingEnum inputEncoding, AudioEncodingEnum outputEncoding);
	FilterRef GetNewFilter(int rtpPayloadType);
	FilterRef GetNewFilter(CStdString& filterName);

private:
	std::list<FilterRef> m_Filters;

	static FilterRegistry* m_singleton;
};


#endif
