INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

SET(SpanningDagSelection_SRCS SpanningDagSelection.cpp SpanningDagSelection.h)
ADD_LIBRARY(SpanningDagSelection-${TulipVersion} SHARED ${SpanningDagSelection_SRCS})
TARGET_LINK_LIBRARIES(SpanningDagSelection-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ SpanningDagSelection-${TulipVersion})

SET(ReachableSubGraphSelection_SRCS ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp)
ADD_LIBRARY(ReachableSubGraphSelection-${TulipVersion} SHARED ${ReachableSubGraphSelection_SRCS})
TARGET_LINK_LIBRARIES(ReachableSubGraphSelection-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ReachableSubGraphSelection-${TulipVersion})

SET(SpanningTreeSelection_SRCS SpanningTreeSelection.cpp SpanningTreeSelection.h)
ADD_LIBRARY(SpanningTreeSelection-${TulipVersion} SHARED ${SpanningTreeSelection_SRCS})
TARGET_LINK_LIBRARIES(SpanningTreeSelection-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ SpanningTreeSelection-${TulipVersion})

SET(InducedSubGraphSelection_SRCS InducedSubGraphSelection.cpp InducedSubGraphSelection.h)
ADD_LIBRARY(InducedSubGraphSelection-${TulipVersion} SHARED ${InducedSubGraphSelection_SRCS})
TARGET_LINK_LIBRARIES(InducedSubGraphSelection-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ InducedSubGraphSelection-${TulipVersion})

SET(LoopSelection_SRCS LoopSelection.cpp LoopSelection.h)
ADD_LIBRARY(LoopSelection-${TulipVersion} SHARED ${LoopSelection_SRCS})
TARGET_LINK_LIBRARIES(LoopSelection-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ LoopSelection-${TulipVersion})

SET(MultipleSelection_SRCS MultipleEdgeSelection.cpp MultipleEdgeSelection.h)
ADD_LIBRARY(MultipleSelection-${TulipVersion} SHARED ${MultipleSelection_SRCS})
TARGET_LINK_LIBRARIES(MultipleSelection-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ MultipleSelection-${TulipVersion})

SET(Kruskal_SRCS Kruskal.cpp Kruskal.h)
ADD_LIBRARY(Kruskal-${TulipVersion} SHARED ${Kruskal_SRCS})
TARGET_LINK_LIBRARIES(Kruskal-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Kruskal-${TulipVersion})

