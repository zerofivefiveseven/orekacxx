METASOURCES = AUTO
plugindir= $(prefix)/lib/orkaudio/plugins
plugin_LTLIBRARIES = libg729codec.la
libg729codec_la_SOURCES = G729Codec.cpp
libg729codec_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
#-L/home/revyakin/oreka/arm-buildroot-linux-gnueabihf_sdk-buildroot/include/bcg729
libg729codec_la_LIBADD = -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -lbcg729 -llog4cxx
LDFLAGS += --sysroot= /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I/home/revyakin/bcg729/src -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1
#-I/home/revyakin/bcg729/src problem with portability
#-I$(SYSROOT)/usr/include/apr-1

all-local: libg729codec.la
	cp .libs/libg729codec.so.0.0.0 ../../plugins/libg729codec.so
