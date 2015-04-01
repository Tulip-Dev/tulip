ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED MetricMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS MetricSizeMapping-${TulipVersion} RUNTIME DESTINATION ${TulipPluginsInstallDir} COMPONENT MetricSizeMapping)
INSTALL(TARGETS AutoSize-${TulipVersion} RUNTIME DESTINATION ${TulipPluginsInstallDir} COMPONENT AutoSize)
