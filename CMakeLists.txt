CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

# Remove CMake warnings related to Qt5
IF(POLICY CMP0020)
   CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(POLICY CMP0020)

PROJECT(tulip)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

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
STRING(COMPARE EQUAL "${CMAKE_SIZEOF_VOID_P}" "8" I64)
IF(I64)
  SET(PROCESSOR_ARCHITECTURE "64")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DI64")
ELSE(I64)
  SET(PROCESSOR_ARCHITECTURE "32")
ENDIF(I64)

## ========================================================
## Consider FreeBSD as Linux
## ========================================================
EXEC_PROGRAM(uname OUTPUT_VARIABLE UNAME_OUT)
STRING(COMPARE EQUAL "${UNAME_OUT}" "Linux" LINUX)
IF(NOT LINUX)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "GNU/kFreeBSD" LINUX)
ENDIF(NOT LINUX)

## ========================================================
## Operating system preprocessor macros
## ========================================================
IF(LINUX)
  ADD_DEFINITIONS("-D_LINUX")
ENDIF(LINUX)
IF(WIN32)
  ADD_DEFINITIONS("-D_WIN32")
ENDIF(WIN32)
IF(APPLE)
  ADD_DEFINITIONS("-D__APPLE__")
ENDIF(APPLE)

## ========================================================
## Tulip version variables
## ========================================================
SET(TulipMajorVersion 4)
SET(TulipMinorVersion 6)
SET(TulipReleaseVersion 1)
SET(TulipVersionSeparator .)
IF(MSVC) #specific options for Visual Studio
  SET(TulipVersionSeparator _)
ENDIF()
SET(TulipVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}${TulipVersionSeparator}${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}")
SET(TulipIntVersion "${TulipMajorVersion}${TulipMinorVersion}${TulipReleaseVersion}")

## ========================================================
## Tulip libraries names
## ========================================================
SET(LibTulipCoreName "tulip-core-${TulipMMVersion}")
SET(LibTulipOglName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipGUIName "tulip-gui-${TulipMMVersion}")
SET(LibTulipPythonName "tulip-python-${TulipMMVersion}")
SET(LibTulipOGDFName "tulip-ogdf-${TulipMMVersion}")
SET(LibStlPythonBindingsName "stl-python-bindings")
SET(LibTulipCorePythonBindingsName "tulip-core-python-bindings")
SET(LibTulipOGLPythonBindingsName "tulip-opengl-python-bindings")
SET(LibTulipGUIPythonBindingsName "tulip-gui-python-bindings")
SET(TulipLibs ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${LibTulipOGDFName})

## ========================================================
## Current year (used in documentation config files)
## ========================================================
IF (WIN32)
  EXECUTE_PROCESS(COMMAND "cmd.exe" "/c" "echo" "%DATE:~6,4%" OUTPUT_VARIABLE CurrentYear OUTPUT_STRIP_TRAILING_WHITESPACE)
ELSEIF(UNIX)
  EXECUTE_PROCESS(COMMAND "date" "+%Y" OUTPUT_VARIABLE CurrentYear OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDIF()

## ========================================================
## Configure file for uninstall target
## ========================================================
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake VERBATIM)

## ========================================================
## Additional macros
## ========================================================
INCLUDE(TulipUseFile.cmake)

## ========================================================
## Point RPATH to lib folder
## ========================================================
FUNCTION(ADD_LIBRARY name)
  _ADD_LIBRARY(${name} ${ARGN})
  IF(APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_NAME_DIR "@executable_path/../lib")
  ENDIF(APPLE)
  SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN")
  IF(MSVC)
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
  ENDIF(MSVC)
