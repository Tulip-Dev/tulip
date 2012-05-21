## -----------------------------------------------------------------------------------------------
## ***********************************************************************************************
## -----------------------------------------------------------------------------------------------
##
## Options for cmake (use -D):
## CMAKE_INSTALL_PREFIX=directory_where_tulip_will_be_installed
## NO_DEPRECATE=1 disable display of deprecated warning
##
## -----------------------------------------------------------------------------------------------
## ***********************************************************************************************
## -----------------------------------------------------------------------------------------------
PROJECT(tulip)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

# Configure file for uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

## -----------------------------------------------------------------------------------------------
## Include macro definitions
## -----------------------------------------------------------------------------------------------
INCLUDE(TulipUseFile.cmake)
SET_COMPILER_OPTIONS()

# Hack: on Apple, we need to set specifically the value of INSTALL_NAME_DIR for every single bloody target
# since RPATH is not supported for the DyLib format.
IF(APPLE)
  FUNCTION(ADD_LIBRARY name)
    _ADD_LIBRARY(${name} ${ARGN})
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_NAME_DIR "@executable_path/../lib")
  ENDFUNCTION()
ENDIF(APPLE)


## -----------------------------------------------------------------------------------------------
## build type detection
IF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
   SET(CMAKE_DEBUG_MODE TRUE)
ELSE(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
   SET(CMAKE_DEBUG_MODE FALSE)
ENDIF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
## -----------------------------------------------------------------------------------------------
## Tulip version numbers
## TulipVersion : Ma.Mi.Re
## TulipMMVersion : Ma.Mi
## -----------------------------------------------------------------------------------------------
SET(TulipMajorVersion 4)
SET(TulipMinorVersion 92)
SET(TulipReleaseVersion 0)
SET(TulipVersionSeparator .)
IF(MSVC) #specific options for Visual Studio
  IF(${CMAKE_GENERATOR} MATCHES "Visual Studio 9") #visual studio 2008 needs boost
    FIND_PACKAGE(BOOST REQUIRED)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}/boost/tr1)
  ENDIF()
  SET(TulipVersionSeparator _)
ENDIF()
SET(TulipVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}${TulipVersionSeparator}${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}")
SET(TulipIntVersion "${TulipMajorVersion}${TulipMinorVersion}${TulipReleaseVersion}")

## -----------------------------------------------------------------------------------------------
## Lib names
## -----------------------------------------------------------------------------------------------
SET(LibTulipCoreName "tulip-core-${TulipMMVersion}")
SET(LibTulipOglName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipGUIName "tulip-gui-${TulipMMVersion}")
SET(LibTulipOGDFName "tulip-ogdf-${TulipMMVersion}")
SET(LibStlPythonName "stlpython")
SET(LibTulipCorePythonName "tulip-core-python-${TulipMMVersion}")
SET(LibTulip3CompatName "tulip3-compat-${TulipMMVersion}")

## -----------------------------------------------------------------------------------------------
## Find needed packages
## -----------------------------------------------------------------------------------------------
FIND_PACKAGE(Freetype REQUIRED)
IF(WIN32)
FIND_PACKAGE(Qt4 4.8.0 REQUIRED)
ELSE(WIN32)
FIND_PACKAGE(Qt4 4.7.0 REQUIRED)
ENDIF(WIN32)
SET(QT_USE_QTOPENGL true)
SET(QT_USE_QTXML true)
SET(QT_USE_QTNETWORK true)
SET(QT_USE_QTWEBKIT true)
SET(QT_USE_QTTEST true)
SET(QT_USE_QTDBUS false)
INCLUDE(${QT_USE_FILE})

SET(YAJL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/api ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/yajl
    ${CMAKE_CURRENT_BUILD_DIR}/thirdparty/yajl/yajl-2.0.1/include)
