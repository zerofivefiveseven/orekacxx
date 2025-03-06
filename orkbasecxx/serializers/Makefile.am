METASOURCES = AUTO
noinst_LTLIBRARIES = libserializers.la
libserializers_la_SOURCES = DomSerializer.cpp DomSerializer.h Serializer.cpp\
	Serializer.h SingleLineSerializer.cpp SingleLineSerializer.h\
	UrlSerializer.cpp UrlSerializer.h XmlRpcSerializer.cpp\
	XmlRpcSerializer.h
#libserializers_la_LIBADD = -L/projects/ext/xmlrpc++/xmlrpc++0.7/ -lXmlRpc
INCLUDES = -I@top_srcdir@ -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1 -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom/impl
#-I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1.0

AM_CXXFLAGS = -O2 -D_REENTRANT -std=c++17 -Wall -Wextra -fno-omit-frame-pointer -fno-omit-frame-pointer
