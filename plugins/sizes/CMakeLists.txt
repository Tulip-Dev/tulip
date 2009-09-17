INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${LIBXML2_INCLUDE_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(MetricSizeMapping_SRCS MetricMapping.cpp )
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED ${MetricSizeMapping_SRCS})
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(MetricSizeMapping-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(AutoSize_SRCS AutoSize.cpp)
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED ${AutoSize_SRCS})
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(AutoSize-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(FitToLabels_SRCS FitToLabel.cpp)
ADD_LIBRARY(FitToLabels-${TulipVersion} SHARED ${FitToLabels_SRCS})
TARGET_LINK_LIBRARIES(FitToLabels-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(FitToLabels-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(FitToLabels-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	MetricSizeMapping-${TulipVersion}
	AutoSize-${TulipVersion}
	FitToLabels-${TulipVersion}
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
