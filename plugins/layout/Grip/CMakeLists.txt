INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(Grip-${Tulip_VERSION} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(Grip-${Tulip_VERSION}  ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(Grip-${Tulip_VERSION} ${TulipPluginsInstallDir})
