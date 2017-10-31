#!/bin/bash

cd

# clean packages cache
yum -y clean all
yum -y clean expire-cache

# install base build system
yum -y install epel-release
yum -y install rpm-build gcc gcc-c++ cmake git subversion tar gzip make wget ccache

# install tulip deps
yum -y install zlib-devel qhull-devel
yum -y install freetype-devel libpng-devel libjpeg-devel glew-devel
yum -y install binutils-devel

# needed for generating the doc
yum -y install doxygen graphviz

# needed for generating the appimage
yum -y install fuse fuse-libs

# install precompiled Qt 4.8.4 (could not find latest 4.8 version but it is still better than old Qt 4.6 bundled with CentOS 6)
wget http://mirror.yandex.ru/fedora/russianfedora/russianfedora/free/el/releases/6/Everything/x86_64/os/puias-release-6-2.R.noarch.rpm
wget http://mirror.yandex.ru/fedora/russianfedora/russianfedora/free/el/releases/6/Everything/x86_64/os/russianfedora-free-release-6-3.R.noarch.rpm
rpm -i puias-release-6-2.R.noarch.rpm
rpm -i russianfedora-free-release-6-3.R.noarch.rpm
yum -y install qt48-devel

# compile and install Python 2.7 (the Python version bundled with CentOS 6 is Python 2.6, which is quite old)
yum install -y zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel readline-devel tk-devel gdbm-devel db4-devel libpcap-devel xz-devel expat-devel
wget http://python.org/ftp/python/2.7.14/Python-2.7.14.tar.xz
tar xf Python-2.7.14.tar.xz
cd Python-2.7.14
./configure --prefix=/usr --enable-unicode=ucs4 --enable-shared --enable-optimizations
make -j4 && make altinstall

# some extra setup for cmake et appimage build to succeed
cp -r /usr/lib/libpython2.7* /usr/lib64/
cp -r /usr/lib/python2.7 /usr/lib64/
export PATH=/usr/lib64/qt48/bin/:$PATH

# install pip in order to grab the latest sphinx version
wget https://bootstrap.pypa.io/get-pip.py
python2.7 get-pip.py
pip2.7 install sphinx

# build and install tulip
cd && mkdir tulip_build && cd tulip_build
cmake -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=/usr/bin/python2.7 -DCMAKE_INSTALL_PREFIX=$PWD/install -DTULIP_USE_CCACHE=ON /tulip
make -j4 install

# build a bundle dir suitable for AppImageKit
sh bundlers/linux/make_appimage_bundle.sh --appdir $PWD

# get appimagetool
wget "https://github.com/probonopd/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod a+x appimagetool-x86_64.AppImage

# finally build the portable app
./appimagetool-x86_64.AppImage Tulip.AppDir Tulip-$(install/bin/tulip-config --version)
