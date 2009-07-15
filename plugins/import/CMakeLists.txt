FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")	

INCLUDE_DIRECTORIES(./ ${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

SET(PlanarGraph_SRCS PlanarGraph.cpp)
ADD_LIBRARY(PlanarGraph-${TulipVersion} SHARED ${PlanarGraph_SRCS})
TARGET_LINK_LIBRARIES(PlanarGraph-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ PlanarGraph-${TulipVersion})

SET(DotImport_SRCS dotImport.cpp dotImportCLUT.h dotImportLexer.h dotImportParser.h)
ADD_LIBRARY(DotImport-${TulipVersion} SHARED ${DotImport_SRCS})
TARGET_LINK_LIBRARIES(DotImport-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ DotImport-${TulipVersion})

SET(Grid_SRCS Grid.cpp)
ADD_LIBRARY(Grid-${TulipVersion} SHARED ${Grid_SRCS})
TARGET_LINK_LIBRARIES(Grid-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Grid-${TulipVersion})

SET(GMLImport_SRCS GMLImport.cpp GMLParser.h)
ADD_LIBRARY(GMLImport-${TulipVersion} SHARED ${GMLImport_SRCS})
TARGET_LINK_LIBRARIES(GMLImport-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ GMLImport-${TulipVersion})

SET(RandomGraph_SRCS RandomGraph.cpp)
ADD_LIBRARY(RandomGraph-${TulipVersion} SHARED ${RandomGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomGraph-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ RandomGraph-${TulipVersion})

SET(RandomSimpleGraph_SRCS RandomSimpleGraph.cpp)
ADD_LIBRARY(RandomSimpleGraph-${TulipVersion} SHARED ${RandomSimpleGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomSimpleGraph-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ RandomSimpleGraph-${TulipVersion})

SET(AdjacencyMatrixImport_SRCS AdjacencyMatrixImport.cpp)
ADD_LIBRARY(AdjacencyMatrixImport-${TulipVersion} SHARED ${AdjacencyMatrixImport_SRCS})
TARGET_LINK_LIBRARIES(AdjacencyMatrixImport-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ AdjacencyMatrixImport-${TulipVersion})

SET(CompleteGraph_SRCS CompleteGraph.cpp)
ADD_LIBRARY(CompleteGraph-${TulipVersion} SHARED ${CompleteGraph_SRCS})
TARGET_LINK_LIBRARIES(CompleteGraph-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ CompleteGraph-${TulipVersion})

SET(RandomTree_SRCS RandomTree.cpp)
ADD_LIBRARY(RandomTree-${TulipVersion} SHARED ${RandomTree_SRCS})
TARGET_LINK_LIBRARIES(RandomTree-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ RandomTree-${TulipVersion})

SET(RandomTreeGeneral_SRCS RandomTreeGeneral.cpp)
ADD_LIBRARY(RandomTreeGeneral-${TulipVersion} SHARED ${RandomTreeGeneral_SRCS})
TARGET_LINK_LIBRARIES(RandomTreeGeneral-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ RandomTreeGeneral-${TulipVersion})

SET(CompleteTree_SRCS CompleteTree.cpp)
ADD_LIBRARY(CompleteTree-${TulipVersion} SHARED ${CompleteTree_SRCS})
TARGET_LINK_LIBRARIES(CompleteTree-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ CompleteTree-${TulipVersion})

SET(SmallWorldGraph_SRCS SmallWorldGraph.cpp)
ADD_LIBRARY(SmallWorldGraph-${TulipVersion} SHARED ${SmallWorldGraph_SRCS})
TARGET_LINK_LIBRARIES(SmallWorldGraph-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ SmallWorldGraph-${TulipVersion})

SET(FileSystem_SRCS FileSystem.cpp)
ADD_LIBRARY(FileSystem-${TulipVersion} SHARED ${FileSystem_SRCS})
TARGET_LINK_LIBRARIES(FileSystem-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ FileSystem-${TulipVersion})

SET(WebImport_SRCS WebImport.cpp WebImport.h)
SET(WebImport_MOC WebImport.h)
TULIP_QT4_WRAP_CPP(WebImport_MOC_SRCS ${WebImport_MOC})
ADD_LIBRARY(WebImport-${TulipVersion} SHARED ${WebImport_SRCS} ${WebImport_MOC_SRCS})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${QT_QTNETWORK_LIBRARY})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ WebImport-${TulipVersion})