ENDFUNCTION()
FUNCTION(ADD_EXECUTABLE name)
  _ADD_EXECUTABLE(${name} ${ARGN})
  IF(APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_NAME_DIR "@executable_path/../lib")
  ENDIF(APPLE)
  IF(LINUX OR APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
  ELSE(LINUX OR APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN")
  ENDIF(LINUX OR APPLE)
  IF(MSVC)
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
  ENDIF(MSVC)
ENDFUNCTION()

## ========================================================
## Profiling
## ========================================================
SET(BUILD_CODE_COVERAGE OFF CACHE BOOL "Do you want to build for code coverage ? [OFF|ON]")

## ========================================================
## Compiler-specific options
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_CXX_COMPILER_ID}" "Clang" CLANG)

SET_COMPILER_OPTIONS()

IF(BUILD_CODE_COVERAGE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF()

## ========================================================
## CORE ONLY
## this used to have a minimal build of Tulip without
## any of the opengl, qt or python components
## ========================================================
IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/FindQtX.cmake OR EMSCRIPTEN)
  SET(BUILD_CORE_ONLY ON)
  MESSAGE("You can only configure a core build of Tulip.")
ELSE()
SET(BUILD_CORE_ONLY OFF CACHE BOOL "Do you want a minimal build of Tulip ? [ON|OFF]")
ENDIF()

IF(BUILD_CORE_ONLY AND LINUX)
# the macro ADD_CORE_FILES will be used
# to collect the files needed to build a minimal Tulip
# in the directory pointed by CMAKE_CURRENT_SOURCE_DIR
  macro(ADD_CORE_FILES files)
    set(CURRENT_SOURCE_DIR ".")
    if(${CMAKE_CURRENT_SOURCE_DIR} STRGREATER ${PROJECT_SOURCE_DIR})
      execute_process(COMMAND echo -n ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND sed "s,${PROJECT_SOURCE_DIR}/,,"
	OUTPUT_VARIABLE CURRENT_SOURCE_DIR)
    endif()
    if(${files} STREQUAL ".")
      SET(ENV{CORE_FILES} "$ENV{CORE_FILES} \$(find ${CURRENT_SOURCE_DIR} -path \"*/.svn\" -prune -o -type f -print)")
    else()
      foreach(core_file ${ARGV})
	SET(ENV{CORE_FILES} "$ENV{CORE_FILES} ${CURRENT_SOURCE_DIR}/${core_file}")
      endforeach()
    endif()
  endmacro()
  ADD_CORE_FILES(CMakeLists.txt cmake_uninstall.cmake.in TulipUseFile.cmake)
# the macro ADD_CORE_DIRECTORY_EXCEPT will be used
# to exclude the files not needed to build a minimal Tulip
# in the directory pointed by CMAKE_CURRENT_SOURCE_DIR
  macro(ADD_CORE_DIRECTORY_EXCEPT files)
    set(CURRENT_SOURCE_DIR ".")
    if(${CMAKE_CURRENT_SOURCE_DIR} STRGREATER ${PROJECT_SOURCE_DIR})
      execute_process(COMMAND echo -n ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND sed "s,${PROJECT_SOURCE_DIR}/,,"
	OUTPUT_VARIABLE CURRENT_SOURCE_DIR)
    endif()
    SET(find_command "\$(find ${CURRENT_SOURCE_DIR} -path \"*/.svn\" -prune -o -type f")
    foreach(exclude_file ${ARGV})
      SET(find_command "${find_command} -a ! -name \"${exclude_file}\"")
    endforeach()
    SET(ENV{CORE_FILES} "$ENV{CORE_FILES} ${find_command} -print)")
  endmacro()
ELSE()
  macro(ADD_CORE_FILES files)
  endmacro()
  macro(ADD_CORE_DIRECTORY_EXCEPT files)
  endmacro()
ENDIF()

SET(BUILD_PYTHON_COMPONENTS ON CACHE BOOL "Do you want to build Tulip Python components ? [ON|OFF]")

## ========================================================
## Required packages
## ========================================================
IF(MINGW)
# Set some common libraries and headers search paths for MinGW
SET(CMAKE_LIBRARY_PATH "${MINGW_BIN_PATH};${MINGW_LIB_PATH};${MINGW_LIB64_PATH};${CMAKE_LIBRARY_PATH}")
SET(CMAKE_INCLUDE_PATH "${MINGW_INCLUDE_PATH};${MINGW_INCLUDE_PATH}/freetype2;${MINGW_INCLUDE_PATH}/libpng*;${CMAKE_INCLUDE_PATH}")

# Check if libintl is present in the MinGW build environment
FIND_LIBRARY(LIBINTL_FOUND intl PATHS ${MINGW_LIB_PATH} ${MINGW_LIB64_PATH} NO_DEFAULT_PATH)
ENDIF(MINGW)


# ZLIB
FIND_PACKAGE(ZLIB REQUIRED)

# Yajl
SET(YAJL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/api ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/yajl/yajl-2.1.0/include)

# OpenMP
IF(NOT CLANG)
FIND_PACKAGE(Threads)
OPTION(ENABLE_OPENMP "Do you want to enable OpenMP?" ON)
IF(ENABLE_OPENMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS}")
        SET(OPENMP_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
        IF(WIN32)
            IF(MSVC)
                SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /openmp")
                SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /openmp")
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
        ## ========================================================
        ## Enable debugging of OpenMP
        ## ========================================================
        IF(CMAKE_DEBUG_MODE AND NOT CHECK_MEMORY)
            SET(DEBUG_OPENMP OFF CACHE BOOL "Do you want to enable OpenMP even in Debug mode ? [OFF|ON]")
            IF(DEBUG_OPENMP)
                SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OPENMP_CXX_FLAGS}")
                SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OPENMP_LINKER_FLAGS}")
                SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${OPENMP_LIBRARIES}")
            ENDIF(DEBUG_OPENMP)
        ENDIF(CMAKE_DEBUG_MODE AND NOT CHECK_MEMORY)
    ENDIF(OPENMP_FOUND)
