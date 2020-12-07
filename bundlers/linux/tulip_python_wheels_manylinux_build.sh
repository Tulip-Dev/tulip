#!/bin/bash

# This script is only intended to be run using
# a pypa/manylinux2010 docker image (based on Centos 6.10)
TULIP_PYTHON_TEST_WHEEL_SUFFIX=$1

# install tulip-core wheel deps
# yum -y install epel-release
yum -y install zlib-devel qhull-devel ccache cmake3

# install Python 3.6 from the IUS Community Project
curl -LO https://repo.ius.io/ius-release-el6.rpm
rpm -Uvh ius-release*rpm
yum -y install python36u-devel

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

# configure Tulip build with Python 3.6 to ensure correct sip build
cmake3 ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DPYTHON_EXECUTABLE=/usr/bin/python3.6 -DTULIP_BUILD_CORE_ONLY=ON -DTULIP_USE_CCACHE=ON

TULIP_PYTHON_TEST="from tulip import tlp; from platform import python_version; str = '==> Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str)"
# iterate on available Python versions
for CPYBIN in /opt/python/cp3*/bin
do
   ${CPYBIN}/pip install wheel
  # configure and build python wheel with specific Python version
  cmake3 ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DPYTHON_EXECUTABLE=${CPYBIN}/python -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_PYTHON_TEST_WHEEL_SUFFIX=${TULIP_PYTHON_TEST_WHEEL_SUFFIX} -DTULIP_USE_CCACHE=ON
  make -j4
  make test-wheel
  if [ $? -ne 0 ]
  then
     break
  fi
  # check the test wheel
  pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
  ${CPYBIN}/pip install $(ls *${TULIP_PYTHON_TEST_WHEEL_SUFFIX}*.whl -t | head -1)
  ${CPYBIN}/python -c "$TULIP_PYTHON_TEST"
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
  ${CPYBIN}/python -c "$TULIP_PYTHON_TEST"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
done
