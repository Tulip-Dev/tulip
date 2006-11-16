if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libmixedmodel.la \
	libconnnecteccomponnentpacking.la \
	librandom.la \
	libgemlayout.la \
	libtreereingoldextended.la \
	libtreemap.la \
	libconetreeextended.la\
	libcircular.la \
	libhierarchicalgraph.la \
	libtreeleaf.la \
	libtreeradial.la \
	libgeneralgraph3d.la \
	libbubbletree.la \
	libembedder.la \
	libtutte.la \
	libscatter.la \
	libdendrogram.la \
	libimprovedwalker.la \
	libsquarifiedtreemap.la

pluginsdir = $(libdir)/tlp/plugins/layout


libmixedmodel_la_SOURCES = MixedModel.cpp MixedModel.h
libconnnecteccomponnentpacking_la_SOURCES = ConnnectedComponnentPacking.cpp  ConnnectedComponnentPacking.h Number.cpp Number.h RectanglePacking.cpp RectanglePacking.h rectanglePackingFonctions.cpp rectanglePackingFonctions.h RectangleRelativePositionList.cpp RectangleRelativePositionList.h
librandom_la_SOURCES = Random.cpp Random.h
libgemlayout_la_SOURCES = GEMLayout.cpp GEMLayout.h
libtreereingoldextended_la_SOURCES = TreeReingoldAndTilfordExtended.cpp TreeReingoldAndTilfordExtended.h 
libconetreeextended_la_SOURCES = ConeTreeExtended.h ConeTreeExtended.cpp
libtreemap_la_SOURCES = TreeMap.h TreeMap.cpp 
libtreeradial_la_SOURCES = TreeRadial.cpp 
libtreeleaf_la_SOURCES = TreeLeaf.h TreeLeaf.cpp 
libbubbletree_la_SOURCES = BubbleTree.h BubbleTree.cpp 
libcircular_la_SOURCES = Circular.h Circular.cpp
libhierarchicalgraph_la_SOURCES = HierarchicalGraph.h HierarchicalGraph.cpp
libgeneralgraph3d_la_SOURCES = GeneralGraph3D.cpp GeneralGraph3D.h 
libtutte_la_SOURCES = Tutte.h Tutte.cpp
libembedder_la_SOURCES = Embedder.cpp EmbedderEmbedGraph.cpp EmbedderDijkstra.cpp EmbedderBfs.cpp EmbedderParameters.cpp EmbedderPowerIterationDense.cpp EmbedderBfs.h EmbedderDefs.h EmbedderDijkstra.h EmbedderEmbedGraph.h EmbedderParameters.h
libscatter_la_SOURCES = ScatterPlot.h ScatterPlot.cpp
libdendrogram_la_SOURCES = DatasetTools.h DatasetTools.cpp EdgeTools.h EdgeTools.cpp OrientableConstants.h OrientableCoord.h OrientableCoord.cpp OrientableLayout.h OrientableLayout.cpp OrientableLayoutInterface.h OrientableSize.h OrientableSize.cpp OrientableSizeProxy.h OrientableSizeProxy.cpp Orientation.h Orientation.cpp TreeTools.h TreeTools.cpp Dendrogram.h Dendrogram.cpp
libimprovedwalker_la_SOURCES = DatasetTools.h DatasetTools.cpp EdgeTools.h EdgeTools.cpp ImprovedWalker.h ImprovedWalker.cpp OrientableCoord.h OrientableCoord.cpp OrientableLayout.h OrientableLayout.cpp OrientableSize.h OrientableSize.cpp OrientableSizeProxy.h OrientableSizeProxy.cpp Orientation.h Orientation.cpp TreeTools.h TreeTools.cpp
libsquarifiedtreemap_la_SOURCES = SquarifiedTreeMap.h SquarifiedTreeMap.cpp TreeTools.h TreeTools.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ et @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
