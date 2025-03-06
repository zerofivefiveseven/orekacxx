METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = libsilkcodec.la
libsilkcodec_la_SOURCES = SilkCodec.cpp 
libsilkcodec_la_LDFLAGS = -module 
AM_CPPFLAGS = -D_REENTRANT
libsilkcodec_la_LIBADD =  -llog4cxx \
			-L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -lSKP_SILK_SDK
LDFLAGS += --sysroot= /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/lib
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I/opt/silk/SILKCodec/SILK_SDK_SRC_FIX/interface -I/opt/silk/SILKCodec/SILK_SDK_SRC_FIX/src  -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1
#-I$(SYSROOT)/usr/include/apr-1

all-local: libsilkcodec.la
	cp .libs/libsilkcodec.so.0.0.0 ../../plugins/libsilkcodec.so
