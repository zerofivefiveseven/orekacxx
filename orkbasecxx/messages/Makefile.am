METASOURCES = AUTO
noinst_LTLIBRARIES = libmessages.la
libmessages_la_SOURCES = AsyncMessage.cpp Message.cpp \
	SyncMessage.cpp CaptureMsg.cpp DeleteTapeMsg.cpp \
	PingMsg.cpp TapeMsg.cpp RecordMsg.cpp InitMsg.cpp \
	CrashMessage.cpp AddTagMsg.cpp\
	ReadLoggingPropertiesMsg.cpp\
	OrkaudioVersionMsg.cpp
#libmessages_la_LIBADD = -L/projects/ext/xmlrpc++/xmlrpc++0.7/ -lXmlRpc
INCLUDES = -I@top_srcdir@ -I../../orkaudio -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/usr/local/include/xercesc/dom -I@SYSROOT@/usr/usr/local/include/xercesc/dom/impl -I@SYSROOT@/usr/usr/local/include/xercesc

#-I@SYSROOT@/usr/include/apr-1
AM_CXXFLAGS = -O2 -D_REENTRANT -std=c++17 -Wall -Wextra -fno-omit-frame-pointer -fno-omit-frame-pointer
