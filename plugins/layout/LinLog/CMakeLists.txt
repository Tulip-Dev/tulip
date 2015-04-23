INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(LinLog-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(LinLog-${TulipVersion}  ${LibTulipCoreName})

INSTALL_TULIP_PLUGIN(LinLog-${TulipVersion} ${TulipPluginsInstallDir})
