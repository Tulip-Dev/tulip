CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# Remove CMake warnings related to Qt5
IF(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(POLICY CMP0020)

# Remove CMake warnings related to rpath on MacOS
IF(POLICY CMP0042)
  CMAKE_POLICY(SET CMP0042 NEW)
ENDIF(POLICY CMP0042)
IF(POLICY CMP0068)
  CMAKE_POLICY(SET CMP0068 NEW)
ENDIF(POLICY CMP0068)

# enable the use of ccache for Tulip developers to speed up the build process
IF(NOT MSVC)
  SET(TULIP_USE_CCACHE OFF CACHE BOOL "Do you want to use ccache to speed up the build process during Tulip development ? [OFF|ON]")
  IF(TULIP_USE_CCACHE)
    FIND_PROGRAM(CCACHE_PROGRAM ccache)
    IF(CCACHE_PROGRAM)
      SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    ENDIF(CCACHE_PROGRAM)
  ENDIF(TULIP_USE_CCACHE)
ENDIF(NOT MSVC)

PROJECT(tulip)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

#convert CMAKE_INSTALL_PREFIX path into a cmake style path with unix /
#this is to avoid a path with a '\' as the last character which may be seen as
#an escape character (e.g. \")
FILE(TO_CMAKE_PATH "${CMAKE_INSTALL_PREFIX}" CMAKE_INSTALL_PREFIX)

# Force build type to release with debug info if none has been provided
IF(NOT MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
  SET(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
ENDIF(NOT MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "")

## ========================================================
## Build type detection
## ========================================================

IF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  SET(CMAKE_DEBUG_MODE TRUE)
ELSE(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  SET(CMAKE_DEBUG_MODE FALSE)
ENDIF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")

## ========================================================
## Check processor architecture
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_SIZEOF_VOID_P}" "8" X86_64)
IF(X86_64)
  SET(PROCESSOR_ARCHITECTURE "64")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DX86_64")
ELSE(X86_64)
  SET(PROCESSOR_ARCHITECTURE "32")
ENDIF(X86_64)

## ========================================================
## Consider *BSD as Linux
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" LINUX)
IF(NOT LINUX)
  STRING(FIND "${CMAKE_SYSTEM_NAME}" "BSD" BSD_POS)
  IF(BSD_POS GREATER -1)
    SET(LINUX TRUE)
    SET(BSD TRUE)
  ENDIF()
ENDIF(NOT LINUX)

## ========================================================
## Operating system preprocessor macros
## ========================================================
IF(LINUX)
  ADD_DEFINITIONS("-D_LINUX")
ENDIF(LINUX)
IF(WIN32)
  ADD_DEFINITIONS("-D_WIN32")
  # ensure WIN32 is defined (as it is not the case when compiling with MinGW and C++11 standard activated)
  ADD_DEFINITIONS("-DWIN32")
  # ensure math defines (e.g. M_PI) are available (as they have been dropped from C++11 standard)
  ADD_DEFINITIONS("-D_USE_MATH_DEFINES")
ENDIF(WIN32)
IF(APPLE)
  ADD_DEFINITIONS("-D__APPLE__")
ENDIF(APPLE)

## ========================================================
## Install paths
## ========================================================

INCLUDE(GNUInstallDirs)

# workaround an install issue on Debian when using CMake < 3
# see https://github.com/Kitware/CMake/commit/620939e4e6f5a61cd5c0fac2704de4bfda0eb7ef
IF(LINUX AND CMAKE_VERSION VERSION_LESS 3.0)
  IF(EXISTS "/etc/debian_version" AND NOT "${CMAKE_INSTALL_PREFIX}" MATCHES "^/usr/?$")
    SET(CMAKE_INSTALL_LIBDIR lib)
  ENDIF(EXISTS "/etc/debian_version" AND NOT "${CMAKE_INSTALL_PREFIX}" MATCHES "^/usr/?$")
ENDIF(LINUX AND CMAKE_VERSION VERSION_LESS 3.0)

SET(TulipLibInstallDir ${CMAKE_INSTALL_LIBDIR})
SET(TulipPluginsInstallDir ${TulipLibInstallDir}/tulip)
SET(TulipGlyphPluginsInstallDir ${TulipPluginsInstallDir}/glyph)
SET(TulipViewPluginsInstallDir ${TulipPluginsInstallDir}/view)
SET(TulipInteractorPluginsInstallDir ${TulipPluginsInstallDir}/interactor)
SET(TulipPerspectivePluginsInstallDir ${TulipPluginsInstallDir}/perspective)
SET(TulipBinInstallDir ${CMAKE_INSTALL_BINDIR})
SET(TulipIncludeInstallDir ${CMAKE_INSTALL_INCLUDEDIR})
SET(TulipShareInstallDir ${CMAKE_INSTALL_DATADIR}/tulip)
SET(TulipBitmapInstallDir ${TulipShareInstallDir}/bitmaps)

## ========================================================
## Tulip CMake macros
## ========================================================
INCLUDE(cmake/TulipUseFile.cmake)

## ========================================================
## Tulip version variables
## ========================================================
SET(TulipMajorVersion 5)
SET(TulipMinorVersion 2)
SET(TulipReleaseVersion 0)
SET(TulipVersionSeparator .)
IF(MSVC) #specific options for Visual Studio
  SET(TulipVersionSeparator _)
ENDIF()
SET(TulipVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}${TulipVersionSeparator}${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}")
SET(TulipMinorVersionTwoDigits "${TulipMinorVersion}")
# Use two digits for the minor version when generating the Tulip integer version number
# as there was a 4.10 release that breaks preprocessor test like TULIP_INT_VERSION >= 4100 since the 5.0 release
# (see https://sourceforge.net/p/auber/bugs/869/)
STRING(LENGTH "${TulipMinorVersion}" TulipMinorVersionNbDigits)
IF(TulipMinorVersionNbDigits LESS 2)
  SET(TulipMinorVersionTwoDigits "0${TulipMinorVersion}")
ENDIF(TulipMinorVersionNbDigits LESS 2)
SET(TulipIntMMVersion "${TulipMajorVersion}${TulipMinorVersionTwoDigits}")
IF(TulipReleaseVersion MATCHES "^[0-9]+$")
  SET(TulipIntVersion "${TulipMajorVersion}${TulipMinorVersionTwoDigits}${TulipReleaseVersion}")
ELSE(TulipReleaseVersion MATCHES "^[0-9]+$")
  # just a hack to force the display of the full Tulip version in window title bar
  # when it contains non digit characters (like alpha, beta, dev, ...)
  SET(TulipIntVersion "${TulipMajorVersion}${TulipMinorVersion}1")
  INCLUDE(cmake/GetGitRevisionDescription.cmake)
  IF(EXISTS ${PROJECT_SOURCE_DIR}/.git AND NOT TULIP_BUILD_FOR_APPIMAGE)
    SET(TULIP_SOURCE_DIR ${PROJECT_SOURCE_DIR})
    GET_GIT_HEAD_REVISION(TULIP_REFSPEC TULIP_GIT_REVISION)
    CONFIGURE_FILE(utils/cmake-templates/GIT_COMMIT.in ${CMAKE_CURRENT_BINARY_DIR}/GIT_COMMIT)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/GIT_COMMIT
            DESTINATION ${TulipShareInstallDir})
  ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/.git AND NOT TULIP_BUILD_FOR_APPIMAGE)
ENDIF(TulipReleaseVersion MATCHES "^[0-9]+$")

## ========================================================
## Tulip libraries names
## ========================================================
SET(LibTulipCoreName "tulip-core-${TulipMMVersion}")
SET(LibTulipOGLName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipGUIName "tulip-gui-${TulipMMVersion}")
SET(LibTulipPythonName "tulip-python-${TulipMMVersion}")
SET(LibTulipOGDFName "tulip-ogdf-${TulipMMVersion}")
SET(LibStlPythonBindingsName "stl-python-bindings")
SET(LibTulipCorePythonBindingsName "tulip-core-python-bindings")
SET(LibTulipGUIPythonBindingsName "tulip-gui-python-bindings")
SET(TulipLibs ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${LibTulipOGDFName})

## ========================================================
## Current year (used in documentation config files)
## ========================================================
STRING(TIMESTAMP CurrentYear "%Y")

## ========================================================
## Compiler-specific options
## ========================================================

TULIP_SET_COMPILER_OPTIONS()

## ========================================================
## CORE ONLY
## this used to have a minimal build of Tulip without
## any of the opengl, qt or python components
## ========================================================
IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/cmake/FindQtX.cmake OR EMSCRIPTEN)
  SET(TULIP_BUILD_CORE_ONLY ON)
  MESSAGE("You can only configure a core build of Tulip.")
