INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
SET(LIB_SRCS 
    BendsTools.cpp
    Dijkstra.cpp
    EdgeBundling.cpp
    OctreeBundle.cpp
    QuadTree.cpp
    SphereUtils.cpp)

ADD_LIBRARY(EdgeBundling-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(EdgeBundling-${TulipVersion} ${LibTulipCoreName})

INSTALL(TARGETS EdgeBundling-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT EdgeBundling)
