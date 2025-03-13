METASOURCES = AUTO
noinst_LTLIBRARIES = libserializers.la
libserializers_la_SOURCES = DomSerializer.cpp DomSerializer.h Serializer.cpp\
	Serializer.h SingleLineSerializer.cpp SingleLineSerializer.h\
	UrlSerializer.cpp UrlSerializer.h XmlRpcSerializer.cpp\
	XmlRpcSerializer.h
#libserializers_la_LIBADD = -L/projects/ext/xmlrpc++/xmlrpc++0.7/ -lXmlRpc
INCLUDES = -I@top_srcdir@ -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/usr/local/include/xercesc/dom -I@SYSROOT@/usr/usr/local/include/xercesc -I@SYSROOT@/usr/usr/local/include/xercesc/dom/impl
#-I@SYSROOT@/usr/include/apr-1.0

AM_CXXFLAGS = -O2 -D_REENTRANT -std=c++17 -Wall -Wextra -fno-omit-frame-pointer -fno-omit-frame-pointer
