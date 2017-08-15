INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
SET(LIB_SRCS 
    BendsTools.cpp
    Dijkstra.cpp
    EdgeBundling.cpp
    OctreeBundle.cpp
    QuadTree.cpp
    SphereUtils.cpp)

ADD_LIBRARY(EdgeBundling-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(EdgeBundling-${TulipVersion} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(EdgeBundling-${TulipVersion} ${TulipPluginsInstallDir})