ENDIF(ENABLE_OPENMP)
ENDIF(NOT CLANG)

IF(NOT BUILD_CORE_ONLY)

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

IF(BUILD_PYTHON_COMPONENTS)
# Python
# After finding the Python interpreter, try to find if SIP and its dev tools are installed on the host system.
# If not, compile the SIP version located in thirdparty.
FIND_PACKAGE(PythonInterp REQUIRED)
IF(PYTHONINTERP_FOUND)
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version OUTPUT_VARIABLE PYTHON_VERSION_RAW ERROR_VARIABLE PYTHON_VERSION_RAW)
  STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
  STRING(REGEX MATCH "[0-9]\\.[0-9]" PYTHON_VERSION "${PYTHON_VERSION_RAW}")
  STRING(REGEX MATCH "[0-9]\\.[0-9]\\.[0-9]" PYTHON_VERSION_WITH_PATCH "${PYTHON_VERSION_RAW}")

  # Unset the previous values of the CMake cache variables related to Python libraries
  # in case the value of PYTHON_EXECUTABLE CMake variable changed
  UNSET(PYTHON_LIBRARY CACHE)
  UNSET(PYTHON_INCLUDE_DIR CACHE)

  # Find the Python library with the same version as the interpreter
  # Python 3.2 library is suffixed by mu on some systems, also handle that case
  SET(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION}mu ${PYTHON_VERSION})
  FIND_PACKAGE(PythonLibs REQUIRED)

  IF(PYTHONLIBS_FOUND)

    # Python 64bits does not provide a dll import library for MinGW.
    # Fortunately, we can directly link to the Python dll with that compiler.
    # So find the location of that dll and overwrite the PYTHON_LIBRARY CMake cache variable with it
    IF(MINGW AND X64)
        STRING(REPLACE "\\" "/" WINDIR $ENV{WINDIR})
        GET_FILENAME_COMPONENT(PYTHON_HOME_PATH ${PYTHON_EXECUTABLE} PATH)
        STRING(REPLACE "." "" PYTHON_VERSION_NO_DOT ${PYTHON_VERSION})
        # Check if the Python dll is located in the Python home directory (when Python is installed for current user only)
        IF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
            SET(PYTHON_LIBRARY ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
        # If not, the Python dll is located in %WINDIR%/System32 (when Python is installed for all users)
        ELSE(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
            SET(PYTHON_LIBRARY ${WINDIR}/System32/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
        ENDIF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
    ENDIF(MINGW AND X64)


    SET(SIP_OK FALSE CACHE INTERNAL "")
    FIND_PACKAGE(SIP)
    SET(SIP_OK ${SIP_FOUND})
    IF(SIP_OK)
      SET(SIP_EXE "${SIP_EXECUTABLE}")
      SET(SYSTEM_SIP TRUE)
    ELSE(SIP_OK)
      SET(SIP_LIB sip)
      SET(SYSTEM_SIP FALSE)
      SET(SIP_VERSION_THIRDPARTY 4.15.5)
      TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen sip
                  CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE} -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} -DPYTHON_INCLUDE_DIR2=${PYTHON_INCLUDE_DIR2} -DPYTHON_INCLUDE_PATH=${PYTHON_INCLUDE_PATH} -DPYTHON_LIBRARY=${PYTHON_LIBRARY})
      IF(SIP_OK)
        SET(SIP_VERSION_STR "${SIP_VERSION_THIRDPARTY}")
        IF(WIN32 AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/Debug/sip.exe")
          SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/Debug/sip.exe)
        ELSE()
          SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/sipgen/sip)
        ENDIF()
        SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-${SIP_VERSION_THIRDPARTY}/siplib/)
      ENDIF(SIP_OK)
    ENDIF(SIP_OK)

  ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)
