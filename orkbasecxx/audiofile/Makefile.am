METASOURCES = AUTO
noinst_LTLIBRARIES = libaudiofile.la
libaudiofile_la_SOURCES = MediaChunkFile.cpp AudioFile.cpp  LibSndFileFile.cpp  PcmFile.cpp opus_header.cpp OggOpusFile.cpp
AM_CPPFLAGS = -D_REENTRANT -std=c++17 -Wall -Wextra
libaudiofile_la_LIBADD = -logg -L@SYSROOT@/usr/lib -lopusstatic
INCLUDES = -I@top_srcdir@ -I../../orkbasecxx -I@SYSROOT@/usr/include/opus -I@SYSROOT@/usr/include/apr-1/ -I@SYSROOT@/usr/include/apr-1.0/
#-I@SYSROOT@/usr/include/apr-1