ELSE()
  SET(TULIP_BUILD_CORE_ONLY OFF CACHE BOOL "Do you want a minimal build of Tulip (no OpenGL rendering, no Qt gui, no documentation) ? [ON|OFF]")
ENDIF()

IF(TULIP_BUILD_CORE_ONLY AND LINUX)
# the macro ADD_CORE_FILES will be used
# to collect the files needed to build a minimal Tulip
# in the directory pointed by CMAKE_CURRENT_SOURCE_DIR
  MACRO(ADD_CORE_FILES files)
    SET(CURRENT_SOURCE_DIR ".")
    IF(${CMAKE_CURRENT_SOURCE_DIR} STRGREATER ${PROJECT_SOURCE_DIR})
      EXECUTE_PROCESS(COMMAND echo -n ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMAND sed "s,${PROJECT_SOURCE_DIR}/,,"
                      OUTPUT_VARIABLE CURRENT_SOURCE_DIR)
    ENDIF()
    IF(${files} STREQUAL ".")
      SET(ENV{CORE_FILES} "$ENV{CORE_FILES} \$(find ${CURRENT_SOURCE_DIR} -path \"*/.svn\" -prune -o -type f -print)")
    ELSE()
      FOREACH(core_file ${ARGV})
        SET(ENV{CORE_FILES} "$ENV{CORE_FILES} ${CURRENT_SOURCE_DIR}/${core_file}")
      ENDFOREACH()
    ENDIF()
  ENDMACRO()
  ADD_CORE_FILES(CMakeLists.txt cmake/cmake_uninstall.cmake.in cmake/TulipUseFile.cmake cmake/FindCppUnit.cmake cmake/FindTulip.cmake)
