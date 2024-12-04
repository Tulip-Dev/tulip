#!/bin/bash
TULIP_SOURCE_DIR=$1
TULIP_BUILD_DIR=$2
TULIP_WHEELS_DIR=$3
CLANG_VERSION=$4
INSTALL_MACPORTS=$5

# set columns in terminal, required for curl to work correctly
export COLUMNS=80

pushd /tmp

# install python supported versions
for version in 3.9.13 3.10.11 3.11.9 3.12.8 3.13.1
do
  # download installer
  curl -LO https://www.python.org/ftp/python/${version}/python-${version}-macos11.pkg
  # standard installation
  sudo installer -store -pkg python-${version%.*}.*.pkg -target /
  # cleanup
  rm python-*.pkg
done

popd

# install macports and tulip needed packages
export PATH=/opt/local/bin:$PATH

if [ "${INSTALL_MACPORTS}" != "" ]
then
source ${TULIP_SOURCE_DIR}/bundlers/mac/macports_init.sh
sudo port -N install cmake ccache clang-${CLANG_VERSION} pkgconfig
sudo port -N install qhull yajl
fi

cd ${TULIP_BUILD_DIR}
# iterate on available Python versions
for PY_EXE in $(find /Library/Frameworks/Python.framework/Versions -name python3)
do
  # upgrade pip and setuptools
  export DYLD_LIBRARY_PATH=$(dirname $(dirname $PY_EXE))/lib
  ${PY_EXE} -m pip install --upgrade pip
  ${PY_EXE} -m pip install --upgrade setuptools
  # install needed modules
  ${PY_EXE} -m pip install build delocate sip wheel
  PY_DIR=$(dirname $(dirname ${PY_EXE}))
  PY_VERSION=$(basename ${PY_DIR})
  # configure and build python wheel with specific Python version
  cmake ${TULIP_SOURCE_DIR} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_C_COMPILER=/opt/local/bin/clang-mp-${CLANG_VERSION} -DCMAKE_CXX_COMPILER=/opt/local/bin/clang++-mp-${CLANG_VERSION} -DZLIB_INCLUDE_DIR=/opt/local/include -DZLIB_LIBRARY_RELEASE=/opt/local/lib/libz.dylib -DPython_EXECUTABLE=${PY_EXE} -DPython_INCLUDE_DIRS=${PY_DIR}/includes -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_WHEELS_PREFIX=${TULIP_WHEELS_DIR}
  make -j4
  # build & check the tulip-core wheel
  make wheel
  # minimize contents removal for next wheel build
  rm CMakeCache.txt
  rm -rf library/tulip-python

  pushd ${TULIP_WHEELS_DIR}
  ${PY_EXE} -m pip install --user $(ls -t | head -1)
  popd
  ${PY_EXE} -c "from tulip import tlp
from platform import python_version
str = '===> Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version()
print(str)
l = tlp.getSizeAlgorithmPluginsList()
exit(1) if not l else exit(0)"
if [ $? -ne 0 ]
then
   break
fi
  ${PY_EXE} -m pip uninstall -y tulip-python
done
