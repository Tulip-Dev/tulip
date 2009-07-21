INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude} ${LIBXML2_INCLUDE_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(MetricSizeMapping_SRCS MetricMapping.cpp )
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED ${MetricSizeMapping_SRCS})
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TulipVersion} tulip)
ADD_DEPENDENCIES(MetricSizeMapping-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(AutoSize_SRCS AutoSize.cpp)
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED ${AutoSize_SRCS})
TARGET_LINK_LIBRARIES(AutoSize-${TulipVersion} tulip)
ADD_DEPENDENCIES(AutoSize-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(FitToLabels_SRCS FitToLabel.cpp)
ADD_LIBRARY(FitToLabels-${TulipVersion} SHARED ${FitToLabels_SRCS})
TARGET_LINK_LIBRARIES(FitToLabels-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(FitToLabels-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(FitToLabels-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	MetricSizeMapping-${TulipVersion}
	AutoSize-${TulipVersion}
	FitToLabels-${TulipVersion}
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