# the macro ADD_CORE_DIRECTORY_EXCEPT will be used
# to exclude the files not needed to build a minimal Tulip
# in the directory pointed by CMAKE_CURRENT_SOURCE_DIR
  MACRO(ADD_CORE_DIRECTORY_EXCEPT files)
    SET(CURRENT_SOURCE_DIR ".")
    IF(${CMAKE_CURRENT_SOURCE_DIR} STRGREATER ${PROJECT_SOURCE_DIR})
      EXECUTE_PROCESS(COMMAND echo -n ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMAND sed "s,${PROJECT_SOURCE_DIR}/,,"
                      OUTPUT_VARIABLE CURRENT_SOURCE_DIR)
    ENDIF()
    SET(find_command "\$(find ${CURRENT_SOURCE_DIR} -path \"*/.svn\" -prune -o -type f")
    FOREACH(exclude_file ${ARGV})
      SET(find_command "${find_command} -a ! -name \"${exclude_file}\"")
    ENDFOREACH()
    SET(ENV{CORE_FILES} "$ENV{CORE_FILES} ${find_command} -print)")
  ENDMACRO()
ELSE()
  MACRO(ADD_CORE_FILES files)
  ENDMACRO()
  MACRO(ADD_CORE_DIRECTORY_EXCEPT files)
  ENDMACRO()
ENDIF()

SET(TULIP_BUILD_PYTHON_COMPONENTS ON CACHE BOOL "Do you want to build Tulip Python components ? [ON|OFF]")
IF(TULIP_BUILD_PYTHON_COMPONENTS)
  SET(TULIP_PYTHON_ROOT_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-core/tulip_module)
  SET(TULIP_PYTHON_FOLDER ${TULIP_PYTHON_ROOT_FOLDER}/tulip)
  SET(TULIPGUI_PYTHON_ROOT_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-gui/tulipgui_module)
  SET(TULIPGUI_PYTHON_FOLDER ${TULIPGUI_PYTHON_ROOT_FOLDER}/tulipgui)
  SET(TULIP_PYTHON_NATIVE_FOLDER ${TULIP_PYTHON_FOLDER}/native)
  SET(TULIPGUI_PYTHON_NATIVE_FOLDER ${TULIPGUI_PYTHON_FOLDER}/native)
  SET(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET OFF CACHE BOOL "Add a wheel make target for generating Tulip Python module wheel ? [ON|OFF]")
  IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET AND APPLE)
    # build universal binaries when generating Tulip Python wheels for MacOs
    SET(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
    # build binaries with install rpath when generating Tulip Python wheels for MacOs
    # as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  ELSEIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET AND LINUX)
    FILE(READ /etc/issue LINUX_ISSUE)
    IF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT EXISTS /usr/local/bin/auditwheel)
      MESSAGE(FATAL_ERROR "Python binary wheels for Linux platform must be compiled using the CentOS 5.11 docker image provided by the manylinux project from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
    ENDIF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT EXISTS /usr/local/bin/auditwheel)
    # build binaries with install rpath when generating Tulip Python wheels for Linux
    # as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET AND APPLE)
ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)

## ========================================================
## Required packages
## ========================================================
IF(MINGW)
  # Set some common libraries and headers search paths for MinGW
  SET(CMAKE_LIBRARY_PATH "${MINGW_BIN_PATH};${MINGW_LIB_PATH};${MINGW_LIB64_PATH};${CMAKE_LIBRARY_PATH}")
  SET(CMAKE_INCLUDE_PATH "${MINGW_INCLUDE_PATH};${MINGW_INCLUDE_PATH}/freetype2;${MINGW_INCLUDE_PATH}/libpng*;${CMAKE_INCLUDE_PATH}")

  # Check if libintl is present in the MinGW build environment
  FIND_LIBRARY(INTL_LIBRARY intl PATHS ${MINGW_LIB_PATH} ${MINGW_LIB64_PATH} NO_DEFAULT_PATH)
ELSE(MINGW)
  FIND_LIBRARY(INTL_LIBRARY intl)
