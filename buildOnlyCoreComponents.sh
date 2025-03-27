echo "user directory $(./whoami.sh)"
DEFAULTVALUE=$(./whoami.sh)
VARIABLE="${1:-$DEFAULTVALUE}"

# Set Buildroot SDK path
export BUILDROOT_SDK="/home/"$VARIABLE"/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot"

export SYSROOT="$BUILDROOT_SDK/arm-buildroot-linux-gnueabihf/sysroot"               #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#BUILDROOT_SDK="/home/revyakin/oreka/arm-buildroot-linux-gnueabihf_sdk-buildroot"
export PATH="$BUILDROOT_SDK/bin:$BUILDROOT_SDK/bin:$PATH"
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
export LDFLAGS=" -L"$SYSROOT"/usr/lib -Wl,-rpath="$SYSROOT"/usr/lib --sysroot=$SYSROOT"
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
#export CXXFLAGS="-fPIC -std=c++17 -O0  -D_GLIBCXX_USE_CXX11_ABI=1-D_REENTRANT  --sysroot=$SYSROOT"
export CXXFLAGS="-std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1"
sudo autoconf=$BUILDROOT_SDK/bin/autoconf aclocal="$BUILDROOT_SDK/bin/aclocal" automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate
sudo automake=$BUILDROOT_SDK/bin/automake aclocal="$BUILDROOT_SDK/bin/aclocal" LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake
sudo automake=$BUILDROOT_SDK/bin/automake LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"
sudo automake=$BUILDROOT_SDK/bin/automake aclocal="$BUILDROOT_SDK/bin/aclocal" LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/autoconf
automake=$BUILDROOT_SDK/bin/automake aclocal="$BUILDROOT_SDK/bin/aclocal" CC=$CC CXX=$CXX autom4te=$autom4te LIBTOOLIZE=$BUILDROOT_SDK/bin/libtoolize m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoreconf  -fvi
sudo env PATH="$PATH" aclocal="$BUILDROOT_SDK/bin/aclocal" LDFLAGS="-L$SYSROOT/usr/lib --sysroot=$SYSROOT" CXXFLAGS=$CXXFLAGS CFLAGS="--sysroot=$SYSROOT" CC=$CC CXX=$CXX autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL ./configure SYSROOT=$SYSROOT  \
    --host=arm-buildroot-linux-gnueabihf \
    --build=x86_64-linux-gnu \
    --prefix="$SYSROOT" \
    --libdir="$SYSROOT/usr/lib" \
    --bindir="$SYSROOT/usr/bin"
sudo env PATH="$PATH" aclocal="$BUILDROOT_SDK/bin/aclocal" CC=$CC CXX=$CXX make -j$(nproc)
sudo env PATH="$PATH" aclocal="$BUILDROOT_SDK/bin/aclocal" CC=$CC CXX=$CXX make install
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
sudo chmod -R 777 ../orkaudio/
sudo chmod -R 777 ../dependencies/
# Update obsolete macros in configure.ac
sudo make distclean
export CFLAGS="--sysroot=$SYSROOT"
export LIBTOOL=$LIBTOOL
export ACLOCAL_PATH="/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/aclocal-1.15:$ACLOCAL_PATH"

mkdir -p /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/automake-1.15/Automake
cp /home/revyakin/Documents/arm-buildroot-linux-gnueabihf_sdk-buildroot-STABLE/share/automake-1.15/Automake/Config.pm    /home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/automake-1.15/Automake/
export PERL5LIB="/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/autoconf:$PERL5LIB"sed -i 's/AM_PROG_LIBTOOL/LT_INIT/g' configure.ac
#export CXXFLAGS="-O0  --sysroot=$SYSROOT -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1-D_REENTRANT -fPIC"
export CXXFLAGS="-I$SYSROOT/usr/include -std=c++17"

autoconf=$BUILDROOT_SDK/bin/autoconf PERL5LIB=$PERL5LIB automake=$BUILDROOT_SDK/bin/automake autom4te=$autom4te m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoupdate

automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/libtoolize --force --copy --automake

automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/aclocal -I m4 -I /usr/share/aclocal -I "$BUILDROOT_SDK/share/aclocal/"

automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB LIBTOOL=$LIBTOOL autom4te=$autom4te m4=$m4 $BUILDROOT_SDK/bin/autoconf

automake=$BUILDROOT_SDK/bin/automake PERL5LIB=$PERL5LIB CC=$CC CXX=$CXX autom4te=$autom4te LIBTOOLIZE=$BUILDROOT_SDK/bin/libtoolize m4=$m4 LIBTOOL=$LIBTOOL $BUILDROOT_SDK/bin/autoreconf -fiv
# Run configure script

sudo env PATH="$PATH" ACLOCAL_PATH=$ACLOCAL_PATH PERL5LIB=$PERL5LIB autom4te=$autom4te LDFLAGS="-L$SYSROOT/usr/lib  --sysroot=$SYSROOT" CXXFLAGS=$CXXFLAGS CFLAGS="--sysroot=$SYSROOT"  CC=$CC CXX=$CXX autom4te=$autom4te m4="$BUILDROOT_SDK/bin/m4" LIBTOOL=$LIBTOOL ./configure SYSROOT="$SYSROOT" \
    --host=arm-buildroot-linux-gnueabihf \
    --build=x86_64-linux-gnu \
    --prefix="$SYSROOT" \
    --libdir="$SYSROOT/usr/lib" \
    --bindir="$SYSROOT/usr/bin"
    #    --prefix="$SYSROOT" \
#sudo env PATH="$PATH" automake-1.15 --add-missing
# Build and install the project
sudo env PATH="$PATH" autom4te=$autom4te ACLOCAL_PATH=$ACLOCAL_PATH PERL5LIB=$PERL5LIB m4="$BUILDROOT_SDK/bin/m4" make -j$(nproc)
sudo CC=$CC CXX=$CXX env PATH="$PATH" make install

# Set capabilities for orkaudio (if needed)
sudo setcap cap_net_raw,cap_net_admin+ep /usr/sbin/orkaudio
popd
