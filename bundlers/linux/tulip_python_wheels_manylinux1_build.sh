#!/bin/bash

# This script is only intended to be run in a pypa/manylinux1
# docker image (based on Centos 5.11)
TULIP_PYTHON_TEST_WHEEL_SUFFIX=$1

# install tulip-core wheel deps
yum -y install zlib-devel qhull-devel ccache
# install tulip-gui wheel deps
yum -y install freetype-devel libpng-devel libjpeg-devel glew-devel fontconfig

pushd /tmp

# download and install our own build of Qt 4.8.7
curl http://tulip.labri.fr/code/qt-4.8.7_$(uname -m).tar.gz -o /tmp/qt-4.8.7.tar.gz
pushd /opt
tar zxvf /tmp/qt-4.8.7.tar.gz
rm -rf /tmp/qt-4.8.7.tar.gz
export LD_LIBRARY_PATH=/opt/qt-4.8.7/lib
popd

# download, build and install cmake-2.8.12 needed by tulip
curl -LO https://cmake.org/files/v2.8/cmake-2.8.12.tar.gz
tar zxvf cmake-2.8.12.tar.gz
pushd cmake-2.8.12
cmake28 . && make -j4 install
popd
# remove cmake source
rm -rf cmake-2.8.12*
popd

# get tulip source dir
if [ -d /tulip ]
then
  TULIP_SRC=/tulip
else
# tulip sources install
  cd /tmp
  git clone https://github.com/Tulip-Dev/tulip.git
  TULIP_SRC=/tmp/tulip
fi

# build tulip
if [ -d /tulip_build ]
then
  cd /tulip_build; rm -rf *;
else
  mkdir /tmp/tulip_build; cd /tmp/tulip_build
fi
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  # configure and build python wheels with specific Python version
  cmake ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DPYTHON_EXECUTABLE=${CPYBIN}/python -DQT_QMAKE_EXECUTABLE=/opt/qt-4.8.7/bin/qmake -DTULIP_ACTIVATE_PYTHON_WHEELS_TARGETS=ON -DTULIP_PYTHON_TEST_WHEEL_SUFFIX=${TULIP_PYTHON_TEST_WHEEL_SUFFIX} -DTULIP_USE_CCACHE=ON 
  make -j4 wheels
  if [ $? -ne 0 ]
  then
     break
  fi
  # check the tulip-core wheel
  pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
  ${CPYBIN}/pip install $(ls -t | head -1)
  ${CPYBIN}/python -c "import tulip
from platform import python_version
str = 'tulip successfully imported in Python ' + python_version()
print(str)"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
  # check the tulip-gui wheel
  pushd ./library/tulip-python/bindings/tulip-gui/tulipgui_module/dist
  ${CPYBIN}/pip install $(ls -t | head -1)
  ${CPYBIN}/python -c "import tulipgui
from platform import python_version
str = 'tulipgui successfully imported in Python ' + python_version()
print(str)"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
done
