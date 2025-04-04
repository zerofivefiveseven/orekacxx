#!/bin/bash
sudo apt-get install -y libspeex-dev build-essential libtool automake git cmake  libpcap-dev libapr1-dev libopus-dev  libdw-dev libunwind-dev libssl-dev libsndfile1-dev libxerces-c3-dev libssl-dev libapr1 libapr1-dev
unset BUILDROOT_SDK PERL5LIB ACLOCAL_PATH CXXFLAGS CFLAGS
unset BUILDROOT_SDK SYSROOT PATH PKG_CONFIG_PATH LDFLAGS LIBTOOL pkgconf PATH CC CXX AR LD PATH automake AUTOMAKE autom4te m4 LIBTOOLIZE PATH PKG_CONFIG_PATH
#if [ ! -d ./arm-buildroot-linux-gnueabihf_sdk-buildroot ]; then
#    echo "C++ toolchain not found"
#    exit 1
#fi
mkdir ./dependencies
pushd ./dependencies
echo "user directory $(../whoami.sh)"
DEFAULTVALUE=$(../whoami.sh)
VARIABLE="${1:-$DEFAULTVALUE}"
export SYSROOT=""
export PATH="/usr/lib:/usr/bin"
export CC="/usr/bin/gcc"
export CXX="/usr/bin/g++"
export BUILDROOT_SDK="/usr"
export LD_PATH="/usr/lib"
export LDFLAGS="-L/usr/lib -Wl,-rpath=/usr/lib"

#export SYSROOT="/arm-buildroot-linux-gnueabihf/sysroot"
#export PATH="/bin:/bin:$PATH"
#export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
#export LDFLAGS=" -L"$SYSROOT"/usr/lib -Wl,-rpath="$SYSROOT"/usr/lib --sysroot=$SYSROOT"
#export LIBTOOL=/bin/libtool
#export pkgconf=/bin/pkg-config
#export PATH="/bin:$PATH"
#export CC="/bin/arm-buildroot-linux-gnueabihf-gcc"
#export CXX="/bin/arm-buildroot-linux-gnueabihf-g++"
#export AR="/bin/arm-buildroot-linux-gnueabihf-ar"
#export LD="/bin/arm-buildroot-linux-gnueabihf-ld"
#export PATH="/bin:/bin:$PATH"
#export automake="/bin/automake-1.16"
#export AUTOMAKE="/bin/automake"
#export autom4te="/bin/autom4te"
#export m4="/bin/m4"
#export LIBTOOLIZE="/bin/libtoolize"
#export PATH=/bin:$PATH
#export PKG_CONFIG_PATH=""$SYSROOT"/usr/lib/pkgconfig:$PKG_CONFIG_PATH"



echo "Buildroot SDK environment set up:"
echo "PATH: $PATH"
echo "CC: $CC"
echo "CXX: $CXX"
echo "LD: $LD"
echo "LIBRARY_PATH: $LIBRARY_PATH"
# Verify if the compiler exists
#if [ ! -f "$CXX" ]; then
#    echo "C++ compiler not found at $CXX"
#    exit 1
#fi
rm -rf ./bcg729
git clone --depth 1 https://github.com/BelledonneCommunications/bcg729.git ./bcg729
pushd ./bcg729
CXXFLAGS=" -D_GLIBCXX_USE_CXX11_ABI=1" cmake . \
                                                                                               -DCMAKE_C_COMPILER=$CC \
                                                                                               -DCMAKE_CXX_COMPILER=$CXX \
                                                                                               -DCMAKE_C_FLAGS="--sysroot=$SYSROOT -fPIC"
 make -j$(nproc)
sudo make install

popd

rm -rf ./silk
mkdir -p silk
git clone --depth 1 https://github.com/gaozehua/SILKCodec.git ./silk
pushd ./silk/SILK_SDK_SRC_ARM/
 make clean
export CXXFLAGS=" -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1 -g -O0 -fPIC"
TOOLCHAIN_PREFIX=/bin/arm-buildroot-linux-gnueabihf- CC=$CC CXX=$CXX CXXFLAGS=$CXXFLAGS CFLAGS=" -fPIC -I/usr/include/ -fPIC -g -O0" make all TARGRT_CPU=armv7l
cp libSKP_SILK_SDK.a "$SYSROOT"/usr/lib
cp encoder "$SYSROOT"/usr/lib
cp decoder "$SYSROOT"/usr/lib
popd
rm -rf ./opus-1.2.1
git clone https://github.com/OrecX/dependencies.git
 tar -xf dependencies/opus-1.2.1.tar.gz
pushd opus-1.2.1
 ./autogen.sh
  CC=$CC CXX=$CXX ./configure LDFLAGS="-L/usr/lib" --build=x86_64-linux-gnu --host=x86_64-linux-gnu --enable-shared --with-pic --enable-static
  CC=$CC CXX=$CXX make -j$(nproc) CFLAGS="-fPIC"
sudo  CC=$CC CXX=$CXX make install
 cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
 cp "$SYSROOT"/usr/lib/opus/lib/libopus.a "$SYSROOT"/usr/lib/opus/lib/libopusstatic.a
