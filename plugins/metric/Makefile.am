if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libDepthMetric-@VERSION@.la \
	libArityMetric-@VERSION@.la \
	libClusterMetric-@VERSION@.la \
	libStrengthMetric-@VERSION@.la \
	libBiconnectedComponent-@VERSION@.la \
	libConnectedComponent-@VERSION@.la \
	libStrongComponent-@VERSION@.la \
	libConnectedAndTreeComponent-@VERSION@.la \
	libDagLevelMetric-@VERSION@.la \
	libIdMetric-@VERSION@.la \
	libLeafMetric-@VERSION@.la \
	libNodeMetric-@VERSION@.la \
	libPathLengthMetric-@VERSION@.la \
	libSegmentMetric-@VERSION@.la \
	libStrahlerMetric-@VERSION@.la \
	libTreeArityMax-@VERSION@.la \
	libEccentricity-@VERSION@.la \
	libBetweennessCentrality-@VERSION@.la \
        libRandom-@VERSION@.la	

pluginsdir = $(libdir)/tlp

libEccentricity_@VERSION@_la_SOURCES = Eccentricity.h Eccentricity.cpp
libArityMetric_@VERSION@_la_SOURCES = ArityMetric.cpp ArityMetric.h
libClusterMetric_@VERSION@_la_SOURCES = ClusterMetric.cpp ClusterMetric.h
libStrengthMetric_@VERSION@_la_SOURCES = StrengthMetric.cpp StrengthMetric.h
libBiconnectedComponent_@VERSION@_la_SOURCES =  BiconnectedComponent.cpp
libConnectedComponent_@VERSION@_la_SOURCES =  ConnectedComponent.cpp  ConnectedComponent.h
libConnectedAndTreeComponent_@VERSION@_la_SOURCES =  ConnectedAndTreeComponent.cpp
libStrongComponent_@VERSION@_la_SOURCES = StrongComponent.cpp StrongComponent.h
libDagLevelMetric_@VERSION@_la_SOURCES = DagLevelMetric.cpp DagLevelMetric.h
libIdMetric_@VERSION@_la_SOURCES = IdMetric.cpp IdMetric.h
libLeafMetric_@VERSION@_la_SOURCES = LeafMetric.cpp LeafMetric.h
libNodeMetric_@VERSION@_la_SOURCES = NodeMetric.cpp NodeMetric.h
libDepthMetric_@VERSION@_la_SOURCES = DepthMetric.cpp DepthMetric.h
libPathLengthMetric_@VERSION@_la_SOURCES = PathLengthMetric.cpp PathLengthMetric.h
libSegmentMetric_@VERSION@_la_SOURCES = SegmentMetric.cpp SegmentMetric.h
libStrahlerMetric_@VERSION@_la_SOURCES = StrahlerMetric.cpp StrahlerMetric.h
libTreeArityMax_@VERSION@_la_SOURCES = TreeArityMax.cpp  TreeArityMax.h
libRandom_@VERSION@_la_SOURCES = Random.cpp  Random.h
libBetweennessCentrality_@VERSION@_la_SOURCES = BetweennessCentrality.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
