#!/bin/bash

if [ ! -d ./arm-buildroot-linux-gnueabihf_sdk-buildroot ]; then
    echo "C++ toolchain not found"
    exit 1
fi
mkdir ./dependencies
pushd ./dependencies
sudo mkdir -p /share/aclocal
echo "user directory $(../whoami.sh)"
DEFAULTVALUE=$(../whoami.sh)
VARIABLE="${1:-$DEFAULTVALUE}"

export BUILDROOT_SDK="/home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot"
export SYSROOT="$BUILDROOT_SDK/arm-buildroot-linux-gnueabihf/sysroot"
export PATH="$BUILDROOT_SDK/bin:$BUILDROOT_SDK/bin:$PATH"
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
export LDFLAGS=" -L"$SYSROOT"/usr/lib -Wl,-rpath="$SYSROOT"/usr/lib --sysroot=$SYSROOT"
export LIBTOOL=$BUILDROOT_SDK/bin/libtool
export pkgconf=$BUILDROOT_SDK/bin/pkg-config
export PATH="$BUILDROOT_SDK/bin:$PATH"
export CC="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-gcc"
export CXX="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-g++"
export AR="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-ar"
export LD="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-ld"
export PATH="$BUILDROOT_SDK/bin:$BUILDROOT_SDK/bin:$PATH"
export automake="$BUILDROOT_SDK/bin/automake-1.16"
export AUTOMAKE="$BUILDROOT_SDK/bin/automake"
export autom4te="$BUILDROOT_SDK/bin/autom4te"
export m4="$BUILDROOT_SDK/bin/m4"
export LIBTOOLIZE="$BUILDROOT_SDK/bin/libtoolize"
export PATH=$BUILDROOT_SDK/bin:$PATH
export PKG_CONFIG_PATH=""$SYSROOT"/usr/lib/pkgconfig:$PKG_CONFIG_PATH"



echo "Buildroot SDK environment set up:"
echo "PATH: $PATH"
echo "CC: $CC"
echo "CXX: $CXX"
echo "LD: $LD"
echo "LIBRARY_PATH: $LIBRARY_PATH"
# Verify if the compiler exists
if [ ! -f "$CXX" ]; then
    echo "C++ compiler not found at $CXX"
    exit 1
fi

git clone --depth 1 https://github.com/BelledonneCommunications/bcg729.git ./bcg729
pushd ./bcg729
 CC="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-gcc" CXX="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-g++" CXXFLAGS="--sysroot="$SYSROOT" -D_GLIBCXX_USE_CXX11_ABI=1" CC=$CC CXX=$CXX cmake . \
                                                                                               -DCMAKE_INSTALL_PREFIX="$SYSROOT/usr" \
                                                                                               -DCMAKE_INSTALL_LIBDIR="$SYSROOT/usr/lib" \
                                                                                               -DCMAKE_C_COMPILER=$CC \
                                                                                               -DCMAKE_CXX_COMPILER=$CXX \
                                                                                               -DCMAKE_C_FLAGS="--sysroot=$SYSROOT -fPIC"
 make -j$(nproc)
 make install

popd


mkdir -p silk
git clone --depth 1 https://github.com/gaozehua/SILKCodec.git ./silk
pushd ./silk/SILK_SDK_SRC_ARM/
 make clean
export CXXFLAGS="--sysroot=$SYSROOT -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1 -g -O0 -fPIC"
DCMAKE_INSTALL_PREFIX="$SYSROOT/usr" TOOLCHAIN_PREFIX=$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf- CC=$CC CXX=$CXX CXXFLAGS=$CXXFLAGS CFLAGS="--sysroot="$SYSROOT" -fPIC -I$SYSROOT/usr/include/ -fPIC -g -O0" make all TARGRT_CPU=armv7l
cp libSKP_SILK_SDK.a "$SYSROOT"/usr/lib
cp encoder "$SYSROOT"/usr/lib
cp decoder "$SYSROOT"/usr/lib
popd

git clone https://github.com/OrecX/dependencies.git
 tar -xf dependencies/opus-1.2.1.tar.gz
pushd opus-1.2.1
 ./autogen.sh
  CC=$CC CXX=$CXX ./configure --host=arm-buildroot-linux-gnueabihf --build=x86_64-linux-gnu --prefix=""$SYSROOT"/usr" --libdir=""$SYSROOT"/usr/lib" --bindir=""$SYSROOT"/usr/bin" --enable-shared --with-pic --enable-static
  CC=$CC CXX=$CXX make -j$(nproc) CFLAGS="-fPIC"
  CC=$CC CXX=$CXX make install
 cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
 cp "$SYSROOT"/usr/lib/opus/lib/libopus.a "$SYSROOT"/usr/lib/opus/lib/libopusstatic.a
popd
readelf -h "$SYSROOT"/usr/lib/libopusstatic.a  | grep 'Class\|File\|Machine'


# backward-cpp HEADER ONLY
git clone --depth 1 https://github.com/bombela/backward-cpp.git
 cp ./backward-cpp/backward.hpp "$SYSROOT"/usr/include/backward.hpp

# httplib HEADER ONLY
mkdir -p "$SYSROOT"/usr/include/httplib
git clone --depth 1 --branch v0.12.3 https://github.com/yhirose/cpp-httplib.git
 cp ./cpp-httplib/httplib.h "$SYSROOT"/usr/include/httplib/

git clone --depth 1 https://github.com/nlohmann/json.git
pushd ./json
 make distclean
 CC=$CC CXX=$CXX cmake . -DCMAKE_INSTALL_PREFIX="$SYSROOT"/usr -DCMAKE_INSTALL_LIBDIR="$SYSROOT"/usr/lib -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX
 CC=$CC CXX=$CXX make -j$(nproc)
 CC=$CC CXX=$CXX make install
