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
 
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <string>
#define BACKWARD_HAS_DW 1
#define BACKWARD_HAS_LIBUNWIND 1
#include "backward.hpp"

#include "MultiThreadedServer.h"
#include "OrkAudio.h"
#include "Utils.h"
#include "messages/TapeMsg.h"
#include "messages/PingMsg.h"
#include "messages/DeleteTapeMsg.h"
#include "messages/CaptureMsg.h"
#include "messages/TestMsg.h"
#include "messages/RecordMsg.h"
#include "messages/OrkaudioVersionMsg.h"
#include "messages/InitMsg.h"
#include "messages/ReadLoggingPropertiesMsg.h"
//#include "messages/CrashMessage.cpp"
#include <apr_dso.h>

#include "Config.h"
#include "LogManager.h"
#include "ImmediateProcessing.h"
#include "BatchProcessing.h"
#include "Reporting.h"
#include "CommandProcessing.h"
#include "TapeFileNaming.h"
#include "DirectionSelector.h"
#include "RecorderSender.h"
#include "ConfigManager.h"
#include "Daemon.h"
#include "ObjectFactory.h"
#include "CapturePluginProxy.h"
#include "AudioCapturePlugin.h"
#include "Filter.h"
#include "GsmFilters.h"
#include "IlbcFilters.h"
#include "G722Codec.h"
#include "filters/audiogain/AudioGain.h"
#include "TapeProcessor.h"
#include <list>
#include "EventStreaming.h"
#include "OrkTrack.h"
#include "SocketStreamer.h"
#include "SpeexCodec.h"
#include "G721Codec.h"
#include "OpusCodec.h"
#include <memory>
#include <thread>
#include "apr_signal.h"
#include <sys/prctl.h>

#ifdef linux  
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>       // std::abort
#include <iostream>      // std::cerr
#endif

static volatile bool serviceStop = false;
struct orkaudio_version
{
	unsigned int magic;
	int size;
	const char version[256];
};

struct orkaudio_version orkaudioVersion  { 0x702a6f27, sizeof(struct orkaudio_version), ""};

void MakeDumpable()
{
	// if run as not-root with capabilities, dumpable capability
	// may be turned off. If so, restore it.
	if (!prctl(PR_GET_DUMPABLE, 0, 0, 0, 0))
	{
		if (prctl(PR_SET_DUMPABLE, 1, 0, 0 ,0) != 0)
		{
			CStdString logMsg;
			logMsg.Format("Unable to restore DUMPABLE capability: %d:%s", errno, strerror(errno));
			LOG4CXX_WARN(LOG.rootLog,logMsg);
		}
		else
		{
			LOG4CXX_INFO(LOG.rootLog,"DUMPABLE capability is restored");
		}
	}
}

void StopHandler()
{
	serviceStop = true;
}

#ifdef WIN32
long ExceptionFilter(struct _EXCEPTION_POINTERS *ptr)
{
	return EXCEPTION_CONTINUE_SEARCH;
}
#endif

