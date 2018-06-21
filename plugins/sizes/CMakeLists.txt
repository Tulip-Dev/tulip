ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED SizeMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(MetricSizeMapping-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(AutoSize-${TulipVersion} ${TulipPluginsInstallDir})
