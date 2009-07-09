if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

if HAVE_TULIP_QT
plugins_LTLIBRARIES = \
	libMetricColorMapping-@VERSION@.la \
	libEnumColorValuesMapping-@VERSION@.la

pluginsdir = $(libdir)/tlp

libMetricColorMapping_@VERSION@_la_SOURCES = MetricMapping.cpp 
libEnumColorValuesMapping_@VERSION@_la_SOURCES = EnumColorValuesMapping.cpp

INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @QT_INCLUDES@ @GL_INCLUDES@
AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ 
libEnumColorValuesMapping_@VERSION@_la_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
endif
