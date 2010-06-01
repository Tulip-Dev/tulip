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


CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

## -----------------------------------------------------------------------------------------------
## Tulip version numbers
## TulipVersion : Ma.Mi.Re
## TulipMMVersion : Ma.Mi
## -----------------------------------------------------------------------------------------------
SET(TulipMajorVersion 3)
SET(TulipMinorVersion 4)
SET(TulipReleaseVersion 0)
SET(TulipVersion "${TulipMajorVersion}.${TulipMinorVersion}.${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}.${TulipMinorVersion}")

## -----------------------------------------------------------------------------------------------
## Lib names
## -----------------------------------------------------------------------------------------------
SET(LibTulipName "tulip-${TulipMMVersion}")
SET(LibTulipOglName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipQtName "tulip-qt4-${TulipMMVersion}")
SET(LibTulipPluginsManagerName "tulip-pluginsmanager-${TulipMMVersion}")

## -----------------------------------------------------------------------------------------------
## Find need packages
## -----------------------------------------------------------------------------------------------
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(Qt4  REQUIRED)
INCLUDE(${QT_USE_FILE})
IF(DEFINED CMAKE_VERSION)
  SET(HAVE_QT4_GET_MOC_FLAGS TRUE)
ELSE(DEFINED CMAKE_VERSION)
  MACRO (QT4_GET_MOC_FLAGS _moc_flags)
     SET(${_moc_flags})
     GET_DIRECTORY_PROPERTY(_inc_DIRS INCLUDE_DIRECTORIES)

     FOREACH(_current ${_inc_DIRS})
        SET(${_moc_flags} ${${_moc_flags}} "-I${_current}")
     ENDFOREACH(_current ${_inc_DIRS})
     
     GET_DIRECTORY_PROPERTY(_defines COMPILE_DEFINITIONS)
     FOREACH(_current ${_defines})
        SET(${_moc_flags} ${${_moc_flags}} "-D${_current}")
     ENDFOREACH(_current ${_defines})

     IF(Q_WS_WIN)
       SET(${_moc_flags} ${${_moc_flags}} -DWIN32)
     ENDIF(Q_WS_WIN)

  ENDMACRO(QT4_GET_MOC_FLAGS)
ENDIF(DEFINED CMAKE_VERSION)
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
INCLUDE(FindXML2.cmake)
IF(NOT LIBXML2_FOUND)
   message(FATAL_ERROR "Cannot find the XML2 library (required)")
ENDIF(NOT LIBXML2_FOUND)

## -----------------------------------------------------------------------------------------------
## Include file paths
## -----------------------------------------------------------------------------------------------
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(TulipInclude ${PROJECT_SOURCE_DIR}/library/tulip/include/)
SET(TulipBuildInclude ${PROJECT_BINARY_DIR}/library/tulip/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipQtInclude ${PROJECT_SOURCE_DIR}/library/tulip-qt/include/)
SET(TulipQtBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-qt/include/)
SET(TulipPluginsManagerInclude ${PROJECT_SOURCE_DIR}/software/plugins-manager/include/)

## -----------------------------------------------------------------------------------------------
## Install paths
## -----------------------------------------------------------------------------------------------
SET(TulipLibInstallDir /lib/)
SET(TulipBinInstallDir /bin/)
SET(TulipIncludeInstallDir /include/)
SET(TulipShareInstallDir /share/)
SET(TulipBitmapInstallDir /lib/tlp/bitmaps/)

## -----------------------------------------------------------------------------------------------
## Set UseTulip file name
## -----------------------------------------------------------------------------------------------
SET(TulipUseFile UseTulip.cmake)

## -----------------------------------------------------------------------------------------------
## Disable deprecated compilation output
## -----------------------------------------------------------------------------------------------
if (DEFINED NO_DEPRECATED)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif (DEFINED NO_DEPRECATED)

## -----------------------------------------------------------------------------------------------
## Disable process to make MainController plugin (for developpers)
## -----------------------------------------------------------------------------------------------
SET(TULIP_USE_MAIN_CONTROLLER ON CACHE BOOL "Link Main controller plugin")
IF(NOT TULIP_USE_MAIN_CONTROLLER)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITHOUT_MAIN_CONTROLLER")
ENDIF(NOT TULIP_USE_MAIN_CONTROLLER)

## -----------------------------------------------------------------------------------------------
## Disable process to make MainController plugin (for developpers)
## -----------------------------------------------------------------------------------------------
SET(TULIP_USE_QT_PICTURE_OUTPUT ON CACHE BOOL "Use Qt picture output")
IF(NOT TULIP_USE_QT_PICTURE_OUTPUT)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITHOUT_QT_PICTURE_OUTPUT")
ENDIF(NOT TULIP_USE_QT_PICTURE_OUTPUT)


