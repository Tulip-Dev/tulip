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


CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

## -----------------------------------------------------------------------------------------------
## Tulip version numbers
## TulipVersion : Ma.Mi.Re
## TulipMMVersion : Ma.Mi
## -----------------------------------------------------------------------------------------------
SET(TulipMajorVersion 3)
SET(TulipMinorVersion 3)
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
FIND_PACKAGE(Freetype)
FIND_PACKAGE(Qt4  REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(JPEG)
IF(JPEG_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBJPEG")
ENDIF(JPEG_FOUND)
FIND_PACKAGE(PNG)
IF(PNG_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBPNG")
ENDIF(PNG_FOUND)

INCLUDE(FindGLEW.cmake)
INCLUDE(FindXML2.cmake)

## -----------------------------------------------------------------------------------------------
## Include file paths
## -----------------------------------------------------------------------------------------------
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(GLEInclude ${PROJECT_SOURCE_DIR}/thirdparty/gle/)
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
## Qt assistant config
## -----------------------------------------------------------------------------------------------
IF(WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -DDLL_EXPORT -DQT_ASSISTANT='\"assistant_adp\"'")
ENDIF(WIN32)
IF(APPLE)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -DQT_ASSISTANT='\"Assistant_adp\"'")
ENDIF(APPLE)

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
    CONFIGURE_FILE("docs/doxygen/dox-html.xsl.in" "docs/doxygen/dox-html.xsl")
    CONFIGURE_FILE("docs/doxygen/tulip-plugins.doxygen.in" "docs/doxygen/tulip-plugins.doxygen")
    CONFIGURE_FILE("docs/doxygen/dox-html.xsl.in" "docs/doxygen/dox-html.xsl")
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
SET(VERSION "${TulipVersion}")
SET(WIN_VERSION "${TulipMajorVersion}.${TulipMinorVersion}")
SET(QT_CPPFLAGS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")
SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} -l${OPENGL_glu_LIBRARY} -l${GLEW_LIBRARY}")
SET(QT_LDFLAGS "-l${QT_QTCORE_LIBRARY_RELEASE} -l${QT_QTGUI_LIBRARY_RELEASE} -l${QT_QTNETWORK_LIBRARY_RELEASE} -l${QT_QTOPENGL_LIBRARY_RELEASE} -l${QT_QTXML_LIBRARY_RELEASE}")
SET(QT_INCLUDES "-I${QT_INCLUDE_DIR}")
GET_TARGET_PROPERTY(CMakeTulipLib ${LibTulipName} LOCATION)
GET_TARGET_PROPERTY(CMakeTulipOglLib ${LibTulipOglName} LOCATION)
GET_TARGET_PROPERTY(CMakeTulipQtLib ${LibTulipQtName} LOCATION)
CONFIGURE_FILE("tulip-config.in" "tulip-config" @ONLY)

## -----------------------------------------------------------------------------------------------
## Create tulip.nsi script
## -----------------------------------------------------------------------------------------------
IF(WIN32)
  EXEC_PROGRAM(grep ARGS "mingw /etc/fstab" OUTPUT_VARIABLE MINDIR)
  STRING(REPLACE " /mingw" "" MINDIR ${MINDIR})
  CONFIGURE_FILE("tulip_cmake.nsi.in" "tulip.nsi" @ONLY)
ENDIF(WIN32)

## -----------------------------------------------------------------------------------------------
## Installation of files
## -----------------------------------------------------------------------------------------------
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipBinInstallDir}")
INSTALL(FILES UseTulip.cmake 
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipShareInstallDir}")
INSTALL(FILES UseTulipPlugins.cmake 
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipShareInstallDir}")

