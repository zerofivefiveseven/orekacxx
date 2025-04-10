METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = libsilkcodec.la
libsilkcodec_la_SOURCES = SilkCodec.cpp 
libsilkcodec_la_LDFLAGS = -module 
AM_CPPFLAGS = -D_REENTRANT
libsilkcodec_la_LIBADD =  -llog4cxx \
			-L@SYSROOT@/usr/lib -lSKP_SILK_SDK
LDFLAGS += --sysroot=@SYSROOT@ -L@SYSROOT@/usr/lib -L@SYSROOT@/lib
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../../../dependencies/silk/SILK_SDK_SRC_FIX/interface -I../../../dependencies/silk/SILK_SDK_SRC_FIX/src  -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/include/apr-1.0
#-I$(SYSROOT)/usr/include/apr-1

all-local: libsilkcodec.la
	cp .libs/libsilkcodec.so.0.0.0 ../../plugins/libsilkcodec.so
