cmake_minimum_required(VERSION 2.6)

set(GZSTREAM_INCLUDE ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
set(GLE_INCLUDE ${PROJECT_SOURCE_DIR}/thirdparty/gle/)
set(FTGL_INCLUDE ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
set(TULIP_INCLUDE ${PROJECT_SOURCE_DIR}/library/tulip/include/)
set(TULIP_OGL_INCLUDE ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
set(TULIP_QT_INCLUDE ${PROJECT_SOURCE_DIR}/library/tulip-qt/include/)
set(TULIP_PLUGINSMANAGER_INCLUDE ${PROJECT_SOURCE_DIR}/software/plugins-manager/include/)

set(TULIP_LIB_INSTALL_DIR /lib/)
set(TULIP_BIN_INSTALL_DIR /bin/)
set(TULIP_INCLUDE_INSTALL_DIR /include/)
set(TULIP_BITMAP_INSTALL_DIR /lib/tlp/bitmaps/)

INCLUDE(FindXML2.cmake)

add_subdirectory(thirdparty)
add_subdirectory(library)
add_subdirectory(software)
