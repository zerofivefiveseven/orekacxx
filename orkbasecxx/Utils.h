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

#ifndef __UTILS_H__
#define __UTILS_H__

#define APR_DECLARE_STATIC
#define APU_DECLARE_STATIC
#define _WINSOCKAPI_ 
//#define WIN32_LEAN_AND_MEAN
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <grp.h>
#include <netdb.h>
#include <unistd.h>
#endif 
#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
//#include "winsock2.h"
#endif

#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "StdString.h"

#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "apr_network_io.h"
#include "apr_errno.h"
#include "apr_general.h"
#include "apr_strings.h"
#include "apr_portable.h"
#ifndef CENTOS_6
#include "openssl/ssl.h"
#include "openssl/err.h"
#endif
#include <log4cxx/logger.h>
#include <regex>

//============================================

template <class T> class OrkSingleton
{
public:
    static T* GetInstance(){
        if(m_instance.load() == nullptr){
            std::lock_guard lg(m_singleLock);
            if(m_instance.load() == nullptr){
                m_instance = new T();
            }
        }
        return m_instance;
    }
	//instance() is used to match with old ACE singleton instance() to avoid change everywhere singleton called
	static T* instance(){return GetInstance();}
    
protected:
    static std::atomic<T*> m_instance;
	static std::mutex m_singleLock;
 //   OrkSingleton(){
    ~OrkSingleton()= default;
    
};
template<class T> std::mutex OrkSingleton<T>::m_singleLock;
template<class T> std::atomic<T*> OrkSingleton<T>::m_instance(NULL);

//============================================
class OrkSemaphore {
public:
    OrkSemaphore() : m_count(1){}
	explicit OrkSemaphore(int count) : m_count(count){}

    void release()
    {
		std::unique_lock lock(m_mtx);
		m_count++;
		m_cv.notify_one();
    }

    void acquire()
    {
		std::unique_lock lock(m_mtx);
		m_cv.wait(lock,[&]{return (m_count > 0);});
		
		m_count--;   
    }

    void acquire(int timeoutMs)
	{
		auto now = std::chrono::steady_clock::now();
		std::unique_lock lock(m_mtx);
		m_cv.wait_until(lock, now + std::chrono::milliseconds(timeoutMs), [&]{return m_count > 0;});
		if(m_count > 0){
			m_count--;
		}
	}	
private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    int m_count;
};

//OrkAprSingleton and OrkOpenSslSingleton need to Initialize() in main thread's main()
//============================================
class OrkAprSingleton
{
public:
	static void Initialize();
    ~OrkAprSingleton();
    //static std::shared_ptr<OrkAprSingleton> GetInstance();
    static OrkAprSingleton* GetInstance();
    apr_pool_t* GetAprMp();
private:
    //static std::shared_ptr<OrkAprSingleton> instance;
    static OrkAprSingleton* m_instance;
    OrkAprSingleton();
    static apr_pool_t* m_aprMp;
	static std::mutex aprLock;    
};

class OrkAprSubPool
{
public:
	// N.B. this transfers ownership of the pool
	// when OrkAprSubPool passes out of scope, everything on it
	// will be destroyed, so make sure that nothing on it has a lifetime
	// exceeding OrkAprSubPool
	explicit OrkAprSubPool(apr_pool_t *pool)
	{
		m_aprPool = pool;
	}
	OrkAprSubPool()
	{
		apr_status_t rc = apr_pool_create(&m_aprPool, OrkAprSingleton::GetInstance()->GetAprMp());
		assert(rc == APR_SUCCESS);
	}
	~OrkAprSubPool() { apr_pool_destroy(m_aprPool);}
	apr_pool_t *GetAprPool() { return m_aprPool; }
private:
	apr_pool_t *m_aprPool{};
};
#define AprLp locPool.GetAprPool()


