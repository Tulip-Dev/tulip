ADD_CORE_DIRECTORY_EXCEPT(ConvolutionClustering*.*)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(Delaunay)

##----------------------------------------------------------------------------------------------------------------------------
SET(StrengthClustering_SRCS
  StrengthClustering.h
  StrengthClustering.cpp)

ADD_LIBRARY(StrengthClustering-${TulipVersion} SHARED ${StrengthClustering_SRCS})
TARGET_LINK_LIBRARIES(StrengthClustering-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(HierarchicalClustering_SRCS
  HierarchicalClustering.cpp
  HierarchicalClustering.h)

ADD_LIBRARY(HierarchicalClustering-${TulipVersion} SHARED ${HierarchicalClustering_SRCS})
TARGET_LINK_LIBRARIES(HierarchicalClustering-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
IF(NOT BUILD_CORE_ONLY)
QTX_WRAP_UI(ConvolutionClustering_UI_SRCS
  ConvolutionClusteringSetup.ui)

QTX_WRAP_CPP(ConvolutionClustering_MOC_SRCS
  ConvolutionClusteringSetup.h)

SET(ConvolutionClustering_SRCS
  ConvolutionClustering.h
  ConvolutionClustering.cpp
  ConvolutionClusteringSetup.cpp
  ConvolutionClusteringSetup.h)

ADD_LIBRARY(ConvolutionClustering-${TulipVersion} SHARED ${ConvolutionClustering_SRCS} ${ConvolutionClustering_UI_SRCS} ${ConvolutionClustering_MOC_SRCS})
TARGET_LINK_LIBRARIES(ConvolutionClustering-${TulipVersion} ${LibTulipCoreName} ${QT_LIBRARIES})
ENDIF(NOT BUILD_CORE_ONLY)
##----------------------------------------------------------------------------------------------------------------------------
SET(EqualValueClustering_SRCS
  EqualValueClustering.h
  EqualValueClustering.cpp)

ADD_LIBRARY(EqualValueClustering-${TulipVersion} SHARED ${EqualValueClustering_SRCS})
TARGET_LINK_LIBRARIES(EqualValueClustering-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(QuotientClustering_SRCS
    QuotientClustering.cpp
)

ADD_LIBRARY(QuotientClustering-${TulipVersion} SHARED ${QuotientClustering_SRCS})
TARGET_LINK_LIBRARIES(QuotientClustering-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS StrengthClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)
INSTALL(TARGETS HierarchicalClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT HierarchicalClustering)
IF(NOT BUILD_CORE_ONLY)
INSTALL(TARGETS ConvolutionClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ConvolutionClustering)
ENDIF(NOT BUILD_CORE_ONLY)
INSTALL(TARGETS EqualValueClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT EqualValueClustering)
INSTALL(TARGETS QuotientClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT QuotientClustering)
