ADD_CORE_FILES(.)

ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(FastOverlapRemoval)
ADD_SUBDIRECTORY(Grip)
ADD_SUBDIRECTORY(LinLog)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

IF(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

SET(LayoutUtils_SRCS 
  DatasetTools.cpp
  OrientableCoord.cpp
  OrientableLayout.cpp
  OrientableSize.cpp
  OrientableSizeProxy.cpp
  Orientation.cpp
)

IF(NOT EMSCRIPTEN)
SET(LayoutUtilsLibraryName LayoutUtils)
ADD_LIBRARY(${LayoutUtilsLibraryName} STATIC ${LayoutUtils_SRCS})
ELSE(NOT EMSCRIPTEN)
SET(LayoutUtilsLibraryName "")
ENDIF(NOT EMSCRIPTEN)

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MixedModel-${TulipVersion} SHARED MixedModel.cpp)
TARGET_LINK_LIBRARIES(MixedModel-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConnectedComponentPacking_SRCS 
  ConnectedComponentPacking.cpp
  Number.cpp
  RectanglePacking.cpp
  rectanglePackingFonctions.cpp
  RectangleRelativePositionList.cpp
)
ADD_LIBRARY(ConnectedComponentPacking-${TulipVersion} SHARED ${ConnectedComponentPacking_SRCS})
TARGET_LINK_LIBRARIES(ConnectedComponentPacking-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Random-${TulipVersion} SHARED Random.cpp)
TARGET_LINK_LIBRARIES(Random-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(GemLayout-${TulipVersion} SHARED GEMLayout.cpp)
TARGET_LINK_LIBRARIES(GemLayout-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeReingoldAndTilforExtended-${TulipVersion} SHARED TreeReingoldAndTilfordExtended.cpp)
TARGET_LINK_LIBRARIES(TreeReingoldAndTilforExtended-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ConeTreeExtended-${TulipVersion} SHARED ConeTreeExtended.cpp)
TARGET_LINK_LIBRARIES(ConeTreeExtended-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeRadial-${TulipVersion} SHARED TreeRadial.cpp)
TARGET_LINK_LIBRARIES(TreeRadial-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeLeaf-${TulipVersion} SHARED TreeLeaf.cpp)
TARGET_LINK_LIBRARIES(TreeLeaf-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BubbleTree-${TulipVersion} SHARED BubbleTree.cpp)
TARGET_LINK_LIBRARIES(BubbleTree-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Circular-${TulipVersion} SHARED Circular.cpp)
TARGET_LINK_LIBRARIES(Circular-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(HierarchicalGraph-${TulipVersion} SHARED HierarchicalGraph.cpp)
TARGET_LINK_LIBRARIES(HierarchicalGraph-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Tutte-${TulipVersion} SHARED Tutte.cpp)
TARGET_LINK_LIBRARIES(Tutte-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Dendrogram-${TulipVersion} SHARED Dendrogram.cpp)
TARGET_LINK_LIBRARIES(Dendrogram-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ImprovedWalker-${TulipVersion} SHARED ImprovedWalker.cpp)
TARGET_LINK_LIBRARIES(ImprovedWalker-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SquarifiedTreeMap-${TulipVersion} SHARED SquarifiedTreeMap.cpp)
TARGET_LINK_LIBRARIES(SquarifiedTreeMap-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PerfectLayout-${TulipVersion} SHARED PerfectLayout.cpp)
TARGET_LINK_LIBRARIES(PerfectLayout-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PolyominoPacking-${TulipVersion} SHARED PolyominoPacking.cpp)
TARGET_LINK_LIBRARIES(PolyominoPacking-${TulipVersion} ${LayoutUtilsLibraryName} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(PolyominoPacking-${TulipVersion} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL_TULIP_PLUGIN(MixedModel-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(ConnectedComponentPacking-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(Random-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(GemLayout-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(TreeReingoldAndTilforExtended-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(ConeTreeExtended-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(TreeRadial-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(TreeLeaf-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(BubbleTree-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(Circular-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(HierarchicalGraph-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(Tutte-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(Dendrogram-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(ImprovedWalker-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(SquarifiedTreeMap-${TulipVersion} ${TulipPluginsInstallDir})
INSTALL_TULIP_PLUGIN(PerfectLayout-${TulipVersion} ${TulipPluginsInstallDir})
