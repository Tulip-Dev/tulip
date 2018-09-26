ADD_SUBDIRECTORY(SocialNetwork)
ADD_SUBDIRECTORY(BibTeX)

ADD_CORE_FILES(CMakeLists.txt *Tree*.cpp *Graph.cpp AdjacencyMatrixImport.cpp dotImport*.* EmptyGraph.cpp GML*.* Grid.cpp GEXFImport.cpp Import*.cpp)

INCLUDE_DIRECTORIES(./ ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipGUIInclude} ${UTF8CppInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(ERImport_SRCS ERRandomGraph.cpp)
ADD_LIBRARY(ERImport-${TulipVersion} SHARED ${ERImport_SRCS})
TARGET_LINK_LIBRARIES(ERImport-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(PlanarGraph_SRCS PlanarGraph.cpp)
ADD_LIBRARY(PlanarGraph-${TulipVersion} SHARED ${PlanarGraph_SRCS})
TARGET_LINK_LIBRARIES(PlanarGraph-${TulipVersion} ${LibTulipCoreName})

##------------0----------------------------------------------------------------------------------------------------------------
SET(DotImport_SRCS dotImport.cpp)
ADD_LIBRARY(DotImport-${TulipVersion} SHARED ${DotImport_SRCS})
TARGET_LINK_LIBRARIES(DotImport-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Grid_SRCS Grid.cpp)
ADD_LIBRARY(Grid-${TulipVersion} SHARED ${Grid_SRCS})
TARGET_LINK_LIBRARIES(Grid-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLImport_SRCS GMLImport.cpp)
ADD_LIBRARY(GMLImport-${TulipVersion} SHARED ${GMLImport_SRCS})
TARGET_LINK_LIBRARIES(GMLImport-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomGraph_SRCS RandomGraph.cpp)
ADD_LIBRARY(RandomGraph-${TulipVersion} SHARED ${RandomGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomGraph-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomSimpleGraph_SRCS RandomSimpleGraph.cpp)
ADD_LIBRARY(RandomSimpleGraph-${TulipVersion} SHARED ${RandomSimpleGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomSimpleGraph-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(AdjacencyMatrixImport_SRCS AdjacencyMatrixImport.cpp)
ADD_LIBRARY(AdjacencyMatrixImport-${TulipVersion} SHARED ${AdjacencyMatrixImport_SRCS})
TARGET_LINK_LIBRARIES(AdjacencyMatrixImport-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteGraph_SRCS CompleteGraph.cpp)
ADD_LIBRARY(CompleteGraph-${TulipVersion} SHARED ${CompleteGraph_SRCS})
TARGET_LINK_LIBRARIES(CompleteGraph-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTree_SRCS RandomTree.cpp)
ADD_LIBRARY(RandomTree-${TulipVersion} SHARED ${RandomTree_SRCS})
TARGET_LINK_LIBRARIES(RandomTree-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTreeGeneral_SRCS RandomTreeGeneral.cpp)
ADD_LIBRARY(RandomTreeGeneral-${TulipVersion} SHARED ${RandomTreeGeneral_SRCS})
TARGET_LINK_LIBRARIES(RandomTreeGeneral-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteTree_SRCS CompleteTree.cpp)
ADD_LIBRARY(CompleteTree-${TulipVersion} SHARED ${CompleteTree_SRCS})
TARGET_LINK_LIBRARIES(CompleteTree-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(SmallWorldGraph_SRCS SmallWorldGraph.cpp)
ADD_LIBRARY(SmallWorldGraph-${TulipVersion} SHARED ${SmallWorldGraph_SRCS})
TARGET_LINK_LIBRARIES(SmallWorldGraph-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImportPajek_SRCS ImportPajek.cpp)
ADD_LIBRARY(ImportPajek-${TulipVersion} SHARED ${ImportPajek_SRCS})
TARGET_LINK_LIBRARIES(ImportPajek-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImportUCINET_SRCS ImportUCINET.cpp)
ADD_LIBRARY(ImportUCINET-${TulipVersion} SHARED ${ImportUCINET_SRCS})
TARGET_LINK_LIBRARIES(ImportUCINET-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(EmptyGraph-${TulipVersion} SHARED EmptyGraph.cpp)
TARGET_LINK_LIBRARIES(EmptyGraph-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
QTX_SET_INCLUDES_AND_DEFINITIONS()
SET(FileSystem_SRCS FileSystem.cpp)
ADD_LIBRARY(FileSystem-${TulipVersion} SHARED ${FileSystem_SRCS})
TARGET_LINK_LIBRARIES(FileSystem-${TulipVersion} ${LibTulipCoreName} ${QT_LIBRARIES})

##----------------------------------------------------------------------------------------------------------------------------
SET(WebImport_SRCS WebImport.cpp)
QTX_WRAP_CPP(WebImport_MOC_SRCS WebImport.h)
ADD_LIBRARY(WebImport-${TulipVersion} SHARED ${WebImport_SRCS} ${WebImport_MOC_SRCS})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${LibTulipCoreName} ${LibTulipGUIName} ${QT_LIBRARIES})
##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(GEXFImport-${TulipVersion} SHARED GEXFImport.cpp)
TARGET_LINK_LIBRARIES(GEXFImport-${TulipVersion} ${LibTulipCoreName} ${QT_LIBRARIES})

##----------------------------------------------------------------------------------------------------------------------------
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(PlanarGraph-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DotImport-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Grid-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(GMLImport-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomGraph-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomSimpleGraph-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(AdjacencyMatrixImport-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CompleteGraph-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomTree-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomTreeGeneral-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CompleteTree-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(SmallWorldGraph-${TulipVersion} ${TulipPluginsInstallDir})
IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  TULIP_INSTALL_PLUGIN(FileSystem-${TulipVersion} ${TulipPluginsInstallDir})
  TULIP_INSTALL_PLUGIN(WebImport-${TulipVersion} ${TulipPluginsInstallDir})
  TULIP_INSTALL_PLUGIN(GEXFImport-${TulipVersion} ${TulipPluginsInstallDir})
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
TULIP_INSTALL_PLUGIN(ImportPajek-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ImportUCINET-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(EmptyGraph-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ERImport-${TulipVersion} ${TulipPluginsInstallDir})
