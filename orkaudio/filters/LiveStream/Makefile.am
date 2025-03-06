METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = livestream.la
livestream_la_SOURCES = LiveStreamConfig.cpp LiveStreamFilter.cpp LiveStreamSession.cpp LiveStreamServer.cpp
livestream_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT 
livestream_la_LIBADD = -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -llog4cxx -lapr-1 -lpthread -ldl -lm -lcrypto -lstdc++ -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -lsrs_librtmp
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../../../orkaudio/audiocaptureplugins/common -I../../../orkaudio/audiocaptureplugins/voip -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/httplib -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1 -I/opt/srs-librtmp/objs/include/

all-local: livestream.la
	cp .libs/livestream.so.0.0.0 ../../plugins/livestream.so