#ifdef SUPPORT_TLS_SERVER
//==========================================================
class OrkOpenSslSingleton : public OrkSingleton<OrkOpenSslSingleton>
{
public:
	OrkOpenSslSingleton();
	~OrkOpenSslSingleton();
	SSL_CTX* GetServerCtx();
private:
	void SslInitialize();
	void CreateCTXServer();
	void ConfigureServerCtx();
	SSL_CTX* m_serverCtx;
	log4cxx::LoggerPtr s_log;
};

class OrkSslStructure
{
public:
	explicit OrkSslStructure(SSL_CTX* ctx) { ssl = SSL_new(ctx); };
	OrkSslStructure() { ssl = nullptr; };
	~OrkSslStructure() { 
		if (ssl) {SSL_shutdown(ssl); SSL_free(ssl);};
	};

	void SetSsl(SSL_CTX* ctx) { ssl=SSL_new(ctx);};
	void SetSsl() { ssl=SSL_new(OrkOpenSslSingleton::GetInstance()->GetServerCtx());};
	SSL* GetSsl(SSL_CTX* ctx) { if (ssl == nullptr) ssl=SSL_new(ctx); return ssl;};
	SSL* GetSsl() { return ssl; };
private:
	SSL *ssl;
};

#endif

 const char* inet_ntopV4(int inet, void *srcAddr, char *dst, size_t size);  //AF_INET
int inet_pton4(const char *src, struct in_addr* dstAddr);
//============================================
// String related stuff
#if defined (WIN32) || defined(WIN64)
#undef strncasecmp
#undef getpid 
#define strncasecmp _strnicmp
#define getpid _getpid
#endif 
inline  CStdString IntToString(int integer)
{
	CStdString ret;
	ret.Format("%d", integer);
	return ret;
}

inline int StringToInt(CStdString& value)
{
	char* errorLocation = nullptr;
	auto szValue = (PCSTR)value;
	int intValue = strtol(szValue, &errorLocation, 10);
	if(*errorLocation != '\0')
		throw CStdString(CStdString("StringToInt: invalid integer:") + value);
	return intValue;
}

inline CStdString DoubleToString(double value)
{
	CStdString ret;
	ret.Format("%f", value);
	return ret;
}

inline double StringToDouble(CStdString& value)
{
	char* errorLocation = nullptr;
	auto szValue = (PCSTR)value;
	double doubleValue = strtod(szValue, &errorLocation);
	if(errorLocation == szValue)
		throw CStdString(CStdString("StringToDouble: invalid double:") + value);
	return doubleValue;
}

inline CStdString IpToString(const struct in_addr& ip) {
	char s[16];
	inet_ntopV4(AF_INET, (void*)&ip, s, sizeof(s));
	return {s};
}

bool StringIsDigit(CStdString& string);
bool StringIsPhoneNumber(CStdString& string);
bool MatchesStringList(CStdString& string, std::list<CStdString>& stringList);
CStdString FormatDataSize(unsigned long int size);
CStdString HexToString(const CStdString& hexInput);		//Only return digits
CStdString IntUnixTsToString(int ts);
void StringTokenizeToList(CStdString input, std::list<CStdString>& output);
bool ChopToken(CStdString &token, CStdString separator, CStdString &s);
CStdString ReplaceRegexBy(CStdString input, CStdString pattern, CStdString replacedBy);
void OrkSleepSec(unsigned int sec);
void OrkSleepMs(unsigned int msec);
void OrkSleepMicrSec(unsigned int microsec);
void OrkSleepNs(unsigned int nsec);
int ork_vsnprintf(char *buf, apr_size_t len, const char *format, ...);
CStdString AprGetErrorMsg(apr_status_t ret);
CStdString GetRevertedNormalizedPhoneNumber(CStdString input);
bool CompareNormalizedPhoneNumbers(CStdString input1, CStdString input2);
//========================================================
// file related stuff

