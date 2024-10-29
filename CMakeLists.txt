CMAKE_MINIMUM_REQUIRED(VERSION 3.25...3.28)

#Setting project name and version (W.X.Y.Z if necessary)
PROJECT(Tulip
    VERSION 6.0.0
    DESCRIPTION "Large graphs analysis and drawing"
    HOMEPAGE_URL "https://tulip.labri.fr")
message(STATUS "Configuring ${CMAKE_PROJECT_NAME} ${Tulip_VERSION}")

# First display CMake version
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

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
    add_compile_definitions("IS_64BIT")
  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    add_compile_definitions("ARM64")
  ELSE()
    add_compile_definitions("X86_64")
  ENDIF()
ENDIF(IS_64BIT)

## ========================================================
## Consider *BSD as Linux
## ========================================================
IF(BSD)
    SET(LINUX TRUE)
ENDIF(BSD)

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
SET(LibTulipCorePythonBindingsName "tulip-core-python-bindings")
SET(LibTulipGUIPythonBindingsName "tulip-gui-python-bindings")
SET(TulipLibs ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

## ========================================================
## Compiler-specific options
## ========================================================

TULIP_SET_COMPILER_OPTIONS()

## ========================================================
## CORE ONLY
## this used to have a minimal build of Tulip without
## any of the opengl, qt or python components
## ========================================================
IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/cmake/FindQtX.cmake)
  SET(TULIP_BUILD_CORE_ONLY ON)
  MESSAGE(STATUS "You can only configure a core build of Tulip.")
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
  ADD_CORE_FILES(CMakeLists.txt cmake/cmake_uninstall.cmake.in cmake/TulipUseFile.cmake cmake/FindTulip.cmake)
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

  SET(TULIP_PYTHON_ROOT_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-core/tulip_module)
  SET(TULIP_PYTHON_FOLDER ${TULIP_PYTHON_ROOT_FOLDER}/tulip)
  SET(TULIPGUI_PYTHON_ROOT_FOLDER ${CMAKE_BINARY_DIR}/library/tulip-python/bindings/tulip-gui/tulipgui_module)
  SET(TULIPGUI_PYTHON_FOLDER ${TULIPGUI_PYTHON_ROOT_FOLDER}/tulipgui)
  SET(TULIP_PYTHON_NATIVE_FOLDER ${TULIP_PYTHON_FOLDER}/native)
  SET(TULIPGUI_PYTHON_NATIVE_FOLDER ${TULIPGUI_PYTHON_FOLDER}/native)
  SET(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET OFF CACHE BOOL "Add a wheel make target for generating Python tulip-core module wheel ? [ON|OFF]")
  IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
    SET(TULIP_BUILD_CORE_ONLY ON)
    # build binaries with install rpath when generating Tulip Python wheels
    # as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    IF(LINUX)
        #check we are running under manylinux
        execute_process(
            COMMAND manylinux-interpreters list
            RESULT_VARIABLE EXIT_CODE
            OUTPUT_QUIET
    )
    if (NOT ${EXIT_CODE} EQUAL 0)
        MESSAGE(FATAL_ERROR "Python binary wheels for Linux platform must be compiled using manylinux from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
    endif()
    ENDIF(LINUX)
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

  ## ========================================================
  ## Required packages
  ## ========================================================

  FIND_PACKAGE(ZLIB REQUIRED)

# Python (need to put this here to initialize Qhull properly when compiling Python wheels
#qhull libs from manylinux-2.28 (Almalinux 8) does not work. Paclage cmeel-qhull should be used instead
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
# When building tulip wheels we make a loop of cmake builds,
# with only a change of Python_EXECUTABLE CMake variable;
# so we need to unset the previous values of the CMake Python cache variables
# to force their recomputation
  UNSET(Python_FOUND CACHE)
  #force to rediscover Qhull for each build (use the cmeel package from the same Python version)
  UNSET(PKG_CONFIG_ARGN CACHE)
  UNSET(Qhull_FOUND CACHE)
  UNSET(Qhull_LIBDIR CACHE)
  SET(PYTHON_COMPONENTS Interpreter Development.Module)
ELSE(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  SET(PYTHON_COMPONENTS Interpreter Development)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
FIND_PACKAGE(Python 3.9...<3.14 REQUIRED COMPONENTS ${PYTHON_COMPONENTS})

FIND_PACKAGE(PkgConfig REQUIRED)
IF(LINUX AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
#QHULL bundled with manylinux 2.28 is not working
#use the cmeel-qhull package from Pypi (need to be installed before running cmake)
SET(PKG_CONFIG_ARGN "--with-path=${Python_SITELIB}/cmeel.prefix/lib/pkgconfig")
ENDIF(LINUX AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
PKG_search_module(Qhull REQUIRED qhull_r>8 IMPORTED_TARGET)
PKG_search_module(Yajl REQUIRED yajl>2 IMPORTED_TARGET)

#Python
INCLUDE(cmake/TulipPython.cmake)

IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  # OpenGL
  FIND_PACKAGE(OpenGL REQUIRED COMPONENTS OpenGL)

  # Glew
  FIND_PACKAGE(GLEW REQUIRED)

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

  FIND_PACKAGE(libzip REQUIRED)

ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)

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
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)

SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF/include)
SET(OGDFBuildInclude ${PROJECT_BINARY_DIR}/thirdparty/OGDF/include)
SET(OGDFLibrary OGDF)

#utf8-cpp
find_package(utf8cpp QUIET)
IF(utf8cpp_FOUND)
    IF(TARGET utf8cpp) #Gentoo linux is looking for utf8cpp only
        GET_PROPERTY(UTF8CppInclude TARGET utf8cpp PROPERTY INTERFACE_INCLUDE_DIRECTORIES)  #Gentoo linux is looking for utf8cpp only
    ELSE(TARGET utf8cpp)
        GET_PROPERTY(UTF8CppInclude TARGET utf8cpp::utf8cpp PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    ENDIF(TARGET utf8cpp)
    MESSAGE(STATUS "Using the system utf8cpp header files located in ${UTF8CppInclude}")
ELSE(utf8cpp_FOUND)
    SET(UTF8CppInclude ${PROJECT_SOURCE_DIR}/thirdparty/utf8-cpp/source)
    MESSAGE(STATUS "Using utf8cpp from the thirdparty directory")
ENDIF(utf8cpp_FOUND)

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
SET(TULIP_BUILD_TESTS OFF CACHE BOOL "Do you want to build the unit tests (target called 'test') ? [OFF|ON]")

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
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(plugins)
IF(NOT TULIP_BUILD_CORE_ONLY)
    ADD_SUBDIRECTORY(doc)
    install(DIRECTORY textures/ DESTINATION ${TulipBitmapInstallDir}/ COMPONENT tulip_textures
        PATTERN "headerlogo.bmp" EXCLUDE)
    ADD_SUBDIRECTORY(externalplugins)
    ADD_SUBDIRECTORY(software)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

IF(TULIP_BUILD_TESTS)
  INCLUDE(CTest)
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
  SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES}")
ELSE(WIN32)
  SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES}")
