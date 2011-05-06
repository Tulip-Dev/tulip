SET(tulip_LIB_SRCS AcyclicTest.cpp
Bfs.cpp
BiconnectedTest.cpp
BooleanAlgorithm.cpp
BooleanProperty.cpp
CMakeLists.txt
ColorAlgorithm.cpp
Color.cpp
ColorProperty.cpp
ColorScale.cpp
ConnectedTest.cpp
ConvexHull.cpp
Delaunay.cpp
DoubleAlgorithm.cpp
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
IntegerAlgorithm.cpp
IntegerProperty.cpp
LayoutAlgorithm.cpp
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
PluginLibraryLoader.cpp
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
StringAlgorithm.cpp
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
INCLUDE_DIRECTORIES(../include/ ${TulipBuildInclude} ${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})

ADD_LIBRARY (${LibTulipName} SHARED ${tulip_LIB_SRCS} ../include/tulip/TulipRelease.h)
TARGET_LINK_LIBRARIES(${LibTulipName} gzstream )
IF(WIN32)
	IF(${MSVC})
		TARGET_LINK_LIBRARIES(${LibTulipName} Dbghelp)
		IF(${CMAKE_GENERATOR} MATCHES "Visual Studio 9" OR ${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
			SET_TARGET_PROPERTIES(${LibTulipName} PROPERTIES LINK_FLAGS " /NODEFAULTLIB:LIBCMT.lib ")
		ENDIF()
	ENDIF()
ENDIF(WIN32)

INSTALL(TARGETS ${LibTulipName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT libtulip
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT libtulip
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT libtulip)

IF(WIN32)
  FIND_LIBRARY_GLOB(ICONV_INSTALL_PATH "iconv.dll")
  FIND_LIBRARY_GLOB(ZLIB_INSTALL_PATH "zlib1.dll")
  FIND_LIBRARY_GLOB(MINGWM_INSTALL_PATH "mingwm*.dll")
  FIND_LIBRARY_GLOB(GCC_INSTALL_PATH "libgcc_s_dw2-1.dll")
  FIND_LIBRARY_GLOB(STDCPP_INSTALL_PATH "libstdc++-*.dll")
  FIND_LIBRARY_GLOB(PTHREAD_INSTALL_PATH "*pthread*.dll")
  FIND_LIBRARY_GLOB(GOMP_INSTALL_PATH "libgomp-*.dll")

  INSTALL_EXTERNAL_LIB(ICONV_INSTALL_PATH libtulip)
  INSTALL_EXTERNAL_LIB(ZLIB_INSTALL_PATH libtulip)
  INSTALL_EXTERNAL_LIB(MINGWM_INSTALL_PATH libtulip)
  INSTALL_EXTERNAL_LIB(GCC_INSTALL_PATH libtulip)
  INSTALL_EXTERNAL_LIB(STDCPP_INSTALL_PATH libtulip)
  INSTALL_EXTERNAL_LIB(PTHREAD_INSTALL_PATH libtulip)
  INSTALL_EXTERNAL_LIB(GOMP_INSTALL_PATH libtulip)
ELSEIF(APPLE)
  INSTALL_EXTERNAL_LIB(ZLIB_LIBRARY libtulip)
ENDIF()
