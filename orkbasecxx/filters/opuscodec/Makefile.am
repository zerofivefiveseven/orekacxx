METASOURCES = AUTO
SUBDIRS = 
noinst_LTLIBRARIES = libopuscodec.la
libopuscodec_la_SOURCES = OpusCodec.cpp
INCLUDES = -I@top_srcdir@ -I../.. -I@SYSROOT@/usr/include/opus -I@SYSROOT@/usr/include/apr-1/ -I@SYSROOT@/usr/include/apr-1.0/
#-I@SYSROOT@/usr/include/apr-1.0
libopuscodec_la_LIBADD = -L@SYSROOT@/usr/lib -lopusstatic
AM_CXXFLAGS = -g -O0 -D_REENTRANT -std=c++17 -Wall -Wextra -fPIC

