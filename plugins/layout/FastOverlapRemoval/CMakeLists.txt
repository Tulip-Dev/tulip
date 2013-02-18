INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(FastOverlapRemoval-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(FastOverlapRemoval-${TulipVersion}  ${LibTulipCoreName})

INSTALL(TARGETS FastOverlapRemoval-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT FastOverlapRemoval)