SET(QT_MODELS_TESTS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/qt_models_tests)
SET(QT_MODELS_TESTS_LIBRARY qt4_test_models)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
IF(JPEG_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBJPEG")
ENDIF(JPEG_FOUND)
FIND_PACKAGE(PNG REQUIRED)
IF(PNG_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBPNG")
ENDIF(PNG_FOUND)

INCLUDE(FindGLEW.cmake)
IF(NOT GLEW_FOUND)
   message(FATAL_ERROR "Cannot find the GLEW library (required)")
ENDIF(NOT GLEW_FOUND)

## -----------------------------------------------------------------------------------------------
## OpenMP
## -----------------------------------------------------------------------------------------------

FIND_PACKAGE (Threads)
FIND_PACKAGE(OpenMP QUIET)
IF(OPENMP_FOUND)
   SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
   SET(OPENMP_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
   IF(WIN32)
	  IF(MSVC)
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /openmp")
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
      ## HACK: do not know why OpenMP_CXX_FLAGS
      ## is not set when running on window ?
      SET(CMAKE_CXX_FLAGS_RELEASES "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_C_FLAGS}")
      SET(OPENMP_CXX_FLAGS "${OpenMP_C_FLAGS}")
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
      SET(OPENMP_LINKER_FLAGS "${OpenMP_C_FLAGS}")
      SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp -lpthread")
      SET(OPENMP_LIBRARIES "-lgomp -lpthread")
      SET(OPENMP_FOUND TRUE)
   ELSE(OMP_CFLAGS)
      MESSAGE("OpenMP not found: multithreaded code will not be used")
   ENDIF(OMP_CFLAGS)
  ELSE(WIN32)
    MESSAGE("OpenMP not found: multithreaded code will not be used")
  ENDIF(WIN32)
ENDIF(OPENMP_FOUND)

## -----------------------------------------------------------------------------------------------
## Python (not required)
## -----------------------------------------------------------------------------------------------

# Try to find Python (interpreter and library) in the first place
# If Python is present, try to find if SIP and its dev tools are installed on the host system.
# If not, compile the SIP version located in thirdparty.

FIND_PACKAGE(PythonInterp)

IF(PYTHONINTERP_FOUND)
    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version
                    ERROR_VARIABLE PYTHON_VERSION_RAW)
    STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
    STRING(REGEX MATCH "[0-9]\\.[0-9]"
           PYTHON_VERSION "${PYTHON_VERSION_RAW}")

    # Find the Python library with the same version as the interpreter
    # Starting Python 3.2, the library is suffixed by mu, also handle that case
    SET(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION}mu ${PYTHON_VERSION})
    FIND_PACKAGE(PythonLibs)

    IF(PYTHONLIBS_FOUND)
        SET(SIP_OK FALSE CACHE INTERNAL "")
        FIND_PACKAGE(SIP)
        SET(SIP_OK ${SIP_FOUND})
        IF (SIP_OK)
            SET(SIP_EXE "${SIP_EXECUTABLE}")
            SET(SYSTEM_SIP TRUE)
        ELSE(SIP_OK)
            SET(SIP_LIB sip)
            SET(SYSTEM_SIP FALSE)
            TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.13.2/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-4.13.2/sipgen sip CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES})
            IF(SIP_OK)
                SET(SIP_VERSION_STR "4.13.2")
                IF(WIN32 AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.13.2/sipgen/Debug/sip.exe")
                    SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.13.2/sipgen/Debug/sip.exe)
                ELSE()
                    SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.13.2/sipgen/sip)
                ENDIF()
                SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-4.13.2/siplib/)
            ENDIF(SIP_OK)
        ENDIF(SIP_OK)
        SET(Sphinx_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/python)
        MARK_AS_ADVANCED(Sphinx_DIR)
        FIND_PACKAGE(Sphinx)
    ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)

## -----------------------------------------------------------------------------------------------
## Include file paths
## -----------------------------------------------------------------------------------------------
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(QuazipInclude ${PROJECT_SOURCE_DIR}/thirdparty/quazip)
SET(QxtInclude ${PROJECT_SOURCE_DIR}/thirdparty/qxt/src)
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(qscintilla2Include ${PROJECT_SOURCE_DIR}/thirdparty/QScintilla-gpl-2.5.1/Qt4/)
SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF)
SET(OGDFLibrary OGDF)

SET(TulipCoreInclude ${PROJECT_SOURCE_DIR}/library/tulip-core/include/)
SET(TulipCoreBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-core/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipOGDFInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogdf/)
SET(TulipGUIInclude ${PROJECT_SOURCE_DIR}/library/tulip-gui/include/)
SET(TulipGUIBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/include/)
SET(Tulip3CompatInclude ${PROJECT_SOURCE_DIR}/library/tulip3-compat/include/)
SET(Tulip3CompatBuildInclude ${PROJECT_BINARY_DIR}/library/tulip3-compat/include/)