## -----------------------------------------------------------------------------------------------
## Define NDEBUG when we are in release mode
## -----------------------------------------------------------------------------------------------
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -funroll-loops -pipe")

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
IF(WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -DDLL_EXPORT -DQT_ASSISTANT='\"assistant_adp\"'")
   SET(QT_ASSISTANT assistant_adp)
ENDIF(WIN32)
IF(APPLE)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -DQT_ASSISTANT='\"Assistant_adp\"'")
ENDIF(APPLE)

EXEC_PROGRAM(uname OUTPUT_VARIABLE UNAME_OUT)
STRING(COMPARE EQUAL "${UNAME_OUT}" "Linux" LINUX)
IF(NOT LINUX)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "GNU/kFreeBSD" LINUX)
ENDIF(NOT LINUX)
IF(LINUX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF(LINUX)

## -----------------------------------------------------------------------------------------------
## OpenMP
## -----------------------------------------------------------------------------------------------

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
IF(WIN32)
## HACK: do not know why OpenMP_CXX_FLAGS is not set when running on window ?
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
ELSE(WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF(WIN32)
ENDIF(OPENMP_FOUND)

## -----------------------------------------------------------------------------------------------
## Create library/tulip/include/tulip/TulipRelease.h base on TulipRelease.h.in
## -----------------------------------------------------------------------------------------------
SET(VERSION "${TulipVersion}")
CONFIGURE_FILE("library/tulip/include/tulip/TulipRelease.h.in" "library/tulip/include/tulip/TulipRelease.h")

## -----------------------------------------------------------------------------------------------
## Sub dir (no comment)
## -----------------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(plugins)
FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
  OPTION(GENERATE_DOC "Do you want to generate the documentation ? [OFF|ON]")
  if (GENERATE_DOC)
    ADD_SUBDIRECTORY(docs)
    SET(TULIP_LIB_DESC "Tulip Graph Library")
    SET(TULIP_LIB_NAME "tulip")
    CONFIGURE_FILE("docs/doxygen/tulip-lib.doxygen.in" "docs/doxygen/tulip-lib.doxygen")
    SET(TULIP_LIB_DESC "Tulip Open GL Library")
    SET(TULIP_LIB_NAME "tulip-ogl")
    CONFIGURE_FILE("docs/doxygen/tulip-lib.doxygen.in" "docs/doxygen/tulip-ogl-lib.doxygen")
    SET(TULIP_LIB_DESC "Tulip Qt Library")
    SET(TULIP_LIB_NAME "tulip-qt")
    CONFIGURE_FILE("docs/doxygen/tulip-lib.doxygen.in" "docs/doxygen/tulip-qt-lib.doxygen")
    EXEC_PROGRAM(date ARGS "+%Y" OUTPUT_VARIABLE THIS_YEAR)
    CONFIGURE_FILE("docs/doxygen/tulip.html.in" "docs/doxygen/tulip.html")
    CONFIGURE_FILE("docs/doxygen/header.html.in" "docs/doxygen/header.html")
    CONFIGURE_FILE("docs/doxygen/footer.html.in" "docs/doxygen/footer.html")
    CONFIGURE_FILE("docs/doxygen/header-allPlugins.html.in" "docs/doxygen/header-allPlugins.html")
    CONFIGURE_FILE("docs/doxygen/tulip-plugins.doxygen.in" "docs/doxygen/tulip-plugins.doxygen")    
  endif(GENERATE_DOC)
endif(DOXYGEN_FOUND)

## -----------------------------------------------------------------------------------------------
## Set var to create tulip-config script
## -----------------------------------------------------------------------------------------------
SET(prefix "${CMAKE_INSTALL_PREFIX}")
SET(exec_prefix "\${prefix}")
SET(includedir "\${prefix}${TulipIncludeInstallDir}")
SET(libdir "\${prefix}${TulipLibInstallDir}")
SET(sharedir "\${prefix}${TulipShareInstallDir}")
SET(libversion ${TulipMMVersion})
SET(VERSION "${TulipVersion}")
SET(WIN_VERSION "${TulipMajorVersion}.${TulipMinorVersion}")
SET(QT_CPPFLAGS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")

SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY}")

SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE}")
SET(QT_INCLUDES "-I${QT_INCLUDE_DIR}")

GET_TARGET_PROPERTY(CMakeTulipLib ${LibTulipName} LOCATION)
GET_TARGET_PROPERTY(CMakeTulipOglLib ${LibTulipOglName} LOCATION)
GET_TARGET_PROPERTY(CMakeTulipQtLib ${LibTulipQtName} LOCATION)
CONFIGURE_FILE("tulip-config.cmake.in" "tulip-config" @ONLY)

## -----------------------------------------------------------------------------------------------
## create pluginstest.cpp
## -----------------------------------------------------------------------------------------------
CONFIGURE_FILE("unit_test/plugins/pluginstest.cpp.in" "unit_test/plugins/pluginstest.cpp" @ONLY)

## -----------------------------------------------------------------------------------------------
## Create tulip.nsi script
## -----------------------------------------------------------------------------------------------
#IF(WIN32)
  EXEC_PROGRAM(grep ARGS "mingw /etc/fstab"  OUTPUT_VARIABLE MINDIR)
  STRING(REGEX REPLACE "[ \t]+/mingw" "" MINDIR "${MINDIR}")
  SET(qt_libraries ${QT_LIBRARY_DIR})
  SET(WIN_EXE_VERSION ${TulipMajorVersion}_${TulipMinorVersion}_${TulipReleaseVersion})
  CONFIGURE_FILE("tulip.nsi.in" "tulip-${VERSION}.nsi" @ONLY)
  ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_BINARY_DIR}/COPYING.LESSER
		DEPENDS ${PROJECT_SOURCE_DIR}/COPYING.LESSER
		COMMAND ${CMAKE_COMMAND}
		ARGS -E copy_if_different ${PROJECT_SOURCE_DIR}/COPYING.LESSER ${PROJECT_BINARY_DIR}/COPYING.LESSER)
  ADD_CUSTOM_TARGET(wininst makensis.exe "tulip-${VERSION}.nsi"
		DEPENDS ${PROJECT_BINARY_DIR}/COPYING.LESSER)
#ENDIF(WIN32)

## -----------------------------------------------------------------------------------------------
## Installation of files
## -----------------------------------------------------------------------------------------------
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipBinInstallDir}")
INSTALL(FILES UseTulip.cmake 
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipShareInstallDir}")
INSTALL(FILES UseTulipPlugins.cmake 
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipShareInstallDir}")

