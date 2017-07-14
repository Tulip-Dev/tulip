CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# Remove CMake warnings related to Qt5
IF(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(POLICY CMP0020)

# Remove CMake warnings related to rpath on MacOS
IF(POLICY CMP0042)
  CMAKE_POLICY(SET CMP0042 NEW)
ENDIF(POLICY CMP0042)

# enable the use of ccache for Tulip developers to speed up the build process
SET(TULIP_USE_CCACHE OFF CACHE BOOL "Do you want to use ccache to speed up the build process during Tulip development ? [OFF|ON]")
IF(TULIP_USE_CCACHE)
  FIND_PROGRAM(CCACHE_PROGRAM ccache)
  IF(CCACHE_PROGRAM)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  ENDIF(CCACHE_PROGRAM)
ENDIF(TULIP_USE_CCACHE)

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
## Tulip CMake macros
## ========================================================
INCLUDE(cmake/TulipUseFile.cmake)

## ========================================================
## Tulip version variables
## ========================================================
SET(TulipMajorVersion 5)
SET(TulipMinorVersion 1)
SET(TulipReleaseVersion "0-dev")
SET(TulipVersionSeparator .)
IF(MSVC) #specific options for Visual Studio
  SET(TulipVersionSeparator _)
ENDIF()
SET(TulipVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}${TulipVersionSeparator}${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}")
SET(TulipIntMMVersion "${TulipMajorVersion}${TulipMinorVersion}")
IF(TulipReleaseVersion MATCHES "^[0-9]+$")
  SET(TulipIntVersion "${TulipMajorVersion}${TulipMinorVersion}${TulipReleaseVersion}")
ELSE(TulipReleaseVersion MATCHES "^[0-9]+$")
  # just a hack to force the display of the full Tulip version in window title bar
  # when it contains non digit characters (like alpha, beta, dev, ...)
  SET(TulipIntVersion "${TulipMajorVersion}${TulipMinorVersion}1")
  # get current Tulip SVN revision for a dev build
  IF("$ENV{JENKINS_URL}" STREQUAL "")
    # not invoked thru jenkins integration server
    # find svn to get the source revision
    INCLUDE(FindSubversion)
    IF(Subversion_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.svn)
      SET(TULIP_SOURCE_DIR ${PROJECT_SOURCE_DIR})
      IF(WIN32)
        IF("${CMAKE_GENERATOR}" MATCHES ".*MSYS.*" OR "${CMAKE_GENERATOR}" MATCHES ".*MinGW.*" OR "${CMAKE_GENERATOR}" MATCHES "Ninja")
          WINDOWS_TO_MSYS_PATH(${PROJECT_SOURCE_DIR} TULIP_SOURCE_DIR)
        ENDIF("${CMAKE_GENERATOR}" MATCHES ".*MSYS.*"  OR "${CMAKE_GENERATOR}" MATCHES ".*MinGW.*" OR "${CMAKE_GENERATOR}" MATCHES "Ninja")
      ENDIF(WIN32)
      Subversion_WC_INFO(${TULIP_SOURCE_DIR} TULIP)
      SET(TULIP_SVN_REVISION ${TULIP_WC_REVISION})
    ENDIF(Subversion_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.svn)
  ELSE()
    # in jenkins so get from the dedicated env variable
    SET(TULIP_SVN_REVISION $ENV{SVN_REVISION})
  ENDIF()
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
SET(LibTulipOGLPythonBindingsName "tulip-opengl-python-bindings")
SET(LibTulipGUIPythonBindingsName "tulip-gui-python-bindings")
SET(TulipLibs ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${LibTulipOGDFName})

## ========================================================
## Current year (used in documentation config files)
## ========================================================
STRING(TIMESTAMP CurrentYear "%Y")

## ========================================================
## Configure file for uninstall target
## ========================================================
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall COMMAND ${CMAKE_COMMAND} -DTulipVersion=${TulipVersion} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake VERBATIM)

## ========================================================
## Compiler-specific options
## ========================================================

