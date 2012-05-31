INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude})

ADD_LIBRARY(Delaunay-${TulipVersion} SHARED DelaunayTriangulation.cpp)
TARGET_LINK_LIBRARIES(Delaunay-${TulipVersion} ${LibTulipCoreName})

ADD_LIBRARY(Voronoi-${TulipVersion} SHARED VoronoiDiagram.cpp)
TARGET_LINK_LIBRARIES(Voronoi-${TulipVersion} ${LibTulipCoreName})

INSTALL(TARGETS Delaunay-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT DelaunayTriangulation)
INSTALL(TARGETS Voronoi-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT VoronoiDiagram)
