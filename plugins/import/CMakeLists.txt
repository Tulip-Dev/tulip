INCLUDE_DIRECTORIES(./ ${TulipCoreInclude} ${TulipCoreBuildInclude})

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
SET(FileSystem_SRCS FileSystem.cpp)
ADD_LIBRARY(FileSystem-${TulipVersion} SHARED ${FileSystem_SRCS})
TARGET_LINK_LIBRARIES(FileSystem-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(WebImport_SRCS WebImport.cpp)
QT4_WRAP_CPP(WebImport_MOC_SRCS WebImport.h)
ADD_LIBRARY(WebImport-${TulipVersion} SHARED ${WebImport_SRCS} ${WebImport_MOC_SRCS})
TARGET_LINK_LIBRARIES(WebImport-${TulipVersion} ${LibTulipCoreName} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS PlanarGraph-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT PlanarGraph-${TulipVersion})
INSTALL(TARGETS DotImport-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT DotImport-${TulipVersion})
INSTALL(TARGETS Grid-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Grid-${TulipVersion})
INSTALL(TARGETS GMLImport-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GMLImport-${TulipVersion})
INSTALL(TARGETS RandomGraph-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT RandomGraph-${TulipVersion})
INSTALL(TARGETS RandomSimpleGraph-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT RandomSimpleGraph-${TulipVersion})
INSTALL(TARGETS AdjacencyMatrixImport-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT AdjacencyMatrixImport-${TulipVersion})
INSTALL(TARGETS CompleteGraph-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT CompleteGraph-${TulipVersion})
INSTALL(TARGETS RandomTree-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT RandomTree-${TulipVersion})
INSTALL(TARGETS RandomTreeGeneral-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT RandomTreeGeneral-${TulipVersion})
INSTALL(TARGETS CompleteTree-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT CompleteTree-${TulipVersion})
INSTALL(TARGETS SmallWorldGraph-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT SmallWorldGraph-${TulipVersion})
INSTALL(TARGETS FileSystem-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT FileSystem-${TulipVersion})
INSTALL(TARGETS WebImport-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT WebImport-${TulipVersion})
