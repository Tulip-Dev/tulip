QTX_SET_INCLUDES_AND_DEFINITIONS()
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(SVGExport-${Tulip_VERSION} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${QT_LIBRARIES})

TULIP_INSTALL_PLUGIN(SVGExport-${Tulip_VERSION} ${TulipPluginsInstallDir})
