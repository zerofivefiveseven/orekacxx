#!/bin/bash

#sudo apt-get install -y libspeex-dev build-essential libtool automake git cmake  libpcap-dev libapr1-dev libopus-dev  libdw-dev libunwind-dev libssl-dev libsndfile1-dev libxerces-c3-dev libssl-dev

if [ ! -d ./arm-buildroot-linux-gnueabihf_sdk-buildroot ]; then
    echo "C++ toolchain not found"
    exit 1
fi
mkdir ./dependencies
pushd ./dependencies
echo "user directory $(../whoami.sh)"
DEFAULTVALUE=$(../whoami.sh)
VARIABLE="${1:-$DEFAULTVALUE}"

# Set Buildroot SDK path
export BUILDROOT_SDK="/home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot"

export SYSROOT="$BUILDROOT_SDK/arm-buildroot-linux-gnueabihf/sysroot"               #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#BUILDROOT_SDK="/home/revyakin/oreka/arm-buildroot-linux-gnueabihf_sdk-buildroot"
export PATH="$BUILDROOT_SDK/bin:$BUILDROOT_SDK/bin:$PATH"
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
export LDFLAGS=" -L"$SYSROOT"/usr/lib -Wl,-rpath="$SYSROOT"/usr/lib --sysroot=$SYSROOT"
export CXXFLAGS="--sysroot="$SYSROOT" -D_GLIBCXX_USE_CXX11_ABI=1 -fPIC"
export LIBTOOL=$BUILDROOT_SDK/bin/libtool
export pkgconf=$BUILDROOT_SDK/bin/pkg-config
#CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=1" LDFLAGS="-L/usr/local/lib -Wl,-rpath=/usr/local/lib -lorkbase -llibboost -lxerces-c -lsndfile -lspeex -lapr-1 -lssl -lcrypto -llog4cxx"
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
# Include necessary paths for pkg-config if needed

export PKG_CONFIG_PATH=""$SYSROOT"/usr/lib/pkgconfig:$PKG_CONFIG_PATH"

# BCG729 installation
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
sudo CC="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-gcc" CXX="$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf-g++" CXXFLAGS="--sysroot="$SYSROOT" -D_GLIBCXX_USE_CXX11_ABI=1 -fPIC" CC=$CC CXX=$CXX cmake . \
                                                                                               -DCMAKE_INSTALL_PREFIX="$SYSROOT"/usr \
                                                                                               -DCMAKE_INSTALL_LIBDIR=""$SYSROOT"/usr/lib" \
                                                                                               -DCMAKE_C_COMPILER=$CC \
                                                                                               -DCMAKE_CXX_COMPILER=$CXX \
                                                                                               -DCMAKE_C_FLAGS="--sysroot="$SYSROOT" -fPIC" \
                                                                                               -DCMAKE_CXX_FLAGS="--sysroot="$SYSROOT" -fPIC"
sudo make -j$(nproc)
sudo make install
popd


#/home/revyakin/oreka/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/bin/ranlib

# Установка библиотек в Buildroot
#make -C /home/revyakin/projects/buildroot-dir/ h828_defconfig
#make -C /home/revyakin/projects/buildroot-dir/ sdk
# Установка переменных окружения для сборки


#libace-dev libboost-dev libboost-all-dev
#libsndfile1-dev
#liblog4cxx-dev
#libxerces-c3-dev libssl-dev
#libcap-dev
#
#libspeex-dev
#libdw-dev libunwind-dev libbackward-cpp-dev
#
#
#


#wget https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
#tar -xvzf boost_1_87_0.tar.gz
#rm boost_1_87_0.tar.gz
#cd boost_1_87_0
#./bootstrap.sh --with-libraries=system, thread, date_time, regex, serialization, asio, atomic, multi_index, bind
#sudo ./b2 --with-system --with-thread --with-date_time --with-regex --with-serialization  --with-asio --with-atomic  --with-multi_index --with-bind install