SET_COMPILER_OPTIONS()

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
  SET(TULIP_PYTHON_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-core/tulip_module/tulip)
  SET(TULIPOGL_PYTHON_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-gui/tulipgui_module/tulipogl)
  SET(TULIPGUI_PYTHON_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-gui/tulipgui_module/tulipgui)
  SET(TULIP_PYTHON_NATIVE_FOLDER ${TULIP_PYTHON_FOLDER}/native)
  SET(TULIPOGL_PYTHON_NATIVE_FOLDER ${TULIPOGL_PYTHON_FOLDER}/native)
  SET(TULIPGUI_PYTHON_NATIVE_FOLDER ${TULIPGUI_PYTHON_FOLDER}/native)
  SET(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS OFF CACHE BOOL "Add a wheel make target for generating Tulip Python modules wheels ? [ON|OFF]")
  IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS AND APPLE)
    # build universal binaries when generating Tulip Python wheels for MacOs
    SET(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
    # build binaries with install rpath when generating Tulip Python wheels for MacOs
    # as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  ELSEIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS AND LINUX)
    FILE(READ /etc/issue LINUX_ISSUE)
    IF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT IS_DIRECTORY /opt/python)
      MESSAGE(FATAL_ERROR "Python binary wheels for Linux platform must be compiled using the CentOS 5.11 docker image provided by the manylinux project from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
    ENDIF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT IS_DIRECTORY /opt/python)
    # build binaries with install rpath when generating Tulip Python wheels for Linux
    # as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS AND APPLE)
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
      OPTION(TULIP_ENABLE_OPENMP "Do you want to enable OpenMP (debug mode)?" OFF)
    ELSE()
      OPTION(TULIP_ENABLE_OPENMP "Do you want to enable OpenMP?" ON)
    ENDIF()
    IF(TULIP_ENABLE_OPENMP)
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
            # Force setting OpenMP flags on Windows plateforms
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
            MESSAGE("OpenMP not found: multithreaded code will not be used")
          ENDIF(OMP_CFLAGS)
        ELSE(WIN32)
          MESSAGE("OpenMP not found: multithreaded code will not be used")
        ENDIF(WIN32)
      ENDIF(OPENMP_FOUND)
    ENDIF(TULIP_ENABLE_OPENMP)
  ENDIF(NOT CMAKE_BUILD_TYPE STREQUAL "")
ENDIF(NOT CLANG OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.7.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.7.0)

IF(NOT TULIP_BUILD_CORE_ONLY)
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
ENDIF(NOT TULIP_BUILD_CORE_ONLY)


