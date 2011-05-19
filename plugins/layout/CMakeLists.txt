ADD_SUBDIRECTORY(OGDF)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MixedModel-${TulipVersion} SHARED MixedModel.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(MixedModel-${TulipVersion} ${LibTulipCoreName})

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
TARGET_LINK_LIBRARIES(ConnectedComponentPacking-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Random-${TulipVersion} SHARED Random.cpp)
TARGET_LINK_LIBRARIES(Random-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(GemLayout-${TulipVersion} SHARED GEMLayout.cpp)
TARGET_LINK_LIBRARIES(GemLayout-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(TreeReingoldAndTilforExtended_SRCS 
  TreeReingoldAndTilfordExtended.cpp
  DatasetTools.cpp)
ADD_LIBRARY(TreeReingoldAndTilforExtended-${TulipVersion} SHARED ${TreeReingoldAndTilforExtended_SRCS})
TARGET_LINK_LIBRARIES(TreeReingoldAndTilforExtended-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ConeTreeExtended-${TulipVersion} SHARED ConeTreeExtended.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(ConeTreeExtended-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeRadial-${TulipVersion} SHARED TreeRadial.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(TreeRadial-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(TreeLeaf_SRCS 
  TreeLeaf.cpp
  DatasetTools.cpp
  OrientableCoord.cpp
  OrientableLayout.cpp
  OrientableSize.cpp
  OrientableSizeProxy.cpp)

ADD_LIBRARY(TreeLeaf-${TulipVersion} SHARED ${TreeLeaf_SRCS})
TARGET_LINK_LIBRARIES(TreeLeaf-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BubbleTree-${TulipVersion} SHARED BubbleTree.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(BubbleTree-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Circular-${TulipVersion} SHARED Circular.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(Circular-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(HierarchicalGraph-${TulipVersion} SHARED HierarchicalGraph.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(HierarchicalGraph-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Tutte-${TulipVersion} SHARED Tutte.cpp)
TARGET_LINK_LIBRARIES(Tutte-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Dendrogram_SRCS 
  DatasetTools.cpp
  EdgeTools.cpp
  OrientableCoord.cpp
  OrientableLayout.cpp
  OrientableSize.cpp
  OrientableSizeProxy.cpp
  Orientation.cpp
  Dendrogram.cpp)

ADD_LIBRARY(Dendrogram-${TulipVersion} SHARED ${Dendrogram_SRCS})
TARGET_LINK_LIBRARIES(Dendrogram-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ImprovedWalker_SRCS 
  DatasetTools.cpp
  EdgeTools.cpp
  ImprovedWalker.cpp
  OrientableCoord.cpp
  OrientableLayout.cpp
  OrientableSize.cpp
  OrientableSizeProxy.cpp
  Orientation.cpp)

ADD_LIBRARY(ImprovedWalker-${TulipVersion} SHARED ${ImprovedWalker_SRCS})
TARGET_LINK_LIBRARIES(ImprovedWalker-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SquarifiedTreeMap-${TulipVersion} SHARED SquarifiedTreeMap.cpp)
TARGET_LINK_LIBRARIES(SquarifiedTreeMap-${TulipVersion} ${LibTulipCoreName})

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
DESTINATION ${TulipPluginsInstallDir}
COMPONENT tulip_core_plugins)
