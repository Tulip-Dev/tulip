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

# needed for generating the doc
yum -y install doxygen graphviz

# needed for generating the appimage
yum -y install fuse fuse-libs

# install python 2.7 and the corresponding sphinx version
yum install -y centos-release-scl
yum install -y python27-python-devel python27-python-sphinx

# setup python
source /opt/rh/python27/enable

# install qt5
yum install -y qt5-qtbase-devel qt5-qtwebkit-devel

# build and install tulip
if [ -d /tulip/build ]; then
  rm -rf /tulip/build
fi
mkdir /tulip/build
cd /tulip/build
cmake -DCMAKE_C_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/gcc -DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DCMAKE_PREFIX_PATH=/usr/lib64/qt5 -DTULIP_USE_CCACHE=ON -DTULIP_USE_QT5=ON -DTULIP_BUILD_FOR_APPIMAGE=ON ..
make -j4 install

# build a bundle dir suitable for AppImageKit
sh bundlers/linux/make_appimage_bundle.sh --appdir $PWD

# get appimagetool
wget "https://github.com/probonopd/AppImageKit/releases/download/continuous/appimagetool-$(uname -p).AppImage"
chmod a+x appimagetool-$(uname -p).AppImage

# finally build the portable app
./appimagetool-$(uname -p).AppImage Tulip.AppDir Tulip-$(sh tulip-config --version)-$(uname -p).AppImage
