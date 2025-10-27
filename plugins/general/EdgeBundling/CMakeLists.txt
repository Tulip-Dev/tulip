INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})
SET(LIB_SRCS 
    BendsTools.cpp
    Dijkstra.cpp
    EdgeBundling.cpp
    OctreeBundle.cpp
    QuadTree.cpp
    SphereUtils.cpp)

ADD_LIBRARY(EdgeBundling-${Tulip_VERSION} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(EdgeBundling-${Tulip_VERSION} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(EdgeBundling-${Tulip_VERSION} ${TulipPluginsInstallDir})
