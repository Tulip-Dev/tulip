INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(FastOverlapRemoval-${Tulip_VERSION} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(FastOverlapRemoval-${Tulip_VERSION} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(FastOverlapRemoval-${Tulip_VERSION} ${TulipPluginsInstallDir})