ENDIF(MINGW)
MARK_AS_ADVANCED(INTL_LIBRARY)

# Don't need to find zlib when compiling with emscripten as there is an available emscripten port of it
IF(NOT EMSCRIPTEN)

  # ZLIB
  FIND_PACKAGE(ZLIB REQUIRED)

ENDIF(NOT EMSCRIPTEN)

# Qhull
SET(TULIP_USE_THIRDPARTY_QHULL OFF CACHE BOOL "If activated, force the use of reentrant qhull library located in thirdparty instead of trying to find it on the system.")
IF(NOT TULIP_USE_THIRDPARTY_QHULL)
  FIND_PACKAGE(Qhull)
ELSE(NOT TULIP_USE_THIRDPARTY_QHULL)
  UNSET(QHULL_FOUND CACHE)
ENDIF(NOT TULIP_USE_THIRDPARTY_QHULL)


#Yajl
FIND_PACKAGE(Yajl)

# OpenMP (only available with clang starting the 3.7 version with libomp installed)
IF(NOT CLANG OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.7.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.7.0)

  FIND_PACKAGE(Threads)
  IF (NOT CMAKE_BUILD_TYPE STREQUAL "")
    IF(CMAKE_DEBUG_MODE)
      OPTION(TULIP_ENABLE_MULTI_THREADING "Do you want to enable multithreaded code (debug mode)?" OFF)
    ELSE()
      OPTION(TULIP_ENABLE_MULTI_THREADING "Do you want to enable multithreaded code?" ON)
    ENDIF()
    IF(TULIP_ENABLE_MULTI_THREADING)
      # TULIP_CXX_THREADS can be set to force the use of the cxx threads
      # regardless the OpenMP availability
      IF(NOT TULIP_CXX_THREADS)
	FIND_PACKAGE(OpenMP)
	IF(OPENMP_FOUND)
          SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
          SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS}")
          SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS}")
          SET(OPENMP_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
          IF(WIN32)
            IF(MSVC)
              SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /openmp")
              SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /openmp")
              SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /openmp")
              SET(OPENMP_CXX_FLAGS "/openmp")
            ELSE()
              SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp ${CMAKE_THREAD_LIBS_INIT}")
              SET(OPENMP_LIBRARIES "-lgomp -lpthread")
            ENDIF()
          ENDIF()
	ELSE(OPENMP_FOUND)
          IF(WIN32)
            STRING(COMPARE NOTEQUAL "${OpenMP_C_FLAGS}" "" OMP_CFLAGS)
            IF(OMP_CFLAGS)
              # Force setting OpenMP flags on Windows platforms
              SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_C_FLAGS}")
              SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_C_FLAGS}")
              SET(OPENMP_CXX_FLAGS "${OpenMP_C_FLAGS}")
              IF(NOT MSVC)
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
		SET(OPENMP_LINKER_FLAGS "${OpenMP_C_FLAGS}")
		SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp -lpthread")
		SET(OPENMP_LIBRARIES "-lgomp -lpthread")
              ENDIF(NOT MSVC)
              SET(OPENMP_FOUND TRUE)
            ELSE(OMP_CFLAGS)
              MESSAGE("OpenMP not found: multithreaded code will use c++ threads")
            ENDIF(OMP_CFLAGS)
          ELSE(WIN32)
            MESSAGE("OpenMP not found: multithreaded code will use c++ threads")
          ENDIF(WIN32)
	ENDIF(OPENMP_FOUND)
      ELSE(NOT TULIP_CXX_THREADS)
	MESSAGE("Multithreaded code will use c++ threads")
      ENDIF(NOT TULIP_CXX_THREADS)
    ELSE(TULIP_ENABLE_MULTI_THREADING)
      MESSAGE("Multithreaded code is disabled")
      SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DTLP_NO_THREADS")
      SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DTLP_NO_THREADS")
      SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTLP_NO_THREADS")
    ENDIF(TULIP_ENABLE_MULTI_THREADING)
  ENDIF(NOT CMAKE_BUILD_TYPE STREQUAL "")
ENDIF(NOT CLANG OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.7.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.7.0)

IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  # OpenGL
  FIND_PACKAGE(OpenGL REQUIRED)

  # Glew
  FIND_PACKAGE(GLEW REQUIRED)

  # Jpeg
  FIND_PACKAGE(JPEG REQUIRED)

  # Png
  FIND_PACKAGE(PNG REQUIRED)

  # Freetype
  FIND_PACKAGE(Freetype REQUIRED)

  # Qt
  FIND_PACKAGE(QtX)

  # Quazip
  FIND_PACKAGE(QuaZip)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)


