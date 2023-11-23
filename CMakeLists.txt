CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

#Setting project name and version (W.X.Y.Z if necessary)
CMAKE_POLICY(SET CMP0048 NEW)
PROJECT(Tulip VERSION 5.7.3 DESCRIPTION "Large graphs analysis and drawing" HOMEPAGE_URL "https://tulip.labri.fr")
message(STATUS "Configuring ${CMAKE_PROJECT_NAME} ${Tulip_VERSION}")

# First display CMake version
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

# Remove CMake warnings related to Qt5
CMAKE_POLICY(SET CMP0020 NEW)

# Needed by clang compiler provided by MacPorts or homebrew
CMAKE_POLICY(SET CMP0025 NEW)

# Remove CMake warnings related to rpath on MacOS
CMAKE_POLICY(SET CMP0042 NEW)
CMAKE_POLICY(SET CMP0068 NEW)

# allow FindOpenGL to use GLVND when available
CMAKE_POLICY(SET CMP0072 NEW)

# enable the use of ccache for Tulip developers to speed up the build process
IF(NOT MSVC)
  OPTION(TULIP_USE_CCACHE "Do you want to use ccache to speed up the build process during Tulip development ? [OFF|ON]" OFF)
  FIND_PROGRAM(CCACHE_PROGRAM ccache)
  IF(TULIP_USE_CCACHE AND CCACHE_PROGRAM)
      SET(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
      SET(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  ENDIF(TULIP_USE_CCACHE AND CCACHE_PROGRAM)
ENDIF(NOT MSVC)

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
STRING(COMPARE EQUAL "${CMAKE_SIZEOF_VOID_P}" "8" IS_64BIT)
IF(IS_64BIT)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIS_64BIT")
  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DARM64")
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DX86_64")
  ENDIF()
ENDIF(IS_64BIT)

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
## AppImage build
## ========================================================
IF (TULIP_BUILD_FOR_APPIMAGE)
  ADD_COMPILE_DEFINITIONS(APPIMAGE_BUILD)
ENDIF (TULIP_BUILD_FOR_APPIMAGE)

## ========================================================
## Install paths
## ========================================================

INCLUDE(GNUInstallDirs)

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
SET(TulipVersionSeparator .)
IF(MSVC) #specific options for Visual Studio
  SET(TulipVersionSeparator _)
ENDIF()
SET(TulipMMVersion "${PROJECT_VERSION_MAJOR}${TulipVersionSeparator}${PROJECT_VERSION_MINOR}")
# Use two digits for the minor version when generating the Tulip integer version number
# as there was a 4.10 release that breaks preprocessor test like TULIP_INT_VERSION >= 4100 since the 5.0 release
# (see https://sourceforge.net/p/auber/bugs/869/)
STRING(LENGTH "${Tulip_VERSION_MINOR}" TulipMinorVersionNbDigits)
IF(TulipMinorVersionNbDigits LESS 2)
  SET(TulipMinorVersionTwoDigits "0${Tulip_VERSION_MINOR}")
ELSE(TulipMinorVersionNbDigits LESS 2)
  SET(TulipMinorVersionTwoDigits "${PROJECT_VERSION_MINOR}")
ENDIF(TulipMinorVersionNbDigits LESS 2)
SET(TulipIntMMVersion "${Tulip_VERSION_MAJOR}${TulipMinorVersionTwoDigits}")
SET(TulipIntVersion "${Tulip_VERSION_MAJOR}${TulipMinorVersionTwoDigits}${PROJECT_VERSION_PATCH}")

## ========================================================
## Tulip libraries names
## ========================================================
SET(LibTulipCoreName "tulip-core-${TulipMMVersion}")
SET(LibTulipOGLName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipGUIName "tulip-gui-${TulipMMVersion}")
SET(LibTulipPythonName "tulip-python-${TulipMMVersion}")
SET(LibStlPythonBindingsName "stl-python-bindings")
SET(LibTulipCorePythonBindingsName "tulip-core-python-bindings")
SET(LibTulipGUIPythonBindingsName "tulip-gui-python-bindings")
SET(TulipLibs ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

## ========================================================
## Current year (used in documentation config files)
## ========================================================
STRING(TIMESTAMP CurrentYear "%Y")

## ========================================================
## Compiler-specific options
## ========================================================

TULIP_SET_COMPILER_OPTIONS()


## plugin server
OPTION(TULIP_GENERATE_PLUGINSERVER "Compile and install plugin server" OFF)

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

IF (NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
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
        FILE(READ /etc/centos-release LINUX_ISSUE)
	IF((NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 6\\.10 \\(Final\\).*" AND NOT "${LINUX_ISSUE}" MATCHES ".*CentOS Linux release 7\\.9.*") OR NOT EXISTS /usr/local/bin/auditwheel)
          MESSAGE(FATAL_ERROR "Python binary wheels for Linux platform must be compiled using the CentOS (6.10 or 7.9) docker images provided by the manylinux project from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
	ENDIF()
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
  ENDIF()

  # Don't need to find zlib when compiling with emscripten as there is an available emscripten port of it
  IF(NOT EMSCRIPTEN)
    # ZLIB
    FIND_PACKAGE(ZLIB REQUIRED)
  ENDIF(NOT EMSCRIPTEN)
ENDIF(NOT TRAVIS_BUILD_THIRDPARTY_ONLY)

# use thirdparty static libs when needed
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET OR TULIP_BUILD_FOR_APPIMAGE)
  SET(TULIP_USE_THIRDPARTY_STATIC_LIBS ON)
ELSE(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET OR TULIP_BUILD_FOR_APPIMAGE)
  SET(TULIP_USE_THIRDPARTY_STATIC_LIBS OFF)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET OR TULIP_BUILD_FOR_APPIMAGE)

# Qhull
SET(TULIP_USE_THIRDPARTY_QHULL ${TULIP_USE_THIRDPARTY_STATIC_LIBS} CACHE BOOL "If activated, force the use of reentrant qhull library located in thirdparty instead of trying to find it on the system.")
IF(NOT TULIP_USE_THIRDPARTY_QHULL)
  FIND_PACKAGE(Qhull)
ELSE(NOT TULIP_USE_THIRDPARTY_QHULL)
  UNSET(QHULL_FOUND CACHE)
ENDIF(NOT TULIP_USE_THIRDPARTY_QHULL)

IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
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

IF(TULIP_BUILD_GL_TEX_LOADER)
  # Jpeg
  FIND_PACKAGE(JPEG REQUIRED)

  # Png
  FIND_PACKAGE(PNG REQUIRED)
ENDIF(TULIP_BUILD_GL_TEX_LOADER)

  # Freetype
  FIND_PACKAGE(Freetype REQUIRED)

  # Qt
  FIND_PACKAGE(QtX)
  # Display Qt detected version
  MESSAGE(STATUS "Using Qt version ${QT_FULL_VERSION}")

  # Libzip
  IF(NOT TULIP_USE_THIRDPARTY_LIBZIP)
    FIND_PACKAGE(libzip 1.0 QUIET)
    IF(libzip_DIR)
      SET(LIBZIP_FOUND TRUE)
      MESSAGE(STATUS "Found Libzip (found version \" ${libzip_VERSION}\")")
    ENDIF(libzip_DIR)
  ELSE(NOT TULIP_USE_THIRDPARTY_LIBZIP)
    UNSET(LIBZIP_FOUND CACHE)
  ENDIF(NOT TULIP_USE_THIRDPARTY_LIBZIP)

ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)

# Python
IF(TULIP_BUILD_PYTHON_COMPONENTS)
  INCLUDE(cmake/TulipPython.cmake)
ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)

## ========================================================
## Include and library paths
## ========================================================
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(GZStreamLibrary gzstream)
IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
  SET(FTGLLibrary ftgl)
  ADD_COMPILE_DEFINITIONS(FTGL_LIBRARY_STATIC)
  SET(Tess2Include ${PROJECT_SOURCE_DIR}/thirdparty/libtess2/Include)
  SET(Tess2Library tess2)
  ADD_COMPILE_DEFINITIONS(TESS2_LIBRARY_STATIC)
  IF(LIBZIP_FOUND)
    SET(zipLibrary zip)
  ELSE(LIBZIP_FOUND)
    SET(zipIncludes ${PROJECT_SOURCE_DIR}/thirdparty/libzip/lib ${PROJECT_BINARY_DIR}/thirdparty/libzip)
    SET(zipLibrary ${PROJECT_BINARY_DIR}/thirdparty/libzip/lib/libzip.a)
  ENDIF(LIBZIP_FOUND)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)

SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF/include)
SET(OGDFBuildInclude ${PROJECT_BINARY_DIR}/thirdparty/OGDF/include)
SET(OGDFLibrary OGDF)

IF(QHULL_FOUND)
  SET(QhullInclude ${QHULL_INCLUDE_DIRS})
  SET(QhullLibrary ${QHULL_LIBRARIES})
ELSE(QHULL_FOUND)
  SET(HAVE_REENTRANT_QHULL TRUE)
  SET(QhullInclude ${PROJECT_SOURCE_DIR}/thirdparty/libqhull)
  SET(QhullLibrary qhullstatic)
ENDIF(QHULL_FOUND)

# use thirdparty/yajl to ensure the static lib can be used to build shared libs
SET(YajlInclude ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/api ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/yajl/yajl-2.1.0/include)
SET(YajlLibrary yajl_s)

SET(UTF8CppInclude ${PROJECT_SOURCE_DIR}/thirdparty/utf8-cpp/source)

SET(TulipCoreInclude ${PROJECT_SOURCE_DIR}/library/tulip-core/include/)
SET(TulipCoreBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-core/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipGUIInclude ${PROJECT_SOURCE_DIR}/library/tulip-gui/include/)
SET(TulipUIGUIInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/src/)
SET(TulipGUIBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/include/)
SET(TulipPythonInclude ${PROJECT_SOURCE_DIR}/library/tulip-python/include)

## ========================================================
## Point RPATH to lib folder
## ========================================================
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
## Create library/tulip/include/tulip/TulipRelease.h based on TulipRelease.h.in
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
IF (NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  ADD_SUBDIRECTORY(library)
  ADD_SUBDIRECTORY(plugins)
  IF(NOT TULIP_BUILD_CORE_ONLY)
    ADD_SUBDIRECTORY(doc)
    ADD_SUBDIRECTORY(textures)
    ADD_SUBDIRECTORY(externalplugins)
    ADD_SUBDIRECTORY(software)
  ENDIF(NOT TULIP_BUILD_CORE_ONLY)
ENDIF (NOT TRAVIS_BUILD_THIRDPARTY_ONLY)

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
SET(VERSION "${Tulip_VERSION}")
SET(WIN_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
IF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
  SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
ENDIF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
IF(WIN32)
  SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY}")
ELSE(WIN32)
  SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY}")
ENDIF(WIN32)
SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE} ${QT_QTWEBKIT_LIBRARY_RELEASE}")

