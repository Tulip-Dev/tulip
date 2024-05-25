SET(TULIP_PYTHON_SITE_INSTALL OFF CACHE BOOL "Do you want to install Tulip Python modules in a Python standard module folder on your system ?
The selected folder path will be the first in the list returned by site.getsitepackages() whose prefix equals ${CMAKE_INSTALL_PREFIX}.
If no such folder is found, the path will be the one returned by site.getusersitepackages().
This should only be used when packaging Tulip for a Linux distribution or MSYS2. [OFF|ON]")

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  SET(PYTHON_COMPONENTS Interpreter Development.Module)
ELSE(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  SET(PYTHON_COMPONENTS Interpreter Development)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

FIND_PACKAGE(Python 3.8...<3.13 REQUIRED COMPONENTS ${PYTHON_COMPONENTS})

SET(PYTHON_VERSION_NO_DOT ${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
SET(PYTHON_VERSION ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR})

IF(TULIP_PYTHON_SITE_INSTALL)

  EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -c "
import site
import sys
from distutils.sysconfig import get_python_lib
py_version = str(sys.version_info[0]) + '.' + str(sys.version_info[1])
for path in site.getsitepackages():
  # check that we select a valid install path
  if path.startswith('${CMAKE_INSTALL_PREFIX}') and py_version in path:
    # avoid to install in /usr/local when CMAKE_INSTALL_PREFIX is /usr on debian
    if '${CMAKE_INSTALL_PREFIX}' == '/usr' and '/usr/local' in path:
      continue
    print(path)
    exit()
print(site.getusersitepackages())
"
                  OUTPUT_VARIABLE TulipPythonModulesInstallDir)
  STRING(REPLACE "\n" "" TulipPythonModulesInstallDir "${TulipPythonModulesInstallDir}")

ELSE(TULIP_PYTHON_SITE_INSTALL)
  SET(TulipPythonModulesInstallDir ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir}/tulip/python)
ENDIF(TULIP_PYTHON_SITE_INSTALL)

MACRO(TULIP_DISABLE_COMPILER_WARNINGS_PYTHON)
    TULIP_SET_CXX_COMPILER_FLAG("-Wno-old-style-cast -Wno-deprecated-copy -Wno-unused-variable -Wno-overloaded-virtual")
    TULIP_SET_C_COMPILER_FLAG("-Wno-old-style-cast -Wno-deprecated-copy -Wno-unused-variable -Wno-overloaded-virtual")
ENDMACRO(TULIP_DISABLE_COMPILER_WARNINGS_PYTHON)

# When building tulip wheels we make a loop of cmake builds,
# with only a change of Python_EXECUTABLE CMake variable;
# so we need to unset the previous values of the CMake Python cache variables
# to force their recomputation
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  UNSET(Python_FOUND CACHE)
  UNSET(Python_LIBRARIES CACHE)
  UNSET(Python_INCLUDE_DIRS CACHE)
ENDIF()

# resolve possible symlinks
# this may be needed when Python_EXECUTABLE is defined
# on the cmake configuration command line
GET_FILENAME_COMPONENT(Python_EXECUTABLE ${Python_EXECUTABLE} REALPATH)

GET_FILENAME_COMPONENT(PYTHON_HOME_PATH ${Python_EXECUTABLE} DIRECTORY)

# Ensure the detection of Python library installed through a bundle downloaded from Python.org or through a macports installation
IF(APPLE)
  IF(NOT "${Python_EXECUTABLE}" MATCHES "^/usr/bin/python.*$")
    EXECUTE_PROCESS(COMMAND bash -c "cd ${PYTHON_HOME_PATH}/.. > /dev/null; echo -n $(pwd)" OUTPUT_VARIABLE PYTHON_DIR_PATH)
    SET(CMAKE_PREFIX_PATH ${PYTHON_HOME_PATH}/.. ${CMAKE_PREFIX_PATH})
  ENDIF()
ENDIF(APPLE)

# Ensure that correct Python include path is selected by CMake on Windows
IF(WIN32)
  SET(CMAKE_INCLUDE_PATH ${PYTHON_HOME_PATH}/include ${CMAKE_INCLUDE_PATH})
  # Ensure that correct Python include path and library are selected by CMake on Linux (in case of non standard installation)
ELSEIF(LINUX)
  SET(CMAKE_INCLUDE_PATH ${PYTHON_HOME_PATH}/../include ${CMAKE_INCLUDE_PATH})
  SET(CMAKE_LIBRARY_PATH ${PYTHON_HOME_PATH}/../lib ${CMAKE_LIBRARY_PATH})
ENDIF(WIN32)

IF(MINGW)
  # Check if Python is provided by MSYS2 (it is compiled with GCC in that case instead of MSVC)
  EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -VV OUTPUT_VARIABLE PYTHON_VERSION_FULL ERROR_VARIABLE PYTHON_VERSION_FULL)
  STRING(REGEX MATCH "GCC" MSYS2_PYTHON "${PYTHON_VERSION_FULL}")

  # Python 64bits does not provide a dll import library for MinGW.
  # Fortunately, we can directly link to the Python dll with that compiler.
  # So find the location of that dll and overwrite the Python_LIBRARIES CMake cache variable with it

  IF(MSYS2_PYTHON)
    IF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll)
      SET(Python_LIBRARIES ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll CACHE FILEPATH "" FORCE)
    ELSEIF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}m.dll)
      SET(Python_LIBRARIES ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}m.dll CACHE FILEPATH "" FORCE)
    ENDIF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll)
  ELSE(MSYS2_PYTHON)
    # Check if the Python dll is located in the Python home directory (when Python is installed for current user only)
    IF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
      SET(Python_LIBRARIES ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
      #If not, the Python dll is located in %WINDIR%/System32 (when Python is installed for all users)
    ELSE(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
      STRING(REPLACE "\\" "/" WINDIR $ENV{WINDIR})
      IF(NOT WIN_AMD64 OR X64)
        SET(Python_LIBRARIES ${WINDIR}/System32/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
      ELSE(NOT WIN_AMD64 OR X64)
        SET(Python_LIBRARIES ${WINDIR}/SysWOW64/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
      ENDIF(NOT WIN_AMD64 OR X64)
    ENDIF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
  ENDIF(MSYS2_PYTHON)
ENDIF(MINGW)

# Ensure headers correspond to the ones associated to the detected Python library on MacOS
IF(APPLE AND NOT "${Python_EXECUTABLE}" MATCHES "^/usr/bin/python.*$"
   AND EXISTS ${PYTHON_HOME_PATH}/../Headers)
  SET(Python_INCLUDE_DIRS ${PYTHON_HOME_PATH}/../Headers CACHE PATH "" FORCE)
ENDIF()

SET(SIP_BUILD ${Python_EXECUTABLE} -m sipbuild.tools.build) #instead of sip-build
SET(SIP_VERSION 6.8.3)
SET(SIP_API 13.7)
SET(SIP_API_FULL 13.7.0)
#check if sip is installed (it is up to the user to install it)
#use the detected python interpreter to call sip instead of the command line tool
#to be sure to use the correct version (command line tool may not be in the PATH)
execute_process(
        COMMAND ${SIP_BUILD} --version
        RESULT_VARIABLE EXIT_CODE
        OUTPUT_QUIET
)

if (NOT ${EXIT_CODE} EQUAL 0)
    message(FATAL_ERROR
            "The \"sip\" Python package is not installed. Please install it using a command like this one: \"${Python_EXECUTABLE} -m pip install sip\".")
endif()

#check sip version
EXECUTE_PROCESS(COMMAND ${SIP_BUILD} --version OUTPUT_VARIABLE SIP_MODULE_OUTPUT)
STRING(STRIP ${SIP_MODULE_OUTPUT} SIP_MODULE_OUTPUT)
IF(${SIP_MODULE_OUTPUT} VERSION_GREATER_EQUAL ${SIP_VERSION})
    MESSAGE(STATUS "Found SIP version ${SIP_MODULE_OUTPUT}")
ELSE()
    MESSAGE(FATAL_ERROR "SIP Python package at least version ${SIP_VERSION} not found (found ${SIP_MODULE_OUTPUT}).")
ENDIF()

# sipbuild.module.main not working (does nothing in fact)
# hack to find sip-module which is not available via the Python executable.
# This will be fixed in a future version of SIP (problem reported to the SIP maintener)
# get sip-module possible paths
GET_FILENAME_COMPONENT(PYTHON_EXE_PATH ${Python_EXECUTABLE} DIRECTORY)
EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -m site --user-base OUTPUT_VARIABLE USER_EXE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
IF(LINUX OR APPLE)
  SET(USER_EXE_PATH "${USER_EXE_PATH}/bin")
ELSE()
  SET(USER_EXE_PATH "${USER_EXE_PATH}/../Scripts")
  SET(PYTHON_EXE_PATH "${PYTHON_EXE_PATH}/Scripts")
ENDIF()

FIND_PROGRAM(SIP_MODULE_PROG sip-module HINTS ${USER_EXE_PATH} ${PYTHON_EXE_PATH} REQUIRED)
if(WIN32)
    file(TO_NATIVE_PATH "${SIP_MODULE_PROG}" SIP_MODULE_PROG) #useful?
endif(WIN32)
MESSAGE(STATUS "sip-module found in ${SIP_MODULE_PROG}")


SET(SIP_MODULE tulip.native.sip)
string(REPLACE "." "_" SIP_MODULE_ ${SIP_MODULE})
SET(SIP_LIB sip)

SET(SIP_INCLUDE_DIR ${PROJECT_BINARY_DIR}/thirdparty/sip)
FILE(MAKE_DIRECTORY ${SIP_INCLUDE_DIR})

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  STRING(REGEX REPLACE "[^0-9.]" "" TULIP_PYTHON_WHEEL_VERSION "${Tulip_VERSION}")

  IF(WIN32)
    SET(WHEEL_INSTALL_PATH "\\")
  ELSE(WIN32)
    SET(WHEEL_INSTALL_PATH "/")
  ENDIF(WIN32)

  ADD_CUSTOM_TARGET(wheel
    COMMAND ${Python_EXECUTABLE} setup.py bdist_wheel
    WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})

  # check generation of test wheels
  STRING(COMPARE NOTEQUAL "${TULIP_PYTHON_TEST_WHEEL_SUFFIX}" "" TULIP_GENERATE_TESTPYPI_WHEEL)

  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
    SET(TULIP_PYTHON_TEST_WHEEL_VERSION ${TULIP_PYTHON_WHEEL_VERSION}.${TULIP_PYTHON_TEST_WHEEL_SUFFIX})

    ADD_CUSTOM_TARGET(test-wheel
      COMMAND ${Python_EXECUTABLE} setuptest.py bdist_wheel
            WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})
    ADD_DEPENDENCIES(test-wheel wheel)
  ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)

  IF(NOT LINUX)

    EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -c "import wheel" RESULT_VARIABLE WHEEL_OK OUTPUT_QUIET ERROR_QUIET)
    EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -c "import twine" RESULT_VARIABLE TWINE_OK OUTPUT_QUIET ERROR_QUIET)
    IF(NOT WHEEL_OK EQUAL 0)
      MESSAGE("The 'wheel' Python module has to be installed to generate wheels for tulip modules.")
      MESSAGE("You can install it through the 'pip' tool ($ pip install wheel)")
    ENDIF(NOT WHEEL_OK EQUAL 0)
    IF(NOT TWINE_OK EQUAL 0)
      MESSAGE("The 'twine' Python module has to be installed to upload tulip wheels on PyPi.")
      MESSAGE("You can install it through the 'pip' tool ($ pip install twine)")
    ENDIF(NOT TWINE_OK EQUAL 0)

  ELSE(NOT LINUX)
    # we need PYTHON_INCLUDE_DIR for linux wheels build
    # see bundlers/linux/tulip_python_wheels_manylinux_build.sh
    IF(PYTHON_INCLUDE_DIR)
      SET(Python_INCLUDE_DIRS ${PYTHON_INCLUDE_DIR})
    ENDIF()
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
      EXECUTE_PROCESS(COMMAND bash -c "${PYBIN}/pip uninstall -y auditwheel; cd /tmp; curl -LO ${PROJECT_HOMEPAGE_URL}/code/auditwheel.tar.gz; tar zxvf auditwheel.tar.gz; ${PYBIN}/pip install /tmp/auditwheel")
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
    SET(TWINE ${Python_INCLUDE_DIRS}/../Scripts/twine.exe)
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
##########################################################
#generate the sip module sources and sip.h
SET(SIP_MODULE_SRC ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}.tar.gz)
IF(NOT EXISTS ${SIP_MODULE_SRC} OR NOT EXISTS "${SIP_INCLUDE_DIR}/sip.h")
    MESSAGE(STATUS "Generating SIP module and include file")
    if(WIN32)
        file(TO_NATIVE_PATH ${SIP_MODULE_SRC} SIP_MODULE_SRC)
    endif(WIN32)
    execute_process(
        COMMAND ${SIP_MODULE_PROG} --sdist --abi-version=${SIP_API} --sip-h --target-dir=${SIP_INCLUDE_DIR} ${SIP_MODULE}
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar zxf  ${SIP_MODULE_SRC}
    WORKING_DIRECTORY ${SIP_INCLUDE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
    )
ENDIF()
#######################
#compile the sip module on our own instead of pip (produce faulty binaries on Windows and module is not compiled with gcc)
SET(SIP_PYTHON_MODULE_SRC
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_array.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_core.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_enum.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_descriptors.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_int_convertors.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_object_map.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_threads.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_voidptr.c
)

INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS})

Python_add_library(${SIP_LIB} MODULE WITH_SOABI ${SIP_PYTHON_MODULE_SRC})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES COMPILE_OPTIONS -w)
TARGET_COMPILE_DEFINITIONS(${SIP_LIB} PUBLIC "DYNAMIC_ANNOTATIONS_ENABLED=1")
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES OUTPUT_NAME sip)
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${SIP_LIB})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

#######################"

TULIP_INSTALL_PYTHON_FILES(tulip/native ${SIP_LIB})

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${SIP_LIB})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
