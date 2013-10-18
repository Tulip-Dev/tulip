INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(LinLog-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(LinLog-${TulipVersion}  ${LibTulipCoreName})

INSTALL(TARGETS LinLog-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT LinLog)
