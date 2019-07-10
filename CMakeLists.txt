CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# Remove CMake warnings related to Qt5
IF(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(POLICY CMP0020)

# Needed by clang compiler provided by MacPorts or homebrew
IF(POLICY CMP0025)
  CMAKE_POLICY(SET CMP0025 NEW)
ENDIF(POLICY CMP0025)

# Remove CMake warnings related to rpath on MacOS
IF(POLICY CMP0042)
  CMAKE_POLICY(SET CMP0042 NEW)
ENDIF(POLICY CMP0042)

# Ensure CMAKE_BUILD_WITH_INSTALL_RPATH is still honored with CMake >= 3.9
# This is needed in order to build tulip-python wheels on MacOS
IF(POLICY CMP0068)
  CMAKE_POLICY(SET CMP0068 OLD)
ENDIF(POLICY CMP0068)

IF(POLICY CMP0072)
  CMAKE_POLICY(SET CMP0072 NEW)
ENDIF(POLICY CMP0072)

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
## AppImage build
## ========================================================
IF (TULIP_BUILD_FOR_APPIMAGE)
  ADD_DEFINITIONS("-DAPPIMAGE_BUILD")
ENDIF (TULIP_BUILD_FOR_APPIMAGE)

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
SET(TulipMinorVersion 3)
SET(TulipReleaseVersion "1")
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
  SET(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET OFF CACHE BOOL "Add a wheel make target for generating Python tulip-core module wheel ? [ON|OFF]")
  IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
    SET(TULIP_BUILD_CORE_ONLY ON)
    IF(APPLE)
      # build binaries with install rpath when generating Tulip Python wheels
      # for MacOs as we don't install the project in that case
      SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    ELSEIF(LINUX)
      FILE(READ /etc/issue LINUX_ISSUE)
      IF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT EXISTS /usr/local/bin/auditwheel)
        MESSAGE(FATAL_ERROR "Python binary wheels for Linux platform must be compiled using the CentOS 5.11 docker image provided by the manylinux project from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
      ENDIF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT EXISTS /usr/local/bin/auditwheel)
      # build binaries with install rpath when generating Tulip Python wheels for Linux
      # as we don't install the project in that case
      SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    ENDIF(APPLE)
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
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

IF(NOT TULIP_BUILD_CORE_ONLY)
  # OpenGL
  FIND_PACKAGE(OpenGL REQUIRED)

  # Glew
  FIND_PACKAGE(GLEW REQUIRED)
  # if needed, when using cmake 3.15, define some undefined glew variables
  IF(NOT ${CMAKE_VERSION} VERSION_LESS "3.15.0")
    IF(NOT GLEW_INCLUDE_DIR)
      GET_TARGET_PROPERTY(GLEW_INCLUDE_DIR GLEW::GLEW INTERFACE_INCLUDE_DIRECTORIES)
    ENDIF(NOT GLEW_INCLUDE_DIR)
    IF(NOT GLEW_LIBRARY)
      SET(GLEW_LIBRARY GLEW::GLEW)
    ENDIF(NOT GLEW_LIBRARY)
  ENDIF(NOT ${CMAKE_VERSION} VERSION_LESS "3.15.0")

  # Jpeg
  FIND_PACKAGE(JPEG REQUIRED)

  # Png
  FIND_PACKAGE(PNG REQUIRED)

  # Freetype
  FIND_PACKAGE(Freetype REQUIRED)

  # Qt
  FIND_PACKAGE(QtX)

  # Quazip
SET(TULIP_USE_THIRDPARTY_QUAZIP OFF CACHE BOOL "If activated, force the use of quazip library located in thirdparty instead of trying to find it on the system.")
IF(NOT TULIP_USE_THIRDPARTY_QUAZIP)
  FIND_PACKAGE(QuaZip)
ELSE(NOT TULIP_USE_THIRDPARTY_QUAZIP)
  UNSET(QUAZIP_FOUND CACHE)
ENDIF(NOT TULIP_USE_THIRDPARTY_QUAZIP)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

# Python
IF(TULIP_BUILD_PYTHON_COMPONENTS)
  INCLUDE(cmake/TulipPython.cmake)
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
    # relative locations of Tulip dylibs dependencies are not the sames when generating Python wheel
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
  ADD_SUBDIRECTORY(textures)
  ADD_SUBDIRECTORY(externalplugins)
  ADD_SUBDIRECTORY(demos)
  ADD_SUBDIRECTORY(software)
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

INSTALL(FILES cmake/FindTULIP.cmake
        RENAME TULIPConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TULIP/)

CONFIGURE_FILE(cmake/TULIPConfigVersion.cmake.in ${CMAKE_BINARY_DIR}/cmake/TULIPConfigVersion.cmake @ONLY)

INSTALL(FILES cmake/TulipUseFile.cmake
              ${CMAKE_BINARY_DIR}/cmake/TULIPConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TULIP/)

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

IF(NOT TULIP_BUILD_CORE_ONLY)

  ADD_SUBDIRECTORY(bundlers)
  # ADD_SUBDIRECTORY(deploy)

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
                          COMMAND cmd //C win_bundle.bat \"${NSIS_PATH}\" \"${TLP_DIR}\" \"${BINARY_DIR}\" ${DEBUG_MODE}
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win)

      ELSE(NOT TULIP_BUILD_PYTHON_COMPONENTS OR EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

        MESSAGE("NSIS Inetc plugin is required to build the Tulip installer.")
        MESSAGE("Extract ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/Inetc.zip in ${NSIS_PATH}.")

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