popd
readelf -h "$SYSROOT"/usr/lib/libopusstatic.a  | grep 'Class\|File\|Machine'


# backward-cpp HEADER ONLY
git clone --depth 1 https://github.com/bombela/backward-cpp.git
 cp ./backward-cpp/backward.hpp /usr/include/backward.hpp

# httplib HEADER ONLY
mkdir -p "$SYSROOT"/usr/include/httplib
git clone --depth 1 --branch v0.12.3 https://github.com/yhirose/cpp-httplib.git
 cp ./cpp-httplib/httplib.h /usr/include/httplib/

rm -rf ./json
git clone --depth 1 https://github.com/nlohmann/json.git
pushd ./json
 make distclean
 CC=$CC CXX=$CXX cmake . -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX
 CC=$CC CXX=$CXX make -j$(nproc)
 sudo CC=$CC CXX=$CXX make install
popd

# srs-librtmp
rm -rf ./srs
 git clone --depth 1 --branch 3.0release https://github.com/ossrs/srs.git
pushd ./srs/trunk
 rm -f ./srs-librtmp/objs/lib/srs_librtmp.a
 rm -rf ./srs-librtmp
 rm -f "$SYSROOT"/usr/lib/libsrs_librtmp.a


 CC=$CC CXX=$CXX AR=$AR RANLIB=$RANLIB CFLAGS="$CFLAGS" CXXFLAGS="--sysroot="$SYSROOT" -I"$SYSROOT"/usr/include -fPIC" ./configure LDFLAGS="-L/usr/lib" --build=x86_64-linux-gnu --host=x86_64-linux-gnu \
                                                                                                --with-librtmp \
                                                                                                --without-ssl \
                                                                                                --export-librtmp-project=./srs-librtmp \
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
export SYSROOT=""
export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
export CC="/usr/bin/gcc"
export CXX="/usr/bin/g++"
export BUILDROOT_SDK="/usr"
export LD_PATH="/usr/lib"
export LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/usr/lib"
export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu"

 make distclean
 cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
export CXXFLAGS="-std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1"
 autoconf=/bin/autoconf aclocal="/bin/aclocal" automake=/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL /bin/autoupdate
 automake=/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 /bin/automake --add-missing
 aclocal="/bin/aclocal" LDFLAGS="-L$SYSROOT/usr/lib" CXXFLAGS=$CXXFLAGS CFLAGS="" CC=$CC CXX=$CXX autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL ././configure LDFLAGS="-L/usr/lib" SYSROOT="" --build=x86_64-linux-gnu --host=x86_64-linux-gnu
 aclocal="/bin/aclocal" CC=$CC CXX=$CXX make -j$(nproc)
sudo env PATH="$PATH" aclocal="/bin/aclocal" CC=$CC CXX=$CXX make install
popd

pushd ./orkaudio

 make distclean
export SYSROOT=""
export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
export CC="/usr/bin/gcc"
export CXX="/usr/bin/g++"
export BUILDROOT_SDK="/usr"
export LD_PATH="/usr/lib"
export LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/usr/lib"
export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu"
unset BUILDROOT_SDK PERL5LIB ACLOCAL_PATH

mkdir -p /home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/automake-1.15/Automake
cp /home/"$VARIABLE"/Documents/arm-buildroot-linux-gnueabihf_sdk-buildroot-STABLE/share/automake-1.15/Automake/Config.pm    /home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/automake-1.15/Automake/
export PERL5LIB="/home/$VARIABLE/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/autoconf:$PERL5LIB"sed -i 's/AM_PROG_LIBTOOL/LT_INIT/g' configure.ac
export CXXFLAGS="-I/usr/include -std=c++17"

autoconf=/bin/autoconf PERL5LIB=$PERL5LIB automake=/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL /bin/autoupdate
automake=/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 /bin/libtoolize --force --copy --automake
automake=/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 /bin/aclocal -I m4 -I /usr/share/aclocal -I "/share/aclocal/"
automake=/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 /bin/autoconf
automake=/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 /bin/automake --add-missing
automake=/bin/automake PERL5LIB=$PERL5LIB CC=$CC CXX=$CXX autom4te=$autom4te LIBTOOLIZE=/bin/libtoolize m4=$m4 LIBTOOL=$LIBTOOL /bin/autoreconf -fvi

 ACLOCAL_PATH=$ACLOCAL_PATH PERL5LIB=$PERL5LIB autom4te=$autom4te LDFLAGS="-L$SYSROOT/usr/lib" CXXFLAGS=$CXXFLAGS CFLAGS=""  CC=$CC CXX=$CXX autom4te=$autom4te m4="/bin/m4" LIBTOOL=$LIBTOOL ./configure LDFLAGS="-L/usr/lib"  SYSROOT="" --build=x86_64-linux-gnu --host=x86_64-linux-gnu

 autom4te=$autom4te ACLOCAL_PATH=$ACLOCAL_PATH PERL5LIB=$PERL5LIB m4="/bin/m4" make -j$(nproc)
 sudo CC=$CC CXX=$CXX env PATH="$PATH" make install

 setcap cap_net_raw,cap_net_admin+ep /usr/sbin/orkaudio
popd

