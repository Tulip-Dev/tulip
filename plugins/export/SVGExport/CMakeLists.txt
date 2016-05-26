INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR} ${QT_USE_FILE})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(SVGExport-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${QT_LIBRARIES})

INSTALL_TULIP_PLUGIN(SVGExport-${TulipVersion} ${TulipPluginsInstallDir})
