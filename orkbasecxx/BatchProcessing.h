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

#ifndef __BATCHPROCESSING_H__
#define __BATCHPROCESSING_H__

#include "ThreadSafeQueue.h"
#include "TapeProcessor.h"
#include "AudioTape.h"
#include <map>
#include <mutex>
#include <memory>

class  BatchProcessing;
typedef oreka::shared_ptr<BatchProcessing> BatchProcessingRef;

/**
 * This tape processor handles the audio transcoding
 */
class BatchProcessing : public TapeProcessor
{
public:
	static void Initialize();

	CStdString __CDECL__ GetName();
	TapeProcessorRef __CDECL__ Instanciate();
	void __CDECL__ AddAudioTape(AudioTapeRef& audioTapeRef);


	static void ThreadHandler();
	static bool SkipChunk(AudioTapeRef& audioTapeRef, AudioChunkRef& chunkRef, int& channelToSkip);

	void SetQueueSize(int size);

private:
	BatchProcessing();
	static TapeProcessorRef m_singleton;

	ThreadSafeQueue<AudioTapeRef> m_audioTapeQueue;

	size_t m_threadCount;
	std::mutex m_mutex;
	int m_currentDay;
};

#endif

