CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

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

ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(software)
