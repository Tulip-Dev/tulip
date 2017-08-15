INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(Grip-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(Grip-${TulipVersion}  ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(Grip-${TulipVersion} ${TulipPluginsInstallDir})