IF(TULIP_BUILD_PYTHON_COMPONENTS)

  SET(TULIP_PYTHON_SITE_INSTALL OFF CACHE BOOL "Do you want to install Tulip Python modules in a Python standard module folder on your system ?
The selected folder path will be the first in the list returned by site.getsitepackages() whose prefix equals ${CMAKE_INSTALL_PREFIX}.
If no such folder is found, the path will be the one returned by site.getusersitepackages().
This should only be used when packaging Tulip for a Linux distribution or MSYS2. [OFF|ON]")

  # Python
  # After finding the Python interpreter, try to find if SIP and its dev tools are installed on the host system.
  # If not, compile the SIP version located in thirdparty.
  FIND_PACKAGE(PythonInterp REQUIRED)
  IF(PYTHONINTERP_FOUND)
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

    IF(PYTHONLIBS_FOUND)

      # Check if Python is provided by MSYS2 (it is compiled with GCC in that case instead of MSVC)
      EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(sys.version)" OUTPUT_VARIABLE PYTHON_VERSION_FULL ERROR_VARIABLE PYTHON_VERSION_FULL)
      STRING(REGEX MATCH "GCC" MSYS2_PYTHON "${PYTHON_VERSION_FULL}")

      # Python 64bits does not provide a dll import library for MinGW.
      # Fortunately, we can directly link to the Python dll with that compiler.
      # So find the location of that dll and overwrite the PYTHON_LIBRARY CMake cache variable with it
      IF(MINGW)
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

      SET(SIP_VERSION_THIRDPARTY 4.19.12)
      SET(SIP_OK FALSE CACHE INTERNAL "")
      FIND_PACKAGE(SIP)
      SET(SIP_OK ${SIP_FOUND})
      IF(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})
        SET(SIP_EXE "${SIP_EXECUTABLE}")
        SET(SYSTEM_SIP TRUE)
      ELSE(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})
        IF(SIP_OK)
          MESSAGE(STATUS "SIP was found on the system but its version is lesser than the required one (${SIP_VERSION_THIRDPARTY}).")
          SET(SIP_FOUND FALSE)
        ENDIF(SIP_OK)
        MESSAGE(STATUS "Using SIP ${SIP_VERSION_THIRDPARTY} located in thirdparty for generating the Python bindings.")
        SET(SIP_LIB sip)
        SET(SYSTEM_SIP FALSE)

        TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen sip
          CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
          -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} -DPYTHON_INCLUDE_DIR2=${PYTHON_INCLUDE_DIR2}
          -DPYTHON_INCLUDE_PATH=${PYTHON_INCLUDE_PATH} -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
          -DSIP_LIB=${SIP_LIB})

        IF(SIP_OK)
          SET(SIP_VERSION_STR "${SIP_VERSION_THIRDPARTY}")
          IF(WIN32 AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/Debug/sip.exe")
            SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/Debug/sip.exe)
          ELSE()
            SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/sip)
          ENDIF()
          SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/siplib/)
        ENDIF(SIP_OK)
      ENDIF(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})

      IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
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

          ADD_CUSTOM_TARGET(wheel
            COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
            WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})

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
          ADD_CUSTOM_TARGET(wheel
            COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
            COMMAND bash -c "auditwheel repair -L native -w ./dist ./dist/$(ls -t ./dist/ | head -1)"
            WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER} VERBATIM)

        ENDIF(NOT LINUX)

        # In order to upload the generated wheels, an account must be created on PyPi
        # and the following configuration must be stored in the ~/.pypirc file
        ##############################################################
        # [distutils]
        # index-servers=
        #     pypi
        #     testpypi
        #
        # [pypitest]
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
        SET(WHEEL_FILES_REGEXP "*")
        IF(LINUX)
          SET(WHEEL_FILES_REGEXP "*manylinux*")
        ENDIF(LINUX)
        ADD_CUSTOM_TARGET(wheel-upload-test
          COMMAND ${TWINE} upload -r pypitest dist/${WHEEL_FILES_REGEXP}
          WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})
        ADD_CUSTOM_TARGET(wheel-upload
          COMMAND ${TWINE} upload -r pypi dist/${WHEEL_FILES_REGEXP}
          WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})

      ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

    ENDIF(PYTHONLIBS_FOUND)
  ENDIF(PYTHONINTERP_FOUND)
ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)

## ========================================================
## Include and library paths
## ========================================================
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(GZStreamLibrary gzstream-tulip-${TulipMMVersion})
IF(NOT TULIP_BUILD_CORE_ONLY)
  IF(QUAZIP_FOUND)
    SET(QuazipInclude ${QUAZIP_INCLUDE_DIRS})
    SET(QuazipLibrary ${QUAZIP_LIBRARIES})
  ELSE(QUAZIP_FOUND)
    SET(QuazipInclude ${PROJECT_SOURCE_DIR}/thirdparty/quazip/)
    SET(QuazipLibrary quazip-tulip-${TulipMMVersion})
  ENDIF(QUAZIP_FOUND)

  SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
  SET(FTGLLibrary ftgl-tulip-${TulipMMVersion})
  SET(Tess2Include ${PROJECT_SOURCE_DIR}/thirdparty/libtess2/Include)
  SET(Tess2Library tess2-tulip-${TulipMMVersion})
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF/include)
SET(OGDFLibrary OGDF-tulip-${TulipMMVersion})

