INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(SpanningDagSelection_SRCS SpanningDagSelection.cpp SpanningDagSelection.h)
ADD_LIBRARY(SpanningDagSelection-${TulipVersion} SHARED ${SpanningDagSelection_SRCS})
TARGET_LINK_LIBRARIES(SpanningDagSelection-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(SpanningDagSelection-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ReachableSubGraphSelection_SRCS ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp)
ADD_LIBRARY(ReachableSubGraphSelection-${TulipVersion} SHARED ${ReachableSubGraphSelection_SRCS})
TARGET_LINK_LIBRARIES(ReachableSubGraphSelection-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(ReachableSubGraphSelection-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(SpanningTreeSelection_SRCS SpanningTreeSelection.cpp SpanningTreeSelection.h)
ADD_LIBRARY(SpanningTreeSelection-${TulipVersion} SHARED ${SpanningTreeSelection_SRCS})
TARGET_LINK_LIBRARIES(SpanningTreeSelection-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(SpanningTreeSelection-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InducedSubGraphSelection_SRCS InducedSubGraphSelection.cpp InducedSubGraphSelection.h)
ADD_LIBRARY(InducedSubGraphSelection-${TulipVersion} SHARED ${InducedSubGraphSelection_SRCS})
TARGET_LINK_LIBRARIES(InducedSubGraphSelection-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(InducedSubGraphSelection-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(LoopSelection_SRCS LoopSelection.cpp LoopSelection.h)
ADD_LIBRARY(LoopSelection-${TulipVersion} SHARED ${LoopSelection_SRCS})
TARGET_LINK_LIBRARIES(LoopSelection-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(LoopSelection-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(MultipleSelection_SRCS MultipleEdgeSelection.cpp MultipleEdgeSelection.h)
ADD_LIBRARY(MultipleSelection-${TulipVersion} SHARED ${MultipleSelection_SRCS})
TARGET_LINK_LIBRARIES(MultipleSelection-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(MultipleSelection-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Kruskal_SRCS Kruskal.cpp Kruskal.h)
ADD_LIBRARY(Kruskal-${TulipVersion} SHARED ${Kruskal_SRCS})
TARGET_LINK_LIBRARIES(Kruskal-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(Kruskal-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	SpanningDagSelection-${TulipVersion}
	ReachableSubGraphSelection-${TulipVersion}
	SpanningTreeSelection-${TulipVersion}
	InducedSubGraphSelection-${TulipVersion}
	LoopSelection-${TulipVersion}
	MultipleSelection-${TulipVersion}
	Kruskal-${TulipVersion}
  DESTINATION ${TulipPluginsInstallDir}/
  COMPONENT libtulip_plugins)
