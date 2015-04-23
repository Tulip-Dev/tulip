ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED MetricMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL_TULIP_PLUGIN(MetricSizeMapping-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(AutoSize-${TulipVersion} ${TulipPluginsInstallDir})
