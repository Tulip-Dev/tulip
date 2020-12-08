SET(TULIP_PYTHON_SITE_INSTALL OFF CACHE BOOL "Do you want to install Tulip Python modules in a Python standard module folder on your system ?
The selected folder path will be the first in the list returned by site.getsitepackages() whose prefix equals ${CMAKE_INSTALL_PREFIX}.
If no such folder is found, the path will be the one returned by site.getusersitepackages().
This should only be used when packaging Tulip for a Linux distribution or MSYS2. [OFF|ON]")

# After finding the Python interpreter, try to find if SIP and its dev tools are installed on the host system.
# If not, compile the SIP version located in thirdparty.
FIND_PACKAGE(PythonInterp REQUIRED)

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version OUTPUT_VARIABLE PYTHON_VERSION_RAW ERROR_VARIABLE PYTHON_VERSION_RAW)
STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
STRING(REGEX MATCH "[0-9]\\.[0-9]+" PYTHON_VERSION "${PYTHON_VERSION_RAW}")
STRING(REGEX MATCH "[0-9]\\.[0-9]+\\.[0-9]+" PYTHON_VERSION_WITH_PATCH "${PYTHON_VERSION_RAW}")
STRING(REPLACE "." "" PYTHON_VERSION_NO_DOT ${PYTHON_VERSION})

IF(TULIP_PYTHON_SITE_INSTALL)

  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "
from __future__ import print_function
import site
import sys
from distutils.sysconfig import get_python_lib
py_version = str(sys.version_info[0]) + '.' + str(sys.version_info[1])
if sys.version_info >= (2, 7):
  for path in site.getsitepackages():
    # check that we select a valid install path
    if path.startswith('${CMAKE_INSTALL_PREFIX}') and py_version in path:
      # avoid to install in /usr/local when CMAKE_INSTALL_PREFIX is /usr on debian
      if '${CMAKE_INSTALL_PREFIX}' == '/usr' and '/usr/local' in path:
        continue
      print(path)
      exit()
  print(site.getusersitepackages())
else:
  path = get_python_lib(1)
  if path.startswith('${CMAKE_INSTALL_PREFIX}') and py_version in path:
    print(path)
    exit()
  print(site.USER_SITE)
"
                  OUTPUT_VARIABLE TulipPythonModulesInstallDir)
  STRING(REPLACE "\n" "" TulipPythonModulesInstallDir "${TulipPythonModulesInstallDir}")

ELSE(TULIP_PYTHON_SITE_INSTALL)
  SET(TulipPythonModulesInstallDir ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir}/tulip/python)
ENDIF(TULIP_PYTHON_SITE_INSTALL)

# Unset the previous values of the CMake cache variables related to Python libraries
# in case the value of PYTHON_EXECUTABLE CMake variable changed
UNSET(PYTHONLIBS_FOUND CACHE)
UNSET(PYTHON_LIBRARY CACHE)
UNSET(PYTHON_INCLUDE_DIR CACHE)
UNSET(PYTHON_INCLUDE_PATH CACHE)

# Find the Python library with the same version as the interpreter
# Python 3.2 library is suffixed by mu and Python >= 3.3 by m on some systems, also handle these cases
SET(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION}mu ${PYTHON_VERSION}m ${PYTHON_VERSION})

# Ensure the detection of Python library installed trough a bundle downloaded from Python.org
IF(APPLE)
  IF(NOT "${PYTHON_EXECUTABLE}" MATCHES "^/usr/bin/python.*$" AND NOT "${PYTHON_EXECUTABLE}" MATCHES "^/System/Library/Frameworks/Python.framework/.*/python.*$")
    SET(CMAKE_PREFIX_PATH /Library/Frameworks/Python.framework/Versions/${PYTHON_VERSION} ${CMAKE_PREFIX_PATH})
  ELSE()
    SET(CMAKE_PREFIX_PATH /System/Library/Frameworks/Python.framework/Versions/${PYTHON_VERSION} ${CMAKE_PREFIX_PATH})
  ENDIF()
ENDIF(APPLE)

GET_FILENAME_COMPONENT(PYTHON_HOME_PATH ${PYTHON_EXECUTABLE} PATH)

