echo "user directory $(./whoami.sh)"
DEFAULTVALUE=$(./whoami.sh)
VARIABLE="${1:-$DEFAULTVALUE}"
unset BUILDROOT_SDK PERL5LIB ACLOCAL_PATH CXXFLAGS CFLAGS
unset BUILDROOT_SDK SYSROOT PATH PKG_CONFIG_PATH LDFLAGS LIBTOOL pkgconf PATH CC CXX AR LD PATH automake AUTOMAKE autom4te m4 LIBTOOLIZE PATH PKG_CONFIG_PATH
#if [ ! -d ./arm-buildroot-linux-gnueabihf_sdk-buildroot ]; then
#    echo "C++ toolchain not found"
#    exit 1
#fi
chmod -R 777 ./
echo "user directory $(./whoami.sh)"
DEFAULTVALUE=$(./whoami.sh)
VARIABLE="${1:-$DEFAULTVALUE}"
export SYSROOT=""
export PATH="/usr/lib:/usr/bin"
export CC="/usr/bin/gcc"
export CXX="/usr/bin/g++"
export BUILDROOT_SDK="/usr"
export LD_PATH="/usr/lib"
export LDFLAGS="-L/usr/lib -Wl,-rpath=/usr/lib"
echo "Buildroot SDK environment set up:"
echo "PATH: $PATH"
echo "CC: $CC"
echo "CXX: $CXX"
echo "LD: $LD"
echo "LIBRARY_PATH: $LIBRARY_PATH"


cd ./orkbasecxx
find . -name "*.la" -type f -delete
unset BUILDROOT_SDK
export SYSROOT=""
export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
export CC="/usr/bin/gcc"
export CXX="/usr/bin/g++"
export BUILDROOT_SDK="/usr"
export LD_PATH="/usr/lib"
export LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/usr/lib"
export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu"

 make distclean
sudo cp "$SYSROOT"/usr/lib/libopus.a "$SYSROOT"/usr/lib/libopusstatic.a
export CXXFLAGS="-std=c++17 -D_GLIBCXX_USE_CXX11_ABI=1"
 autoupdate
 automake --add-missing
 autoreconf -fvi
 LDFLAGS="-L$SYSROOT/usr/lib" CXXFLAGS=$CXXFLAGS CFLAGS="" CC=$CC CXX=$CXX ././configure LDFLAGS="-L/usr/lib" --build=x86_64-linux-gnu --host=x86_64-linux-gnu
 CC=$CC CXX=$CXX make -j$(nproc)
sudo env PATH="$PATH" aclocal="/bin/aclocal" CC=$CC CXX=$CXX make install
cd ../

cd ./orkaudio
find . -name "*.la" -type f -delete
make clean
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

# export PERL5LIB="/home/$VARIABLE/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/share/autoconf:$PERL5LIB"sed -i 's/AM_PROG_LIBTOOL/LT_INIT/g' configure.ac
export CXXFLAGS="-I/usr/include -std=c++17"
libtoolize --force --copy --automake

autoupdate
aclocal -I m4 -I /usr/share/aclocal -I "/share/aclocal/"
autoconf
automake --add-missing
autoreconf -fvi

LDFLAGS="-L$SYSROOT/usr/lib" CXXFLAGS=$CXXFLAGS CFLAGS=""  CC=$CC CXX=$CXX ./configure LDFLAGS="-L/usr/lib"  SYSROOT="" --build=x86_64-linux-gnu --host=x86_64-linux-gnu
make -j$(nproc)
sudo CC=$CC CXX=$CXX env PATH="$PATH" make install

setcap cap_net_raw,cap_net_admin+ep /usr/sbin/orkaudio
cd ../