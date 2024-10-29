#!/bin/bash
#magic command: docker run --name manylinux_2_28_x86_64 -v /home/bruno/tulip:/tulip -v /home/bruno/compil/tulip_wheels:/tulip_build --rm quay.io/pypa/manylinux_2_28_x86_64 bash -xc "bash -x /tulip/bundlers/linux/tulip_python_wheels_manylinux_build.sh"

# This script is only intended to be run using
# a pypa/manylinux-2.28 docker image (based on Almalinux 8)

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

TULIP_PYTHON_TEST="from tulip import tlp; from platform import python_version; str = '==> Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str); l = tlp.getSizeAlgorithmPluginsList();exit(1) if not l else exit(0)"
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  PY_VERSION=$(${CPYBIN}/python -c "from platform import python_version; print(python_version())")
  IFS='.' read -a PY_VERSION <<< "$PY_VERSION"
  # Python < 3.9 no longer supported, 3.14 not yet supported
  if [[ ${PY_VERSION[0]} -ne 3 ]] || [[ ${PY_VERSION[1]} -lt 9 ]] || [[ ${PY_VERSION[1]} -ge 14 ]]
  then
     continue
  fi
  # install sip and build packages
  #cmeel-qhull is a qhull release which is installed via pip. it works but the configuration is a bit tough!!
  ${CPYBIN}/python -m pip install -U pip build setuptools
  ${CPYBIN}/python -m pip install sip cmeel-qhull
  pushd $CPYBIN
  cd ..
  CPYDIR=$(basename $PWD)
  popd
  CPYINC=/opt/python/$CPYDIR/include/$(ls ${CPYBIN}/../include)
  # configure and build python wheel with specific Python version
  cmake -S ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INCLUDE_PATH=${CPYINC} -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install -DTULIP_WHEELS_PREFIX=${TULIP_WHEELS_PREFIX} -DPython_EXECUTABLE=${CPYBIN}/python -DPython_INCLUDE_DIRS=${CPYINC} -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_USE_CCACHE=ON
  TULIP_VERSION=$(bash ./tulip-config --version)
  cmake --build . -t wheel -j4
  if [ $? -ne 0 ]
  then
     break
  fi

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