IF(QHULL_FOUND)
  SET(QhullInclude ${QHULL_INCLUDE_DIRS})
  SET(QhullLibrary ${QHULL_LIBRARIES})
ELSE(QHULL_FOUND)
  SET(HAVE_REENTRANT_QHULL TRUE)
  SET(QhullInclude ${PROJECT_SOURCE_DIR}/thirdparty/libqhull)
  SET(QhullLibrary qhullstatic)
ENDIF(QHULL_FOUND)

IF(YAJL_FOUND)
  SET(YajlInclude ${YAJL_INCLUDE_DIRS})
  SET(YajlLibrary ${YAJL_LIBRARIES})
ELSE(YAJL_FOUND)
  SET(YajlInclude ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/api ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/yajl/yajl-2.1.0/include)
  SET(YajlLibrary yajl-tulip-${TulipMMVersion})
ENDIF(YAJL_FOUND)

SET(UTF8CppInclude ${PROJECT_SOURCE_DIR}/thirdparty/utf8-cpp/source)

SET(TulipCoreInclude ${PROJECT_SOURCE_DIR}/library/tulip-core/include/)
SET(TulipCoreBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-core/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipOGDFInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogdf/)
SET(TulipGUIInclude ${PROJECT_SOURCE_DIR}/library/tulip-gui/include/)
SET(TulipUIGUIInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/src/)
SET(TulipGUIBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/include/)
SET(TulipPythonInclude ${PROJECT_SOURCE_DIR}/library/tulip-python/include)

## ========================================================
## Point RPATH to lib folder
## ========================================================
# On MacOS, use rpaths (i.e. the "install_name" field of shared libraries will be @rpath)
# This is already enabled by default for CMake >= 3.0 (see http://www.cmake.org/cmake/help/v3.0/prop_tgt/MACOSX_RPATH.html)
SET(CMAKE_MACOSX_RPATH ON)
FUNCTION(ADD_LIBRARY name)
  _ADD_LIBRARY(${name} ${ARGN})
  IF(APPLE)
    # relative locations of Tulip dylibs dependencies are not the sames when generating Python wheels
    IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/;@loader_path/../")
    ELSE(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../Frameworks;${QT_FRAMEWORKS_DIR}")
    ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ELSE(APPLE)
    # CMake >= 3.1.0 does not allow to set the INSTALL_RPATH property on INTERFACE_LIBRARY target type
    GET_TARGET_PROPERTY(TARGET_TYPE ${name} TYPE)
    IF(NOT "${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN:$ORIGIN/../:$ORIGIN/../../")
      IF(MSVC)
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}")
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
        # MSVC linker PDB files are only generated for shared libraries
        IF("${ARGV1}" STREQUAL "SHARED")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_DEBUG "${name}")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_RELWITHDEBINFO "${name}")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
          INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TulipShareInstallDir}/pdb CONFIGURATIONS Debug)
          INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TulipShareInstallDir}/pdb CONFIGURATIONS RelWithDebInfo)
        ENDIF("${ARGV1}" STREQUAL "SHARED")
      ENDIF(MSVC)
    ENDIF(NOT "${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
  ENDIF(APPLE)
  IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
    ADD_DEPENDENCIES(wheel ${name})
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
ENDFUNCTION()

FUNCTION(ADD_EXECUTABLE name)
  _ADD_EXECUTABLE(${name} ${ARGN})
  IF(APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR};@loader_path/../Frameworks;${QT_FRAMEWORKS_DIR}")
  ELSEIF(LINUX)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
  ELSE()
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN")
  ENDIF()
  IF(MSVC)
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_DEBUG "${name}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_RELWITHDEBINFO "${name}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
    INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TulipShareInstallDir}/pdb CONFIGURATIONS Debug)
    INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TulipShareInstallDir}/pdb CONFIGURATIONS RelWithDebInfo)
  ENDIF(MSVC)
ENDFUNCTION()

