ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED MetricMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS MetricSizeMapping-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT MetricSizeMapping)
INSTALL(TARGETS AutoSize-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT AutoSize)
