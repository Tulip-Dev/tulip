#!/bin/bash
#magic command: docker run --name manylinux_2_28_x86_64 -v /home/bruno/tulip:/tulip -v /home/bruno/compil/docker_manylinux2010_x86_64:/tulip_build --rm quay.io/pypa/manylinux_2_28_x86_64 bash -xc "bash -x /tulip/bundlers/linux/tulip_python_wheels_manylinux_build.sh dev0"


# This script is only intended to be run using
# a pypa/manylinux-2.28 docker image (based on Almalinux 8)
TULIP_PYTHON_TEST_WHEEL_SUFFIX=$1

# install tulip-core wheel deps
#yum -y upgrade
yum -y install yajl-devel
# yum -y install qhull-devel #this package does not work with manylinux_2_28

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
  rm -rf /tulip_build/*; mkdir -p /tulip_build/build /tulip_build/wheels;cd build; TULIP_WHEELS_PREFIX=/tulip_build/wheels

else
  mkdir -p /tmp/tulip_build/build /tmp/tulip_build/wheels; cd /tmp/tulip_build/build; TULIP_WHEELS_PREFIX=/tmp/tulip_build/wheels
fi

TULIP_PYTHON_TEST="from tulip import tlp; from platform import python_version; str = '==> Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str)"
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  PY_VERSION=$(${CPYBIN}/python -c "from platform import python_version; print(python_version())")
  IFS='.' read -a PY_VERSION <<< "$PY_VERSION"
  # Python < 3.8 no longer supported, 3.13 not yet supported
  if [[ ${PY_VERSION[0]} -ne 3 ]] || [[ ${PY_VERSION[1]} -lt 8 ]] || [[ ${PY_VERSION[1]} -ge 13 ]]
  then
     continue
  fi
  # install sip and wheel (to be removed soon from manylinux with Python >= 3.12)
  #cmeel-qhull is a qhull release which is installed via pip. it works but the configuration is a bit tough!!
  ${CPYBIN}/python -m pip install sip cmeel-qhull
  ${CPYBIN}/python -m pip install -U wheel
  pushd $CPYBIN
  cd ..
  CPYDIR=$(basename $PWD)
  popd
  CPYINC=/opt/python/$CPYDIR/include/$(ls ${CPYBIN}/../include)
  # configure and build python wheel with specific Python version
  cmake -S ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INCLUDE_PATH=${CPYINC} -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DTULIP_WHEELS_PREFIX=${TULIP_WHEELS_PREFIX} -DPython_EXECUTABLE=${CPYBIN}/python -DPython_INCLUDE_DIRS=${CPYINC} -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_PYTHON_TEST_WHEEL_SUFFIX=${TULIP_PYTHON_TEST_WHEEL_SUFFIX} -DTULIP_USE_CCACHE=ON
  TULIP_VERSION=$(bash ./tulip-config --version)
  cmake --build . -j4
  cmake --build . -t test-wheel -j4
  if [ $? -ne 0 ]
  then
     break
  fi
  # check the test wheel
  pushd ${TULIP_WHEELS_PREFIX}
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
  pushd ${TULIP_WHEELS_PREFIX}
  ${CPYBIN}/pip install $(ls *${TULIP_VERSION}-${CPYDIR}-*.whl)
  ${CPYBIN}/python -c "$TULIP_PYTHON_TEST"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd

  #clean tulip_python folder before using another Python version
  pushd ./library/tulip-python
  cmake --build . -t clean
  popd
done
