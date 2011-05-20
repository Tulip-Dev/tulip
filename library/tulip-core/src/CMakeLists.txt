SET(tulip_LIB_SRCS ${tulip_LIB_SRCS}
PluginLibraryLoader.cpp
AcyclicTest.cpp
Bfs.cpp
BiconnectedTest.cpp
BooleanProperty.cpp
CMakeLists.txt
Color.cpp
ColorProperty.cpp
ColorScale.cpp
ConnectedTest.cpp
ConvexHull.cpp
Delaunay.cpp
DoubleProperty.cpp
DrawingTools.cpp
ExtendedClusterOperation.cpp
FaceIterator.cpp
GraphAbstract.cpp
Graph.cpp
GraphDecorator.cpp
GraphImpl.cpp
GraphIterator.cpp
GraphMeasure.cpp
GraphProperty.cpp
GraphStorage.cpp
GraphTools.cpp
GraphUpdatesRecorder.cpp
GraphView.cpp
IdManager.cpp
IntegerProperty.cpp
LayoutProperty.cpp
MapIterator.cpp
Observable.cpp
ObservableGraph.cpp
ObservableProperty.cpp
Ordering.cpp
OuterPlanarTest.cpp
PlanarConMap.cpp
PlanarityTest.cpp
PlanarityTestEmbed.cpp
PlanarityTestImpl.cpp
PlanarityTestObstr.cpp
PlanarityTestTools.cpp
Plane.cpp
PluginLoaderTxt.cpp
Polynome.cpp
PreferenceManager.cpp
PropertyInterface.cpp
PropertyManager.cpp
PropertyTypes.cpp
Reflect.cpp
SimplePluginProgress.cpp
SimpleTest.cpp
SizeProperty.cpp
StlFunctions.cpp
StringCollection.cpp
StringProperty.cpp
TLPExport.cpp
TLPImport.cpp
TlpTools.cpp
TreeTest.cpp
TriconnectedTest.cpp
vectorgraph.cpp
WithParameter.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_TULIP_LIB_DIR=\"\\\"${CMAKE_INSTALL_PREFIX}/lib/\\\"\"")
INCLUDE_DIRECTORIES(../include/ ${TulipCoreBuildInclude} ${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR} ${QT_INCLUDE_DIR})

ADD_LIBRARY(${LibTulipCoreName} SHARED ${tulip_LIB_SRCS} ../include/tulip/TulipRelease.h)
TARGET_LINK_LIBRARIES(${LibTulipCoreName} gzstream ${QT_QTCORE_LIBRARY})
IF(WIN32)
  IF(${MSVC})
    TARGET_LINK_LIBRARIES(${LibTulipCoreName} Dbghelp)
    IF(${CMAKE_GENERATOR} MATCHES "Visual Studio 9" OR ${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
      SET_TARGET_PROPERTIES(${LibTulipCoreName} PROPERTIES LINK_FLAGS " /NODEFAULTLIB:LIBCMT.lib ")
    ENDIF()
  ENDIF()
ENDIF(WIN32)

INSTALL(TARGETS ${LibTulipCoreName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_core
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_core
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_core)
