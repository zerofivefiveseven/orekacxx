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

#ifndef __LIBSNDFILEFILE_H__
#define __LIBSNDFILEFILE_H__


#include "StdString.h"
#include "AudioCapture.h"
#include "sndfile.h"
#include "AudioFile.h"


/** file accessor class for all file types supported by the libsndfile library. 
	The library can be found at http://www.mega-nerd.com/libsndfile/
*/
class LibSndFileFile : public AudioFile
{
public:
	explicit LibSndFileFile(int fileFormat);	// fileFormat is described at http://www.mega-nerd.com/libsndfile/api.html
	~LibSndFileFile() override;

	void Open(CStdString& filename, fileOpenModeEnum mode, bool stereo, int sampleRate) override;
	void Close() override;

	void WriteChunk(AudioChunkRef chunkRef) override;
	int ReadChunkMono(AudioChunkRef& chunk) override;

	CStdString GetExtension() override;
	void SetNumOutputChannels(int numChan) override;
private:
	SF_INFO	m_fileInfo;
	SNDFILE*	m_pFile;
};

#endif

