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

#ifndef __COMMANDPROCESSING_H__
#define __COMMANDPROCESSING_H__

#include "ThreadSafeQueue.h"
#include "TapeProcessor.h"
#include "AudioTape.h"

class  CommandProcessing;
typedef oreka::shared_ptr<CommandProcessing> CommandProcessingRef;

/**
 * This tape processor handles the audio transcoding
 */
class CommandProcessing : public TapeProcessor
{
public:
	static void Initialize();

	CStdString __CDECL__ GetName() override;
	TapeProcessorRef __CDECL__ Instanciate() override;
	void __CDECL__ AddAudioTape(AudioTapeRef& audioTapeRef) override;

	static void ThreadHandler();

	void SetQueueSize(int size);
	
private:
	CommandProcessing();
	static TapeProcessorRef m_singleton;

	ThreadSafeQueue<AudioTapeRef> m_audioTapeQueue;

	size_t m_threadCount;
    std::mutex m_mutex;
	//int m_currentDay;
};

#endif