# Ensure that correct Python include path is selected by CMake on Windows
IF(WIN32)
  SET(CMAKE_INCLUDE_PATH ${PYTHON_HOME_PATH}/include ${CMAKE_INCLUDE_PATH})
  # Ensure that correct Python include path and library are selected by CMake on Linux (in case of non standard installation)
ELSEIF(LINUX)
  SET(CMAKE_INCLUDE_PATH ${PYTHON_HOME_PATH}/../include ${CMAKE_INCLUDE_PATH})
  SET(CMAKE_LIBRARY_PATH ${PYTHON_HOME_PATH}/../lib ${CMAKE_LIBRARY_PATH})
ENDIF(WIN32)

FIND_PACKAGE(PythonLibs REQUIRED)

IF(MINGW)
  # Check if Python is provided by MSYS2 (it is compiled with GCC in that case instead of MSVC)
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(sys.version)" OUTPUT_VARIABLE PYTHON_VERSION_FULL ERROR_VARIABLE PYTHON_VERSION_FULL)
  STRING(REGEX MATCH "GCC" MSYS2_PYTHON "${PYTHON_VERSION_FULL}")

  # Python 64bits does not provide a dll import library for MinGW.
  # Fortunately, we can directly link to the Python dll with that compiler.
  # So find the location of that dll and overwrite the PYTHON_LIBRARY CMake cache variable with it
  STRING(REPLACE "\\" "/" WINDIR $ENV{WINDIR})

  IF(MSYS2_PYTHON)
    IF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll)
      SET(PYTHON_LIBRARY ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll CACHE FILEPATH "" FORCE)
    ELSEIF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}m.dll)
      SET(PYTHON_LIBRARY ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}m.dll CACHE FILEPATH "" FORCE)
    ENDIF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll)
  ELSE(MSYS2_PYTHON)
    # Check if the Python dll is located in the Python home directory (when Python is installed for current user only)
    IF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
      SET(PYTHON_LIBRARY ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
      #If not, the Python dll is located in %WINDIR%/System32 (when Python is installed for all users)
    ELSE(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
      IF(NOT WIN_AMD64 OR X64)
        SET(PYTHON_LIBRARY ${WINDIR}/System32/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
      ELSE(NOT WIN_AMD64 OR X64)
        SET(PYTHON_LIBRARY ${WINDIR}/SysWOW64/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
      ENDIF(NOT WIN_AMD64 OR X64)
    ENDIF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
  ENDIF(MSYS2_PYTHON)

ENDIF(MINGW)

# Ensure headers correspond to the ones associated to the detected Python library on MacOS
IF(APPLE)
  IF("${PYTHON_LIBRARY}" MATCHES "^/Library/Frameworks/Python.framework/Versions/${PYTHON_VERSION}.*$")
    SET(PYTHON_INCLUDE_DIR /Library/Frameworks/Python.framework/Versions/${PYTHON_VERSION}/Headers CACHE PATH "" FORCE)
  ENDIF()
  IF("${PYTHON_LIBRARY}" MATCHES "^/System/Library/Frameworks/Python.framework/Versions/${PYTHON_VERSION}.*$")
    SET(PYTHON_INCLUDE_DIR /System/Library/Frameworks/Python.framework/Versions/${PYTHON_VERSION}/Headers CACHE PATH "" FORCE)
  ENDIF()
ENDIF(APPLE)

SET(SIP_VERSION_THIRDPARTY 4.19.24)
SET(SIP_OK FALSE CACHE INTERNAL "")
FIND_PACKAGE(SIP)
SET(SIP_OK ${SIP_FOUND})
IF(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})
  SET(SIP_EXE "${SIP_EXECUTABLE}")
  SET(SYSTEM_SIP TRUE)
  SET(SIP_MODULE "sip")
ELSE(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})
  SET(SIP_LIB sip)
  SET(SYSTEM_SIP FALSE)
  SET(SIP_MODULE "tulip.native.sip")
  IF(SIP_OK)
    IF(NOT "${SIP_VERSION}" STREQUAL "${LAST_FOUND_SIP_VERSION}")
      MESSAGE(STATUS "SIP was found on the system but its version is lesser than the required one (${SIP_VERSION_THIRDPARTY}).")
    ENDIF(NOT "${SIP_VERSION}" STREQUAL "${LAST_FOUND_SIP_VERSION}")
    SET(SIP_FOUND FALSE)
  ENDIF(SIP_OK)

  TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen sip
    CMAKE_FLAGS -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT} -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
    -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} -DPYTHON_INCLUDE_DIR2=${PYTHON_INCLUDE_DIR2}
    -DPYTHON_INCLUDE_PATH=${PYTHON_INCLUDE_PATH} -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
    -DSIP_LIB=${SIP_LIB}
    OUTPUT_VARIABLE SIP_COMPILE_OUTPUT)

  IF(SIP_OK)
    SET(SIP_VERSION_STR "${SIP_VERSION_THIRDPARTY}")
    IF(WIN32 AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/Debug/sip.exe")
      SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/Debug/sip.exe)
    ELSE()
      SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/sip)
    ENDIF()
    SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/siplib/)
  ELSE(SIP_OK)
    MESSAGE(FATAL_ERROR "SIP build failed:\n${SIP_COMPILE_OUTPUT}")
  ENDIF(SIP_OK)