popd

# srs-librtmp
 git clone --depth 1 --branch 3.0release https://github.com/ossrs/srs.git
pushd ./srs/trunk
 rm -f ./srs-librtmp/objs/lib/srs_librtmp.a
 rm -rf ./srs-librtmp
 rm -f "$SYSROOT"/usr/lib/libsrs_librtmp.a


 CC=$CC CXX=$CXX AR=$AR RANLIB=$RANLIB CFLAGS="$CFLAGS" CXXFLAGS="--sysroot="$SYSROOT" -I"$SYSROOT"/usr/include -fPIC" ./configure  \
                                                                                                --prefix=""$SYSROOT"/usr" \
                                                                                                --with-librtmp \
                                                                                                --without-ssl \
                                                                                                --export-librtmp-project=./srs-librtmp \
                                                                                                --prefix=""$SYSROOT"/usr" \
                                                                                                --cc=$CC \
                                                                                                --cxx=$CXX

popd
pushd ./srs/trunk/srs-librtmp
 sed -i '/Building the srs-librtmp example/,+1d' Makefile
 CC=$CC CXX=$CXX AR=$AR RANLIB=$RANLIB CFLAGS="$CFLAGS" make
 cp ./objs/lib/srs_librtmp.a "$SYSROOT"/usr/lib
 cp -r ./objs/include/* "$SYSROOT"/usr/include
pushd "$SYSROOT"/usr/lib
 mv srs_librtmp.a libsrs_librtmp.a
readelf -h libsrs_librtmp.a  | grep 'Class\|File\|Machine' | head
popd
popd
 chmod -R 777 /srs


popd

pushd ./orkbasecxx
 make distclean
 cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
export CFLAGS="--sysroot=$SYSROOT"
export LDFLAGS+="--sysroot=$SYSROOT"
export CXXFLAGS="-std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1"
 autoconf=$BUILDROOT_SDK/bin/autoconf aclocal="$BUILDROOT_SDK/bin/aclocal" automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
 automake=$BUILDROOT_SDK/bin/automake aclocal="$BUILDROOT_SDK/bin/aclocal" LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake
 automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"
 automake=$BUILDROOT_SDK/bin/automake aclocal="$BUILDROOT_SDK/bin/aclocal" LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/autoconf
 env PATH="$PATH" aclocal="$BUILDROOT_SDK/bin/aclocal" LDFLAGS="-L$SYSROOT/usr/lib --sysroot=$SYSROOT" CXXFLAGS=$CXXFLAGS CFLAGS="--sysroot=$SYSROOT" CC=$CC CXX=$CXX autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL ./configure SYSROOT=$SYSROOT  \
    --host=arm-buildroot-linux-gnueabihf \
    --build=x86_64-linux-gnu \
    --prefix="$SYSROOT" \
    --libdir="$SYSROOT/usr/lib" \
    --bindir="$SYSROOT/usr/bin"
 env PATH="$PATH" aclocal="$BUILDROOT_SDK/bin/aclocal" CC=$CC CXX=$CXX make -j$(nproc)
 env PATH="$PATH" aclocal="$BUILDROOT_SDK/bin/aclocal" CC=$CC CXX=$CXX make install
popd

pushd ./orkaudio
 make distclean
export CFLAGS="--sysroot=$SYSROOT"
export LIBTOOL=$LIBTOOL
export ACLOCAL_PATH="/home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/aclocal-1.15:$ACLOCAL_PATH"

mkdir -p /home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/automake-1.15/Automake
cp /home/"$VARIABLE"/Documents/arm-buildroot-linux-gnueabihf_sdk-buildroot-STABLE/share/automake-1.15/Automake/Config.pm    /home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/automake-1.15/Automake/
export PERL5LIB="/home/$VARIABLE/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/autoconf:$PERL5LIB"sed -i 's/AM_PROG_LIBTOOL/LT_INIT/g' configure.ac
export CXXFLAGS="-I$SYSROOT/usr/include -std=c++17"

autoconf=$BUILDROOT_SDK/bin/autoconf PERL5LIB=$PERL5LIB automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake
automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"
automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/autoconf
automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/automake --add-missing
automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB CC=$CC CXX=$CXX autom4te=$autom4te LIBTOOLIZE=$BUILDROOT_SDK/bin/libtoolize m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoreconf -fvi

 env PATH="$PATH" ACLOCAL_PATH=$ACLOCAL_PATH PERL5LIB=$PERL5LIB autom4te=$autom4te LDFLAGS="-L$SYSROOT/usr/lib  --sysroot=$SYSROOT" CXXFLAGS=$CXXFLAGS CFLAGS="--sysroot=$SYSROOT"  CC=$CC CXX=$CXX autom4te=$autom4te m4="$BUILDROOT_SDK/bin/m4" LIBTOOL=$LIBTOOL ./configure SYSROOT="$SYSROOT" \
    --host=arm-buildroot-linux-gnueabihf \
    --build=x86_64-linux-gnu \
    --prefix="$SYSROOT" \
    --libdir="$SYSROOT/usr/lib" \
    --bindir="$SYSROOT/usr/bin"

 env PATH="$PATH" autom4te=$autom4te ACLOCAL_PATH=$ACLOCAL_PATH PERL5LIB=$PERL5LIB m4="$BUILDROOT_SDK/bin/m4" make -j$(nproc)
 CC=$CC CXX=$CXX env PATH="$PATH" make install

sudo setcap cap_net_raw,cap_net_admin+ep /usr/sbin/orkaudio
popd

