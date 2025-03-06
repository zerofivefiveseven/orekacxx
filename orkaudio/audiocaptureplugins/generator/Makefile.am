METASOURCES = AUTO
lib_LTLIBRARIES = libgenerator.la
libgenerator_la_SOURCES = GeneratorConfig.cpp Generator.cpp \
	AudioCapturePluginCommon.cpp
libgenerator_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
LDFLAGS += --sysroot= /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/lib
libgenerator_la_LIBADD = -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/lib -lxerces-c -llog4cxx -lorkbase
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../common -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1 -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom/impl
#-I$(SYSROOT)/usr/include/apr-1
AudioCapturePluginCommon.cpp:
	ln -s ../common/AudioCapturePluginCommon.cpp AudioCapturePluginCommon.cpp
