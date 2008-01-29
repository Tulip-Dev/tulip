if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libMetricColorMapping-@VERSION@.la

pluginsdir = $(libdir)/tlp

libMetricColorMapping_@VERSION@_la_SOURCES = MetricMapping.cpp EnumColorValuesMapping.cpp

INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @QT_INCLUDES@
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@  @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@ @LIB_GL@ @GL_LDFLAGS@
AM_CPPFLAGS= @QT_CPPFLAGS@
