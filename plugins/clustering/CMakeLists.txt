ADD_CORE_DIRECTORY_EXCEPT(ConvolutionClustering*.*)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(Delaunay)

##----------------------------------------------------------------------------------------------------------------------------
SET(StrengthClustering_SRCS
  StrengthClustering.h
  StrengthClustering.cpp)

ADD_LIBRARY(StrengthClustering-${Tulip_VERSION} SHARED ${StrengthClustering_SRCS})
TARGET_LINK_LIBRARIES(StrengthClustering-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(HierarchicalClustering_SRCS
  HierarchicalClustering.cpp
  HierarchicalClustering.h)

ADD_LIBRARY(HierarchicalClustering-${Tulip_VERSION} SHARED ${HierarchicalClustering_SRCS})
TARGET_LINK_LIBRARIES(HierarchicalClustering-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CliqueEnumeration_SRCS
  CliqueEnumeration.cpp
  CliqueEnumeration.h)

ADD_LIBRARY(CliqueEnumeration-${Tulip_VERSION} SHARED ${CliqueEnumeration_SRCS})
TARGET_LINK_LIBRARIES(CliqueEnumeration-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
IF(NOT TULIP_BUILD_CORE_ONLY)
QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_UI(ConvolutionClustering_UI_SRCS
  ConvolutionClusteringSetup.ui)

QTX_WRAP_CPP(ConvolutionClustering_MOC_SRCS
  ConvolutionClusteringSetup.h)

SET(ConvolutionClustering_SRCS
  ConvolutionClustering.h
  ConvolutionClustering.cpp
  ConvolutionClusteringSetup.cpp
  ConvolutionClusteringSetup.h)

ADD_LIBRARY(ConvolutionClustering-${Tulip_VERSION} SHARED ${ConvolutionClustering_SRCS} ${ConvolutionClustering_UI_SRCS} ${ConvolutionClustering_MOC_SRCS})
TARGET_LINK_LIBRARIES(ConvolutionClustering-${Tulip_VERSION} ${LibTulipCoreName}  ${LibTulipGUIName} ${QT_LIBRARIES})
ENDIF(NOT TULIP_BUILD_CORE_ONLY)
##----------------------------------------------------------------------------------------------------------------------------
SET(EqualValueClustering_SRCS
  EqualValueClustering.h
  EqualValueClustering.cpp)

ADD_LIBRARY(EqualValueClustering-${Tulip_VERSION} SHARED ${EqualValueClustering_SRCS})
TARGET_LINK_LIBRARIES(EqualValueClustering-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(QuotientClustering_SRCS
    QuotientClustering.cpp
)

ADD_LIBRARY(QuotientClustering-${Tulip_VERSION} SHARED ${QuotientClustering_SRCS})
TARGET_LINK_LIBRARIES(QuotientClustering-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(StrengthClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(HierarchicalClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
IF(NOT TULIP_BUILD_CORE_ONLY)
TULIP_INSTALL_PLUGIN(ConvolutionClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
ENDIF(NOT TULIP_BUILD_CORE_ONLY)
TULIP_INSTALL_PLUGIN(EqualValueClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(QuotientClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CliqueEnumeration-${Tulip_VERSION} ${TulipPluginsInstallDir})