void LoadPlugins(std::vector<void*>& pluginDlls)
{
	OrkAprSubPool locPool;

	std::string pluginsDirectory{CONFIG.m_pluginsDirectory};
	if(pluginsDirectory.empty())
	{
		// default unix plugins directory
		pluginsDirectory = "/usr/lib/orkaudio/plugins/";
		LOG4CXX_WARN(LOG.rootLog, "Config provided plugin directory could not be found. Using built-in path for plugins:"
                                              << pluginsDirectory << " check your config.xml");
	}
	std::string pluginExtension = ".so";

    if(!std::filesystem::exists(pluginsDirectory)){
		LOG4CXX_ERROR(LOG.rootLog, "Plugins directory which is default could not be found:" << pluginsDirectory << " check your config.xml");
    }
    else
    {
    	LOG4CXX_WARN(LOG.rootLog, CStdString("Trying to find .so"));
    	for (auto const& dirEntry : std::filesystem::directory_iterator{pluginsDirectory}) {
    		if(dirEntry.path().extension() == pluginExtension) {
    			LOG4CXX_WARN(LOG.rootLog, "Found *.so; Path:" << dirEntry.path());
    			void* soHandle = dlopen(dirEntry.path().c_str(), RTLD_NOW);
    			if(!soHandle) {
    				LOG4CXX_WARN(LOG.rootLog, "Can't open .so; Path:" << dirEntry.path() << " Reason:" << dlerror());
    			}
    			else {
    				LOG4CXX_WARN(LOG.rootLog, "Successfully open .so; Path:" << dirEntry.path());
    				dlerror();

    				using InitFuncType = void(*)();
    				InitFuncType initFunc = reinterpret_cast<InitFuncType>(dlsym(soHandle, "OrkInitialize"));

    				char* error = dlerror();
    				if (error) {
    					LOG4CXX_WARN(LOG.rootLog, "Can't load symbol from .so; Path:" << dirEntry.path() << " Reason:" << error);
    					dlclose(soHandle);
    				}
    				else if (initFunc) {
    					LOG4CXX_WARN(LOG.rootLog, "Successfully loaded .so; Path:" << dirEntry.path());
    					initFunc();
    					pluginDlls.push_back(soHandle);
    				}
    				else {
    					LOG4CXX_WARN(LOG.rootLog, "Can't load .so; Path:" << dirEntry.path());
    					dlclose(soHandle);
    				}
    			}
    		}
    	}
    }
//        LOG4CXX_WARN(LOG.rootLog, CStdString("Trying to find .so"));
//		CStdString pluginPath;
//		apr_finfo_t finfo;
//		apr_int32_t wanted = APR_FINFO_NAME | APR_FINFO_SIZE;
//		while((ret = apr_dir_read(&finfo, wanted, dir)) == APR_SUCCESS)
//		{
//			apr_dso_handle_t *dsoHandle;
//			CStdString fileName;
//			fileName.Format("%s", finfo.name);
//			int extensionPos = fileName.Find(pluginExtension);
//            LOG4CXX_WARN(LOG.rootLog, CStdString("Trying to find .so . Path:" + pluginsDirectory + "Filename:" + fileName + "finfo.name: " + finfo.name ));
//			if((extensionPos != -1) && ((fileName.size() - extensionPos) == pluginExtension.size()))
//			{
//				pluginPath = pluginsDirectory + finfo.name;
//				LOG4CXX_INFO(LOG.rootLog, CStdString("Loaded plugin: ") + pluginPath);
//				char errstr[256];
//				// dsoHandle needs to persist beyond this function, so we need to use
//				// a pool that also persists -- use the global pool. this is safe here because
//				// we're running in the main thread where the pool was created.
//				ret = apr_dso_load(&dsoHandle, (PCSTR)pluginPath, OrkAprSingleton::GetInstance()->GetAprMp());
//				if(ret != APR_SUCCESS)
//				{
//					apr_dso_error(dsoHandle, errstr, sizeof(errstr));
//					LOG4CXX_ERROR(LOG.rootLog, CStdString("Failed to load plugin: ") + pluginPath + " error:" + errstr);
//				}
//				else
//				{
//					LOG4CXX_INFO(LOG.rootLog, CStdString("Loaded plugin2222222: ") + pluginPath);
//
//					InitializeFunction initfunction;
//					ret = apr_dso_sym((apr_dso_handle_sym_t*)&initfunction, dsoHandle, "OrkInitialize");
//					if (ret == APR_SUCCESS)
//					{
//						initfunction();
//						pluginDlls.push_back(dsoHandle);
//					}
//					else
//					{
//						LOG4CXX_ERROR(LOG.rootLog, CStdString("Failed to initialize plugin: ") + pluginPath);
//					}
//				}
//
//			}
//		}
//		apr_dir_close(dir);
	}

