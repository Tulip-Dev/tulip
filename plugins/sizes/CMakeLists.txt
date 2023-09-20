ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${Tulip_VERSION} SHARED SizeMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${Tulip_VERSION} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(MetricSizeMapping-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(AutoSize-${Tulip_VERSION} ${TulipPluginsInstallDir})