ENDIF(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})

IF (NOT "${SIP_VERSION_STR}${SYSTEM_SIP}" STREQUAL "${LAST_FOUND_SIP_CONFIG}")
  MESSAGE(STATUS "Found SIP ${SIP_VERSION_STR}: ${SIP_EXE}")
ENDIF (NOT "${SIP_VERSION_STR}${SYSTEM_SIP}" STREQUAL "${LAST_FOUND_SIP_CONFIG}")

SET(LAST_FOUND_SIP_CONFIG "${SIP_VERSION_STR}${SYSTEM_SIP}" CACHE INTERNAL "")

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  STRING(REGEX REPLACE "[^0-9.]" "" TULIP_PYTHON_WHEEL_VERSION "${TulipMajorVersion}.${TulipMinorVersion}.${TulipReleaseVersion}")

  IF(WIN32)
    SET(WHEEL_INSTALL_PATH "\\")
  ELSE(WIN32)
    SET(WHEEL_INSTALL_PATH "/")
  ENDIF(WIN32)

  ADD_CUSTOM_TARGET(wheel
    COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
    WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})

  # check generation of test wheels
  STRING(COMPARE NOTEQUAL "${TULIP_PYTHON_TEST_WHEEL_SUFFIX}" "" TULIP_GENERATE_TESTPYPI_WHEEL)

  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
    SET(TULIP_PYTHON_TEST_WHEEL_VERSION ${TULIP_PYTHON_WHEEL_VERSION}.${TULIP_PYTHON_TEST_WHEEL_SUFFIX})

    ADD_CUSTOM_TARGET(test-wheel
      COMMAND ${PYTHON_EXECUTABLE} setuptest.py bdist_wheel
            WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})
    ADD_DEPENDENCIES(test-wheel wheel)
  ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)

  IF(NOT LINUX)

    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import wheel" RESULT_VARIABLE WHEEL_OK OUTPUT_QUIET ERROR_QUIET)
    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import twine" RESULT_VARIABLE TWINE_OK OUTPUT_QUIET ERROR_QUIET)
    IF(NOT WHEEL_OK EQUAL 0)
      MESSAGE("The 'wheel' Python module has to be installed to generate wheels for tulip modules.")
      MESSAGE("You can install it through the 'pip' tool ($ pip install wheel)")
    ENDIF(NOT WHEEL_OK EQUAL 0)
    IF(NOT TWINE_OK EQUAL 0)
      MESSAGE("The 'twine' Python module has to be installed to upload tulip wheels on PyPi.")
      MESSAGE("You can install it through the 'pip' tool ($ pip install twine)")
    ENDIF(NOT TWINE_OK EQUAL 0)

  ELSE(NOT LINUX)

    IF(NOT EXISTS ${PYTHON_HOME_PATH}/wheel)
      EXECUTE_PROCESS(COMMAND ${PYTHON_HOME_PATH}/pip install --upgrade wheel)
    ENDIF(NOT EXISTS ${PYTHON_HOME_PATH}/wheel)
    IF(NOT EXISTS ${PYTHON_HOME_PATH}/twine)
      EXECUTE_PROCESS(COMMAND ${PYTHON_HOME_PATH}/pip install --upgrade twine)
    ENDIF(NOT EXISTS ${PYTHON_HOME_PATH}/twine)

    # When building Python binary wheels on Linux, produced binaries have to be patched
    # in order for the tulip modules to be successfully imported and loaded on every computer.
    # The 'auditwheel' tool (see https://github.com/pypa/auditwheel) has been developed
    # in order to ease that patching task.
    # We use our patched version of the auditwheel tool
    # as the official one does not repair tulip-gui wheel correctly
    IF(NOT IS_DIRECTORY /tmp/auditwheel)
      EXECUTE_PROCESS(COMMAND bash -c "echo $(dirname $(readlink /usr/local/bin/auditwheel))" OUTPUT_VARIABLE PYBIN OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND bash -c "${PYBIN}/pip uninstall -y auditwheel; cd /tmp; curl -LO http://tulip.labri.fr/code/auditwheel.tar.gz; tar zxvf auditwheel.tar.gz; ${PYBIN}/pip install /tmp/auditwheel")
    ENDIF(NOT IS_DIRECTORY /tmp/auditwheel)

    ADD_CUSTOM_COMMAND(TARGET wheel POST_BUILD
      COMMAND bash -c "auditwheel repair -L native -w ./dist ./dist/$(ls -t ./dist/ | head -1)"
      COMMAND bash -c "rm ./dist/$(ls -t ./dist/ | head -2 | tail -1)"
      WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
      COMMENT "patching linux tulip-core wheel" VERBATIM)

    IF(TULIP_GENERATE_TESTPYPI_WHEEL)
      ADD_CUSTOM_COMMAND(TARGET test-wheel POST_BUILD
        COMMAND bash -c "auditwheel repair -L native -w ./dist ./dist/$(ls -t ./dist/ | head -1)"
        COMMAND bash -c "rm ./dist/$(ls -t ./dist/ | head -2 | tail -1)"
        WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
        COMMENT "patching linux tulip-core test wheel" VERBATIM)
    ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)

  ENDIF(NOT LINUX)

  # In order to upload the generated wheels, an account must be created on PyPi
  # and the following configuration must be stored in the ~/.pypirc file
  ##############################################################
  # [distutils]
  # index-servers=
  #     pypi
  #     testpypi
  #
  # [testpypi]
  # repository: https://test.pypi.org/legacy/
  # username: <your user name goes here>
  # password: <your password goes here>
  #
  # [pypi]
  # repository: https://upload.pypi.org/legacy/
  # username: <your user name goes here>
  # password: <your password goes here>
  ###############################################################


  SET(TWINE twine)
  IF(EXISTS ${PYTHON_HOME_PATH}/twine)
    SET(TWINE ${PYTHON_HOME_PATH}/twine)
  ENDIF(EXISTS ${PYTHON_HOME_PATH}/twine)
  IF(WIN32)
    SET(TWINE ${PYTHON_INCLUDE_DIR}/../Scripts/twine.exe)
  ENDIF(WIN32)
  SET(WHEEL_FILES_REGEXP "*${TULIP_PYTHON_WHEEL_VERSION}-cp*")
  ADD_CUSTOM_TARGET(wheel-upload
    COMMAND bash -c "echo -e 'uploading wheels:\\n' $(ls ${TULIP_PYTHON_ROOT_FOLDER}/dist/${WHEEL_FILES_REGEXP})"
    COMMAND ${TWINE} upload -r pypi dist/${WHEEL_FILES_REGEXP}
    WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER} VERBATIM)

  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
    SET(TEST_WHEEL_FILES_REGEXP "*${TULIP_PYTHON_TEST_WHEEL_VERSION}*")
    ADD_CUSTOM_TARGET(test-wheel-upload
      COMMAND bash -c "echo -e 'uploading test wheels:\\n' $(ls ${TULIP_PYTHON_ROOT_FOLDER}/dist/${TEST_WHEEL_FILES_REGEXP})"
      COMMAND ${TWINE} upload -r testpypi dist/${TEST_WHEEL_FILES_REGEXP}
      WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER} VERBATIM)
  ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

