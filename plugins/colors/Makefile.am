if WIN32
DLL_FLAGS = -no-undefined
ADDLDFLAGS = -lxml2 
else
ADDLDFLAGS = `xml2-config --libs`
ADD_INCLUDES = `xml2-config --cflags` 
if MACOSX
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif
endif

plugins_LTLIBRARIES = \
	libMetricColorMapping-@VERSION@.la

pluginsdir = $(libdir)/tlp

libMetricColorMapping_@VERSION@_la_SOURCES = MetricMapping.cpp EnumColorValuesMapping.cpp

INCLUDES = $(ADD_INCLUDES) @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @QT_INCLUDES@
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@  @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@ @LIB_GL@ @GL_LDFLAGS@
AM_CPPFLAGS= @QT_CPPFLAGS@
