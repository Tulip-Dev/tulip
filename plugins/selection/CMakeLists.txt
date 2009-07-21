INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(SpanningDagSelection_SRCS SpanningDagSelection.cpp SpanningDagSelection.h)
ADD_LIBRARY(SpanningDagSelection-${TulipVersion} SHARED ${SpanningDagSelection_SRCS})
TARGET_LINK_LIBRARIES(SpanningDagSelection-${TulipVersion} tulip)
ADD_DEPENDENCIES(SpanningDagSelection-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(ReachableSubGraphSelection_SRCS ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp)
ADD_LIBRARY(ReachableSubGraphSelection-${TulipVersion} SHARED ${ReachableSubGraphSelection_SRCS})
TARGET_LINK_LIBRARIES(ReachableSubGraphSelection-${TulipVersion} tulip)
ADD_DEPENDENCIES(ReachableSubGraphSelection-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(SpanningTreeSelection_SRCS SpanningTreeSelection.cpp SpanningTreeSelection.h)
ADD_LIBRARY(SpanningTreeSelection-${TulipVersion} SHARED ${SpanningTreeSelection_SRCS})
TARGET_LINK_LIBRARIES(SpanningTreeSelection-${TulipVersion} tulip)
ADD_DEPENDENCIES(SpanningTreeSelection-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(InducedSubGraphSelection_SRCS InducedSubGraphSelection.cpp InducedSubGraphSelection.h)
ADD_LIBRARY(InducedSubGraphSelection-${TulipVersion} SHARED ${InducedSubGraphSelection_SRCS})
TARGET_LINK_LIBRARIES(InducedSubGraphSelection-${TulipVersion} tulip)
ADD_DEPENDENCIES(InducedSubGraphSelection-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(LoopSelection_SRCS LoopSelection.cpp LoopSelection.h)
ADD_LIBRARY(LoopSelection-${TulipVersion} SHARED ${LoopSelection_SRCS})
TARGET_LINK_LIBRARIES(LoopSelection-${TulipVersion} tulip)
ADD_DEPENDENCIES(LoopSelection-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(MultipleSelection_SRCS MultipleEdgeSelection.cpp MultipleEdgeSelection.h)
ADD_LIBRARY(MultipleSelection-${TulipVersion} SHARED ${MultipleSelection_SRCS})
TARGET_LINK_LIBRARIES(MultipleSelection-${TulipVersion} tulip)
ADD_DEPENDENCIES(MultipleSelection-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
SET(Kruskal_SRCS Kruskal.cpp Kruskal.h)
ADD_LIBRARY(Kruskal-${TulipVersion} SHARED ${Kruskal_SRCS})
TARGET_LINK_LIBRARIES(Kruskal-${TulipVersion} tulip)
ADD_DEPENDENCIES(Kruskal-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	SpanningDagSelection-${TulipVersion}
	ReachableSubGraphSelection-${TulipVersion}
	SpanningTreeSelection-${TulipVersion}
	InducedSubGraphSelection-${TulipVersion}
	LoopSelection-${TulipVersion}
	MultipleSelection-${TulipVersion}
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
