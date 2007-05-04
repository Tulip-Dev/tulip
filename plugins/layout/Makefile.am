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

libMixedModel_@VERSION@_la_SOURCES = MixedModel.cpp MixedModel.h
libConnectedComponentPacking_@VERSION@_la_SOURCES = ConnectedComponentPacking.cpp ConnectedComponentPacking.h Number.cpp Number.h RectanglePacking.cpp RectanglePacking.h rectanglePackingFonctions.cpp rectanglePackingFonctions.h RectangleRelativePositionList.cpp RectangleRelativePositionList.h
libRandom_@VERSION@_la_SOURCES = Random.cpp Random.h
libGemLayout_@VERSION@_la_SOURCES = GEMLayout.cpp GEMLayout.h
libTreeReingoldAndTilforExtended_@VERSION@_la_SOURCES = TreeReingoldAndTilfordExtended.cpp TreeReingoldAndTilfordExtended.h
libConeTreeExtended_@VERSION@_la_SOURCES = ConeTreeExtended.h ConeTreeExtended.cpp
libTreeMap_@VERSION@_la_SOURCES = TreeMap.h TreeMap.cpp 
libTreeRadial_@VERSION@_la_SOURCES = TreeRadial.cpp
libTreeLeaf_@VERSION@_la_SOURCES = TreeLeaf.h TreeLeaf.cpp DatasetTools.cpp OrientableCoord.cpp OrientableLayout.cpp
libBubbleTree_@VERSION@_la_SOURCES = BubbleTree.h BubbleTree.cpp 
libCircular_@VERSION@_la_SOURCES = Circular.h Circular.cpp
libHierarchicalGraph_@VERSION@_la_SOURCES = HierarchicalGraph.h HierarchicalGraph.cpp
libGeneralGraph3D_@VERSION@_la_SOURCES = GeneralGraph3D.cpp GeneralGraph3D.h 
libTutte_@VERSION@_la_SOURCES = Tutte.h Tutte.cpp
libEmbedder_@VERSION@_la_SOURCES = Embedder.cpp EmbedderEmbedGraph.cpp EmbedderDijkstra.cpp EmbedderBfs.cpp EmbedderParameters.cpp EmbedderPowerIterationDense.cpp EmbedderBfs.h EmbedderDefs.h EmbedderDijkstra.h EmbedderEmbedGraph.h EmbedderParameters.h
libScatterPlot_@VERSION@_la_SOURCES = ScatterPlot.h ScatterPlot.cpp
libDendrogram_@VERSION@_la_SOURCES = DatasetTools.h DatasetTools.cpp EdgeTools.h EdgeTools.cpp OrientableConstants.h OrientableCoord.h OrientableCoord.cpp OrientableLayout.h OrientableLayout.cpp OrientableLayoutInterface.h OrientableSize.h OrientableSize.cpp OrientableSizeProxy.h OrientableSizeProxy.cpp Orientation.h Orientation.cpp Dendrogram.h Dendrogram.cpp
libImprovedWalker_@VERSION@_la_SOURCES = DatasetTools.h DatasetTools.cpp EdgeTools.h EdgeTools.cpp ImprovedWalker.h ImprovedWalker.cpp OrientableCoord.h OrientableCoord.cpp OrientableLayout.h OrientableLayout.cpp OrientableSize.h OrientableSize.cpp OrientableSizeProxy.h OrientableSizeProxy.cpp Orientation.h Orientation.cpp
libSquarifiedTreeMap_@VERSION@_la_SOURCES = SquarifiedTreeMap.h SquarifiedTreeMap.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ et @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
