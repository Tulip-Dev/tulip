if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
	libdepth.la \
	libarity.la \
	libcluster.la \
	libstrength.la \
	libbiconnectedcomponents.la \
	libconnectedcomponnents.la \
	libstrongcomponnet.la \
	libconnectedandtreecomponnents.la \
	libdaglevel.la \
	libid.la \
	libleaf.la \
	libnode.la \
	libpathlength.la \
	libpathsum.la \
	libsegment.la \
	libstrahler.la \
	libtreearitymax.la \
	libeccentricity.la \
	libetweennesscentrality.la \
        librandom.la	

pluginsdir = $(libdir)/tlp/plugins/metric

libeccentricity_la_SOURCES = Eccentricity.h Eccentricity.cpp
libarity_la_SOURCES = ArityMetric.cpp ArityMetric.h
libcluster_la_SOURCES = ClusterMetric.cpp ClusterMetric.h
libstrength_la_SOURCES = StrengthMetric.cpp StrengthMetric.h
libbiconnectedcomponents_la_SOURCES =  BiconnectedComponent.cpp
libconnectedcomponnents_la_SOURCES =  ConnectedComponent.cpp  ConnectedComponent.h
libconnectedandtreecomponnents_la_SOURCES =  ConnectedAndTreeComponent.cpp
libstrongcomponnet_la_SOURCES = StrongComponent.cpp StrongComponent.h
libdaglevel_la_SOURCES = DagLevelMetric.cpp DagLevelMetric.h
libid_la_SOURCES = IdMetric.cpp IdMetric.h
libleaf_la_SOURCES = LeafMetric.cpp LeafMetric.h
libnode_la_SOURCES = NodeMetric.cpp NodeMetric.h
libdepth_la_SOURCES = DepthMetric.cpp DepthMetric.h
libpathlength_la_SOURCES = PathLengthMetric.cpp PathLengthMetric.h
libpathsum_la_SOURCES = PathSumMetric.cpp
libsegment_la_SOURCES = SegmentMetric.cpp SegmentMetric.h
libstrahler_la_SOURCES = StrahlerMetric.cpp StrahlerMetric.h
libtreearitymax_la_SOURCES = TreeArityMax.cpp  TreeArityMax.h
librandom_la_SOURCES = Random.cpp  Random.h
libetweennesscentrality_la_SOURCES = BetweennessCentrality.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
