#!/bin/sh
cd `dirname $0`
TOP=`pwd`
TOOLCHAIN_PATH=${TOP}/arm-linux-androideabi-4.6/

if ! [ -d $TOOLCHAIN_PATH ]; then
	tar jxf arm-linux-androideabi-4.6.tar.bz2 || exit 1
fi

CROSS=arm-linux-androideabi-
export PATH=${TOOLCHAIN_PATH}/bin/:$PATH
export CC="${CROSS}gcc"
export CXX="${CROSS}g++"
export AR="${AR}ar"
make clean
make
make install
