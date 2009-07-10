CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(TulipMajorVersion 3)
SET(TulipMinorVersion 2)
SET(TulipReleaseVersion 0)
SET(TulipVersion "${TulipMajorVersion}.${TulipMinorVersion}.${TulipReleaseVersion}")

FIND_PACKAGE(Freetype)
FIND_PACKAGE(Qt4)
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

SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(GLEInclude ${PROJECT_SOURCE_DIR}/thirdparty/gle/)
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(TulipInclude ${PROJECT_SOURCE_DIR}/library/tulip/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipQtInclude ${PROJECT_SOURCE_DIR}/library/tulip-qt/include/)
SET(TulipPluginsManagerInclude ${PROJECT_SOURCE_DIR}/software/plugins-manager/include/)

SET(TulipLibInstallDir /lib/)
SET(TulipBinInstallDir /bin/)
SET(TulipIncludeInstallDir /include/)
SET(TulipBitmapInstallDir /lib/tlp/bitmaps/)

SET(TulipUseFile /UseTulip.cmake)

INCLUDE(FindXML2.cmake)
INCLUDE(FindGLEW.cmake)

if (DEFINED NO_DEPRECATED)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif (DEFINED NO_DEPRECATED)

SET(TULIP_RELEASE "${TulipVersion}")
CONFIGURE_FILE("library/tulip/include/tulip/TulipRelease.h.in" "library/tulip/include/tulip/TulipRelease.h")

SET(prefix "${CMAKE_INSTALL_PREFIX}")
SET(exec_prefix "\${prefix}")
SET(includedir "\${prefix}${TulipIncludeInstallDir}")
SET(libdir "\${prefix}${TulipLibInstallDir}")
SET(VERSION "${TulipVersion}")
SET(WIN_VERSION "${TulipMajorVersion}.${TulipMinorVersion}")
SET(QT_CPPFLAGS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")
SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} -l${OPENGL_glu_LIBRARY} -l${GLEW_LIBRARY}")
SET(QT_LDFLAGS "-l${QT_QTCORE_LIBRARY_RELEASE} -l${QT_QTGUI_LIBRARY_RELEASE} -l${QT_QTNETWORK_LIBRARY_RELEASE} -l${QT_QTOPENGL_LIBRARY_RELEASE} -l${QT_QTXML_LIBRARY_RELEASE}")
SET(QT_INCLUDES "-I${QT_INCLUDE_DIR}")
CONFIGURE_FILE("tulip-config.in" "tulip-config")

INSTALL(PROGRAMS tulip-config
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipBinInstallDir}" 
)

ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(plugins)
