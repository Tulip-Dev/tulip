INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
ADD_LIBRARY(ToLabels-${Tulip_VERSION} SHARED ToLabels.cpp)
TARGET_LINK_LIBRARIES(ToLabels-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(ToLabels-${Tulip_VERSION} ${TulipPluginsInstallDir})
