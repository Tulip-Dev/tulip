INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(Grip-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(Grip-${TulipVersion}  ${LibTulipCoreName})

INSTALL(TARGETS Grip-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Grip)
