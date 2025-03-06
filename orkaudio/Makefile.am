# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

MY_SYSROOT=$$SYSROOT ; echo $$SYSROOT
sbin_PROGRAMS = orkaudio
orkaudio_LDADD =
orkaudio_LDFLAGS = -L/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib -lxerces-c -llog4cxx -lorkbase -lsndfile -l@speex_lib@ -lapr-1 -lssl -lcrypto -lpthread -ldl
orkaudio_SOURCES = 	OrkAudio.cpp
INCLUDES = -I@top_srcdir@ -I../orkbasecxx -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opus -I../orkbasecxx/filters/gsm -I../orkbasecxx/filters/gsm/gsm610 -I../orkbasecxx/filters/ilbc -I../orkbasecxx/filters/ilbc/ilbc -I../orkbasecxx/filters/audiogain -I../orkbasecxx/filters/g722codec -I../orkbasecxx/filters/speex -I../orkbasecxx/filters/g726codecs -I../orkbasecxx/filters/g726codecs/g72x -I../orkbasecxx/filters/opuscodec  -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/apr-1 -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc/dom/impl -I /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/usr/local/include/xercesc
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