## -----------------------------------------------------------------------------------------------
## Install paths
## -----------------------------------------------------------------------------------------------
SET(TulipLibInstallDir lib/)
SET(TulipPluginsInstallDir ${TulipLibInstallDir}/tulip/)
SET(TulipBinInstallDir bin/)
SET(TulipIncludeInstallDir include/)
SET(TulipShareInstallDir share/tulip)
SET(TulipBitmapInstallDir ${TulipShareInstallDir}/bitmaps/)

## -----------------------------------------------------------------------------------------------
## Disable deprecated compilation output
## -----------------------------------------------------------------------------------------------
IF(DEFINED NO_DEPRECATED)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
ENDIF(DEFINED NO_DEPRECATED)

## -----------------------------------------------------------------------------------------------
## Disable Qt picture output. This is a developer flag used for VMs
## -----------------------------------------------------------------------------------------------
IF(${TULIP_DISABLE_QT_PICTURE_OUTPUT})
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITHOUT_QT_PICTURE_OUTPUT")
ENDIF()

## -----------------------------------------------------------------------------------------------
## Use debug mode weith GLIBC
## this is only useful when using a GNU CXX compiler.
## The second test is for the case where ccache is used (CMAKE_CXX_COMPILER_ARG1 contains the path to the g++ compiler)
## -----------------------------------------------------------------------------------------------
IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ARG1}" MATCHES ".*[g][+][+].*")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGLIBCXX")
ENDIF()

## -----------------------------------------------------------------------------------------------
## Define NDEBUG when we are in release mode
## -----------------------------------------------------------------------------------------------
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

