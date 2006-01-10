if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
	libpolyline.la \
	libconnnecteccomponnentpacking.la \
	librandom.la \
	libgem.la \
	libgem3d.la \
	libtreereingoldextended.la \
	libtreemap.la \
	libconetree.la\
	libconetreeextended.la\
	libcircular.la \
	libhierarchicalgraph.la \
	libspringelectrical.la \
	libtreeleaf.la \
	libtreeradial.la \
	libgeneralgraph3d.la \
	libtreeortho.la \
	libbubbletree.la \
	libembedder.la \
	libtutte.la \
	libscatter.la

pluginsdir = $(libdir)/tlp/plugins/layout


libpolyline_la_SOURCES = Polyline.cpp
libconnnecteccomponnentpacking_la_SOURCES = ConnnectedComponnentPacking.cpp  ConnnectedComponnentPacking.h Number.cpp Number.h RectanglePacking.cpp RectanglePacking.h rectanglePackingFonctions.cpp rectanglePackingFonctions.h RectangleRelativePositionList.cpp RectangleRelativePositionList.h
librandom_la_SOURCES = Random.cpp Random.h
libgem_la_SOURCES = GEM.cpp GEM.h
libgem3d_la_SOURCES = GEM3D.cpp GEM3D.h
libtreereingoldextended_la_SOURCES = TreeReingoldAndTilfordExtended.cpp TreeReingoldAndTilfordExtended.h 
libconetree_la_SOURCES = ConeTree.h ConeTree.cpp 
libconetreeextended_la_SOURCES = ConeTreeExtended.h ConeTreeExtended.cpp
libtreemap_la_SOURCES = TreeMap.h TreeMap.cpp 
libtreeradial_la_SOURCES = TreeRadial.cpp 
libtreeleaf_la_SOURCES = TreeLeaf.h TreeLeaf.cpp 
libbubbletree_la_SOURCES = BubbleTree.h BubbleTree.cpp 
libtreeortho_la_SOURCES = TreeReingoldOrtho.cpp TreeReingoldOrtho.h
libcircular_la_SOURCES = Circular.h Circular.cpp
libspringelectrical_la_SOURCES = SpringElectrical.h SpringElectrical.cpp
libhierarchicalgraph_la_SOURCES = HierarchicalGraph.h HierarchicalGraph.cpp
libgeneralgraph3d_la_SOURCES = GeneralGraph3D.cpp GeneralGraph3D.h 
libtutte_la_SOURCES = Tutte.h Tutte.cpp
libembedder_la_SOURCES = Embedder.cpp EmbedderEmbedGraph.cpp EmbedderDijkstra.cpp EmbedderBfs.cpp EmbedderParameters.cpp EmbedderPowerIterationDense.cpp EmbedderBfs.h EmbedderDefs.h EmbedderDijkstra.h EmbedderEmbedGraph.h EmbedderParameters.h
libscatter_la_SOURCES = ScatterPlot.h ScatterPlot.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ et @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
