METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = librtpmixer.la
librtpmixer_la_SOURCES = RtpMixer.cpp 
librtpmixer_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
librtpmixer_la_LIBADD = -lxerces-c -llog4cxx -lorkbase
LDFLAGS += --sysroot=@SYSROOT@ -L@SYSROOT@/usr/lib -L@SYSROOT@/lib
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../common -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/include/apr-1.0  -I@SYSROOT@/usr/usr/local/include/xercesc/dom -I@SYSROOT@/usr/usr/local/include/xercesc/dom/impl -I@SYSROOT@/usr/usr/local/include/xercesc
 #-I$(SYSROOT)/usr/include/apr-1
all-local: librtpmixer.la
	cp .libs/librtpmixer.so.0.0.0 ../../plugins/librtpmixer.so
