#!/bin/bash

# This script is only intended to be run using
# a pypa/manylinux1 docker image (based on Centos 5.11)
# or a pypa/manylinux2010 docker image (based on Centos 6.10)
TULIP_PYTHON_TEST_WHEEL_SUFFIX=$1

# install tulip-core wheel deps
yum -y install zlib-devel qhull-devel ccache

pushd /tmp

# download, build and install cmake-3.1.3 needed by tulip
curl -LO https://cmake.org/files/v3.1/cmake-3.1.3.tar.gz
tar zxvf cmake-3.1.3.tar.gz
pushd cmake-3.1.3
./bootstrap && make -j4 install
popd
# remove cmake source
rm -rf cmake-3.1.3*
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

# looking for python lib
ls /usr/lib64/libpython*.so

# add link if needed
if [ $? -ne 0 ]; then
  PYLIB=$(ls /usr/lib64/libpython*.so* | cut --delimiter=. --fields=1,2,3)
  ln -s $(ls $PYLIB.*) $PYLIB
fi

# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  # configure and build python wheel with specific Python version
  cmake ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DPYTHON_EXECUTABLE=${CPYBIN}/python -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_PYTHON_TEST_WHEEL_SUFFIX=${TULIP_PYTHON_TEST_WHEEL_SUFFIX} -DTULIP_USE_CCACHE=ON 
  make -j4
  make test-wheel
  if [ $? -ne 0 ]
  then
     break
  fi
  # check the test wheel
  pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
  ${CPYBIN}/pip install $(ls *${TULIP_PYTHON_TEST_WHEEL_SUFFIX}*.whl -t | head -1)
  ${CPYBIN}/python -c "import tulip
from platform import python_version
str = '===> tulip successfully imported in Python ' + python_version()
print(str)"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
  # uninstall test wheel
  ${CPYBIN}/pip uninstall -y tulip-python

  # check the tulip-core wheel
  pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
  ${CPYBIN}/pip install $(ls -t | head -1)
  ${CPYBIN}/python -c "from tulip import tlp; from platform import python_version; str = 'Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str)"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
done
