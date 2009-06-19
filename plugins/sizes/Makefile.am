if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
		libAutoSize-@VERSION@.la \
		libMetricSizeMapping-@VERSION@.la

pluginsdir = $(libdir)/tlp

libMetricSizeMapping_@VERSION@_la_SOURCES = MetricMapping.cpp 
libAutoSize_@VERSION@_la_SOURCES = AutoSize.cpp 

if HAVE_TULIP_OGL

if WIN32
ADD_INCLUDES = -I/mingw/include/freetype2
else
ADD_INCLUDES = `xml2-config --cflags` 
endif

pluginsogl_LTLIBRARIES = libFitToLabels-@VERSION@.la
pluginsogldir = $(libdir)/tlp
libFitToLabels_@VERSION@_la_SOURCES = FitToLabel.cpp 

INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ ${ADD_INCLUDES}
else
INCLUDES = @TULIP_INCLUDE@
endif

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@
