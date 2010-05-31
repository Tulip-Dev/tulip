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
	libConeTreeExtended-@VERSION@.la\
	libCircular-@VERSION@.la \
	libHierarchicalGraph-@VERSION@.la \
	libTreeLeaf-@VERSION@.la \
	libTreeRadial-@VERSION@.la \
	libBubbleTree-@VERSION@.la \
	libEmbedder-@VERSION@.la \
	libTutte-@VERSION@.la \
	libDendrogram-@VERSION@.la \
	libImprovedWalker-@VERSION@.la \
	libSquarifiedTreeMap-@VERSION@.la

pluginsdir = $(libdir)/tlp

EXTRA_DIST = CMakeLists.txt

libMixedModel_@VERSION@_la_SOURCES = MixedModel.h MixedModel.cpp DatasetTools.h DatasetTools.cpp
libConnectedComponentPacking_@VERSION@_la_SOURCES = ConnectedComponentPacking.h ConnectedComponentPacking.cpp Number.h Number.cpp RectanglePacking.h RectanglePacking.cpp rectanglePackingFonctions.h rectanglePackingFonctions.cpp RectangleRelativePositionList.h RectangleRelativePositionList.cpp DatasetTools.h DatasetTools.cpp
libRandom_@VERSION@_la_SOURCES = Random.h Random.cpp
libGemLayout_@VERSION@_la_SOURCES = GEMLayout.h GEMLayout.cpp
libTreeReingoldAndTilforExtended_@VERSION@_la_SOURCES = TreeReingoldAndTilfordExtended.h TreeReingoldAndTilfordExtended.cpp DatasetTools.h DatasetTools.cpp
libConeTreeExtended_@VERSION@_la_SOURCES = ConeTreeExtended.h ConeTreeExtended.cpp DatasetTools.h DatasetTools.cpp
libTreeRadial_@VERSION@_la_SOURCES = TreeRadial.cpp DatasetTools.h DatasetTools.cpp
libTreeLeaf_@VERSION@_la_SOURCES = TreeLeaf.h TreeLeaf.cpp DatasetTools.h DatasetTools.cpp OrientableCoord.h OrientableLayout.h OrientableSize.h OrientableSizeProxy.h OrientableCoord.cpp OrientableLayout.cpp OrientableSize.cpp OrientableSizeProxy.cpp
libBubbleTree_@VERSION@_la_SOURCES = BubbleTree.h BubbleTree.cpp DatasetTools.h DatasetTools.cpp
libCircular_@VERSION@_la_SOURCES = Circular.h Circular.cpp DatasetTools.h DatasetTools.cpp
libHierarchicalGraph_@VERSION@_la_SOURCES = HierarchicalGraph.h HierarchicalGraph.cpp DatasetTools.h DatasetTools.cpp
libTutte_@VERSION@_la_SOURCES = Tutte.h Tutte.cpp
libEmbedder_@VERSION@_la_SOURCES = Embedder.cpp EmbedderEmbedGraph.h EmbedderEmbedGraph.cpp EmbedderDefs.h EmbedderDijkstra.h EmbedderDijkstra.cpp EmbedderBfs.h EmbedderBfs.cpp EmbedderParameters.h EmbedderParameters.cpp EmbedderPowerIterationDense.cpp
libDendrogram_@VERSION@_la_SOURCES = DatasetTools.h DatasetTools.cpp EdgeTools.h EdgeTools.cpp OrientableCoord.h OrientableConstants.h OrientableLayout.h OrientableLayoutInterface.h OrientableSize.h OrientableSizeProxy.h Orientation.h Dendrogram.h OrientableCoord.cpp OrientableLayout.cpp OrientableSize.cpp OrientableSizeProxy.cpp Orientation.cpp TreeTools.h Dendrogram.cpp
libImprovedWalker_@VERSION@_la_SOURCES = DatasetTools.h DatasetTools.cpp EdgeTools.cpp ImprovedWalker.h ImprovedWalker.cpp OrientableCoord.cpp OrientableLayout.cpp OrientableSize.cpp OrientableSizeProxy.cpp Orientation.cpp
libSquarifiedTreeMap_@VERSION@_la_SOURCES = SquarifiedTreeMap.h SquarifiedTreeMap.cpp

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@
