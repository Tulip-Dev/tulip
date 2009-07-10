INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

SET(MixedModel_SRCS MixedModel.h MixedModel.cpp DatasetTools.h DatasetTools.cpp)
ADD_LIBRARY(MixedModel-${TulipVersion} SHARED ${MixedModel_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ MixedModel-${TulipVersion})

SET(ConnectedComponentPacking_SRCS 
    ConnectedComponentPacking.h 
    ConnectedComponentPacking.cpp 
    Number.h 
    Number.cpp 
    RectanglePacking.h 
    RectanglePacking.cpp 
    rectanglePackingFonctions.h 
    rectanglePackingFonctions.cpp 
    RectangleRelativePositionList.h 
    RectangleRelativePositionList.cpp 
    DatasetTools.h 
    DatasetTools.cpp
)
ADD_LIBRARY(ConnectedComponentPacking-${TulipVersion} SHARED ${ConnectedComponentPacking_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ConnectedComponentPacking-${TulipVersion})

SET(Random_SRCS Random.h Random.cpp)
ADD_LIBRARY(Random-${TulipVersion} SHARED ${Random_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Random-${TulipVersion})

SET(GemLayout_SRCS GEMLayout.h GEMLayout.cpp)
ADD_LIBRARY(GemLayout-${TulipVersion} SHARED ${GemLayout_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ GemLayout-${TulipVersion})

SET(TreeReingoldAndTilforExtended_SRCS 
    TreeReingoldAndTilfordExtended.h 
    TreeReingoldAndTilfordExtended.cpp 
    DatasetTools.h 
    DatasetTools.cpp)
ADD_LIBRARY(TreeReingoldAndTilforExtended-${TulipVersion} SHARED ${TreeReingoldAndTilforExtended_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ TreeReingoldAndTilforExtended-${TulipVersion})

SET(ConeTreeExtended_SRCS ConeTreeExtended.h ConeTreeExtended.cpp DatasetTools.h DatasetTools.cpp)
ADD_LIBRARY(ConeTreeExtended-${TulipVersion} SHARED ${ConeTreeExtended_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ConeTreeExtended-${TulipVersion})

SET(TreeMap_SRCS TreeMap.h TreeMap.cpp)
ADD_LIBRARY(TreeMap-${TulipVersion} SHARED ${TreeMap_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ TreeMap-${TulipVersion})

SET(TreeRadial_SRCS TreeRadial.cpp DatasetTools.h DatasetTools.cpp)
ADD_LIBRARY(TreeRadial-${TulipVersion} SHARED ${TreeRadial_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ TreeRadial-${TulipVersion})

SET(TreeLeaf_SRCS 
    TreeLeaf.h 
    TreeLeaf.cpp 
    DatasetTools.h 
    DatasetTools.cpp 
    OrientableCoord.h 
    OrientableLayout.h 
    OrientableSize.h 
    OrientableSizeProxy.h 
    OrientableCoord.cpp 
    OrientableLayout.cpp 
    OrientableSize.cpp 
    OrientableSizeProxy.cpp
)
ADD_LIBRARY(TreeLeaf-${TulipVersion} SHARED ${TreeLeaf_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ TreeLeaf-${TulipVersion})

SET(BubbleTree_SRCS BubbleTree.h BubbleTree.cpp DatasetTools.h DatasetTools.cpp)
ADD_LIBRARY(BubbleTree-${TulipVersion} SHARED ${BubbleTree_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ BubbleTree-${TulipVersion})

SET(Circular_SRCS Circular.h Circular.cpp DatasetTools.h DatasetTools.cpp)
ADD_LIBRARY(Circular-${TulipVersion} SHARED ${Circular_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Circular-${TulipVersion})

SET(Tutte_SRCS Tutte.h Tutte.cpp)
ADD_LIBRARY(Tutte-${TulipVersion} SHARED ${Tutte_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Tutte-${TulipVersion})

SET(Embedder_SRCS 
    Embedder.cpp 
    EmbedderEmbedGraph.h 
    EmbedderEmbedGraph.cpp 
    EmbedderDefs.h 
    EmbedderDijkstra.h 
    EmbedderDijkstra.cpp 
    EmbedderBfs.h 
    EmbedderBfs.cpp 
    EmbedderParameters.h 
    EmbedderParameters.cpp 
    EmbedderPowerIterationDense.cpp
)
ADD_LIBRARY(Embedder-${TulipVersion} SHARED ${Embedder_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Embedder-${TulipVersion})

SET(Dendrogram_SRCS 
    DatasetTools.h 
    DatasetTools.cpp 
    EdgeTools.h 
    EdgeTools.cpp 
    OrientableCoord.h 
    OrientableConstants.h 
    OrientableLayout.h 
    OrientableLayoutInterface.h 
    OrientableSize.h 
    OrientableSizeProxy.h 
    Orientation.h 
    Dendrogram.h 
    OrientableCoord.cpp 
    OrientableLayout.cpp 
    OrientableSize.cpp 
    OrientableSizeProxy.cpp 
    Orientation.cpp 
    TreeTools.h 
    Dendrogram.cpp
)
ADD_LIBRARY(Dendrogram-${TulipVersion} SHARED ${Dendrogram_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Dendrogram-${TulipVersion})

SET(ImprovedWalker_SRCS 
    DatasetTools.h 
    DatasetTools.cpp 
    EdgeTools.cpp 
    ImprovedWalker.h 
    ImprovedWalker.cpp 
    OrientableCoord.cpp 
    OrientableLayout.cpp 
    OrientableSize.cpp 
    OrientableSizeProxy.cpp 
    Orientation.cpp    
)
ADD_LIBRARY(ImprovedWalker-${TulipVersion} SHARED ${ImprovedWalker_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ImprovedWalker-${TulipVersion})

SET(SquarifiedTreeMap_SRCS SquarifiedTreeMap.h SquarifiedTreeMap.cpp)
ADD_LIBRARY(SquarifiedTreeMap-${TulipVersion} SHARED ${SquarifiedTreeMap_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ SquarifiedTreeMap-${TulipVersion})
