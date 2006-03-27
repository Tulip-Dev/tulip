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
AM_LDFLAGS = ${DLL_FLAGS} @GZSTREAM_LDFLAGS@ ../../../thirdparty/triangle/libtriangle.la -release ${RELEASE}

libtulip_la_SOURCES = \
	Bfs.cpp \
	FaceIterator2.cpp \
	SuperGraphConMap.cpp \
	CanonicalOrdering.cpp \
	ConvexHull.cpp \
	Delaunay.cpp \
	DrawingTools.cpp \
	StringCollection.cpp \
	ExtendedClusterOperation.cpp \
	Reflect.cpp \
	Types.cpp \
	PropertyProxy.cpp \
	GraphTools.cpp \
	BiconnectedTest.cpp \
	TriconnectedTest.cpp   \
	ConnectedTest.cpp   \
	TreeTest.cpp   \
	AcyclicTest.cpp   \
	SimpleTest.cpp \
	Color.cpp  \
	Colors.cpp \
	ColorsProxy.cpp \
	Coord.cpp \
	GraphIterator.cpp \
	MapIterator.cpp \
	IdManager.cpp \
	Int.cpp \
	IntProxy.cpp \
	Layout.cpp \
	LayoutProxy.cpp \
	Metric.cpp \
	MetricProxy.cpp \
	MetaGraph.cpp \
	MetaGraphProxy.cpp \
	Observable.cpp \
	ObservableGraph.cpp \
	PluginLoaderTxt.cpp \
	PropertyManager.cpp \
	Selection.cpp \
	SelectionProxy.cpp \
	Sizes.cpp \
	SizesProxy.cpp \
	String.cpp \
	StringProxy.cpp \
	SuperGraph.cpp \
	SuperGraphAbstract.cpp \
	SuperGraphImpl.cpp \
	SuperGraphView.cpp \
	TlpTools.cpp \
	WithParameter.cpp \
	PluginProgress.cpp \
	GraphMeasure.cpp \
	PluginsCreation.cpp \
	StlFunctions.cpp \
	StatisticsNodeModule.cpp \
	Polynome.cpp \
	Plane.cpp \
	SuperGraphMap.cpp \
	FaceIterator.cpp \
	PlanarityTest.cpp \
	PlanarityTestImpl.cpp \
	PlanarityTestEmbed.cpp \
	PlanarityTestObstr.cpp \
	PlanarityTestTools.cpp \
	RectangleArea.cpp \
	Border.cpp