void Transcode(CStdString &file)
{
	OrkLogManager::Instance()->Initialize();

	ObjectFactory::GetSingleton()->Initialize();

	ConfigManager::Instance()->Initialize();

	std::vector<void*> pluginDlls;
	LoadPlugins(pluginDlls);

	// Register in-built filters
	FilterRef filter(new AlawToPcmFilter());
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<UlawToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<GsmToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<IlbcToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<AudioGainFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<G722ToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<SpeexDecoder>( );
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<G721CodecDecoder>();
	FilterRegistry::instance()->RegisterFilter(filter);
    filter = std::make_shared<OpusCodecDecoder>();
	FilterRegistry::instance()->RegisterFilter(filter);
	
	// Register in-built tape processors and build the processing chain
	BatchProcessing::Initialize();
	Reporting::Initialize();
	TapeFileNaming::Initialize();

	try{
		std::thread handler(&BatchProcessing::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		CStdString logMsg;
		logMsg.Format("Failed to start BatchProcessing thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);
	}

	// Transmit the tape to the BatchProcessing
	CStdString ProcessorName("BatchProcessing");
	TapeProcessorRef bp = TapeProcessorRegistry::instance()->GetNewTapeProcessor(ProcessorName);
	CStdString portName("SinglePort");
	AudioTapeRef tape(new AudioTape(portName, file));
	bp->AddAudioTape(tape);

}

void MainThread()
{
	CStdString logMsg;
    // Avoid program exit on broken pipe
    apr_signal(SIGPIPE, SIG_IGN);
	OrkLogManager::Instance()->Initialize();
	RegisterOrkaudioVersion(orkaudioVersion.version);
	logMsg.Format("\n\nOrkAudio version %s: service starting\n", orkaudioVersion.version);
	LOG4CXX_INFO(LOG.rootLog, logMsg);

	MakeDumpable();  //allow corefiles to be generated

	ConfigManager::Instance()->Initialize();

	// Initialize object factory and register existing objects
	ObjectFactory::GetSingleton()->Initialize();

	ObjectRef objRef;
	objRef = std::make_shared<PingMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<TapeMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<TapeResponse>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<SimpleResponseMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<DeleteTapeMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<CaptureMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<TcpPingMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<ReportingSkipTapeMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<RecordMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<PauseMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<StopMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<InitMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<ReadLoggingPropertiesMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<ListLoggingPropertiesMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	objRef = std::make_shared<OrkaudioVersionMsg>();
	ObjectFactory::GetSingleton()->RegisterObject(objRef);
	//objRef.reset(new CrashMsg);
	//ObjectFactory::GetSingleton()->RegisterObject(objRef);
	//objRef.reset(new TestMsg);
	//ObjectFactory::GetSingleton()->RegisterObject(objRef);

	bool capturePluginOk = false;
	if(CapturePluginProxy::Singleton()->Initialize())
	{
		capturePluginOk = true;
	}
	std::vector<void*> pluginDlls;
	LoadPlugins(pluginDlls);
	// Register in-built filters
	FilterRef filter(new AlawToPcmFilter());
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<UlawToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<GsmToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<IlbcToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<AudioGainFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<G722ToPcmFilter>();
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<SpeexDecoder>( );
	FilterRegistry::instance()->RegisterFilter(filter);
	filter = std::make_shared<G721CodecDecoder>();
	FilterRegistry::instance()->RegisterFilter(filter);
    filter = std::make_shared<OpusCodecDecoder>();
	FilterRegistry::instance()->RegisterFilter(filter);
	
	// Register in-built tape processors and build the processing chain
	OrkTrack::Initialize(CONFIG.m_trackerHostname, CONFIG.m_trackerServicename, CONFIG.m_trackerTcpPort,  CONFIG.m_trackerTlsPort);
	BatchProcessing::Initialize();
	CommandProcessing::Initialize();
	Reporting::Initialize();
	TapeFileNaming::Initialize();
	DirectionSelector::Initialize();
	RecorderSender::Initialize();
	TapeProcessorRegistry::instance()->CreateProcessingChain();

	try{
		std::thread handler(&ImmediateProcessing::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		logMsg.Format("Failed to start ImmediateProcessing thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);	
	}

	try{
		std::thread handler(&RecorderSender::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		logMsg.Format("Failed to start RecorderSender thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);
	}
	
	if(CONFIG.m_storageAudioFormat != FfNative)
	{
		// storage format is not native, which means we need batch workers to compress to wanted format 
		for(int i = 0; i < CONFIG.m_numBatchThreads; i++)
		{
			try{
				std::thread handler(&BatchProcessing::ThreadHandler);
				handler.detach();
			} catch(const std::exception &ex){
				logMsg.Format("Failed to start BatchProcessing thread reason:%s",  ex.what());
				LOG4CXX_ERROR(LOG.rootLog, logMsg);	
			}
		}
	}

	try{
		std::thread handler(&Reporting::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		logMsg.Format("Failed to start Reporting thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);	
	}

	try{
		std::thread handler(&TapeFileNaming::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		logMsg.Format("Failed to start TapeFileNaming thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);	
	}

	try{
		std::thread handler(&CommandProcessing::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		logMsg.Format("Failed to start CommandProcessing thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);	
	}

	try{
		std::thread handler(&DirectionSelector::ThreadHandler);
		handler.detach();
	} catch(const std::exception &ex){
		logMsg.Format("Failed to start DirectionSelector thread reason:%s",  ex.what());
		LOG4CXX_ERROR(LOG.rootLog, logMsg);	
	}
	// Create command line server
	// CommandLineServer commandLineServer(CONFIG.m_commandLineServerPort);
	// if(commandLineServer.Initialize())
	// {
	// 	std::thread handler(&CommandLineServer::Run, &commandLineServer);
	// 	handler.detach();
	// }

	HttpServer httpServ(CONFIG.m_httpServerPort);
	if(httpServ.Initialize())
	{
		std::thread handler(&HttpServer::Run, &httpServ);
		handler.detach();
	}
#ifdef SUPPORT_TLS_SERVER
	HttpsServer httpsServ;
	// TlsServePort is deprecated in favor of HttpTlsServerPort
	// Either is accepted, but WARN if both are defined
	if (CONFIG.m_httpTlsServerPort != 0 && CONFIG.m_tlsServerPort != 0)
	{
		LOG4CXX_WARN(LOG.rootLog,"TlsServerPort and HttpServerPort are both defined! TlsServerPort is ignored.");
	}
	if(httpsServ.Initialize(CONFIG.m_httpTlsServerPort ? CONFIG.m_httpTlsServerPort : CONFIG.m_tlsServerPort))
	{
		std::thread handler(&HttpsServer::Run, &httpsServ);
		handler.detach();
	}
#endif

	EventStreamingServer eventStreamingSvc(CONFIG.m_eventStreamingServerPort);
	if(eventStreamingSvc.Initialize())
	{
		std::thread handler(&EventStreamingServer::Run, &eventStreamingSvc);
		handler.detach();
	}

	if(capturePluginOk)
	{
		CapturePluginProxy::Singleton()->Run();
	}

	SocketStreamer::Initialize(CONFIG.m_socketStreamerTargets);

	while(!Daemon::Singleton()->IsStopping())
	{
		OrkSleepSec(1);
	}

	CapturePluginProxy::Singleton()->Shutdown();

	OrkSleepSec(2);
	
	//***** This is to avoid an exception when NT service exiting
	//***** Need to find out the real problem and fix
#ifdef WIN32
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);
#endif
	//*****

	LOG4CXX_INFO(LOG.rootLog, CStdString("Service stopped"));
	OrkLogManager::Instance()->Shutdown();
}

int main(int argc, char* argv[])
{

	backward::SignalHandling sh; //install fatal error backtrace handler.

	OrkAprSingleton::Initialize();

	// the "service name" reported on the tape messages uses CONFIG.m_serviceName
	// which also defaults to orkaudio-[hostname] but can be different depending on the
	// value set in config.xml
	char hostname[ORKMAXHOSTLEN];
	OrkGetHostname(hostname, sizeof(hostname));
	CStdString serviceName = CStdString("orkaudio-") + hostname;
	Daemon::Initialize(serviceName, MainThread, StopHandler);

	CStdString argument = argv[1];
	if (argc>1)
	{
		if ((argument.CompareNoCase("debug") == 0) || (argument.CompareNoCase("fg") == 0))
		{
			MainThread();
		}
		else if (argument.CompareNoCase("transcode") == 0)
		{
			if(argc == 3)
			{
				Daemon::Singleton()->SetShortLived();
				CStdString file = argv[2];
				Transcode(file);
			}
			else
			{
				printf("Please specify file to transcode\n\n");
			}
		}
		else if (argument.CompareNoCase("install") == 0)
		{
			Daemon::Singleton()->Install();
		}
		else if  (argument.CompareNoCase("uninstall") == 0)
		{
			Daemon::Singleton()->Uninstall();
		}
		else if  (argument.CompareNoCase("version") == 0)
		{
			std::cout << "orkaudio version: " << orkaudioVersion.version << std::endl;
		}

		else
		{
#ifdef WIN32
	printf("Argument incorrect. Possibilies are:\ninstall: install NT service\nuninstall: uninstall NT service\ntranscode <file>: convert .mcf file to storage format specified in config.xml\n\n");
#else
	printf("Argument incorrect. Possibilies are:\ndebug or fg: run attached to tty\n"
			"transcode <file>: convert .mcf file to storage format specified in config.xml\n"
			"version: display orkaudio version\n\n");
#endif
		}
	}
	else
	{
		// No arguments, launch the daemon
		printf("Starting orkaudio daemon ... (type 'orkaudio debug' if you prefer running attached to tty)\n");
		Daemon::Singleton()->Start();
	}
	return 0;
}
