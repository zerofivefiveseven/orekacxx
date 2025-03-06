# Initialize Autoconf with the package name, version, and bug report email
AC_INIT([orkbase],[1.0],[your-email@example.com])
AC_SUBST(SYSROOT,'$(SYSROOT)')
# Initialize Automake with the specified options
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])  # Add this line

# Check for the C compiler
AC_PROG_CC

# Check for the C++ compiler (if needed)
AC_PROG_CXX

# Initialize Libtool
LT_INIT

# Specify the source directory (replace 'src/file.c' with an actual file in your project)
AC_CONFIG_SRCDIR([configure.in])

# Generate the config.h header file
AC_CONFIG_HEADERS([config.h])

# Finalize the configuration
AC_OUTPUT



AC_ARG_ENABLE( [tsan],
	AS_HELP_STRING([--enable-tsan], [enable thread sanitizer])
)
AC_ARG_ENABLE( [tsan],
	AS_HELP_STRING([--enable-asan], [enable address sanitizer])
)

boost_lib=-lboost_system

AC_PREFIX_DEFAULT(/usr)

AC_LANG([C++])
AC_PROG_CXX

AS_IF([test "x$enable_tsan" = "xyes"], [
	CXXFLAGS+=" -fsanitize=thread "
	LDFLAGS+=" -fsanitize=thread "
])

AS_IF([test "x$enable_asan" = "xyes"], [
	CXXFLAGS+=" -fsanitize=address "
	LDFLAGS+=" -fsanitize=address "
])

# look for XERCES_3
AC_CHECK_HEADERS([/usr/include/xercesc/dom/DOMLSSerializer.hpp],[CXXFLAGS+=" -DXERCES_3 "])
CXXFLAGS+=" -DXERCES_3 "
if [grep "release 7" /etc/redhat-release]; then
    boost_lib=/usr/lib64/static_libboost_system.a
fi

# don't support TLS under Centos 6
if [! grep "release 6" /etc/redhat-release]; then
    CXXFLAGS+=" -DSUPPORT_TLS_SERVER -DSUPPORT_TLS_CLIENT"
fi

if [cat /etc/redhat-release | grep "release 6"]; then
    CXXFLAGS+=" -DCENTOS_6 -D__STDC_CONSTANT_MACROS"
    boost_lib=
fi

AC_SUBST(boost_lib,$boost_lib)

# Check if gcc supports cpp11 
if [echo "int main(){}" | $CXX -std=c++17 -xc++ -S -  &>/dev/null]; then 
	CXXFLAGS+=" -std=c++17 -DSUPPORTS_CPP11 -fPIC"
else	
	AC_MSG_FAILURE("Compiler must support C++ 11")
fi


LT_INIT

AC_CONFIG_FILES([Makefile messages/Makefile serializers/Makefile audiofile/Makefile filters/Makefile filters/gsm/Makefile filters/gsm/gsm610/Makefile filters/ilbc/Makefile filters/ilbc/ilbc/Makefile filters/audiogain/Makefile filters/g722codec/Makefile filters/speex/Makefile filters/g726codecs/Makefile filters/g726codecs/g72x/Makefile filters/opuscodec/Makefile])
AC_OUTPUT


echo ""
echo "========= Configuration ==========="
echo ""
echo "     cxx : $CXX"
echo "cxxflags : $CXXFLAGS"
echo "boost_lib: $boost_lib"
echo ""
echo "==================================="
