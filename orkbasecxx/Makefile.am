# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4
SHELL := '/bin/bash'
lib_LTLIBRARIES = liborkbase.la
liborkbase_la_LDFLAGS = -lapr-1 -lpthread -lssl @boost_lib@ -lcap -llog4cxx
liborkbase_la_SOURCES = Filter.cpp g711.c \
			OrkBase.cpp Object.cpp ObjectFactory.cpp \
			OrkClient.cpp AudioCapture.cpp Utils.cpp  \
			AudioTape.cpp CommandProcessing.cpp  \
			Config.cpp ConfigManager.cpp LogManager.cpp \
			MultiThreadedServer.cpp TapeProcessor.cpp \
			MemUtils.cpp BatchProcessing.cpp \
			CapturePluginProxy.cpp CapturePort.cpp \
			Daemon.cpp ImmediateProcessing.cpp \
			Reporting.cpp TapeFileNaming.cpp \
			PartyFilter.cpp EventStreaming.cpp \
			OrkTrack.cpp SocketStreamer.cpp	\
			SizedBuffer.cpp DirectionSelector.cpp \
			SslUtils.cpp
#INCLUDES = -I/projects/ext/xmlrpc++/xmlrpc++0.7/src
INCLUDES = -I@SYSROOT@/usr/include/opus -I@SYSROOT@/usr/include/apr-1/ -I@SYSROOT@/usr/include/apr-1/
SUBDIRS = messages serializers audiofile filters
liborkbase_la_LIBADD = $(top_builddir)/serializers/libserializers.la \
	$(top_builddir)/messages/libmessages.la \
	$(top_builddir)/audiofile/libaudiofile.la \
        $(top_builddir)/filters/gsm/libgsm.la \
        $(top_builddir)/filters/gsm/gsm610/libgsm610.la \
	$(top_builddir)/filters/ilbc/libilbc.la \
	$(top_builddir)/filters/ilbc/ilbc/libilbcrfc.la \
	$(top_builddir)/filters/audiogain/libaudiogain.la \
	$(top_builddir)/filters/g722codec/libg722codec.la \
	$(top_builddir)/filters/speex/libspeexcodec.la \
	$(top_builddir)/filters/g726codecs/libg721codec.la \
	$(top_builddir)/filters/g726codecs/g72x/libg72x.la \
	$(top_builddir)/filters/opuscodec/libopuscodec.la
	-lm

AM_CXXFLAGS = -g -O0 -D_REENTRANT -std=c++17 -Wall -Wextra -fPIC