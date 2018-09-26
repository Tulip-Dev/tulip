#!/bin/bash

# This script is only intended to be run in the pypa/manylinux1
# docker image based on Centos 5.11

cd

# install tulip-python wheel deps
yum -y install zlib-devel qhull-devel ccache

# we need at least CMake 2.8.12 in order to build tulip-python wheel
# so let's grab it, compile it and install it
curl -LO https://cmake.org/files/v2.8/cmake-2.8.12.tar.gz
tar -xvzf cmake-2.8.12.tar.gz
cd cmake-2.8.12
./bootstrap --enable-ccache
make -j4
make -j4 install

cd

mkdir tulip_build && cd tulip_build
# iterate on available Python versions
for PYTHON_BIN_PATH in /opt/python/cp*/bin
do
  # configure and build tulip-python with specific Python version
  cmake -DTULIP_USE_CCACHE=ON -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DPYTHON_EXECUTABLE=$PYTHON_BIN_PATH/python -DCMAKE_BUILD_TYPE=Release /tulip
  make -j4 wheel
  # install and check that the produced module can be successfully imported
  cd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
  $PYTHON_BIN_PATH/pip install $(ls -t | head -1)
  $PYTHON_BIN_PATH/python -c "import tulip"
  cd && cd tulip_build
done
