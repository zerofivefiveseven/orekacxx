METASOURCES = AUTO
lib_LTLIBRARIES = libgenerator.la
libgenerator_la_SOURCES = GeneratorConfig.cpp Generator.cpp \
	AudioCapturePluginCommon.cpp
libgenerator_la_LDFLAGS = -module
AM_CPPFLAGS = -D_REENTRANT
LDFLAGS += --sysroot=@SYSROOT@ -L@SYSROOT@/usr/lib -L@SYSROOT@/lib
libgenerator_la_LIBADD = -L@SYSROOT@/usr/lib -L@SYSROOT@/lib -lxerces-c -llog4cxx -lorkbase
INCLUDES = -I@top_srcdir@ -I../../../orkbasecxx -I../common -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/include/xercesc/dom -I@SYSROOT@/include/xercesc -I@SYSROOT@/include/xercesc/dom/impl
#-I$(SYSROOT)/usr/include/apr-1
AudioCapturePluginCommon.cpp:
	ln -s ../common/AudioCapturePluginCommon.cpp AudioCapturePluginCommon.cpp
