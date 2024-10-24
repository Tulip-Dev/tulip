ADD_CORE_FILES(.)

ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(FastOverlapRemoval)
ADD_SUBDIRECTORY(FORBID)
ADD_SUBDIRECTORY(Grip)
ADD_SUBDIRECTORY(LinLog)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

SET(LayoutUtils_SRCS 
  DatasetTools.cpp
  OrientableCoord.cpp
  OrientableLayout.cpp
  OrientableSize.cpp
  OrientableSizeProxy.cpp
  Orientation.cpp
)

SET(LayoutUtilsLibraryName LayoutUtils)
ADD_LIBRARY(${LayoutUtilsLibraryName} STATIC ${LayoutUtils_SRCS})
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(${LayoutUtilsLibraryName} PROPERTIES POSITION_INDEPENDENT_CODE ON)

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MixedModel-${Tulip_VERSION} SHARED MixedModel.cpp)
TARGET_LINK_LIBRARIES(MixedModel-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConnectedComponentPacking_SRCS 
  ConnectedComponentPacking.cpp
  RectanglePacking.cpp
  rectanglePackingFonctions.cpp
  RectangleRelativePositionList.cpp
)
ADD_LIBRARY(ConnectedComponentPacking-${Tulip_VERSION} SHARED ${ConnectedComponentPacking_SRCS})
TARGET_LINK_LIBRARIES(ConnectedComponentPacking-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Random-${Tulip_VERSION} SHARED Random.cpp)
TARGET_LINK_LIBRARIES(Random-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(GemLayout-${Tulip_VERSION} SHARED GEMLayout.cpp)
TARGET_LINK_LIBRARIES(GemLayout-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeReingoldAndTilforExtended-${Tulip_VERSION} SHARED TreeReingoldAndTilfordExtended.cpp)
TARGET_LINK_LIBRARIES(TreeReingoldAndTilforExtended-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ConeTreeExtended-${Tulip_VERSION} SHARED ConeTreeExtended.cpp)
TARGET_LINK_LIBRARIES(ConeTreeExtended-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeRadial-${Tulip_VERSION} SHARED TreeRadial.cpp)
TARGET_LINK_LIBRARIES(TreeRadial-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeLeaf-${Tulip_VERSION} SHARED TreeLeaf.cpp)
TARGET_LINK_LIBRARIES(TreeLeaf-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BubblePack-${Tulip_VERSION} SHARED BubblePack.cpp)
TARGET_LINK_LIBRARIES(BubblePack-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BubbleTree-${Tulip_VERSION} SHARED BubbleTree.cpp)
TARGET_LINK_LIBRARIES(BubbleTree-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Circular-${Tulip_VERSION} SHARED Circular.cpp)
TARGET_LINK_LIBRARIES(Circular-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(HierarchicalGraph-${Tulip_VERSION} SHARED HierarchicalGraph.cpp)
TARGET_LINK_LIBRARIES(HierarchicalGraph-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Tutte-${Tulip_VERSION} SHARED Tutte.cpp)
TARGET_LINK_LIBRARIES(Tutte-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Dendrogram-${Tulip_VERSION} SHARED Dendrogram.cpp)
TARGET_LINK_LIBRARIES(Dendrogram-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ImprovedWalker-${Tulip_VERSION} SHARED ImprovedWalker.cpp)
TARGET_LINK_LIBRARIES(ImprovedWalker-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SquarifiedTreeMap-${Tulip_VERSION} SHARED SquarifiedTreeMap.cpp)
TARGET_LINK_LIBRARIES(SquarifiedTreeMap-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PerfectLayout-${Tulip_VERSION} SHARED PerfectLayout.cpp)
TARGET_LINK_LIBRARIES(PerfectLayout-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(OrthoTree-${Tulip_VERSION} SHARED OrthoTree.cpp)
TARGET_LINK_LIBRARIES(OrthoTree-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PolyominoPacking-${Tulip_VERSION} SHARED PolyominoPacking.cpp)
TARGET_LINK_LIBRARIES(PolyominoPacking-${Tulip_VERSION} ${LayoutUtilsLibraryName} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(PolyominoPacking-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(EdgeSeparation-${Tulip_VERSION} SHARED EdgeSeparation.cpp)
TARGET_LINK_LIBRARIES(EdgeSeparation-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(EdgeSeparation-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(AlignNodes-${Tulip_VERSION} SHARED AlignNodes.cpp)
TARGET_LINK_LIBRARIES(AlignNodes-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(AlignNodes-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(MixedModel-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ConnectedComponentPacking-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Random-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(GemLayout-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(TreeReingoldAndTilforExtended-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ConeTreeExtended-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(TreeRadial-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(TreeLeaf-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(BubblePack-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(BubbleTree-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Circular-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(HierarchicalGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Tutte-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Dendrogram-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ImprovedWalker-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(SquarifiedTreeMap-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(OrthoTree-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(PerfectLayout-${Tulip_VERSION} ${TulipPluginsInstallDir})
