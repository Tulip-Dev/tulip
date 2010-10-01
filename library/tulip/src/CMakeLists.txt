SET(tulip_LIB_SRCS
  MQClusteringQualityMeasure.cpp
  QClusteringQualityMeasure.cpp
  ClusteringAlgorithmBase.cpp
  ClusteringQualityMeasure.cpp
	Bfs.cpp 
	Ordering.cpp 
	ConvexHull.cpp 
	Delaunay.cpp 
	DrawingTools.cpp 
	StringCollection.cpp 
	ExtendedClusterOperation.cpp 
	Reflect.cpp 
	Types.cpp 
	AbstractProperty.cpp 
	GraphTools.cpp 
	BiconnectedTest.cpp 
	TriconnectedTest.cpp   
	ConnectedTest.cpp   
	TreeTest.cpp   
	AcyclicTest.cpp   
	SimpleTest.cpp 
	Color.cpp  
	ColorProperty.cpp 
	ColorAlgorithm.cpp 
	Coord.cpp 
	GraphIterator.cpp 
	MapIterator.cpp 
	IdManager.cpp 
	IntegerProperty.cpp 
	IntegerAlgorithm.cpp 
	LayoutProperty.cpp 
	LayoutAlgorithm.cpp 
	DoubleProperty.cpp 
	DoubleAlgorithm.cpp 
	GraphProperty.cpp 
	GraphDecorator.cpp 
	Observable.cpp 
	ObservableGraph.cpp 
	ObservableProperty.cpp 
	PluginLoaderTxt.cpp 
	PropertyManager.cpp 
	BooleanProperty.cpp 
	BooleanAlgorithm.cpp 
	SizeProperty.cpp 
	SizeAlgorithm.cpp 
	StringProperty.cpp 
	StringAlgorithm.cpp 
	Graph.cpp 
	GraphAbstract.cpp 
	GraphImpl.cpp 
	GraphView.cpp 
	TlpTools.cpp 
	WithParameter.cpp 
	PluginProgress.cpp 
	GraphMeasure.cpp 
	PluginLibraryLoader.cpp 
	StlFunctions.cpp 
	Polynome.cpp 
	Plane.cpp 
	FaceIterator.cpp 
	PlanarConMap.cpp 
	PlanarityTest.cpp 
	OuterPlanarTest.cpp 
	PlanarityTestImpl.cpp 
	PlanarityTestEmbed.cpp 
	PlanarityTestObstr.cpp 
	PlanarityTestTools.cpp 
	TLPImport.cpp 
	TLPExport.cpp 
	GraphUpdatesRecorder.cpp 
	ColorScale.cpp
	PreferenceManager.cpp
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_TULIP_LIB_DIR=\"\\\"${CMAKE_INSTALL_PREFIX}/lib/\\\"\"")
INCLUDE_DIRECTORIES(../include/ ${TulipBuildInclude} ${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})

ADD_LIBRARY (${LibTulipName} SHARED ${tulip_LIB_SRCS} ../include/tulip/TulipRelease.h)
TARGET_LINK_LIBRARIES(${LibTulipName} gzstream )

INSTALL_TARGETS(${TulipLibInstallDir} ${LibTulipName})
