#!/bin/bash

cd

# clean packages cache
yum -y clean all
yum -y clean expire-cache
yum -y update

# install base build system
yum -y install epel-release
yum -y install xz cmake git subversion tar gzip make wget ccache

# install GCC 4.8.2 as Tulip requires a C++11 compiler
wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
yum -y upgrade
yum -y install devtoolset-2-gcc devtoolset-2-binutils devtoolset-2-gcc-c++

# install tulip deps
yum -y install zlib-devel qhull-devel
yum -y install freetype-devel libpng-devel libjpeg-devel glew-devel

# needed for generating the appimage
yum -y install fuse fuse-libs

if [ "$(uname -p)" = "x86_64" ]; then

# install python 2.7 and the corresponding sphinx version
yum install -y centos-release-scl
yum install -y python27-python-devel python27-python-sphinx
# setup python
source /opt/rh/python27/enable
# set USR_LIB (needed to set CMAKE_PREFIX_PATH)
USR_LIB=/usr/lib64

else

# download, compile and install Python 2.7 because there is
# no 32 bit version of centos-release-scl
yum install -y zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel readline-devel tk-devel gdbm-devel db4-devel libpcap-devel xz-devel expat-devel
wget http://python.org/ftp/python/2.7.14/Python-2.7.14.tar.xz
tar xf Python-2.7.14.tar.xz
cd Python-2.7.14
./configure --prefix=/usr --enable-unicode=ucs4 --enable-shared --enable-optimizations CC="ccache /opt/rh/devtoolset-2/root/usr/bin/gcc" CXX="ccache /opt/rh/devtoolset-2/root/usr/bin/g++"
make -j4 && make altinstall

# install pip in order to grab the latest sphinx version
wget https://bootstrap.pypa.io/get-pip.py
python2.7 get-pip.py
pip2.7 install sphinx

# set USR_LIB (needed to set CMAKE_PREFIX_PATH)
USR_LIB=/usr/lib

fi

# install qt5
yum install -y qt5-qtbase-devel qt5-qtwebkit-devel

# build and install tulip
if [ -d /tulip/build ]; then
  rm -rf /tulip/build
fi
mkdir /tulip/build
cd /tulip/build
cmake -DCMAKE_C_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/gcc -DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DCMAKE_PREFIX_PATH=$USR_LIB/qt5 -DTULIP_USE_CCACHE=ON -DTULIP_BUILD_FOR_APPIMAGE=ON ..
make -j4 install

# build a bundle dir suitable for AppImageKit
sh bundlers/linux/make_appimage_bundle.sh --appdir $PWD

# get appimagetool
wget "https://github.com/probonopd/AppImageKit/releases/download/continuous/appimagetool-$(uname -p).AppImage"
chmod a+x appimagetool-$(uname -p).AppImage

# finally build the portable app
./appimagetool-$(uname -p).AppImage Tulip.AppDir Tulip-$(sh tulip-config --version)-$(uname -p).AppImage
