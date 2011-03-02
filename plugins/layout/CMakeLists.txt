ADD_SUBDIRECTORY(OGDF)

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(MixedModel_SRCS MixedModel.cpp DatasetTools.cpp)
ADD_LIBRARY(MixedModel-${TulipVersion} SHARED ${MixedModel_SRCS})
TARGET_LINK_LIBRARIES(MixedModel-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(MixedModel-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConnectedComponentPacking_SRCS 
    ConnectedComponentPacking.cpp 
    Number.cpp 
    RectanglePacking.cpp 
    rectanglePackingFonctions.cpp 
    RectangleRelativePositionList.cpp 
    DatasetTools.cpp
)
ADD_LIBRARY(ConnectedComponentPacking-${TulipVersion} SHARED ${ConnectedComponentPacking_SRCS})
TARGET_LINK_LIBRARIES(ConnectedComponentPacking-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(ConnectedComponentPacking-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Random_SRCS Random.cpp)
ADD_LIBRARY(Random-${TulipVersion} SHARED ${Random_SRCS})
TARGET_LINK_LIBRARIES(Random-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(Random-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(GemLayout_SRCS GEMLayout.cpp)
ADD_LIBRARY(GemLayout-${TulipVersion} SHARED ${GemLayout_SRCS})
TARGET_LINK_LIBRARIES(GemLayout-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(GemLayout-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(TreeReingoldAndTilforExtended_SRCS 
    TreeReingoldAndTilfordExtended.cpp 
    DatasetTools.cpp)
ADD_LIBRARY(TreeReingoldAndTilforExtended-${TulipVersion} SHARED ${TreeReingoldAndTilforExtended_SRCS})
TARGET_LINK_LIBRARIES(TreeReingoldAndTilforExtended-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(TreeReingoldAndTilforExtended-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConeTreeExtended_SRCS ConeTreeExtended.cpp DatasetTools.cpp)
ADD_LIBRARY(ConeTreeExtended-${TulipVersion} SHARED ${ConeTreeExtended_SRCS})
TARGET_LINK_LIBRARIES(ConeTreeExtended-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(ConeTreeExtended-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(TreeRadial_SRCS TreeRadial.cpp DatasetTools.cpp)
ADD_LIBRARY(TreeRadial-${TulipVersion} SHARED ${TreeRadial_SRCS})
TARGET_LINK_LIBRARIES(TreeRadial-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(TreeRadial-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(TreeLeaf_SRCS 
    TreeLeaf.cpp 
    DatasetTools.cpp 
    OrientableCoord.cpp 
    OrientableLayout.cpp 
    OrientableSize.cpp 
    OrientableSizeProxy.cpp
)
ADD_LIBRARY(TreeLeaf-${TulipVersion} SHARED ${TreeLeaf_SRCS})
TARGET_LINK_LIBRARIES(TreeLeaf-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(TreeLeaf-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(BubbleTree_SRCS BubbleTree.cpp DatasetTools.cpp)
ADD_LIBRARY(BubbleTree-${TulipVersion} SHARED ${BubbleTree_SRCS})
TARGET_LINK_LIBRARIES(BubbleTree-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(BubbleTree-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Circular_SRCS Circular.cpp DatasetTools.cpp)
ADD_LIBRARY(Circular-${TulipVersion} SHARED ${Circular_SRCS})
TARGET_LINK_LIBRARIES(Circular-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(Circular-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(HierarchicalGraph_SRCS HierarchicalGraph.cpp DatasetTools.cpp)
ADD_LIBRARY(HierarchicalGraph-${TulipVersion} SHARED ${HierarchicalGraph_SRCS})
TARGET_LINK_LIBRARIES(HierarchicalGraph-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(HierarchicalGraph-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Tutte_SRCS Tutte.cpp)
ADD_LIBRARY(Tutte-${TulipVersion} SHARED ${Tutte_SRCS})
TARGET_LINK_LIBRARIES(Tutte-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(Tutte-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Dendrogram_SRCS 
    DatasetTools.cpp 
    EdgeTools.cpp 
    OrientableCoord.cpp 
    OrientableLayout.cpp 
    OrientableSize.cpp 
    OrientableSizeProxy.cpp 
    Orientation.cpp 
    Dendrogram.cpp
)
ADD_LIBRARY(Dendrogram-${TulipVersion} SHARED ${Dendrogram_SRCS})
TARGET_LINK_LIBRARIES(Dendrogram-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(Dendrogram-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImprovedWalker_SRCS 
    DatasetTools.cpp 
    EdgeTools.cpp 
    ImprovedWalker.cpp 
    OrientableCoord.cpp 
    OrientableLayout.cpp 
    OrientableSize.cpp 
    OrientableSizeProxy.cpp 
    Orientation.cpp    
)
ADD_LIBRARY(ImprovedWalker-${TulipVersion} SHARED ${ImprovedWalker_SRCS})
TARGET_LINK_LIBRARIES(ImprovedWalker-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(ImprovedWalker-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(SquarifiedTreeMap_SRCS SquarifiedTreeMap.cpp)
ADD_LIBRARY(SquarifiedTreeMap-${TulipVersion} SHARED ${SquarifiedTreeMap_SRCS})
TARGET_LINK_LIBRARIES(SquarifiedTreeMap-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(SquarifiedTreeMap-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	MixedModel-${TulipVersion}
	ConnectedComponentPacking-${TulipVersion}
	Random-${TulipVersion}
	GemLayout-${TulipVersion}
	TreeReingoldAndTilforExtended-${TulipVersion}
	ConeTreeExtended-${TulipVersion}
	TreeRadial-${TulipVersion}
	TreeLeaf-${TulipVersion}
	BubbleTree-${TulipVersion}
	Circular-${TulipVersion}
	HierarchicalGraph-${TulipVersion}
	Tutte-${TulipVersion}
	Dendrogram-${TulipVersion}
	ImprovedWalker-${TulipVersion}
	SquarifiedTreeMap-${TulipVersion}
  DESTINATION ${TulipPluginsInstallDir}/
  COMPONENT libtulip_plugins)
