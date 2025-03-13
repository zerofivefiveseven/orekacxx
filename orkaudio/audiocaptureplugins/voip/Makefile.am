METASOURCES = AUTO
lib_LTLIBRARIES = libvoip.la
libvoip_la_SOURCES = VoIpConfig.cpp VoIp.cpp ../common/Rtp.cpp Iax2Session.cpp VoIpSession.cpp Win1251.cpp\
	../common/SipTcp.cpp AudioCapturePluginCommon.cpp ../common/PacketHeaderDefs.cpp ParsingUtils.cpp\
	SipParsers.cpp SkinnyParsers.cpp Iax2Parsers.cpp SipHeaders.cpp IpFragmentation.cpp ../common/DtmfHandling.cpp ../common/AcpConfig.cpp ../common/OrkSession.cpp \
	../common/SafeBuffer.cpp
libvoip_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
LDFLAGS += --sysroot=/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/lib
libvoip_la_LIBADD = -lxerces-c -llog4cxx -lorkbase -lpcap
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../common -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1 -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/xercesc
#-I$(SYSROOT)/usr/include/apr-1
AudioCapturePluginCommon.cpp:
	ln -s ../common/AudioCapturePluginCommon.cpp AudioCapturePluginCommon.cpp
