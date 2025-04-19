METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = livestream.la
livestream_la_SOURCES = LiveStreamConfig.cpp LiveStreamFilter.cpp LiveStreamSession.cpp LiveStreamServer.cpp RecorderSenderPool.cpp
livestream_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
livestream_la_LIBADD = -L@SYSROOT@/usr/lib -llog4cxx -lapr-1 -lpthread -ldl -lm -lcrypto -lstdc++ -L@SYSROOT@/usr/lib -lsrs_librtmp
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../../../orkaudio/audiocaptureplugins/common -I../../../orkaudio/audiocaptureplugins/voip -I@SYSROOT@/usr/include/httplib -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/include/apr-1.0 -I../../../dependencies/srs/trunk/srs-librtmp/objs/include/ -I../../../dependencies/srs/trunk/srs-librtmp/objs/

all-local: livestream.la
	cp .libs/livestream.so.0.0.0 ../../plugins/livestream.so
