#!/bin/bash

# This script is only intended to be run using
# a pypa/manylinux2010 docker image (based on Centos 6.10)
TULIP_PYTHON_TEST_WHEEL_SUFFIX=$1

# install tulip-core wheel deps
# yum -y install epel-release
yum -y install zlib-devel qhull-devel ccache python-devel
# install wheels build deps
yum -y install libffi-devel

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

TULIP_PYTHON_TEST="from tulip import tlp; from platform import python_version; str = '==> Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str)"
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  PY_VERSION=$(${CPYBIN}/python -c "from platform import python_version; print(python_version())")
  IFS='.' read -a PY_VERSION <<< "$PY_VERSION"
  # Python < 3.8 no longer supported, 3.12 not yet supported
  if [[ ${PY_VERSION[0]} -ne 3 ]] || [[ ${PY_VERSION[1]} -lt 8 ]] || [[ ${PY_VERSION[1]} -ge 12 ]]
  then
     continue
  fi
  pushd $CPYBIN
  cd ..
  CPYDIR=$(basename $PWD)
  popd
  CPYINC=/opt/python/$CPYDIR/include/$(ls ${CPYBIN}/../include)
  # configure and build python wheel with specific Python version
  cmake ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INCLUDE_PATH=${CPYINC} -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DPython_EXECUTABLE=${CPYBIN}/python -DPYTHON_INCLUDE_DIR=${CPYINC} -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_PYTHON_TEST_WHEEL_SUFFIX=${TULIP_PYTHON_TEST_WHEEL_SUFFIX} -DTULIP_USE_CCACHE=ON
  TULIP_VERSION=$(bash ./tulip-config --version)
  make -j4
  make test-wheel
  if [ $? -ne 0 ]
  then
     break
  fi
  # check the test wheel
  pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
  ${CPYBIN}/pip install $(ls *${TULIP_VERSION}.${TULIP_PYTHON_TEST_WHEEL_SUFFIX}-${CPYDIR}-*.whl)
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
  ${CPYBIN}/pip install $(ls *${TULIP_VERSION}-${CPYDIR}-*.whl)
  ${CPYBIN}/python -c "$TULIP_PYTHON_TEST"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
done
