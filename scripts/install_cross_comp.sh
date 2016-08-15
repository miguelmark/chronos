#!/bin/bash

# The export commands will not work if you do not
# excute this script in the currect shell context. To
# do this you can use the source command or .

function get_filename_from_url {
   url_no_proto=$(echo $1 | sed 's/http(s)?:\/\/|ftp:\/\///')
   echo $(echo $1 | awk '{n=split($0,a,"/"); print a[n]}')
}

function has_ext {
  ext=${1##*.}
  test "$ext" = "$2"
}

function get_ext {
  ext=${1##*.}
  echo $ext
}

function strip_ext {
  echo ${1%.*}
}

function strip_known_ext {
  echo $(echo $1 | sed 's/.tar\|.gz\|.xz//g')
}

function get_dir_from_url {
  filename=$(get_filename_from_url $1)
  echo $(strip_known_ext $filename)
}


function un_compress {
  ext=$(get_ext $1)
  if [ "$ext" = gz ]; then
     tar xvfz $1
  elif [ "$ext" = xz ]; then
    tar xvfJ $1
  elif [[ "ext" == *"bz"* ]]; then
    tar xvfj $1
  else
    echo "Extension $ext is not supported"
    exit 1
  fi
  #echo $(strip_ext $(strip_ext $1))
}

function download_to_dir {
  filename=$(get_filename_from_url $1)
  wget -O $filename $1
  un_compress $filename
  rm -f $filename
}

function package_is_installed {
  # assumes you have dpkg installed :)
  # if not use an alternative method
  text=$(dpkg -s $1)
}

BINUTILS_URL=ftp://ftp.gnu.org/gnu/binutils/binutils-2.26.tar.gz
GCC_URL=ftp://ftp.gnu.org/gnu/gcc/gcc-5.4.0/gcc-5.4.0.tar.gz
GMP_URL=https://ftp.gnu.org/gnu/gmp/gmp-6.1.1.tar.xz
MPFR_URL=http://www.mpfr.org/mpfr-current/mpfr-3.1.4.tar.xz
ISL_URL=http://isl.gforge.inria.fr/isl-0.17.1.tar.gz
CLOOG_URL=http://www.bastoul.net/cloog/pages/download/count.php3?url=./cloog-0.18.4.tar.gz
MPC_URL=ftp://ftp.gnu.org/gnu/mpc/mpc-1.0.2.tar.gz
TEX_INFO_URL=http://ftp.gnu.org/gnu/texinfo/texinfo-6.1.tar.gz

BINUTILS_DIR=$(get_dir_from_url $BINUTILS_URL)
GCC_DIR=$(get_dir_from_url $GCC_URL)
GMP_DIR=$(get_dir_from_url $GMP_URL)
MPFR_DIR=$(get_dir_from_url $MPFR_URL)
ISL_DIR=$(get_dir_from_url $ISL_URL)
CLOOG_DIR=$(get_dir_from_url $CLOOG_URL)
MPC_DIR=$(get_dir_from_url $MPC_URL)
TEX_INFO_DIR=$(get_dir_from_url $TEX_INFO_URL)

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

echo "*** Downloading needed source files.... ****"
download_to_dir $BINUTILS_URL
download_to_dir $GCC_URL
download_to_dir $GMP_URL 
download_to_dir $MPFR_URL
download_to_dir $ISL_URL
download_to_dir $CLOOG_URL
download_to_dir $MPC_URL
download_to_dir $TEX_INFO_URL

GCC_MULTILIB=gcc-multilib
GPP=g++
echo "*** Installing prerequisites ***"

if package_is_installed $GCC_MULTILIB; then
  echo "$GCC_MULTILIB is already installed. Skipping...."
else
  echo "Installing ${GCC_MULTILIB}...."
  sudo apt-get install $GCC_MULTILIB
fi

if package_is_installed $GPP; then
  echo "$GPP is already installed. Skipping...."
else
  echo "Installing ${GPP}...."
  sudo apt-get install $GPP
fi


echo "**** Preparing for build step ***"
mv $GMP_DIR ${GCC_DIR}/gmp
mv $MPFR_DIR ${GCC_DIR}/mpfr
mv $MPC_DIR ${GCC_DIR}/mpc
mv $ISL_DIR ${GCC_DIR}/isl
mv $CLOOG_DIR ${GCC_DIR}/cloog

mv $TEX_INFO_DIR ${BINUTILS_DIR}/texinfo

BINUTILS_BUILD_DIR=build-binutils
echo "*** Building binutils ***"
mkdir $BINUTILS_BUILD_DIR
cd $BINUTILS_BUILD_DIR
../${BINUTILS_DIR}/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ..

GCC_BUILD_DIR=build-gcc
echo "*** Building GCC ***"
mkdir $GCC_BUILD_DIR
cd $GCC_BUILD_DIR
../${GCC_DIR}/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..

echo "**** All done! ***"

