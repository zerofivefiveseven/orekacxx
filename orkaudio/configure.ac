AC_SUBST(SYSROOT,'$(SYSROOT)')
AC_INIT([orkaudio],[1.0])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([subdir-objects])

speex_lib=speex

AC_ARG_ENABLE( [tsan],
	AS_HELP_STRING([--enable-tsan], [enable thread sanitizer])
)
AC_ARG_ENABLE( [tsan],
	AS_HELP_STRING([--enable-asan], [enable address sanitizer])
)

AC_LANG([C++])
AC_PROG_CXX
LT_INIT

AS_IF([test "x$enable_tsan" = "xyes"], [
	CXXFLAGS+=" -fsanitize=thread "
	LDFLAGS+=" -fsanitize=thread "
])

AS_IF([test "x$enable_asan" = "xyes"], [
	CXXFLAGS+=" -fsanitize=address "
	LDFLAGS+=" -fsanitize=address "
])

# Check if gcc supports cpp11
if [echo "int main(){}" | $CXX -std=c++17 -xc++ -S -  &>/dev/null] ; then
	CXXFLAGS+=" -std=c++17 -DSUPPORTS_CPP11 -fPIC"
	LDFLAGS+=" -ldl"
else
	AC_MSG_FAILURE("Compiler must support C++ 11")
fi

# don't support TLS under Centos 6
if [! grep "release 6" /etc/redhat-release]; then
    CXXFLAGS+=" -DSUPPORT_TLS_SERVER -DSUPPORT_TLS_CLIENT"
fi

if [ egrep -i "release [67]" /etc/redhat-release ]; then
	speex_lib=orkspeex
fi

LT_INIT
AC_SEARCH_LIBS([dwarf_begin],[dw],[][
	LDFLAGS+=" -ldw"
], AC_MSG_ERROR([libdw is not installed.]))

AC_SEARCH_LIBS([unw_backtrace],[unwind],[][
	LDFLAGS+=" -lunwind"
], AC_MSG_ERROR([libunwind is not installed.]))

AC_PREFIX_DEFAULT(/usr)

AC_SUBST(speex_lib,$speex_lib)
	
AC_CONFIG_FILES([Makefile audiocaptureplugins/Makefile audiocaptureplugins/generator/Makefile audiocaptureplugins/voip/Makefile filters/Makefile filters/rtpmixer/Makefile filters/silkcodec/Makefile filters/g729codec/Makefile filters/LiveStream/Makefile])
AC_OUTPUT

echo ""
echo "========= Configuration ==========="
echo ""
echo "     cxx : $CXX"
echo "cxxflags : $CXXFLAGS"
echo " ldflags : $LDFLAGS"
echo "   speex : $speex_lib"
echo ""
echo "==================================="
