# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4
sbin_PROGRAMS = orkaudio
orkaudio_LDADD =
orkaudio_LDFLAGS = -L@SYSROOT@/usr/lib -lxerces-c -llog4cxx -lorkbase -lsndfile -l@speex_lib@ -lapr-1 -lssl -lcrypto -lpthread -ldl -lstdc++fs
orkaudio_SOURCES = 	OrkAudio.cpp
INCLUDES = -I@top_srcdir@ -I../orkbasecxx -I@SYSROOT@/usr/include/opus -I../orkbasecxx/filters/gsm -I../orkbasecxx/filters/gsm/gsm610 -I../orkbasecxx/filters/ilbc -I../orkbasecxx/filters/ilbc/ilbc -I../orkbasecxx/filters/audiogain -I../orkbasecxx/filters/g722codec -I../orkbasecxx/filters/speex -I../orkbasecxx/filters/g726codecs -I../orkbasecxx/filters/g726codecs/g72x -I../orkbasecxx/filters/opuscodec  -I@SYSROOT@/usr/usr/local/include/xercesc/dom -I@SYSROOT@/usr/include/apr-1 -I@SYSROOT@/usr/usr/local/include/xercesc/dom/impl -I@SYSROOT@/usr/usr/local/include/xercesc
#-I$(SYSROOT)/usr/include/apr-1.0
AM_CPPFLAGS = -D_REENTRANT -std=c++17
SUBDIRS = audiocaptureplugins filters
#orkaudio_LDADD = $(top_builddir)/messages/libmessages.la 

orkaudioconfdir = /etc/orkaudio
orkaudioconf_DATA = \
	logging-linux-template.properties \
	config-linux-template.xml
install-orkaudioconfDATA:
	mkdir -p /etc/orkaudio
	cp logging-linux-template.properties /etc/orkaudio/logging.properties
	cp config-linux-template.xml /etc/orkaudio/config.xml
	ldconfig
	mkdir -p /var/log/orkaudio


