INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude})

ADD_LIBRARY(Delaunay-${Tulip_VERSION} SHARED DelaunayTriangulation.cpp)
TARGET_LINK_LIBRARIES(Delaunay-${Tulip_VERSION} ${LibTulipCoreName})

ADD_LIBRARY(Voronoi-${Tulip_VERSION} SHARED VoronoiDiagram.cpp)
TARGET_LINK_LIBRARIES(Voronoi-${Tulip_VERSION} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(Delaunay-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(Voronoi-${Tulip_VERSION} ${TulipPluginsInstallDir})