#find . -name "*.am" -exec sed -i 's/-std=c++11/-std=c++17/g' {} \;

#opus
#sudo mkdir -p /opt/opus  sudo chmod 644 /opt/opus
#sudo git clone  https://github.com/xiph/opus.git /opt/opus
#sudo pushd /opt/opus
#sudo git checkout v1.2.1
#sudo ./autogen.sh
#sudo make distclean
#sudo CC=$CC CXX=$CXX ./configure --host=arm-buildroot-linux-gnueabihf --build=x86_64-linux-gnu --prefix=""$SYSROOT"/usr" --enable-shared --with-pic --enable-static
#sudo make
#sudo make install
#sudo ln -s /usr/local/lib/libopus.so "$SYSROOT"/usr/lib/libopusstatic.so
#sudo ln -s /usr/include/opus "$SYSROOT"/usr/include/
#popd

#silk
mkdir -p silk
git clone --depth 1 https://github.com/gaozehua/SILKCodec.git ./silk
pushd ./silk/SILK_SDK_SRC_ARM/
sudo make clean
sudo DCMAKE_INSTALL_PREFIX=""$SYSROOT"/usr" TOOLCHAIN_PREFIX=$BUILDROOT_SDK/bin/arm-buildroot-linux-gnueabihf- CC=$CC CXX=$CXX CFLAGS="--sysroot="$SYSROOT" -fPIC -I"$SYSROOT"/usr/include/ " make all TARGRT_CPU=armv7l
cp libSKP_SILK_SDK.a "$SYSROOT"/usr/lib
cp encoder "$SYSROOT"/usr/lib
cp decoder "$SYSROOT"/usr/lib
popd

git clone https://github.com/OrecX/dependencies.git
sudo tar -xf dependencies/opus-1.2.1.tar.gz
pushd opus-1.2.1
sudo ./autogen.sh
sudo  CC=$CC CXX=$CXX ./configure --host=arm-buildroot-linux-gnueabihf --build=x86_64-linux-gnu --prefix=""$SYSROOT"/usr" --libdir=""$SYSROOT"/usr/lib" --bindir=""$SYSROOT"/usr/bin" --enable-shared --with-pic --enable-static
sudo  CC=$CC CXX=$CXX make -j$(nproc) CFLAGS="-fPIC"
sudo  CC=$CC CXX=$CXX make install
#$BUILDROOT_SDK/bin/libtool --finish "$SYSROOT"/usr/lib/opus/lib
sudo cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
sudo cp "$SYSROOT"/usr/lib/opus/lib/libopus.a "$SYSROOT"/usr/lib/opus/lib/libopusstatic.a
popd
readelf -h "$SYSROOT"/usr/lib/libopusstatic.a  | grep 'Class\|File\|Machine'



# backward-cpp HEADER ONLY
git clone --depth 1 https://github.com/bombela/backward-cpp.git
sudo ln -s ./backward-cpp/backward.hpp "$SYSROOT"/usr/include/backward.hpp

# httplib HEADER ONLY
mkdir -p "$SYSROOT"/usr/include/httplib
git clone --depth 1 --branch v0.12.3 https://github.com/yhirose/cpp-httplib.git
sudo cp ./httplib/httplib.h "$SYSROOT"/usr/include/httplib/

# json
git clone --depth 1 https://github.com/nlohmann/json.git
pushd ./json
sudo make distclean
sudo CC=$CC CXX=$CXX cmake . -DCMAKE_INSTALL_PREFIX="$SYSROOT"/usr -DCMAKE_INSTALL_LIBDIR="$SYSROOT"/usr/lib -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX
sudo CC=$CC CXX=$CXX make -j$(nproc)
sudo CC=$CC CXX=$CXX make install
popd

# srs-librtmp
sudo git clone --depth 1 --branch 3.0release https://github.com/ossrs/srs.git

