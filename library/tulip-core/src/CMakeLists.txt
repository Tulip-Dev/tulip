SET(tulip_LIB_SRCS ${tulip_LIB_SRCS}
Plugin.cpp
AcyclicTest.cpp
Bfs.cpp
BiconnectedTest.cpp
BooleanProperty.cpp
BoundingBox.cpp
Color.cpp
ColorProperty.cpp
ColorScale.cpp
ConnectedTest.cpp
ConvexHull.cpp
DataSet.cpp
Delaunay.cpp
DoubleProperty.cpp
DrawingTools.cpp
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
NumericProperty.cpp
Observable.cpp
Ordering.cpp
OuterPlanarTest.cpp
ParametricCurves.cpp
ParallelTools.cpp
PlanarConMap.cpp
PlanarityTest.cpp
PlanarityTestEmbed.cpp
PlanarityTestImpl.cpp
PlanarityTestObstr.cpp
PlanarityTestTools.cpp
Plane.cpp
PluginLibraryLoader.cpp
PluginLoaderTxt.cpp
PropertyAlgorithm.cpp
PropertyInterface.cpp
PropertyManager.cpp
PropertyTypes.cpp
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
WithParameter.cpp
YajlFacade.cpp
PluginLister.cpp
tulipconf.cpp
TulipException.cpp
PluginProgress.cpp
TulipViewSettings.cpp
TlpJsonExport.cpp
TlpJsonImport.cpp
TLPBExport.cpp
TLPBImport.cpp
)

IF(EMSCRIPTEN)
SET(tulip_LIB_SRCS ${tulip_LIB_SRCS}
                    ../../../plugins/layout/DatasetTools.cpp
                    ../../../plugins/layout/OrientableCoord.cpp
                    ../../../plugins/layout/OrientableLayout.cpp
                    ../../../plugins/layout/OrientableSize.cpp
                    ../../../plugins/layout/OrientableSizeProxy.cpp
                    ../../../plugins/layout/Orientation.cpp)
ENDIF(EMSCRIPTEN)

IF(HAVE_REENTRANT_QHULL)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_REENTRANT_QHULL")
ENDIF(HAVE_REENTRANT_QHULL)
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR} ${YajlInclude} ${QhullInclude} ${GZStreamInclude} ${UTF8CppInclude})

ADD_LIBRARY(${LibTulipCoreName} SHARED ${tulip_LIB_SRCS} ../include/tulip/TulipRelease.h)
TARGET_LINK_LIBRARIES(${LibTulipCoreName} ${GZStreamLibrary} ${YajlLibrary} ${QhullLibrary} ${CMAKE_DL_LIBS})
IF(WIN32)
  IF(MSVC)
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

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
TULIP_COPY_TARGET_LIBRARY_POST_BUILD(${LibTulipCoreName} ${TULIP_PYTHON_NATIVE_FOLDER} wheel)
IF(LINUX)
TULIP_COPY_TARGET_LIBRARY_POST_BUILD(${LibTulipCoreName} ${TULIPGUI_PYTHON_NATIVE_FOLDER} wheel)
ENDIF(LINUX)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
