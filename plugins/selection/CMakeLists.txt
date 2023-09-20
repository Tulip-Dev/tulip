ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SpanningDagSelection-${Tulip_VERSION} SHARED SpanningDagSelection.cpp SpanningDagSelection.h)
TARGET_LINK_LIBRARIES(SpanningDagSelection-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ReachableSubGraphSelection-${Tulip_VERSION} SHARED ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp)
TARGET_LINK_LIBRARIES(ReachableSubGraphSelection-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SpanningTreeSelection-${Tulip_VERSION} SHARED SpanningTreeSelection.cpp SpanningTreeSelection.h)
TARGET_LINK_LIBRARIES(SpanningTreeSelection-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InducedSubGraphSelection-${Tulip_VERSION} SHARED InducedSubGraphSelection.cpp InducedSubGraphSelection.h)
TARGET_LINK_LIBRARIES(InducedSubGraphSelection-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LoopSelection-${Tulip_VERSION} SHARED LoopSelection.cpp LoopSelection.h)
TARGET_LINK_LIBRARIES(LoopSelection-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MultipleSelection-${Tulip_VERSION} SHARED MultipleEdgeSelection.cpp MultipleEdgeSelection.h)
TARGET_LINK_LIBRARIES(MultipleSelection-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Kruskal-${Tulip_VERSION} SHARED Kruskal.cpp Kruskal.h)
TARGET_LINK_LIBRARIES(Kruskal-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MakeGraph-${Tulip_VERSION} SHARED MakeSelectionGraph.cpp MakeSelectionGraph.h)
TARGET_LINK_LIBRARIES(MakeGraph-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(SpanningDagSelection-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ReachableSubGraphSelection-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(SpanningTreeSelection-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InducedSubGraphSelection-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LoopSelection-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(MultipleSelection-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Kruskal-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(MakeGraph-${Tulip_VERSION} ${TulipPluginsInstallDir})