ENDIF(WIN32)
SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE}")

IF (NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  CONFIGURE_FILE("tulip-config.in" "tulip-config" @ONLY)
  INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config DESTINATION ${TulipBinInstallDir})

  IF(NOT TULIP_BUILD_CORE_ONLY)

    ADD_SUBDIRECTORY(bundlers)

    IF(MSVC)
      INCLUDE(InstallRequiredSystemLibraries)
      IF(OPENMP_FOUND)
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
      GET_FILENAME_COMPONENT(NSIS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\NSIS]" ABSOLUTE)
      STRING(COMPARE EQUAL "${NSIS_PATH}" "" NSIS_PATH_EMPTY)
      IF(NSIS_PATH_EMPTY)
	GET_FILENAME_COMPONENT(NSIS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\NSIS]" ABSOLUTE)
	STRING(COMPARE EQUAL "${NSIS_PATH}" "" NSIS_PATH_EMPTY)
      ENDIF(NSIS_PATH_EMPTY)

      IF(NOT NSIS_PATH_EMPTY)

	IF(EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

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
                            COMMAND cmd //C win_bundle.bat \"${NSIS_PATH}\" \"${TLP_DIR}\" \"${BINARY_DIR}\" "WITH_PYTHON" ${DEBUG_MODE}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win)

           ADD_CUSTOM_TARGET(no_python_bundle
                            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/win_bundle.bat ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/win_bundle.bat
                            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/FileAssociation.nsh ${CMAKE_CURRENT_BINARY_DIR}/FileAssociation.nsh
                            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/COPYING.LESSER ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/COPYING.LESSER
                            COMMAND cmd //C win_bundle.bat \"${NSIS_PATH}\" \"${TLP_DIR}\" \"${BINARY_DIR}\" "WITHOUT_PYTHON" ${DEBUG_MODE}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win)

        ELSE(EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

          MESSAGE("NSIS Inetc plugin is required to build the Tulip installer.")
          MESSAGE("Extract ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/Inetc.zip in ${NSIS_PATH}.")

        ENDIF(EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

      ELSE(NOT NSIS_PATH_EMPTY)

	MESSAGE("Nullsoft Scriptable Install System is not installed on your system")
	MESSAGE("Tulip installer can not be generated without Nullsoft NSIS.")

      ENDIF(NOT NSIS_PATH_EMPTY)

    ENDIF(WIN32)

    IF(APPLE)

      SET(TULIP_PYTHON_BUNDLE ON)
      CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac/mac_bundle.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac/mac_bundle.sh" @ONLY)
      ADD_CUSTOM_TARGET(bundle
                        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                        COMMAND sh mac_bundle.sh ${CMAKE_CURRENT_BINARY_DIR}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac)
      SET(TULIP_PYTHON_BUNDLE OFF)
      CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac/mac_bundle.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac/mac_no_python_bundle.sh" @ONLY)
      ADD_CUSTOM_TARGET(no_python_bundle
                        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                        COMMAND sh mac_no_python_bundle.sh ${CMAKE_CURRENT_BINARY_DIR}
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
