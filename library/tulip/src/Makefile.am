lib_LTLIBRARIES = libtulip.la

if WIN32 
DLL_TULIP = -DDLL_TULIP
DLL_FLAGS = -no-undefined
RELEASE = ${VERSION}
else
RELEASE = @TULIP_VERSION@ -version-info 0:0:0
endif

INCLUDES = ${DLL_TULIP} @TULIP_INCLUDE@ @GZSTREAM_INCLUDE@
AM_CXXFLAGS = -D_TULIP_LIB_DIR=\"$(libdir)"/"\"
if WIN32
AM_LDFLAGS = ${DLL_FLAGS} $(top_builddir)/thirdparty/gzstream/libgzstream.la $(top_builddir)/thirdparty/triangle/libtriangle.la -lz -release ${RELEASE}
else
AM_LDFLAGS = ${DLL_FLAGS} `ls ../../../thirdparty/gzstream/.libs/*.o` `ls ../../../thirdparty/triangle/.libs/*.o` -lz -release ${RELEASE}
endif

libtulip_la_SOURCES = \
	Bfs.cpp \
	CanonicalOrdering.cpp \
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
	GraphAlgorithm.cpp \
	Observable.cpp \
	ObservableGraph.cpp \
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
	PluginsCreation.cpp \
	StlFunctions.cpp \
	StatisticsNodeModule.cpp \
	Polynome.cpp \
	Plane.cpp \
	FaceIterator.cpp \
	PlanarConMap.cpp \
	PlanarityTest.cpp \
	PlanarityTestImpl.cpp \
	PlanarityTestEmbed.cpp \
	PlanarityTestObstr.cpp \
	PlanarityTestTools.cpp \
	RectangleArea.cpp \
	Border.cpp