# if needed, enable C++11 in tulip-config --cxxflags
IF(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
  SET(TULIP_CONFIG_CXX11_FLAGS "-std=c++0x")
ELSE(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
  IF((CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1) OR CLANG)
    SET(TULIP_CONFIG_CXX11_FLAGS "-std=c++11")
  ENDIF((CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1) OR CLANG)
ENDIF(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
IF(CLANG AND APPLE)
  SET(TULIP_CONFIG_CXX11_FLAGS "-std=c++11 -stdlib=libc++")
ENDIF(CLANG AND APPLE)

IF (NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  CONFIGURE_FILE("tulip-config.in" "tulip-config" @ONLY)
  INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config DESTINATION ${TulipBinInstallDir})

  IF(NOT TULIP_BUILD_CORE_ONLY)

    ADD_SUBDIRECTORY(bundlers)

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
      SET(make_tar_file ${PROJECT_BINARY_DIR}/tar-tulip-${Tulip_VERSION}-corefiles.sh)
      EXECUTE_PROCESS(COMMAND rm ${make_tar_file} OUTPUT_QUIET ERROR_QUIET)
      EXECUTE_PROCESS(COMMAND echo "#!/bin/sh
if [ $# -eq 0 ]; then
TAR_FILE=${PROJECT_BINARY_DIR}/tulip-${Tulip_VERSION}-corefiles.tar.gz
else
TAR_FILE=$1
fi
cd ${PROJECT_SOURCE_DIR}
tar zcvf \${TAR_FILE} $ENV{CORE_FILES}
echo archive \${TAR_FILE} successfully generated" OUTPUT_FILE ${make_tar_file})

      MESSAGE(STATUS "${make_tar_file} successfully generated")
    ENDIF(LINUX)

  ENDIF(NOT TULIP_BUILD_CORE_ONLY)
ENDIF(NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
