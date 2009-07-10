INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude} ${LIBXML2_INCLUDE_DIR})

SET(MetricSizeMapping_SRCS MetricMapping.cpp )
ADD_LIBRARY(MetricSizeMapping-${TulipVersion} SHARED ${MetricSizeMapping_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ MetricSizeMapping-${TulipVersion})

SET(AutoSize_SRCS AutoSize.cpp)
ADD_LIBRARY(AutoSize-${TulipVersion} SHARED ${AutoSize_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ AutoSize-${TulipVersion})

SET(FitToLabels_SRCS FitToLabel.cpp)
ADD_LIBRARY(FitToLabels-${TulipVersion} SHARED ${FitToLabels_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ FitToLabels-${TulipVersion})