CStdString FileBaseName(CStdString& path);
CStdString FilePath(CStdString& path);
CStdString FileStripExtension(CStdString& filename);
bool FileCanOpen(CStdString& path);
void FileRecursiveMkdir(CStdString& path, int permissions, CStdString owner, CStdString group, CStdString rootDirectory);
int FileSetPermissions(CStdString filename, int permissions);
int FileSetOwnership(CStdString filename, CStdString owner, CStdString group);
void FileEscapeName(CStdString& in, CStdString& out);
bool FileIsExist(CStdString fileName);
int FileSizeInKb(CStdString fileName);	//return file's size in Kb

//===========================================================
int GetOrekaRtpPayloadTypeForSdpRtpMap(CStdString sdp);

// threading related stuff
typedef std::lock_guard<std::mutex> MutexSentinel;

//=====================================================
// Network related stuff

//We should set the apr_socket_t timeout to minimum, i.e 100ms before using OrkRecv_n or OrkSend_n
//The small socket timeout will minimize the delay of Ork's timeout, i.e max at timeoutMs + 100ms

int OrkAprSocketWait(apr_socket_t *sock, int direction); 	//1:read 0:write
int OrkRecv_n(apr_socket_t* socket, char* buf, int len, int64_t timeoutMs, int &lenRead);
int OrkSend_n(apr_socket_t* socket, const char* buf, int len, int64_t timeoutMs, int &lenSent);
#ifndef CENTOS_6
int SSL_writev (SSL *ssl, const struct iovec *vector, int count);
int OrkSsl_Accept(SSL* ssl, int timeoutMs, CStdString &errstr);
int OrkSsl_Connect(SSL* ssl, int timeoutMs, CStdString &errstr);
int OrkSslRead(apr_socket_t* sock, SSL* ssl, char* buf, int len);
void OrkSslRead_n(apr_socket_t* sock, SSL* ssl, char* buf, int len, int64_t timeoutMs, int &lenRead);
int OrkSslWrite(apr_socket_t* sock, SSL* ssl, const char* buf, int len);
void OrkSslWrite_n(apr_socket_t* sock, SSL* ssl, const char* buf, int len, int64_t timeoutMs, int &lenWritten);
#endif
int OrkGetHostname(char *name, int len);
#define ORKMAXHOSTLEN 256
typedef struct 
{
	void ToString(CStdString& string);

	in_addr ip;
	unsigned short port;
} TcpAddress;

class TcpAddressList
{
public:
	void AddAddress(struct in_addr ip, unsigned short port);
	bool HasAddress(struct in_addr ip, unsigned short port);
	bool HasAddressOrAdd(struct in_addr ip, unsigned short port);
private:
	std::list<TcpAddress> m_addresses;
};

class IpRanges
{
public:
	bool Matches(struct in_addr ip);
	void Compute();
	bool Empty();

	std::list<CStdString> m_asciiIpRanges;
private:
	std::list<unsigned int> m_ipRangePrefixes;
	std::list<unsigned int> m_ipRangeBitWidths;
};

void GetHostFqdn(CStdString& fqdn, int size);

//=====================================================
// Miscellanous stuff

/** A counter that generates a "counting" 3 character strings, i.e. aaa, aab, ..., zzz 
	that represents a number between 0 and 26^3-1 (wrapping counter)
	and starts at a random location in this range.
	useful for generating debugging IDs
*/
class AlphaCounter
{
public:
	inline explicit AlphaCounter(const int start = 0, const std::string& prefix="")
	{
		if(start)
		{
			m_counter = start;
		}
		else
		{
			// Generate pseudo-random number from high resolution time least significant two bytes
			apr_time_t hrtime = apr_time_now();
			unsigned short srandom = (short)hrtime;
			double drandom = (double)srandom/65536.0; 	// 0 <= random < 1 

			m_counter = (unsigned int)(drandom*(26*26*26*26));
		}
	}

