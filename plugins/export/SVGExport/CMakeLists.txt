INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${CMAKE_CURRENT_SOURCE_DIR} ${QT_USE_FILE})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(SVGExport-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${QT_LIBRARIES})

INSTALL_TULIP_PLUGIN(SVGExport-${TulipVersion} ${TulipPluginsInstallDir})
