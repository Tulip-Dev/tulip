#!/bin/bash
WHEELS_DIR=$1

# set columns in terminal, required for curl to work correctly
export COLUMNS=80

# install python supported versions
for PY_VER in 3.9.13 3.10.11 3.11.9 3.12.8 3.13.1
do
  # download installer
  curl -k -LO https://www.python.org/ftp/python/${PY_VER}/python-${PY_VER}-macos11.pkg
  # standard installation
  sudo installer -store -pkg python-${PY_VER}-*.pkg -target /
  # cleanup
  rm python-*.pkg

  # configure current python
  PY_MMVER=${PY_VER%.*}
  PY_EXE=/Library/Frameworks/Python.framework/Versions/${PY_MMVER}/bin/python3
  ${PY_EXE} -m ensurepip
  ${PY_EXE} -m pip install --upgrade pip
  # install corresponding tulip-python wheel
  ${PY_EXE} -m pip install --user ${WHEELS_DIR}/tulip_python-*-cp${PY_MMVER/./}*.whl
  # test wheel
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
  # uninstall wheel
  ${PY_EXE} -m pip uninstall -y tulip-python
done


