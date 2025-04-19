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
#pragma warning( disable: 4786 ) // disables truncated symbols in browse-info warning
#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
//#include "winsock2.h"
#endif
#include "ConfigManager.h"
#include "MediaChunkFile.h"

#define MAX_CHUNK_SIZE 100000


MediaChunkFile::MediaChunkFile()
{
	m_mode = READ;
	m_numChunksWritten = 0;
	m_sampleRate = 0;

	m_chunkQueueDataSize = 0;
}

MediaChunkFile::~MediaChunkFile()
{
	Close();
}


void MediaChunkFile::Close()
{
	if(m_stream.is_open())
	{
		FlushToDisk();
		m_stream.close();
	}
}

bool MediaChunkFile::FlushToDisk()
{
	bool writeError = false;
	while(m_chunkQueue.size() > 0)
	{
		AudioChunkRef tmpChunk = m_chunkQueue.front();
		m_chunkQueue.pop();
		if(tmpChunk.get() == NULL)
		{
			continue;
		}
		if(tmpChunk->m_pBuffer == NULL)
		{
			continue;
		}
		int numWritten = sizeof(AudioChunkDetails);
		m_stream.write((char*)tmpChunk->GetDetails(), numWritten);
		if(!m_stream.good())
		{
			writeError = true;
			break;
		}
		numWritten = tmpChunk->GetNumBytes()*sizeof(char);
		m_stream.write((char*)tmpChunk->m_pBuffer, numWritten);
		if(!m_stream.good())
		{
			writeError = true;
			break;
		}
	}
	m_chunkQueueDataSize = 0;
	return writeError;
}


void MediaChunkFile::WriteChunk(AudioChunkRef chunkRef)
{
	if(chunkRef.get() == nullptr)
	{
		return;
	}
	if(chunkRef->GetDetails()->m_numBytes == 0)
	{
		return;
	}

	bool writeError = false;

	AudioChunk* pChunk = chunkRef.get();
	m_chunkQueueDataSize += pChunk->GetNumBytes();
	m_chunkQueue.push(chunkRef);

	if(m_chunkQueueDataSize > static_cast<unsigned int>((CONFIG.m_captureFileBatchSizeKByte * 1024)))
	{
		if(m_stream.is_open())
		{
			writeError = FlushToDisk();
		}
		else
		{
			throw(CStdString("Write attempt on unopened file:")+ m_filename);
		}
	}
	if (writeError)
	{
		throw(CStdString("Could not write to file:")+ m_filename);
	}
}

int MediaChunkFile::ReadChunkMono(AudioChunkRef& chunkRef)
{
	int numRead = 0;

	if(m_stream.is_open())
	{
		chunkRef.reset(new AudioChunk());
		short temp[MAX_CHUNK_SIZE];
		numRead = sizeof(AudioChunkDetails);
		m_stream.read(reinterpret_cast<char *>(temp), numRead);
		if(m_stream.eof()){
			return 0;
		}
		if(!m_stream.fail())
		{
			AudioChunkDetails details;
			memcpy(&details, temp, sizeof(AudioChunkDetails));
			if(details.m_marker != MEDIA_CHUNK_MARKER)
			{
				throw(CStdString("Invalid marker in file:")+ m_filename);
			}
			if(details.m_numBytes >= MAX_CHUNK_SIZE)
			{
				throw(CStdString("Chunk too big in file:")+ m_filename);
			}

			int numRead = details.m_numBytes;
			m_stream.read(reinterpret_cast<char *>(temp), numRead);
			if(m_stream.fail())
			{
				throw(CStdString("Incomplete chunk in file:")+ m_filename);
			}
			chunkRef->SetBuffer(temp, details);
		}
	}
	else
	{
		throw(CStdString("Read attempt on unopened file:")+ m_filename);
	}
	return numRead;
}


void MediaChunkFile::Open(CStdString& filename, fileOpenModeEnum mode, bool stereo, int sampleRate)
{
	if(m_sampleRate == 0)
	{
		m_sampleRate = sampleRate;
	}

	if(!m_filename.Equals(filename))
	{
		m_filename = filename + GetExtension();
	}
	m_mode = mode;
	if (mode == READ)
	{
		m_stream.open(m_filename, std::fstream::in | std::fstream::binary);
	}
	else
	{
		FileRecursiveMkdir(m_filename, CONFIG.m_audioFilePermissions, CONFIG.m_audioFileOwner, CONFIG.m_audioFileGroup, CONFIG.m_audioOutputPathMcf);
		m_stream.open(m_filename, std::fstream::out | std::fstream::binary);

		if(CONFIG.m_audioFilePermissions)
		{
			FileSetPermissions(m_filename, CONFIG.m_audioFilePermissions);
		}

		if(CONFIG.m_audioFileGroup.size() && CONFIG.m_audioFileOwner.size())
		{
			FileSetOwnership(m_filename, CONFIG.m_audioFileOwner, CONFIG.m_audioFileGroup);
		}
	}
	if(!m_stream.is_open())
	{
		throw(CStdString("Could not open file: ") + m_filename);
	}
}

CStdString MediaChunkFile::GetExtension()
{
	return ".mcf";
}

void MediaChunkFile::SetNumOutputChannels(int numChan)
{

}








#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

constexpr char SOCKET_PATH[] = "/tmp/buffer_transfer.sock";
constexpr size_t BUFFER_SIZE = 1024; // Example buffer size

bool send_buffer(int sockfd, const std::vector<char>& buffer) {
    // First send the buffer size
    size_t buffer_size = buffer.size();
    if (write(sockfd, &buffer_size, sizeof(buffer_size)) != sizeof(buffer_size)) {
        std::cerr << "Failed to send buffer size: " << strerror(errno) << std::endl;
        return false;
    }

    // Then send the buffer contents
    if (write(sockfd, buffer.data(), buffer_size) != static_cast<ssize_t>(buffer_size)) {
        std::cerr << "Failed to send buffer data: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

int main() {
    // Create a sample buffer with some data
    std::vector<char> buffer(BUFFER_SIZE);
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = 'A' + (i % 26); // Fill with A-Z repeating
    }

    // Create UNIX domain socket
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // Connect to server
    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to server. Sending buffer of size " << buffer.size() << std::endl;

    if (send_buffer(sockfd, buffer)) {
        std::cout << "Buffer sent successfully!" << std::endl;
    } else {
        std::cerr << "Buffer transfer failed" << std::endl;
    }

    close(sockfd);
    return 0;
}
