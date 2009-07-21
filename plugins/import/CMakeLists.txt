FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")	

INCLUDE_DIRECTORIES(./ ${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(PlanarGraph_SRCS PlanarGraph.cpp)
ADD_LIBRARY(PlanarGraph-${TulipVersion} SHARED ${PlanarGraph_SRCS})
TARGET_LINK_LIBRARIES(PlanarGraph-${TulipVersion} tulip)
ADD_DEPENDENCIES(PlanarGraph-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(DotImport_SRCS dotImport.cpp dotImportCLUT.h dotImportLexer.h dotImportParser.h)
ADD_LIBRARY(DotImport-${TulipVersion} SHARED ${DotImport_SRCS})
TARGET_LINK_LIBRARIES(DotImport-${TulipVersion} tulip)
ADD_DEPENDENCIES(DotImport-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(Grid_SRCS Grid.cpp)
ADD_LIBRARY(Grid-${TulipVersion} SHARED ${Grid_SRCS})
TARGET_LINK_LIBRARIES(Grid-${TulipVersion} tulip)
ADD_DEPENDENCIES(Grid-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLImport_SRCS GMLImport.cpp GMLParser.h)
ADD_LIBRARY(GMLImport-${TulipVersion} SHARED ${GMLImport_SRCS})
TARGET_LINK_LIBRARIES(GMLImport-${TulipVersion} tulip)
ADD_DEPENDENCIES(GMLImport-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomGraph_SRCS RandomGraph.cpp)
ADD_LIBRARY(RandomGraph-${TulipVersion} SHARED ${RandomGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomGraph-${TulipVersion} tulip)
ADD_DEPENDENCIES(RandomGraph-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomSimpleGraph_SRCS RandomSimpleGraph.cpp)
ADD_LIBRARY(RandomSimpleGraph-${TulipVersion} SHARED ${RandomSimpleGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomSimpleGraph-${TulipVersion} tulip)
ADD_DEPENDENCIES(RandomSimpleGraph-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(AdjacencyMatrixImport_SRCS AdjacencyMatrixImport.cpp)
ADD_LIBRARY(AdjacencyMatrixImport-${TulipVersion} SHARED ${AdjacencyMatrixImport_SRCS})
TARGET_LINK_LIBRARIES(AdjacencyMatrixImport-${TulipVersion} tulip)
ADD_DEPENDENCIES(AdjacencyMatrixImport-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteGraph_SRCS CompleteGraph.cpp)
ADD_LIBRARY(CompleteGraph-${TulipVersion} SHARED ${CompleteGraph_SRCS})
TARGET_LINK_LIBRARIES(CompleteGraph-${TulipVersion} tulip)
ADD_DEPENDENCIES(CompleteGraph-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTree_SRCS RandomTree.cpp)
ADD_LIBRARY(RandomTree-${TulipVersion} SHARED ${RandomTree_SRCS})
TARGET_LINK_LIBRARIES(RandomTree-${TulipVersion} tulip)
ADD_DEPENDENCIES(RandomTree-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomTreeGeneral_SRCS RandomTreeGeneral.cpp)
ADD_LIBRARY(RandomTreeGeneral-${TulipVersion} SHARED ${RandomTreeGeneral_SRCS})
TARGET_LINK_LIBRARIES(RandomTreeGeneral-${TulipVersion} tulip)
ADD_DEPENDENCIES(RandomTreeGeneral-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(CompleteTree_SRCS CompleteTree.cpp)
ADD_LIBRARY(CompleteTree-${TulipVersion} SHARED ${CompleteTree_SRCS})
TARGET_LINK_LIBRARIES(CompleteTree-${TulipVersion} tulip)
ADD_DEPENDENCIES(CompleteTree-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(SmallWorldGraph_SRCS SmallWorldGraph.cpp)
ADD_LIBRARY(SmallWorldGraph-${TulipVersion} SHARED ${SmallWorldGraph_SRCS})
TARGET_LINK_LIBRARIES(SmallWorldGraph-${TulipVersion} tulip)
ADD_DEPENDENCIES(SmallWorldGraph-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(FileSystem_SRCS FileSystem.cpp)
ADD_LIBRARY(FileSystem-${TulipVersion} SHARED ${FileSystem_SRCS})
TARGET_LINK_LIBRARIES(FileSystem-${TulipVersion} tulip)
ADD_DEPENDENCIES(FileSystem-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(WebImport_SRCS WebImport.cpp WebImport.h)
SET(WebImport_MOC WebImport.h)
TULIP_QT4_WRAP_CPP(WebImport_MOC_SRCS ${WebImport_MOC})
ADD_LIBRARY(WebImport-${TulipVersion} SHARED ${WebImport_SRCS} ${WebImport_MOC_SRCS})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTNETWORK_LIBRARY})
ADD_DEPENDENCIES(WebImport-${TulipVersion} tulip)

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
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
