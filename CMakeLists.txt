PROJECT(tulip)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

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
SET(TulipMinorVersion 2)
SET(TulipReleaseVersion 0)
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
SET(TulipLibs ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${LibTulipOGDFName})

## ========================================================
## Configure file for uninstall target
## ========================================================
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake VERBATIM)

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
ENDFUNCTION()

## ========================================================
## Compiler-specific options
## ========================================================
SET_COMPILER_OPTIONS()
IF(MSVC)
  IF(${CMAKE_GENERATOR} MATCHES "Visual Studio 9") # Visual studio 2008 needs boost
    FIND_PACKAGE(BOOST REQUIRED)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}/boost/tr1)
  ENDIF()

  # Tells VS to use multiple threads to compile
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  # Makes VS define M_PI
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
  # Prevents VS to define min and max macros (name clash with std::min and std::max)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MACHINE:X86")
ENDIF()
# Use debug mode with GLIBC
IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ARG1}" MATCHES ".*[g][+][+].*")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGLIBCXX")
ENDIF()

IF(BUILD_TESTS)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  INCLUDE(CTest)
ENDIF()

## ========================================================
## Required packages
## ========================================================

# Qt
FIND_PACKAGE(Qt4 4.7.0 REQUIRED)
SET(QT_USE_QTOPENGL true)
SET(QT_USE_QTXML true)
SET(QT_USE_QTNETWORK true)
SET(QT_USE_QTWEBKIT true)
SET(QT_USE_QTTEST true)
SET(QT_USE_QTDBUS false)
INCLUDE(${QT_USE_FILE})

# Freetype
FIND_PACKAGE(Freetype REQUIRED)

# ZLIB
FIND_PACKAGE(ZLIB REQUIRED)

# Yajl
SET(YAJL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/api ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yajl/src/yajl ${CMAKE_CURRENT_BUILD_DIR}/thirdparty/yajl/yajl-2.0.1/include)

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)

# Glew
FIND_PACKAGE(GLEW REQUIRED)

# OpenMP
FIND_PACKAGE(Threads)
FIND_PACKAGE(OpenMP)

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
      # Force setting OpenMP flags on Windows plateforms
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

# Python
# After finding the Python interpreter, try to find if SIP and its dev tools are installed on the host system.
# If not, compile the SIP version located in thirdparty.
FIND_PACKAGE(PythonInterp REQUIRED)
IF(PYTHONINTERP_FOUND)
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version ERROR_VARIABLE PYTHON_VERSION_RAW)
  STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
  STRING(REGEX MATCH "[0-9]\\.[0-9]" PYTHON_VERSION "${PYTHON_VERSION_RAW}")

  # Find the Python library with the same version as the interpreter
  # Starting Python 3.2, the library is suffixed by mu, also handle that case
  SET(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION}mu ${PYTHON_VERSION})
  FIND_PACKAGE(PythonLibs REQUIRED)

  IF(PYTHONLIBS_FOUND)
    SET(SIP_OK FALSE CACHE INTERNAL "")
    FIND_PACKAGE(SIP)
    SET(SIP_OK ${SIP_FOUND})
    IF(SIP_OK)
      SET(SIP_EXE "${SIP_EXECUTABLE}")
      SET(SYSTEM_SIP TRUE)
    ELSE(SIP_OK)
      SET(SIP_LIB sip)
      SET(SYSTEM_SIP FALSE)
      TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.14/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-4.14/sipgen sip CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES})
      IF(SIP_OK)
        SET(SIP_VERSION_STR "4.14")
        IF(WIN32 AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.14/sipgen/Debug/sip.exe")
          SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.14/sipgen/Debug/sip.exe)
        ELSE()
          SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.14/sipgen/sip)
        ENDIF()
        SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-4.14/siplib/)
      ENDIF(SIP_OK)
    ENDIF(SIP_OK)
    SET(Sphinx_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/python)
    MARK_AS_ADVANCED(Sphinx_DIR)
    FIND_PACKAGE(Sphinx)
  ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)


## ========================================================
## Include paths
## ========================================================
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(QuazipInclude ${PROJECT_SOURCE_DIR}/thirdparty/quazip)
SET(QxtInclude ${PROJECT_SOURCE_DIR}/thirdparty/qxt/src)
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(qscintilla2Include ${PROJECT_SOURCE_DIR}/thirdparty/QScintilla-gpl-2.5.1/Qt4/)
SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF)
SET(OGDFLibrary OGDF)
SET(QHULLInclude ${PROJECT_SOURCE_DIR}/thirdparty)
SET(TulipCoreInclude ${PROJECT_SOURCE_DIR}/library/tulip-core/include/)
SET(TulipCoreBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-core/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipOGDFInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogdf/)
SET(TulipGUIInclude ${PROJECT_SOURCE_DIR}/library/tulip-gui/include/)
SET(TulipGUIBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-gui/include/)
SET(TulipPythonInclude ${PROJECT_SOURCE_DIR}/library/tulip-python/include ${SIP_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

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
## Enable debugging of OpenMP
## ========================================================
IF(OPENMP_FOUND AND CMAKE_DEBUG_MODE AND NOT CHECK_MEMORY)
  SET(DEBUG_OPENMP OFF CACHE BOOL "Do you want to enable OpenMP in Debug mode ? [OFF|ON]")
  IF(DEBUG_OPENMP)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OPENMP_CXX_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OPENMP_LINKER_FLAGS}")
    SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${OPENMP_LIBRARIES}")
  ENDIF(DEBUG_OPENMP)
ENDIF(OPENMP_FOUND AND CMAKE_DEBUG_MODE AND NOT CHECK_MEMORY)

## ========================================================
## Enable output of rendering time only if OpenMP is enabled
## ========================================================
IF(OPENMP_FOUND AND ((NOT CMAKE_DEBUG_MODE) OR DEBUG_OPENMP))
  SET(ENABLE_RENDERING_TIME_DISPLAY OFF CACHE BOOL "Do you want to enable the rendering time display ? [OFF|ON]")
ENDIF(OPENMP_FOUND AND ((NOT CMAKE_DEBUG_MODE) OR DEBUG_OPENMP))

## ========================================================
## Unit tests
## ========================================================
SET(BUILD_TESTS OFF CACHE BOOL "Do you want to build the tests ? [OFF|ON]")

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
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(textures)
ADD_SUBDIRECTORY(externalplugins)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(demos)
IF(BUILD_TESTS)
  ADD_SUBDIRECTORY(unit_test)
ENDIF()

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
INSTALL(FILES FindTULIP.cmake FindGLEW.cmake TulipUseFile.cmake DESTINATION ${TulipShareInstallDir})
INSTALL(DIRECTORY wizards DESTINATION ${TulipShareInstallDir}
  PATTERN ".svn" EXCLUDE)
