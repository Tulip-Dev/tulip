ADD_CORE_DIRECTORY_EXCEPT(PolyominoPacking.cpp)

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

ADD_LIBRARY(LayoutUtils STATIC ${LayoutUtils_SRCS})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MixedModel-${TulipVersion} SHARED MixedModel.cpp)
TARGET_LINK_LIBRARIES(MixedModel-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConnectedComponentPacking_SRCS 
  ConnectedComponentPacking.cpp
  Number.cpp
  RectanglePacking.cpp
  rectanglePackingFonctions.cpp
  RectangleRelativePositionList.cpp
)
ADD_LIBRARY(ConnectedComponentPacking-${TulipVersion} SHARED ${ConnectedComponentPacking_SRCS})
TARGET_LINK_LIBRARIES(ConnectedComponentPacking-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Random-${TulipVersion} SHARED Random.cpp)
TARGET_LINK_LIBRARIES(Random-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(GemLayout-${TulipVersion} SHARED GEMLayout.cpp)
TARGET_LINK_LIBRARIES(GemLayout-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeReingoldAndTilforExtended-${TulipVersion} SHARED TreeReingoldAndTilfordExtended.cpp)
TARGET_LINK_LIBRARIES(TreeReingoldAndTilforExtended-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ConeTreeExtended-${TulipVersion} SHARED ConeTreeExtended.cpp)
TARGET_LINK_LIBRARIES(ConeTreeExtended-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeRadial-${TulipVersion} SHARED TreeRadial.cpp DatasetTools.cpp)
TARGET_LINK_LIBRARIES(TreeRadial-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(TreeLeaf-${TulipVersion} SHARED TreeLeaf.cpp)
TARGET_LINK_LIBRARIES(TreeLeaf-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BubbleTree-${TulipVersion} SHARED BubbleTree.cpp)
TARGET_LINK_LIBRARIES(BubbleTree-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Circular-${TulipVersion} SHARED Circular.cpp)
TARGET_LINK_LIBRARIES(Circular-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(HierarchicalGraph-${TulipVersion} SHARED HierarchicalGraph.cpp)
TARGET_LINK_LIBRARIES(HierarchicalGraph-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Tutte-${TulipVersion} SHARED Tutte.cpp)
TARGET_LINK_LIBRARIES(Tutte-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Dendrogram-${TulipVersion} SHARED Dendrogram.cpp)
TARGET_LINK_LIBRARIES(Dendrogram-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ImprovedWalker-${TulipVersion} SHARED ImprovedWalker.cpp)
TARGET_LINK_LIBRARIES(ImprovedWalker-${TulipVersion} LayoutUtils ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SquarifiedTreeMap-${TulipVersion} SHARED SquarifiedTreeMap.cpp)
TARGET_LINK_LIBRARIES(SquarifiedTreeMap-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PerfectLayout-${TulipVersion} SHARED PerfectLayout.cpp)
TARGET_LINK_LIBRARIES(PerfectLayout-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
IF(NOT BUILD_CORE_ONLY)
INCLUDE_DIRECTORIES(${TulipOGLInclude})
ADD_LIBRARY(PolyominoPacking-${TulipVersion} SHARED PolyominoPacking.cpp)
TARGET_LINK_LIBRARIES(PolyominoPacking-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName})
INSTALL(TARGETS PolyominoPacking-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT PolyominoPacking)
ENDIF(NOT BUILD_CORE_ONLY)
##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS MixedModel-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT MixedModel)
INSTALL(TARGETS ConnectedComponentPacking-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ConnectedComponentPacking)
INSTALL(TARGETS Random-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Random)
INSTALL(TARGETS GemLayout-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GemLayout)
INSTALL(TARGETS TreeReingoldAndTilforExtended-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT TreeReingoldAndTilforExtended)
INSTALL(TARGETS ConeTreeExtended-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ConeTreeExtended)
INSTALL(TARGETS TreeRadial-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT TreeRadial)
INSTALL(TARGETS TreeLeaf-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT TreeLeaf)
INSTALL(TARGETS BubbleTree-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT BubbleTree)
INSTALL(TARGETS Circular-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Circular)
INSTALL(TARGETS HierarchicalGraph-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT HierarchicalGraph)
INSTALL(TARGETS Tutte-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Tutte)
INSTALL(TARGETS Dendrogram-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Dendrogram)
INSTALL(TARGETS ImprovedWalker-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ImprovedWalker)
INSTALL(TARGETS SquarifiedTreeMap-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT SquarifiedTreeMap)
INSTALL(TARGETS PerfectLayout-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT PerfectLayout)