ENDIF(BUILD_PYTHON_COMPONENTS)

ENDIF(NOT BUILD_CORE_ONLY)

## ========================================================
## Include paths
## ========================================================
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(GZStreamLibrary gzstream-tulip-${TulipMMVersion})
IF(NOT BUILD_CORE_ONLY)
if(QUAZIP_FOUND)
    SET(QuazipInclude ${QUAZIP_INCLUDE_DIR})
    SET(QuazipLibrary ${QUAZIP_LIBRARIES})
else(QUAZIP_FOUND)
    SET(QuazipInclude ${PROJECT_SOURCE_DIR}/thirdparty/quazip/)
    SET(QuazipLibrary quazip-tulip-${TulipMMVersion})
endif(QUAZIP_FOUND)

SET(QxtInclude ${PROJECT_SOURCE_DIR}/thirdparty/qxt/src)
SET(QxtLibrary qxt-tulip-${TulipMMVersion})
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(FTGLLibrary ftgl-tulip-${TulipMMVersion})
ENDIF(NOT BUILD_CORE_ONLY)

SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF)
SET(OGDFLibrary OGDF-tulip-${TulipMMVersion})
SET(QHULLInclude ${PROJECT_SOURCE_DIR}/thirdparty)
SET(UTF8CppInclude ${PROJECT_SOURCE_DIR}/thirdparty/utf8-cpp/source)
SET(YajlLibrary yajl-tulip-${TulipMMVersion})

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
## Enable memory checks (disable openmp)
## ========================================================
IF(LINUX)
  OPTION(CHECK_MEMORY "Overload new and delete operators to check for memory leaks (This option can't be enabled along with OpenMP debugging)" OFF)
  IF(CHECK_MEMORY)
    SET(CHECK_MEMORY_FLAGS "-include ${CMAKE_SOURCE_DIR}/library/tulip-core/include/tulip/MemoryChecker.h")
  ENDIF(CHECK_MEMORY)
ENDIF(LINUX)

## ========================================================
## Unit tests
## ========================================================
IF(EXISTS ${CMAKE_SOURCE_DIR}/unit_test/CMakeLists.txt)
  SET(BUILD_TESTS OFF CACHE BOOL "Do you want to build the tests ? [OFF|ON]")
ELSE()
  SET(BUILD_TESTS OFF)
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
IF(NOT BUILD_CORE_ONLY)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(textures)
ADD_SUBDIRECTORY(externalplugins)
ADD_SUBDIRECTORY(demos)
ENDIF(NOT BUILD_CORE_ONLY)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(tlpscripten)

IF(BUILD_TESTS)
  INCLUDE(CTest)
  ENABLE_TESTING()
  IF(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure --build-config "$<CONFIGURATION>")
  ELSE(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure)
  ENDIF(CMAKE_CONFIGURATION_TYPES)
  ADD_SUBDIRECTORY(unit_test)
