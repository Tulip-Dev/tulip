#!/bin/bash

# clean packages cache
yum -y clean all
yum -y clean expire-cache

# install base build system
yum -y install epel-release
yum -y install rpm-build gcc gcc-c++ cmake git subversion tar gzip make wget ccache

# install tulip deps
yum -y install zlib-devel yajl-devel qhull-devel
yum -y install freetype-devel libpng-devel libjpeg-devel glew-devel
yum -y install qt-devel qtwebkit-devel quazip-devel binutils-devel
yum -y install python python-devel python-pip
# needed for generating the doc
yum -y install doxygen graphviz python-sphinx

# install a more recent version of sphinx
# as sphinx drops python 2.6 support (version installed in CentOS 6)
# in its 1.5 release, use latest version from the 1.4 branch
pip install sphinx==1.4.9

# build and install tulip
cd && mkdir tulip_build && cd tulip_build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DTULIP_USE_CCACHE=ON /tulip
make -j4 install

# build a bundle dir suitable for AppImageKit
sh bundlers/linux/make_appimage_bundle.sh --appdir $PWD

yum -y install fuse fuse-libs

# get appimagetool
wget "https://github.com/probonopd/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod a+x appimagetool-x86_64.AppImage

# finally build the portable app
./appimagetool-x86_64.AppImage Tulip.AppDir Tulip-$(install/bin/tulip-config --version)
