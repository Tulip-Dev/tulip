if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
	libhistogrammapping.la \
	libmetricmapping.la 

pluginsdir = $(libdir)/tlp/plugins/colors

libmetricmapping_la_SOURCES = MetricMapping.cpp
libhistogrammapping_la_SOURCES = HistogramMapping.cpp

INCLUDES = @TULIP_INCLUDE@
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@
