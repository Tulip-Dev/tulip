INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipInclude} ${TulipBuildInclude})

ADD_LIBRARY(Delaunay-${TulipVersion} SHARED DelaunayTriangulation.cpp)
TARGET_LINK_LIBRARIES(Delaunay-${TulipVersion} ${LibTulipName})

ADD_LIBRARY(Voronoi-${TulipVersion} SHARED VoronoiDiagram.cpp)
TARGET_LINK_LIBRARIES(Voronoi-${TulipVersion} ${LibTulipName})

INSTALL(TARGETS Delaunay-${TulipVersion} DESTINATION ${TulipPluginsInstallDir})
INSTALL(TARGETS Voronoi-${TulipVersion} DESTINATION ${TulipPluginsInstallDir})
