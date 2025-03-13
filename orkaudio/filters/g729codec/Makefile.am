METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = libg729codec.la
libg729codec_la_SOURCES = G729Codec.cpp
libg729codec_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
#-L/home/revyakin/oreka/arm-buildroot-linux-gnueabihf_sdk-buildroot/include/bcg729
libg729codec_la_LIBADD = -L@SYSROOT@/usr/lib -lbcg729 -llog4cxx
LDFLAGS += --sysroot=@SYSROOT@
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I@SYSROOT@/usr/include -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/include/bcg729
#-I/home/revyakin/bcg729/src problem with portability
#-I$(SYSROOT)/usr/include/apr-1

all-local: libg729codec.la
	cp .libs/libg729codec.so.0.0.0 ../../plugins/libg729codec.so
