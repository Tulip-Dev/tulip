FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")	

INCLUDE_DIRECTORIES(./ ${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(PlanarGraph_SRCS PlanarGraph.cpp)
ADD_LIBRARY(PlanarGraph-${TulipVersion} SHARED ${PlanarGraph_SRCS})
TARGET_LINK_LIBRARIES(PlanarGraph-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(PlanarGraph-${TulipVersion} ${LibTulipName})

##------------0----------------------------------------------------------------------------------------------------------------
SET(DotImport_SRCS dotImport.cpp)
ADD_LIBRARY(DotImport-${TulipVersion} SHARED ${DotImport_SRCS})
TARGET_LINK_LIBRARIES(DotImport-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(DotImport-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Grid_SRCS Grid.cpp)
ADD_LIBRARY(Grid-${TulipVersion} SHARED ${Grid_SRCS})
TARGET_LINK_LIBRARIES(Grid-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(Grid-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLImport_SRCS GMLImport.cpp)
ADD_LIBRARY(GMLImport-${TulipVersion} SHARED ${GMLImport_SRCS})
TARGET_LINK_LIBRARIES(GMLImport-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(GMLImport-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomGraph_SRCS RandomGraph.cpp)
ADD_LIBRARY(RandomGraph-${TulipVersion} SHARED ${RandomGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomGraph-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(RandomGraph-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomSimpleGraph_SRCS RandomSimpleGraph.cpp)
ADD_LIBRARY(RandomSimpleGraph-${TulipVersion} SHARED ${RandomSimpleGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomSimpleGraph-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(RandomSimpleGraph-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(AdjacencyMatrixImport_SRCS AdjacencyMatrixImport.cpp)
ADD_LIBRARY(AdjacencyMatrixImport-${TulipVersion} SHARED ${AdjacencyMatrixImport_SRCS})
TARGET_LINK_LIBRARIES(AdjacencyMatrixImport-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(AdjacencyMatrixImport-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteGraph_SRCS CompleteGraph.cpp)
ADD_LIBRARY(CompleteGraph-${TulipVersion} SHARED ${CompleteGraph_SRCS})
TARGET_LINK_LIBRARIES(CompleteGraph-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(CompleteGraph-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTree_SRCS RandomTree.cpp)
ADD_LIBRARY(RandomTree-${TulipVersion} SHARED ${RandomTree_SRCS})
TARGET_LINK_LIBRARIES(RandomTree-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(RandomTree-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTreeGeneral_SRCS RandomTreeGeneral.cpp)
ADD_LIBRARY(RandomTreeGeneral-${TulipVersion} SHARED ${RandomTreeGeneral_SRCS})
TARGET_LINK_LIBRARIES(RandomTreeGeneral-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(RandomTreeGeneral-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteTree_SRCS CompleteTree.cpp)
ADD_LIBRARY(CompleteTree-${TulipVersion} SHARED ${CompleteTree_SRCS})
TARGET_LINK_LIBRARIES(CompleteTree-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(CompleteTree-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(SmallWorldGraph_SRCS SmallWorldGraph.cpp)
ADD_LIBRARY(SmallWorldGraph-${TulipVersion} SHARED ${SmallWorldGraph_SRCS})
TARGET_LINK_LIBRARIES(SmallWorldGraph-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(SmallWorldGraph-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(FileSystem_SRCS FileSystem.cpp)
ADD_LIBRARY(FileSystem-${TulipVersion} SHARED ${FileSystem_SRCS})
TARGET_LINK_LIBRARIES(FileSystem-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(FileSystem-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(WebImport_SRCS WebImport.cpp)
SET(WebImport_MOC WebImport.h)
TULIP_QT4_WRAP_CPP(WebImport_MOC_SRCS ${WebImport_MOC})
ADD_LIBRARY(WebImport-${TulipVersion} SHARED ${WebImport_SRCS} ${WebImport_MOC_SRCS})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTNETWORK_LIBRARY})
ADD_DEPENDENCIES(WebImport-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	PlanarGraph-${TulipVersion}
	DotImport-${TulipVersion}
	Grid-${TulipVersion}
	GMLImport-${TulipVersion}
	RandomGraph-${TulipVersion}
	RandomSimpleGraph-${TulipVersion}
	AdjacencyMatrixImport-${TulipVersion}
	CompleteGraph-${TulipVersion}
	RandomTree-${TulipVersion}
	RandomTreeGeneral-${TulipVersion}
	CompleteTree-${TulipVersion}
	SmallWorldGraph-${TulipVersion}
	FileSystem-${TulipVersion}
	WebImport-${TulipVersion}
  DESTINATION ${TulipPluginsInstallDir}/
  COMPONENT libtulip_plugins)
