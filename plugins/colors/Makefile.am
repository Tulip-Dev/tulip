if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libmetricmapping.la

pluginsdir = $(libdir)/tlp/plugins/colors

libmetricmapping_la_SOURCES = MetricMapping.cpp

INCLUDES = @TULIP_INCLUDE@
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@