## -----------------------------------------------------------------------------------------------
## Check 64 bits
## -----------------------------------------------------------------------------------------------
EXEC_PROGRAM(uname ARGS "-m" OUTPUT_VARIABLE UNAME_OUT)
STRING(COMPARE EQUAL "${UNAME_OUT}" "x86_64" I64)
IF(NOT I64)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "ia64" I64)
ENDIF(NOT I64)
IF(NOT I64)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "Power Macintosh" MACPPC)
ENDIF(NOT I64)
IF(I64)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DI64")
ENDIF(I64)
IF(MACPPC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMACPPC")
ENDIF(MACPPC)

## -----------------------------------------------------------------------------------------------
## Windows, Apple and Linux
## -----------------------------------------------------------------------------------------------
EXEC_PROGRAM(uname OUTPUT_VARIABLE UNAME_OUT)
STRING(COMPARE EQUAL "${UNAME_OUT}" "Linux" LINUX)
IF(NOT LINUX)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "GNU/kFreeBSD" LINUX)
ENDIF(NOT LINUX)
IF(LINUX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF(LINUX)

IF(WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -DQT_ASSISTANT='\"assistant\"'")
   IF(MSVC)
    #tells VS to use multiple threads to compile
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    #makes VS define M_PI
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
    #prevents VS to define min and max macros (name clash with std::min and std::max)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MACHINE:X86")

    #allows for link time optimization
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
    #link time optimization
    #SET(CMAKE_SHARED_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /LTCG")
   ENDIF()
ENDIF(WIN32)
IF(APPLE)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -DQT_ASSISTANT='\"Assistant\"'")
ENDIF(APPLE)
IF(LINUX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_ASSISTANT='\"assistant\"'")
ENDIF(LINUX)

## -----------------------------------------------------------------------------------------------
## Enable memory checks (disable openmp)
## -----------------------------------------------------------------------------------------------
IF(LINUX)
  OPTION(CHECK_MEMORY "Overload new and delete operators to check for memory leaks (This option can't be enabled along with OpenMP debugging)" OFF)
  IF(CHECK_MEMORY)
    SET(CHECK_MEMORY_FLAGS "-include ${CMAKE_SOURCE_DIR}/library/tulip-core/include/tulip/MemoryChecker.h")
  ENDIF(CHECK_MEMORY)
ENDIF(LINUX)

## -----------------------------------------------------------------------------------------------
## Enable debugging of OpenMP
## -----------------------------------------------------------------------------------------------
IF(OPENMP_FOUND AND CMAKE_DEBUG_MODE AND NOT CHECK_MEMORY)
  SET(DEBUG_OPENMP OFF CACHE BOOL "Do you want to enable OpenMP in Debug mode ? [OFF|ON]")
  IF(DEBUG_OPENMP)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OPENMP_CXX_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OPENMP_LINKER_FLAGS}")
    SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${OPENMP_LIBRARIES}")
  ENDIF(DEBUG_OPENMP)
ENDIF(OPENMP_FOUND AND CMAKE_DEBUG_MODE AND NOT CHECK_MEMORY)

## -----------------------------------------------------------------------------------------------
## Enable output of rendering time
## only if OpenMP is enabled
## -----------------------------------------------------------------------------------------------
IF(OPENMP_FOUND AND ((NOT CMAKE_DEBUG_MODE) OR DEBUG_OPENMP))
  SET(ENABLE_RENDERING_TIME_DISPLAY OFF CACHE BOOL "Do you want to enable the rendering time display ? [OFF|ON]")
ENDIF(OPENMP_FOUND AND ((NOT CMAKE_DEBUG_MODE) OR DEBUG_OPENMP))

## -----------------------------------------------------------------------------------------------
## Builds the unit tests
## -----------------------------------------------------------------------------------------------
SET(BUILD_TESTS OFF CACHE BOOL "Do you want to build the tests ? [OFF|ON]")

## -----------------------------------------------------------------------------------------------
## Create library/tulip/include/tulip/TulipRelease.h base on TulipRelease.h.in
## -----------------------------------------------------------------------------------------------
STRING(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "Release" result1)
STRING(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "RelWithDebInfo" result2)
IF(${result1} OR ${result2})
  SET(TulipBuildIsRelease 1)
ELSE()
  SET(TulipBuildIsRelease 0)
ENDIF()
CONFIGURE_FILE("library/tulip-core/include/tulip/TulipRelease.h.in" "library/tulip-core/include/tulip/TulipRelease.h")

## -----------------------------------------------------------------------------------------------
## Sub dir (no comment)
## -----------------------------------------------------------------------------------------------
IF(BUILD_TESTS)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
   SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
   SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
   SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  INCLUDE(CTest)
  ADD_SUBDIRECTORY(unit_test)
ENDIF()

BUNDLE_INIT()

ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(textures)
ADD_SUBDIRECTORY(externalplugins)
ADD_SUBDIRECTORY(doc)

## -----------------------------------------------------------------------------------------------
## Set var to create tulip-config script
## -----------------------------------------------------------------------------------------------
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

## -----------------------------------------------------------------------------------------------
## Installation of files
## -----------------------------------------------------------------------------------------------
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config DESTINATION ${TulipBinInstallDir} COMPONENT tulip_core_dev)
INSTALL(FILES FindTULIP.cmake FindGLEW.cmake TulipUseFile.cmake DESTINATION ${TulipShareInstallDir} COMPONENT tulip_core_dev)
## -----------------------------------------------------------------------------------------------
## Script for linux Tulip bundle building
IF(LINUX)
  CONFIGURE_FILE("make_tulip_linux_bundle.sh.in" "make_tulip_linux_bundle.sh" @ONLY)
  CONFIGURE_FILE("strip_debug_linux.sh.in" "strip_debug_linux.sh" @ONLY)
ENDIF(LINUX)

## -----------------------------------------------------------------------------------------------
## CPack packaging directives
## -----------------------------------------------------------------------------------------------

# Include external plugins configuration file.
# Note: this is done before setting group names variables. User can set the CPACK_CUSTOM_INSTALLER to true and setup its own installer in this file.
IF(EXISTS externalplugins/ExternalPlugins.cmake)
  INCLUDE(externalplugins/ExternalPlugins.cmake)
ENDIF()

# Defines specific software components from tulip.
INCLUDE(CPackComponents.cmake)
INCLUDE(CPackInstaller.cmake)
BUNDLE_CHECK()
BUNDLE_GENERATE_DEPS()

INCLUDE(CPack)


IF(APPLE)

ENDIF(APPLE)
