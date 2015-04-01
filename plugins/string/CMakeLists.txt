INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
ADD_LIBRARY(ToLabels-${TulipVersion} SHARED tolabels.cpp)
TARGET_LINK_LIBRARIES(ToLabels-${TulipVersion} ${LibTulipCoreName})
INSTALL(TARGETS ToLabels-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ToLabels)