IF(TULIP_BUILD_PYTHON_COMPONENTS)
  # Python
  # After finding the Python interpreter, try to find if SIP and its dev tools are installed on the host system.
  # If not, compile the SIP version located in thirdparty.
  FIND_PACKAGE(PythonInterp REQUIRED)
  IF(PYTHONINTERP_FOUND)
    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version OUTPUT_VARIABLE PYTHON_VERSION_RAW ERROR_VARIABLE PYTHON_VERSION_RAW)
    STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
    STRING(REGEX MATCH "[0-9]\\.[0-9]+" PYTHON_VERSION "${PYTHON_VERSION_RAW}")
    STRING(REGEX MATCH "[0-9]\\.[0-9]+\\.[0-9]+" PYTHON_VERSION_WITH_PATCH "${PYTHON_VERSION_RAW}")

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
        STRING(REPLACE "." "" PYTHON_VERSION_NO_DOT ${PYTHON_VERSION})

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

      SET(SIP_VERSION_THIRDPARTY 4.19.3)
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
        SET(SIP_LIB tulipsip)
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
          SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/siplib/)
        ENDIF(SIP_OK)
      ENDIF(SIP_OK AND NOT ${SIP_VERSION_STR} VERSION_LESS ${SIP_VERSION_THIRDPARTY})

      IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
        IF(NOT LINUX)
          EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import wheel" RESULT_VARIABLE WHEEL_OK OUTPUT_QUIET ERROR_QUIET)
          EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import twine" RESULT_VARIABLE TWINE_OK OUTPUT_QUIET ERROR_QUIET)
          IF(NOT WHEEL_OK EQUAL 0)
            MESSAGE("The 'wheel' Python module has to be installed to generate wheels for tulip modules.")
            MESSAGE("You can install it trough the 'pip' tool ($ pip install wheel)")
          ENDIF(NOT WHEEL_OK EQUAL 0)
          IF(NOT TWINE_OK EQUAL 0)
            MESSAGE("The 'twine' Python module has to be installed to upload tulip wheels on PyPi.")
            MESSAGE("You can install it trough the 'pip' tool ($ pip install twine)")
          ENDIF(NOT TWINE_OK EQUAL 0)

          ADD_CUSTOM_TARGET(tulip-core-wheel
            COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
            WORKING_DIRECTORY ${TULIP_PYTHON_FOLDER}/../)

          ADD_CUSTOM_TARGET(tulip-gui-wheel
            COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
            WORKING_DIRECTORY ${TULIPGUI_PYTHON_FOLDER}/../
            DEPENDS tulip-core-wheel)

        ELSE(NOT LINUX)

          EXECUTE_PROCESS(COMMAND ${PYTHON_HOME_PATH}/pip install --upgrade wheel)
          EXECUTE_PROCESS(COMMAND ${PYTHON_HOME_PATH}/pip install --upgrade twine)

          # When building Python binary wheels on Linux, produced binaries have to be patched
          # in order for the tulip modules to be successfully imported and loaded on every computer.
          # The 'auditwheel' tool (see https://github.com/pypa/auditwheel) has been developed
          # in order to ease that patching task, so install its latest version with pip first.
          # The tool is only compatible with Python 3.X, so we will use Python 3.5 installed
          # in the docker image provided by the manylinux project (see https://github.com/pypa/manylinux).

          # Use a patched version of the auditwheel tool as the official one does not
          # repair Tulip-Python wheels correctly
          EXECUTE_PROCESS(COMMAND /opt/python/cp35-cp35m/bin/pip uninstall -y auditwheel)
          EXECUTE_PROCESS(COMMAND git clone -b purelib_fixes https://github.com/anlambert/auditwheel.git ERROR_QUIET)
          EXECUTE_PROCESS(COMMAND /opt/python/cp35-cp35m/bin/pip install auditwheel/)

          ADD_CUSTOM_TARGET(tulip-core-wheel
            COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
            COMMAND /opt/python/cp35-cp35m/bin/auditwheel repair -L native -w ./dist ./dist/`ls -t ./dist/ | head -1`
            WORKING_DIRECTORY ${TULIP_PYTHON_FOLDER}/../)

          ADD_CUSTOM_TARGET(tulip-gui-wheel
            COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel
            COMMAND /opt/python/cp35-cp35m/bin/auditwheel repair -L native -w ./dist ./dist/`ls -t ./dist/ | head -1`
            WORKING_DIRECTORY ${TULIPGUI_PYTHON_FOLDER}/../
            DEPENDS tulip-core-wheel)

        ENDIF(NOT LINUX)  

        # In order to upload the generated wheels, an account must be created on PyPi
        # and the following configuration must be stored in the ~/.pypirc file
        ##############################################################
        # [distutils]
        # index-servers=
        #     pypi
        #     pypitest
        #
        # [pypitest]
        # repository = https://test.pypi.org/legacy/
        # username = <your user name goes here>
        # password = <your password goes here>
        #
        # [pypi]
        # repository = https://upload.pypi.org/legacy/
        # username = <your user name goes here>
        # password = <your password goes here>
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
        ADD_CUSTOM_TARGET(tulip-core-wheel-upload-test
          COMMAND ${TWINE} upload -r pypitest dist/${WHEEL_FILES_REGEXP}
          WORKING_DIRECTORY ${TULIP_PYTHON_FOLDER}/../)
        ADD_CUSTOM_TARGET(tulip-gui-wheel-upload-test
          COMMAND ${TWINE} upload -r pypitest dist/${WHEEL_FILES_REGEXP}
          WORKING_DIRECTORY ${TULIPGUI_PYTHON_FOLDER}/../)
        ADD_CUSTOM_TARGET(tulip-core-wheel-upload
          COMMAND ${TWINE} upload -r pypi dist/${WHEEL_FILES_REGEXP}
          WORKING_DIRECTORY ${TULIP_PYTHON_FOLDER}/../)
        ADD_CUSTOM_TARGET(tulip-gui-wheel-upload
          COMMAND ${TWINE} upload -r pypi dist/${WHEEL_FILES_REGEXP}
          WORKING_DIRECTORY ${TULIPGUI_PYTHON_FOLDER}/../)

        ADD_CUSTOM_TARGET(wheels DEPENDS tulip-core-wheel tulip-gui-wheel)
        ADD_CUSTOM_TARGET(wheels-upload-test DEPENDS tulip-core-wheel-upload-test tulip-gui-wheel-upload-test)
        ADD_CUSTOM_TARGET(wheels-upload DEPENDS tulip-core-wheel-upload tulip-gui-wheel-upload)
      ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

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
SET(QtAwesomeLibrary QtAwesome)
SET(QtAwesomeInclude ${PROJECT_SOURCE_DIR}/thirdparty/QtAwesome)

SET(TulipCoreInclude ${PROJECT_SOURCE_DIR}/library/tulip-core/include/)
SET(TulipCoreBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-core/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipOGDFInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogdf/)
SET(TulipGUIInclude ${PROJECT_SOURCE_DIR}/library/tulip-gui/include/)
SET(TulipGUIBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/include/)
SET(TulipPythonInclude ${PROJECT_SOURCE_DIR}/library/tulip-python/include)

## ========================================================
## Install paths
## ========================================================
SET(TulipLibInstallDir lib/)
SET(TulipPluginsInstallDir ${TulipLibInstallDir}/tulip/)
SET(TulipGlyphPluginsInstallDir ${TulipPluginsInstallDir}/glyph/)
SET(TulipViewPluginsInstallDir ${TulipPluginsInstallDir}/view/)
SET(TulipInteractorPluginsInstallDir ${TulipPluginsInstallDir}/interactor/)
SET(TulipPerspectivePluginsInstallDir ${TulipPluginsInstallDir}/perspective/)
SET(TulipBinInstallDir bin/)
SET(TulipIncludeInstallDir include/)
SET(TulipShareInstallDir share/tulip)
SET(TulipBitmapInstallDir ${TulipShareInstallDir}/bitmaps/)

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
    IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/;@loader_path/../")
    ELSE(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../Frameworks;${QT_FRAMEWORKS_DIR}")
    ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
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
  IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
    ADD_DEPENDENCIES(tulip-core-wheel ${name})
    ADD_DEPENDENCIES(tulip-gui-wheel ${name})
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ENDFUNCTION()

FUNCTION(ADD_EXECUTABLE name)
  _ADD_EXECUTABLE(${name} ${ARGN})
  IF(APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../Frameworks;${QT_FRAMEWORKS_DIR}")
  ELSEIF(LINUX)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
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
## Unit tests
## ========================================================
IF(EXISTS ${CMAKE_SOURCE_DIR}/unit_test/CMakeLists.txt)
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

ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(plugins)
IF(NOT TULIP_BUILD_CORE_ONLY)
  ADD_SUBDIRECTORY(doc)
  ADD_SUBDIRECTORY(textures)
  IF(NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
    ADD_SUBDIRECTORY(externalplugins)
    ADD_SUBDIRECTORY(demos)
    ADD_SUBDIRECTORY(software)
  ELSE(NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
    ADD_CUSTOM_TARGET(copyTulipColorScales ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/software/bitmaps/colorscales ${TULIPOGL_PYTHON_FOLDER}/share/bitmaps/colorscales)
  ENDIF(NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

IF(TULIP_BUILD_TESTS)
  INCLUDE(CTest)
  ENABLE_TESTING()
  IF(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure --build-config "$<CONFIGURATION>")
  ELSE(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure)
  ENDIF(CMAKE_CONFIGURATION_TYPES)
  ADD_SUBDIRECTORY(unit_test)
ENDIF(TULIP_BUILD_TESTS)

INSTALL(FILES AUTHORS
              COPYING.LESSER
              cmake/FindTULIP.cmake
              cmake/TulipUseFile.cmake
        DESTINATION ${TulipShareInstallDir})

IF(NOT TULIP_BUILD_CORE_ONLY)
  INSTALL(FILES cmake/FindGLEW.cmake
                cmake/FindQtX.cmake
                cmake/FindSphinx.cmake
          DESTINATION ${TulipShareInstallDir})
	
  ADD_SUBDIRECTORY(bundlers)

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

  INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config DESTINATION ${TulipBinInstallDir})

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
      IF(MSVC14)
        INSTALL(PROGRAMS "${MSVC14_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC140.OPENMP/vcomp140.dll" DESTINATION ${TulipBinInstallDir})
      ENDIF(MSVC14)
    ENDIF(OPENMP_FOUND)
  ENDIF(MSVC)

  IF(WIN32)
    # initialize the list of bundle libs (second arg of FIXUP_BUNDLE),
    # updated in plugins/view/GeographicView,
    # and finally used in software/tulip_perpective/CMakeLists.txt
    SET_PROPERTY(GLOBAL PROPERTY FIXUP_BUNDLE_LIBS "")
    
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/imageformats DESTINATION ${TulipBinInstallDir})
    IF(USE_QT5)
      INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/platforms DESTINATION ${TulipBinInstallDir})
    ENDIF(USE_QT5)

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
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/COPYING ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/COPYING
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
                      COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/mac_bundle.sh ${CMAKE_CURRENT_BINARY_DIR}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac)

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
