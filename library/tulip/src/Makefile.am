lib_LTLIBRARIES = libtulip.la

if WIN32 
DLL_TULIP = -DDLL_TULIP
DLL_FLAGS = -no-undefined
else
if MACOSX
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif
endif

INCLUDES = ${DLL_TULIP} @TULIP_INCLUDE@ @GZSTREAM_INCLUDE@ -I$(top_srcdir)
AM_CXXFLAGS = -D_TULIP_LIB_DIR=\"$(libdir)"/"\"
if !MACOSX
AM_LDFLAGS = ${DLL_FLAGS} @GZSTREAM_LDFLAGS@ -release @MAJOR_VERSION@.@MINOR_VERSION@
else
AM_LDFLAGS = ${DLL_FLAGS} `ls ../../../thirdparty/gzstream/*.lo` -lz -release @MAJOR_VERSION@.@MINOR_VERSION@
endif

libtulip_la_SOURCES = \
	Bfs.cpp \
	Ordering.cpp \
	ConvexHull.cpp \
	Delaunay.cpp \
	DrawingTools.cpp \
	StringCollection.cpp \
	ExtendedClusterOperation.cpp \
	Reflect.cpp \
	Types.cpp \
	AbstractProperty.cpp \
	GraphTools.cpp \
	BiconnectedTest.cpp \
	TriconnectedTest.cpp   \
	ConnectedTest.cpp   \
	TreeTest.cpp   \
	AcyclicTest.cpp   \
	SimpleTest.cpp \
	Color.cpp  \
	ColorProperty.cpp \
	ColorAlgorithm.cpp \
	Coord.cpp \
	GraphIterator.cpp \
	MapIterator.cpp \
	IdManager.cpp \
	IntegerProperty.cpp \
	IntegerAlgorithm.cpp \
	LayoutProperty.cpp \
	LayoutAlgorithm.cpp \
	DoubleProperty.cpp \
	DoubleAlgorithm.cpp \
	GraphProperty.cpp \
	GraphDecorator.cpp \
	Observable.cpp \
	ObservableGraph.cpp \
	ObservableProperty.cpp \
	PluginLoaderTxt.cpp \
	PropertyManager.cpp \
	BooleanProperty.cpp \
	BooleanAlgorithm.cpp \
	SizeProperty.cpp \
	SizeAlgorithm.cpp \
	StringProperty.cpp \
	StringAlgorithm.cpp \
	Graph.cpp \
	GraphAbstract.cpp \
	GraphImpl.cpp \
	GraphView.cpp \
	TlpTools.cpp \
	WithParameter.cpp \
	PluginProgress.cpp \
	GraphMeasure.cpp \
	PluginLibraryLoader.cpp \
	StlFunctions.cpp \
	StatisticsNodeModule.cpp \
	Polynome.cpp \
	Plane.cpp \
	FaceIterator.cpp \
	PlanarConMap.cpp \
	PlanarityTest.cpp \
	OuterPlanarTest.cpp \
	PlanarityTestImpl.cpp \
	PlanarityTestEmbed.cpp \
	PlanarityTestObstr.cpp \
	PlanarityTestTools.cpp \
	RectangleArea.cpp \
	Border.cpp \
	TLPImport.cpp \
	TLPExport.cpp \
	GraphUpdatesRecorder.cpp \
	ColorScale.cpp \
	PreferenceManager.cpp
