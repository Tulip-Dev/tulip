INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${LIBXML2_INCLUDE_DIR})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED MetricMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(FitToLabels-${TulipVersion} SHARED FitToLabel.cpp)
TARGET_LINK_LIBRARIES(FitToLabels-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
  MetricSizeMapping-${TulipVersion}
  AutoSize-${TulipVersion}
  FitToLabels-${TulipVersion}
DESTINATION ${TulipPluginsInstallDir}
COMPONENT tulip_ogl_plugins)
