if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libMixedModel-@VERSION@.la \
	libConnectedComponentPacking-@VERSION@.la \
	libRandom-@VERSION@.la \
	libGemLayout-@VERSION@.la \
	libTreeReingoldAndTilforExtended-@VERSION@.la \
	libTreeMap-@VERSION@.la \
	libConeTreeExtended-@VERSION@.la\
	libCircular-@VERSION@.la \
	libHierarchicalGraph-@VERSION@.la \
	libTreeLeaf-@VERSION@.la \
	libTreeRadial-@VERSION@.la \
	libGeneralGraph3D-@VERSION@.la \
	libBubbleTree-@VERSION@.la \
	libEmbedder-@VERSION@.la \
	libTutte-@VERSION@.la \
	libScatterPlot-@VERSION@.la \
	libDendrogram-@VERSION@.la \
	libImprovedWalker-@VERSION@.la \
	libSquarifiedTreeMap-@VERSION@.la

pluginsdir = $(libdir)/tlp

libMixedModel_@VERSION@_la_SOURCES = MixedModel.cpp DatasetTools.cpp
libConnectedComponentPacking_@VERSION@_la_SOURCES = ConnectedComponentPacking.cpp Number.cpp RectanglePacking.cpp rectanglePackingFonctions.cpp RectangleRelativePositionList.cpp DatasetTools.cpp
libRandom_@VERSION@_la_SOURCES = Random.cpp
libGemLayout_@VERSION@_la_SOURCES = GEMLayout.cpp
libTreeReingoldAndTilforExtended_@VERSION@_la_SOURCES = TreeReingoldAndTilfordExtended.cpp DatasetTools.cpp
libConeTreeExtended_@VERSION@_la_SOURCES = ConeTreeExtended.cpp DatasetTools.cpp
libTreeMap_@VERSION@_la_SOURCES = TreeMap.cpp 
libTreeRadial_@VERSION@_la_SOURCES = TreeRadial.cpp DatasetTools.cpp
libTreeLeaf_@VERSION@_la_SOURCES = TreeLeaf.cpp DatasetTools.cpp OrientableCoord.cpp OrientableLayout.cpp OrientableSize.cpp OrientableSizeProxy.cpp
libBubbleTree_@VERSION@_la_SOURCES = BubbleTree.cpp DatasetTools.cpp
libCircular_@VERSION@_la_SOURCES = Circular.cpp DatasetTools.cpp
libHierarchicalGraph_@VERSION@_la_SOURCES = HierarchicalGraph.cpp DatasetTools.cpp
libGeneralGraph3D_@VERSION@_la_SOURCES = GeneralGraph3D.cpp DatasetTools.cpp
libTutte_@VERSION@_la_SOURCES = Tutte.cpp
libEmbedder_@VERSION@_la_SOURCES = Embedder.cpp EmbedderEmbedGraph.cpp EmbedderDijkstra.cpp EmbedderBfs.cpp EmbedderParameters.cpp EmbedderPowerIterationDense.cpp
libScatterPlot_@VERSION@_la_SOURCES = ScatterPlot.cpp
libDendrogram_@VERSION@_la_SOURCES = DatasetTools.cpp EdgeTools.cpp OrientableCoord.cpp OrientableLayout.cpp OrientableSize.cpp OrientableSizeProxy.cpp Orientation.cpp Dendrogram.cpp
libImprovedWalker_@VERSION@_la_SOURCES = DatasetTools.cpp EdgeTools.cpp ImprovedWalker.cpp OrientableCoord.cpp OrientableLayout.cpp OrientableSize.cpp OrientableSizeProxy.cpp Orientation.cpp
libSquarifiedTreeMap_@VERSION@_la_SOURCES = SquarifiedTreeMap.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ et @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
