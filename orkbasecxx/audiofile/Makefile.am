METASOURCES = AUTO
noinst_LTLIBRARIES = libaudiofile.la
libaudiofile_la_SOURCES = MediaChunkFile.cpp AudioFile.cpp  LibSndFileFile.cpp  PcmFile.cpp opus_header.cpp OggOpusFile.cpp
AM_CPPFLAGS = -D_REENTRANT -std=c++17 -Wall -Wextra
libaudiofile_la_LIBADD = -logg -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -lopusstatic
INCLUDES = -I@top_srcdir@ -I../../orkbasecxx -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opus -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1
#-I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1
