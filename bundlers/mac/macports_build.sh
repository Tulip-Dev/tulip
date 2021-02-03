#!/bin/bash

# set colums in terminal, required by curl
export COLUMNS=80

# first install MacPorts in order to easily retrieve Tulip dependencies
curl -LO https://raw.githubusercontent.com/GiovanniBussi/macports-ci/master/macports-ci
bash ./macports-ci install

# configure PATH with macports binaries installation dir
export PATH=/opt/local/bin:$PATH

# configure according clang compiler version (9.0, 10, ...)
if [ "$CLANG_COMPILER_VERSION" != "" ]; then
  CLANG_COMPILER_PKG=clang-${CLANG_COMPILER_VERSION}
  CLANG_COMPILER_DEFINES="-DCMAKE_C_COMPILER=/opt/local/bin/clang-mp-${CLANG_COMPILER_VERSION} -DCMAKE_CXX_COMPILER=/opt/local/bin/clang++-mp-${CLANG_COMPILER_VERSION}"
fi

# check if ccache is needed
if [ "$TULIP_USE_CCACHE" == "ON" ]; then
  CCACHE_PKG=ccache
fi

# install Tulip core build dependencies
sudo port -N install cmake $CLANG_COMPILER_PKG $CCACHE_PKG qhull

# check for more dependencies
if [ "$TRAVIS_BUILD_THIRDPARTY_ONLY" != "ON" ]; then
# install Tulip tests build dependencies
  if [ "$TULIP_BUILD_TESTS" == "ON" ]; then
    sudo port -N install pkgconfig cppunit
  fi
  if [ "$PYTHON_EXECUTABLE" == "" ]; then
    PYTHON_EXECUTABLE=/usr/bin/python2.7
  fi
# install Tulip complete build dependencies
  if [ "$TULIP_BUILD_CORE_ONLY" != "ON" ]; then
    sudo port -N install freetype glew
    TULIP_GUI_DEFINES="-DZLIB_INCLUDE_DIR=/opt/local/include -DZLIB_LIBRARY_RELEASE=/opt/local/lib/libz.dylib -DGLEW_SHARED_LIBRARY_RELEASE=/opt/local/lib/libGLEW.dylib"
    if [ "$Qt5_DIR" != "" ]; then
      CMAKE_PREFIX_PATH_DEFINE="-DCMAKE_PREFIX_PATH=$Qt5_DIR"
    else
      sudo port -N install qt5-qtbase qt5-qttools qt5-qtwebkit quazip
    fi
    if [ "$TULIP_BUILD_DOC" == "ON" ]; then
      curl -O https://bootstrap.pypa.io/get-pip.py
      sudo $PYTHON_EXECUTABLE get-pip.py
      $PYTHON_EXECUTABLE -m pip install --user sphinx==1.7.9
      PYTHON_MAJOR_MINOR=$($PYTHON_EXECUTABLE -c "print(__import__('sys').version.split(' ')[0])" | cut -d'.' -f '1 2')
      SPHINX_EXECUTABLE=$HOME/Library/Python/$PYTHON_MAJOR_MINOR
    fi
  fi
fi

# create build directory
mkdir build && cd build

# configure Tulip build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install ${CLANG_COMPILER_DEFINES} ${CMAKE_PREFIX_PATH_DEFINE} -DPYTHON_EXECUTABLE=$PYTHON_EXECUTABLE -DTRAVIS_BUILD_THIRDPARTY_ONLY=$TRAVIS_BUILD_THIRDPARTY_ONLY -DTULIP_BUILD_CORE_ONLY=$TULIP_BUILD_CORE_ONLY -DTULIP_BUILD_DOC=$TULIP_BUILD_DOC -DTULIP_BUILD_TESTS=$TULIP_BUILD_TESTS -DTULIP_USE_CCACHE=$TULIP_USE_CCACHE ${TULIP_GUI_DEFINES}
# compile Tulip
make -j$(getconf _NPROCESSORS_ONLN) install

if [ "$TRAVIS_BUILD_THIRDPARTY_ONLY" != "ON" ]; then
# run Tulip unit tests
  if [ "$TULIP_BUILD_TESTS" == "ON" ]; then
    make test
  fi
# build Tulip bundle
  if [ "$TULIP_MAKE_BUNDLE" == "ON" ]; then
    make bundle
  fi
fi
