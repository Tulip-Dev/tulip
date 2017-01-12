INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(SVGExport-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

INSTALL_TULIP_PLUGIN(SVGExport-${TulipVersion} ${TulipPluginsInstallDir})
