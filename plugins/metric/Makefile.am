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
	libDagLevelMetric-@VERSION@.la \
	libIdMetric-@VERSION@.la \
	libLeafMetric-@VERSION@.la \
	libNodeMetric-@VERSION@.la \
	libPathLengthMetric-@VERSION@.la \
	libStrahlerMetric-@VERSION@.la \
	libEccentricity-@VERSION@.la \
	libBetweennessCentrality-@VERSION@.la \
        libRandomMetric-@VERSION@.la	

pluginsdir = $(libdir)/tlp

libEccentricity_@VERSION@_la_SOURCES = Eccentricity.h Eccentricity.cpp
libArityMetric_@VERSION@_la_SOURCES = ArityMetric.cpp ArityMetric.h
libClusterMetric_@VERSION@_la_SOURCES = ClusterMetric.cpp ClusterMetric.h
libStrengthMetric_@VERSION@_la_SOURCES = StrengthMetric.cpp StrengthMetric.h
libBiconnectedComponent_@VERSION@_la_SOURCES =  BiconnectedComponent.cpp
libConnectedComponent_@VERSION@_la_SOURCES =  ConnectedComponent.cpp  ConnectedComponent.h
libStrongComponent_@VERSION@_la_SOURCES = StrongComponent.cpp StrongComponent.h
libDagLevelMetric_@VERSION@_la_SOURCES = DagLevelMetric.cpp DagLevelMetric.h
libIdMetric_@VERSION@_la_SOURCES = IdMetric.cpp IdMetric.h
libLeafMetric_@VERSION@_la_SOURCES = LeafMetric.cpp LeafMetric.h
libNodeMetric_@VERSION@_la_SOURCES = NodeMetric.cpp NodeMetric.h
libDepthMetric_@VERSION@_la_SOURCES = DepthMetric.cpp DepthMetric.h
libPathLengthMetric_@VERSION@_la_SOURCES = PathLengthMetric.cpp PathLengthMetric.h
libStrahlerMetric_@VERSION@_la_SOURCES = StrahlerMetric.cpp StrahlerMetric.h
libRandomMetric_@VERSION@_la_SOURCES = Random.cpp  Random.h
libBetweennessCentrality_@VERSION@_la_SOURCES = BetweennessCentrality.cpp

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@
