INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(LinLog-${Tulip_VERSION} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(LinLog-${Tulip_VERSION}  ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(LinLog-${Tulip_VERSION} ${TulipPluginsInstallDir})
