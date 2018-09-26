ADD_CORE_DIRECTORY_EXCEPT(ConvolutionClustering*.*)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${CMAKE_CURRENT_BINARY_DIR})

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
SET(CliqueEnumeration_SRCS
  CliqueEnumeration.cpp
  CliqueEnumeration.h)

ADD_LIBRARY(CliqueEnumeration-${TulipVersion} SHARED ${CliqueEnumeration_SRCS})
TARGET_LINK_LIBRARIES(CliqueEnumeration-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
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

ADD_LIBRARY(ConvolutionClustering-${TulipVersion} SHARED ${ConvolutionClustering_SRCS} ${ConvolutionClustering_UI_SRCS} ${ConvolutionClustering_MOC_SRCS})
TARGET_LINK_LIBRARIES(ConvolutionClustering-${TulipVersion} ${LibTulipCoreName} ${QT_LIBRARIES})
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
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
TULIP_INSTALL_PLUGIN(StrengthClustering-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(HierarchicalClustering-${TulipVersion} ${TulipPluginsInstallDir})
IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  TULIP_INSTALL_PLUGIN(ConvolutionClustering-${TulipVersion} ${TulipPluginsInstallDir})
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
TULIP_INSTALL_PLUGIN(EqualValueClustering-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(QuotientClustering-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CliqueEnumeration-${TulipVersion} ${TulipPluginsInstallDir})
