if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libMetricColorMapping-@VERSION@.la

pluginsdir = $(libdir)/tlp

libMetricColorMapping_@VERSION@_la_SOURCES = MetricMapping.cpp EnumColorValuesMapping.cpp

INCLUDES = @TULIP_INCLUDE@
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@