pushd ./srs/trunk
sudo rm -f ./srs-librtmp/objs/lib/srs_librtmp.a
sudo rm -rf ./srs-librtmp
sudo rm -f "$SYSROOT"/usr/lib/libsrs_librtmp.a


sudo CC=$CC CXX=$CXX AR=$AR RANLIB=$RANLIB CFLAGS="$CFLAGS" CXXFLAGS="--sysroot="$SYSROOT" -I"$SYSROOT"/usr/include -fPIC" ./configure  \
                                                                                                --prefix=""$SYSROOT"/usr" \
                                                                                                --with-librtmp \
                                                                                                --without-ssl \
                                                                                                --export-librtmp-project=./srs-librtmp \
                                                                                                --prefix=""$SYSROOT"/usr" \
                                                                                                --cc=$CC \
                                                                                                --cxx=$CXX

popd
pushd ./srs/trunk/srs-librtmp
sudo sed -i '/Building the srs-librtmp example/,+1d' Makefile
sudo CC=$CC CXX=$CXX AR=$AR RANLIB=$RANLIB CFLAGS="$CFLAGS" make
sudo cp ./objs/lib/srs_librtmp.a "$SYSROOT"/usr/lib
sudo cp -r ./objs/include/* "$SYSROOT"/usr/include
pushd "$SYSROOT"/usr/lib
sudo mv srs_librtmp.a libsrs_librtmp.a
readelf -h libsrs_librtmp.a  | grep 'Class\|File\|Machine' | head
popd
popd
sudo chmod -R 777 /srs

sudo rm -rf logging-log4cxx
git clone https://github.com/apache/logging-log4cxx.git
pushd logging-log4cxx
mkdir build
cd build
export LDFLAGS="--sysroot="$SYSROOT" -L"$SYSROOT"/usr/lib -Wl,-rpath="$SYSROOT"/usr/lib"
export CXXFLAGS="--sysroot="$SYSROOT" -D_GLIBCXX_USE_CXX11_ABI=1 -fPIC --enable-debug"
sudo make distclean
CC=$CC CXX=$CXX CXXFLAGS=$CXXFLAGS cmake .. \
  -DCMAKE_INSTALL_PREFIX=""$SYSROOT"/usr" \
  -DCMAKE_INSTALL_LIBDIR=""$SYSROOT"/usr/lib" \
  -DCMAKE_CXX_STANDARD=11 \
  -D_GLIBCXX_USE_CXX11_ABI=1 \
  -DCMAKE_C_COMPILER=$CC \
  -DCMAKE_CXX_COMPILER=$CXX
sudo  CC=$CC CXX=$CXX make -j$(nproc)
sudo  CC=$CC CXX=$CXX make install
popd


#from deps to main
popd

#find /home/revyakin/orekacxx/orkbasecxx/ -type f -name "*.am" -exec sed -i 's|$$(SYSROOT)|/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot|g' {} +
pushd ./orkbasecxx
sudo make distclean
sudo chmod -R 777 $BUILDROOT_SDK
sudo chmod -R 777 ../orkbasecxx/
sudo chmod -R 777 ../dependencies/
sudo cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
export PATH=/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin:$PATH
#-Wl,-rpath="$SYSROOT"/usr/lib
#-L$SYSROOT/usr/lib
export CFLAGS="--sysroot=$SYSROOT"
export LDFLAGS+="--sysroot=$SYSROOT"
export CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=1 -fPIC"
sudo autoconf=$BUILDROOT_SDK/bin/autoconf automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
sudo automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake
sudo automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"
sudo automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/autoconf
automake=$BUILDROOT_SDK/bin/automake CC=$CC CXX=$CXX autom4te=$autom4te LIBTOOLIZE=$BUILDROOT_SDK/bin/libtoolize m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoreconf  -fvi
sudo env PATH="$PATH" LDFLAGS="-L$SYSROOT/usr/lib --sysroot=$SYSROOT" CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=1 -fPIC" CFLAGS="--sysroot=$SYSROOT -fPIC"  CC=$CC CXX=$CXX autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL ./configure SYSROOT=$SYSROOT  \
    --host=arm-buildroot-linux-gnueabihf \
    --build=x86_64-linux-gnu \
    --prefix="$SYSROOT" \
    --libdir=""$SYSROOT"/usr/lib" \
    --bindir=""$SYSROOT"/usr/bin"
sudo env PATH="$PATH" CC=$CC CXX=$CXX make -j$(nproc)
sudo env PATH="$PATH" CC=$CC CXX=$CXX make install
popd
#LT_INIT

#pushd ./orkaudio
##find . -name "*.am" -exec sed -i 's/-std=c++11/-std=c++17/g' {} \;
#
##sudo make clean
#sudo CC=$CC CXX=$CXX LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoreconf  --force --install
#sudo make distclean
##aclocal
##autoconf
##automake --add-missing
#
#
##LIBS=""
#sudo mv configure.in configure.ac
#sudo chmod 777 ./configure.ac
#sed -i 's/AM_PROG_LIBTOOL/LT_INIT/g' configure.ac
#
#automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
#automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake
#sudo automake=$BUILDROOT_SDK/bin/automake --add-missing --copy
#automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"
#
#automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
#
#sudo automake=$BUILDROOT_SDK/bin/automake CC=$CC CXX=$CXX autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL ./configure --host=arm-buildroot-linux-gnueabihf --build=x86_64-linux-gnu --prefix=""$SYSROOT"/usr" --libdir=""$SYSROOT"/usr/lib" --bindir=""$SYSROOT"/usr/bin"
##LDFLAGS="-L/usr/local/lib -Wl,-rpath=/usr/local/lib"
#sudo CC=$CC CXX=$CXX make -j$(аnproc)
#sudo CC=$CC CXX=$CXX make install
#sudo setcap cap_net_raw,cap_net_admin+ep /usr/sbin/orkaudio
#
#popd

pushd ./orkaudio
sudo chmod -R 777 ./orkaudio/
sudo chmod -R 777 ../dependencies/
# Update obsolete macros in configure.ac
sudo make distclean
export CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=1 -fPIC"
export CFLAGS="--sysroot=$SYSROOT"
export LIBTOOL=$LIBTOOL
sed -i 's/AM_PROG_LIBTOOL/LT_INIT/g' configure.ac
export automake_1.15=$BUILDROOT_SDK/bin/automake-1.15
# Run autotools commands to regenerate build files
autoconf=$BUILDROOT_SDK/bin/autoconf automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake
automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"
automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/autoconf
automake=$BUILDROOT_SDK/bin/automake CC=$CC CXX=$CXX autom4te=$autom4te LIBTOOLIZE=$BUILDROOT_SDK/bin/libtoolize m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoreconf  -fvi
# Run configure script
#--isysroot="$SYSROOT"
sudo env PATH="$PATH" LDFLAGS="-L$SYSROOT/usr/lib" CXXFLAGS="--sysroot=$SYSROOT  -D_GLIBCXX_USE_CXX11_ABI=1 -fPIC" CFLAGS="--sysroot=$SYSROOT -fPIC"  CC=$CC CXX=$CXX autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL ./configure SYSROOT="$SYSROOT" \
    --host=arm-buildroot-linux-gnueabihf \
    --build=x86_64-linux-gnu \
    --prefix="$SYSROOT" \
    --libdir=""$SYSROOT"/usr/lib" \
    --bindir=""$SYSROOT"/usr/bin"
#sudo env PATH="$PATH" automake-1.15 --add-missing
# Build and install the project
sudo env PATH="$PATH" make -j$(nproc)
sudo CC=$CC CXX=$CXX env PATH="$PATH" make install

# Set capabilities for orkaudio (if needed)
sudo setcap cap_net_raw,cap_net_admin+ep /usr/sbin/orkaudio
popd

#"$SYSROOT"/usr/lib/libuuid.so