	inline CStdString GetNext()
	{
		m_counter++;
		if(m_counter >= (26*26*26*26) )
		{
			m_counter = 0;
		}
		unsigned int char1val = m_counter/(26*26*26);
		unsigned int remains = m_counter%(26*26*26);
		unsigned int char2val = remains/(26*26);
		unsigned int remains2 = remains%(26*26);
		unsigned int char3val = remains2/26;
		unsigned int char4val = remains2%26;

		char1val += 65;
		char2val += 65;
		char3val += 65;
		char4val += 65;

		CStdString string;
		string.Format("%c%c%c%c", char1val, char2val, char3val, char4val);
		return string;
	}

	inline void Reset(int value = 0)
	{
		m_counter = value;
	}
private:
	unsigned int m_counter;
};
#ifdef WIN32
inline void SetThreadName(const char *name) {;}
#else
inline void SetThreadName(const char *name)
{
	char threadname[17];   //maximum pthread custom name is 16 characters
	strncpy(threadname,name,16);
	threadname[16] = '\0'; //make sure string is ASCIIZ
	pthread_setname_np(pthread_self(),threadname);
}
#endif
class OrkTimeValue
{
public:
	OrkTimeValue(){
		timeVal = apr_time_now();
	}
	OrkTimeValue(apr_time_t sec, apr_time_t usec){
        timeVal = sec*1000*1000 + usec;
    }
	explicit OrkTimeValue(apr_time_t us){
        timeVal = us;
    }
    OrkTimeValue operator=(OrkTimeValue other);

	OrkTimeValue operator+(const OrkTimeValue other) const{
        apr_time_t timeV = timeVal + other.timeVal;
        return OrkTimeValue(timeV);
    }
    OrkTimeValue operator-(const OrkTimeValue other) const{
        apr_time_t timeV = timeVal - other.timeVal;
        return OrkTimeValue(timeV);
    }
    bool operator<(const OrkTimeValue other) const{
        if(other.timeVal < timeVal) return false;
        else return true;
    }
    bool operator>(const OrkTimeValue other) const{
        if(other.timeVal < timeVal) return true;
        else return false;
    }
    bool operator==(const OrkTimeValue other) const{
        if(other.timeVal == timeVal) return true;
        else return false;
    }
    bool operator!=(const OrkTimeValue other) const{
        if(other.timeVal != timeVal) return true;
        else return false;
    }
    bool operator>=(const OrkTimeValue other) const{
        if(other.timeVal <= timeVal) return true;
        else return false;
    }
    bool operator<=(const OrkTimeValue other) const{
        if(other.timeVal >= timeVal) return true;
        else return false;
    }
    void SetTimeValue(apr_time_t sec, apr_time_t usec){
        timeVal = sec*1000*1000 + usec;
    }
	[[nodiscard]] apr_time_t sec() const{
		return timeVal/(1000*1000);
	}
	[[nodiscard]] apr_time_t usec() const{
		return timeVal;
	}
	[[nodiscard]] apr_time_t msec() const{
		return timeVal/1000;
	}
	void GetTimeNow(){
		timeVal = apr_time_now();

	}
private:
	apr_time_t timeVal;

};

void set_socket_buffer_size(log4cxx::LoggerPtr log, const char *msg, apr_socket_t *sock, int size);



typedef enum
{
	pt_Unknown = -1,
	pt_PCMU = 0,
	pt_GSM = 3,
	pt_G723 = 4,
	pt_PCMA = 8,
	pt_G722 = 9,
	pt_G729 = 18,
	// the following payloads are dynamically assigned. The following values
	// are used internally to discriminate the payload type.
	pt_OPUS	= 60,
	pt_AMRNB = 61,
	pt_AMRWB = 62,
	pt_ILBC = 63,
	pt_SILK = 64,
	pt_SPEEX = 66,
	pt_TEL_EVENT =67
} RtpPayloadType;

CStdString RtpPayloadTypeEnumToString(char pt);
size_t ciFind(const std::string &Haystack, const std::string &Needle);
#ifdef SUPPORT_TLS_SERVER
CStdString SSLErrorQ();
#endif

#ifndef WIN32
void check_pcap_capabilities(log4cxx::LoggerPtr log);
#endif
#endif