## ========================================================
## Tests
## ========================================================
IF(EXISTS ${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
  SET(TULIP_BUILD_TESTS OFF CACHE BOOL "Do you want to build the tests ? [OFF|ON]")
ELSE()
  SET(TULIP_BUILD_TESTS OFF)
ENDIF()

## ========================================================
## Create library/tulip/include/tulip/TulipRelease.h base on TulipRelease.h.in
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "Release" result1)
STRING(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "RelWithDebInfo" result2)
IF(${result1} OR ${result2})
  SET(TulipBuildIsRelease 1)
ELSE()
  SET(TulipBuildIsRelease 0)
ENDIF()
CONFIGURE_FILE("library/tulip-core/include/tulip/TulipRelease.h.in" "library/tulip-core/include/tulip/TulipRelease.h")

## ========================================================
## Configure file for uninstall target
## ========================================================

IF(LINUX)
  IF("${CMAKE_INSTALL_PREFIX}" MATCHES "^/home.*$")
    SET(DESKTOP_CONFIG_INSTALL_PREFIX "$ENV{HOME}/.local")
  ELSE("${CMAKE_INSTALL_PREFIX}" MATCHES "^/home.*$")
    SET(DESKTOP_CONFIG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  ENDIF("${CMAKE_INSTALL_PREFIX}" MATCHES "^/home.*$")
ENDIF(LINUX)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake VERBATIM)

ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(plugins)
IF(NOT TULIP_BUILD_CORE_ONLY)
  ADD_SUBDIRECTORY(doc)
  IF(NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
    ADD_SUBDIRECTORY(textures)
    ADD_SUBDIRECTORY(externalplugins)
    ADD_SUBDIRECTORY(demos)
    ADD_SUBDIRECTORY(software)
  ENDIF(NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

IF(TULIP_BUILD_TESTS)
  INCLUDE(CTest)
  ENABLE_TESTING()
  IF(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure --build-config "$<CONFIGURATION>")
  ELSE(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure)
  ENDIF(CMAKE_CONFIGURATION_TYPES)
  ADD_SUBDIRECTORY(tests)
ENDIF(TULIP_BUILD_TESTS)

INSTALL(FILES AUTHORS
              COPYING.LESSER
        DESTINATION ${TulipShareInstallDir})

IF(NOT TULIP_BUILD_FOR_APPIMAGE)
INSTALL(FILES cmake/FindTULIP.cmake
        RENAME TULIPConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TULIP/)

CONFIGURE_FILE(cmake/TULIPConfigVersion.cmake.in ${CMAKE_BINARY_DIR}/cmake/TULIPConfigVersion.cmake @ONLY)

INSTALL(FILES cmake/TulipUseFile.cmake
              ${CMAKE_BINARY_DIR}/cmake/TULIPConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TULIP/)
ENDIF(NOT TULIP_BUILD_FOR_APPIMAGE)

IF(NOT TULIP_BUILD_CORE_ONLY)

  ADD_SUBDIRECTORY(bundlers)
#  ADD_SUBDIRECTORY(deploy)

  ## ========================================================
  ## tulip-config script
  ## ========================================================
  SET(prefix "${CMAKE_INSTALL_PREFIX}")
  SET(exec_prefix "\${prefix}")
  SET(includedir "\${prefix}/${TulipIncludeInstallDir}")
  SET(libdir "\${prefix}/${TulipLibInstallDir}")
  SET(libversion "${TulipMMVersion}")
  SET(VERSION "${TulipVersion}")
  SET(WIN_VERSION "${TulipMajorVersion}.${TulipMinorVersion}")
  SET(QT_CPPFLAGS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")
  IF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
    SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
  ENDIF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
  IF(WIN32)
    SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY}")
  ELSE(WIN32)
    SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY}")
  ENDIF(WIN32)
  SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE} ${QT_QTWEBKIT_LIBRARY_RELEASE}")

  CONFIGURE_FILE("tulip-config.in" "tulip-config" @ONLY)

IF(NOT TULIP_BUILD_FOR_APPIMAGE)
  INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config DESTINATION ${TulipBinInstallDir})
