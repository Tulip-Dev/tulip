ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SpanningDagSelection-${TulipVersion} SHARED SpanningDagSelection.cpp SpanningDagSelection.h)
TARGET_LINK_LIBRARIES(SpanningDagSelection-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ReachableSubGraphSelection-${TulipVersion} SHARED ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp)
TARGET_LINK_LIBRARIES(ReachableSubGraphSelection-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SpanningTreeSelection-${TulipVersion} SHARED SpanningTreeSelection.cpp SpanningTreeSelection.h)
TARGET_LINK_LIBRARIES(SpanningTreeSelection-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InducedSubGraphSelection-${TulipVersion} SHARED InducedSubGraphSelection.cpp InducedSubGraphSelection.h)
TARGET_LINK_LIBRARIES(InducedSubGraphSelection-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LoopSelection-${TulipVersion} SHARED LoopSelection.cpp LoopSelection.h)
TARGET_LINK_LIBRARIES(LoopSelection-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MultipleSelection-${TulipVersion} SHARED MultipleEdgeSelection.cpp MultipleEdgeSelection.h)
TARGET_LINK_LIBRARIES(MultipleSelection-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Kruskal-${TulipVersion} SHARED Kruskal.cpp Kruskal.h)
TARGET_LINK_LIBRARIES(Kruskal-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS SpanningDagSelection-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT SpanningDagSelection)
INSTALL(TARGETS ReachableSubGraphSelection-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ReachableSubGraphSelection)
INSTALL(TARGETS SpanningTreeSelection-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT SpanningTreeSelection)
INSTALL(TARGETS InducedSubGraphSelection-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT InducedSubGraphSelection)
INSTALL(TARGETS LoopSelection-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT LoopSelection)
INSTALL(TARGETS MultipleSelection-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT MultipleSelection)
INSTALL(TARGETS Kruskal-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Kruskal)
