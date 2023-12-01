ADD_SUBDIRECTORY(SocialNetwork)
ADD_SUBDIRECTORY(BibTeX)

ADD_CORE_FILES(CMakeLists.txt *Tree*.cpp *Graph.cpp AdjacencyMatrixImport.cpp dotImport*.* EmptyGraph.cpp GML*.* Grid.cpp GEXFImport.cpp Import*.cpp)

INCLUDE_DIRECTORIES(./ ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipGUIInclude} ${UTF8CppInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(ERImport_SRCS ERRandomGraph.cpp)
ADD_LIBRARY(ERImport-${Tulip_VERSION} SHARED ${ERImport_SRCS})
TARGET_LINK_LIBRARIES(ERImport-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(PlanarGraph_SRCS PlanarGraph.cpp)
ADD_LIBRARY(PlanarGraph-${Tulip_VERSION} SHARED ${PlanarGraph_SRCS})
TARGET_LINK_LIBRARIES(PlanarGraph-${Tulip_VERSION} ${LibTulipCoreName})

##------------0----------------------------------------------------------------------------------------------------------------
SET(DotImport_SRCS dotImport.cpp)
ADD_LIBRARY(DotImport-${Tulip_VERSION} SHARED ${DotImport_SRCS})
TARGET_LINK_LIBRARIES(DotImport-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Grid_SRCS Grid.cpp)
ADD_LIBRARY(Grid-${Tulip_VERSION} SHARED ${Grid_SRCS})
TARGET_LINK_LIBRARIES(Grid-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLImport_SRCS GMLImport.cpp)
ADD_LIBRARY(GMLImport-${Tulip_VERSION} SHARED ${GMLImport_SRCS})
TARGET_LINK_LIBRARIES(GMLImport-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomGraph_SRCS RandomGraph.cpp)
ADD_LIBRARY(RandomGraph-${Tulip_VERSION} SHARED ${RandomGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomGraph-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(AdjacencyMatrixImport_SRCS AdjacencyMatrixImport.cpp)
ADD_LIBRARY(AdjacencyMatrixImport-${Tulip_VERSION} SHARED ${AdjacencyMatrixImport_SRCS})
TARGET_LINK_LIBRARIES(AdjacencyMatrixImport-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteGraph_SRCS CompleteGraph.cpp)
ADD_LIBRARY(CompleteGraph-${Tulip_VERSION} SHARED ${CompleteGraph_SRCS})
TARGET_LINK_LIBRARIES(CompleteGraph-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTree_SRCS RandomTree.cpp)
ADD_LIBRARY(RandomTree-${Tulip_VERSION} SHARED ${RandomTree_SRCS})
TARGET_LINK_LIBRARIES(RandomTree-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTreeGeneral_SRCS RandomTreeGeneral.cpp)
ADD_LIBRARY(RandomTreeGeneral-${Tulip_VERSION} SHARED ${RandomTreeGeneral_SRCS})
TARGET_LINK_LIBRARIES(RandomTreeGeneral-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteTree_SRCS CompleteTree.cpp)
ADD_LIBRARY(CompleteTree-${Tulip_VERSION} SHARED ${CompleteTree_SRCS})
TARGET_LINK_LIBRARIES(CompleteTree-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(SmallWorldGraph_SRCS SmallWorldGraph.cpp)
ADD_LIBRARY(SmallWorldGraph-${Tulip_VERSION} SHARED ${SmallWorldGraph_SRCS})
TARGET_LINK_LIBRARIES(SmallWorldGraph-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImportPajek_SRCS ImportPajek.cpp)
ADD_LIBRARY(ImportPajek-${Tulip_VERSION} SHARED ${ImportPajek_SRCS})
TARGET_LINK_LIBRARIES(ImportPajek-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImportTGF_SRCS ImportTGF.cpp)
ADD_LIBRARY(ImportTGF-${Tulip_VERSION} SHARED ${ImportTGF_SRCS})
TARGET_LINK_LIBRARIES(ImportTGF-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImportUCINET_SRCS ImportUCINET.cpp)
ADD_LIBRARY(ImportUCINET-${Tulip_VERSION} SHARED ${ImportUCINET_SRCS})
TARGET_LINK_LIBRARIES(ImportUCINET-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(EmptyGraph-${Tulip_VERSION} SHARED EmptyGraph.cpp)
TARGET_LINK_LIBRARIES(EmptyGraph-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
IF(NOT TULIP_BUILD_CORE_ONLY)
QTX_SET_INCLUDES_AND_DEFINITIONS()
SET(FileSystem_SRCS FileSystem.cpp)
ADD_LIBRARY(FileSystem-${Tulip_VERSION} SHARED ${FileSystem_SRCS})
TARGET_LINK_LIBRARIES(FileSystem-${Tulip_VERSION} ${LibTulipCoreName} ${QT_LIBRARIES})

##----------------------------------------------------------------------------------------------------------------------------
SET(WebImport_SRCS WebImport.cpp)
QTX_WRAP_CPP(WebImport_MOC_SRCS WebImport.h)
ADD_LIBRARY(WebImport-${Tulip_VERSION} SHARED ${WebImport_SRCS} ${WebImport_MOC_SRCS})
TARGET_LINK_LIBRARIES(WebImport-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipGUIName} ${QT_LIBRARIES})
##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(GEXFImport-${Tulip_VERSION} SHARED GEXFImport.cpp)
TARGET_LINK_LIBRARIES(GEXFImport-${Tulip_VERSION} ${LibTulipCoreName} ${QT_LIBRARIES})

##----------------------------------------------------------------------------------------------------------------------------
ENDIF(NOT TULIP_BUILD_CORE_ONLY)
##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(PlanarGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DotImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Grid-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(GMLImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(AdjacencyMatrixImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CompleteGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomTree-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomTreeGeneral-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CompleteTree-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(SmallWorldGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
IF(NOT TULIP_BUILD_CORE_ONLY)
TULIP_INSTALL_PLUGIN(FileSystem-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(WebImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(GEXFImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
ENDIF(NOT TULIP_BUILD_CORE_ONLY)
TULIP_INSTALL_PLUGIN(ImportPajek-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ImportTGF-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ImportUCINET-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(EmptyGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ERImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