ENDIF(NOT TULIP_BUILD_FOR_APPIMAGE)

  IF(MSVC)
    INCLUDE(InstallRequiredSystemLibraries)
    IF(OPENMP_FOUND)
      IF(MSVC90)
        INSTALL(PROGRAMS "${MSVC90_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC90.OPENMP/vcomp90.dll" DESTINATION ${TulipBinInstallDir})
      ENDIF(MSVC90)
      IF(MSVC10)
        INSTALL(PROGRAMS "${MSVC10_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC100.OPENMP/vcomp100.dll" DESTINATION ${TulipBinInstallDir})
      ENDIF(MSVC10)
      IF(MSVC11)
        INSTALL(PROGRAMS "${MSVC11_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC110.OPENMP/vcomp110.dll" DESTINATION ${TulipBinInstallDir})
      ENDIF(MSVC11)
      IF(MSVC12)
        INSTALL(PROGRAMS "${MSVC12_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC120.OPENMP/vcomp120.dll" DESTINATION ${TulipBinInstallDir})
      ENDIF(MSVC12)
      IF(MSVC14 AND MSVC_VERSION EQUAL 1900)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC140.OPENMP/vcomp140.dll" DESTINATION ${TulipBinInstallDir})
      # for Visual Studio 15.0 2017
      ELSEIF(MSVC14 AND MSVC_VERSION EQUAL 1910)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC150.OPENMP/vcomp140.dll" DESTINATION ${TulipBinInstallDir})
      ELSEIF(MSVC14 AND MSVC_VERSION EQUAL 1912)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC141.OPENMP/vcomp140.dll" DESTINATION ${TulipBinInstallDir})
      ENDIF()
    ENDIF(OPENMP_FOUND)
  ENDIF(MSVC)

  IF(WIN32)
    # initialize the list of bundle libs (second arg of FIXUP_BUNDLE),
    # updated in plugins/view/GeographicView,
    # and finally used in software/tulip_perpective/CMakeLists.txt
    SET_PROPERTY(GLOBAL PROPERTY FIXUP_BUNDLE_LIBS "")

    GET_FILENAME_COMPONENT(NSIS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\NSIS]" ABSOLUTE)
    STRING(COMPARE EQUAL "${NSIS_PATH}" "" NSIS_PATH_EMPTY)
    IF(NSIS_PATH_EMPTY)
      GET_FILENAME_COMPONENT(NSIS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\NSIS]" ABSOLUTE)
      STRING(COMPARE EQUAL "${NSIS_PATH}" "" NSIS_PATH_EMPTY)
    ENDIF(NSIS_PATH_EMPTY)

    IF(NOT NSIS_PATH_EMPTY)

      IF(NOT TULIP_BUILD_PYTHON_COMPONENTS OR EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

        STRING(REPLACE "/" "\\" NSIS_PATH "${NSIS_PATH}")
        STRING(REPLACE "/" "\\" BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
        STRING(REPLACE "/" "\\" TLP_DIR "${CMAKE_INSTALL_PREFIX}")
        STRING(REPLACE "/" "\\" QTX_PLUGINS_DIR "${QT_PLUGINS_DIR}")

        IF(TulipBuildIsRelease OR MSVC)
          SET(DEBUG_MODE "FALSE")
        ELSE(TulipBuildIsRelease OR MSVC)
          SET(DEBUG_MODE "TRUE")
        ENDIF(TulipBuildIsRelease OR MSVC)

        ADD_CUSTOM_TARGET(bundle
                          COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/win_bundle.bat ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/win_bundle.bat
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/FileAssociation.nsh ${CMAKE_CURRENT_BINARY_DIR}/FileAssociation.nsh
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/COPYING.LESSER ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/COPYING.LESSER
                          COMMAND cmd /C "\"win_bundle.bat \\\"${NSIS_PATH}\\\" \\\"${TLP_DIR}\\\" \\\"${BINARY_DIR}\\\" ${DEBUG_MODE}\""
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win)

      ELSE(NOT TULIP_BUILD_PYTHON_COMPONENTS OR EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

        MESSAGE("NSIS Inetc plugin is required to build the Tulip installer.")
        MESSAGE("Download the plugin archive from http://nsis.sourceforge.net/Inetc_plug-in and extract it in ${NSIS_PATH}.")

      ENDIF(NOT TULIP_BUILD_PYTHON_COMPONENTS OR EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

    ELSE(NOT NSIS_PATH_EMPTY)

      MESSAGE("Nullsoft Scriptable Install System is not installed on your system")
      MESSAGE("Tulip installer can not be generated without Nullsoft NSIS.")

    ENDIF(NOT NSIS_PATH_EMPTY)

  ENDIF(WIN32)

  IF(APPLE)

    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac/mac_bundle.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac/mac_bundle.sh" @ONLY)
    ADD_CUSTOM_TARGET(bundle
                      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                      COMMAND sh mac_bundle.sh ${CMAKE_CURRENT_BINARY_DIR}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac)

  ENDIF(APPLE)

  IF(LINUX)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/linux/make_appimage_bundle.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/bundlers/linux/make_appimage_bundle.sh" @ONLY)
  ENDIF(LINUX)

ELSE(NOT TULIP_BUILD_CORE_ONLY)

  IF(LINUX)
  # generate script to make a tar archive of minimal set of tulip core files
    SET(make_tar_file ${PROJECT_BINARY_DIR}/tar-tulip-${TulipVersion}-corefiles.sh)
    EXECUTE_PROCESS(COMMAND rm ${make_tar_file} OUTPUT_QUIET ERROR_QUIET)
    EXECUTE_PROCESS(COMMAND echo "#!/bin/sh
if [ $# -eq 0 ]; then
TAR_FILE=${PROJECT_BINARY_DIR}/tulip-${TulipVersion}-corefiles.tar.gz
else
TAR_FILE=$1
fi
cd ${PROJECT_SOURCE_DIR}
tar zcvf \${TAR_FILE} $ENV{CORE_FILES}
echo archive \${TAR_FILE} successfully generated" OUTPUT_FILE ${make_tar_file})

    MESSAGE(STATUS "${make_tar_file} successfully generated")
  ENDIF(LINUX)

ENDIF(NOT TULIP_BUILD_CORE_ONLY)