ENDIF(BUILD_TESTS)

INSTALL(FILES FindTULIP.cmake FindGLEW.cmake TulipUseFile.cmake DESTINATION ${TulipShareInstallDir})

IF(NOT BUILD_CORE_ONLY)
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
  SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} -l${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY}")
ELSE(WIN32)
  SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY}")
ENDIF(WIN32)
SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE} ${QT_QTWEBKIT_LIBRARY_RELEASE}")
CONFIGURE_FILE("tulip-config.in" "tulip-config" @ONLY)

INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config DESTINATION ${TulipBinInstallDir})
INSTALL(DIRECTORY wizards DESTINATION ${TulipShareInstallDir}
  PATTERN ".svn" EXCLUDE)

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
ENDIF(OPENMP_FOUND)
ENDIF(MSVC)

IF(WIN32)

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

        IF(EXISTS "${NSIS_PATH}/Plugins/inetc.dll")

            STRING(REPLACE "/" "\\" NSIS_PATH "${NSIS_PATH}")
            STRING(REPLACE "/" "\\" BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
            STRING(REPLACE "/" "\\" TLP_DIR "${CMAKE_INSTALL_PREFIX}")
            STRING(REPLACE "/" "\\" QTX_PLUGINS_DIR "${QT_PLUGINS_DIR}")

            IF(TulipBuildIsRelease)
                SET(DEBUG_MODE "FALSE")
            ELSE(TulipBuildIsRelease)
                SET(DEBUG_MODE "TRUE")
            ENDIF(TulipBuildIsRelease)

            ADD_CUSTOM_TARGET(bundle
                              COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                              COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/win_bundle.bat ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/win_bundle.bat
                              COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/COPYING ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/COPYING
                              COMMAND cmd /C win_bundle.bat ${NSIS_PATH} ${TLP_DIR} ${BINARY_DIR} ${DEBUG_MODE}
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win)

        ELSE(EXISTS "${NSIS_PATH}/Plugins/inetc.dll")

            MESSAGE("NSIS Inetc plugin is required to build the Tulip installer.")
            MESSAGE("Download the plugin archive from http://nsis.sourceforge.net/Inetc_plug-in and extract it in ${NSIS_PATH}.")

        ENDIF(EXISTS "${NSIS_PATH}/Plugins/inetc.dll")

ELSE(NOT NSIS_PATH_EMPTY)

        MESSAGE("Nullsoft Scriptable Install System is not installed on your system")
        MESSAGE("Tulip installer can not be generated without Nullsoft NSIS.")

ENDIF(NOT NSIS_PATH_EMPTY)

ENDIF(WIN32)

IF(APPLE)

	CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac/mac_bundle.sh.in" "${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac/mac_bundle.sh" @ONLY)
        ADD_CUSTOM_TARGET(bundle
                          COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                          COMMAND sh mac_bundle.sh ${CMAKE_CURRENT_BINARY_DIR}
                          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac)

ENDIF(APPLE)
ELSEIF(LINUX)
# generate script to make a tar archive of minimal set of tulip core files
  SET(make_tar_file ${PROJECT_BINARY_DIR}/tar-tulip-${TulipVersion}-corefiles.sh)
  execute_process(COMMAND rm ${make_tar_file} OUTPUT_QUIET ERROR_QUIET)
  execute_process(COMMAND echo "#!/bin/sh
if [ $# -eq 0 ]; then
TAR_FILE=${PROJECT_BINARY_DIR}/tulip-${TulipVersion}-corefiles.tar.gz
else
TAR_FILE=$1
fi
cd ${PROJECT_SOURCE_DIR}
tar zcvf \${TAR_FILE} $ENV{CORE_FILES}
echo archive \${TAR_FILE} successfully generated" OUTPUT_FILE ${make_tar_file})

message(STATUS "${make_tar_file} successfully generated")
ENDIF(NOT BUILD_CORE_ONLY)
