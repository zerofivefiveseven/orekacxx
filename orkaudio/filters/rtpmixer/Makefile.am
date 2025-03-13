METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = librtpmixer.la
librtpmixer_la_SOURCES = RtpMixer.cpp 
librtpmixer_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
librtpmixer_la_LIBADD = -lxerces-c -llog4cxx -lorkbase
LDFLAGS += --sysroot=/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/lib
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../common -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1 -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom/impl -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc
 #-I$(SYSROOT)/usr/include/apr-1
all-local: librtpmixer.la
	cp .libs/librtpmixer.so.0.0.0 ../../plugins/librtpmixer.so
