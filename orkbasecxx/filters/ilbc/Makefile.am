METASOURCES = AUTO
SUBDIRS = ilbc
noinst_LTLIBRARIES = libilbc.la
libilbc_la_SOURCES = IlbcFilters.cpp
INCLUDES = -I@top_srcdir@ -I./ilbc -I@SYSROOT@/usr/include/apr-1
#-I@SYSROOT@/usr/include/apr-1
AM_CXXFLAGS = -g -O0 -D_REENTRANT -std=c++17 -Wall -Wextra -fPIC