if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

pluginsdir = $(libdir)/tlp

if HAVE_TULIP_OGL

if WIN32
ADD_INCLUDES = -I/mingw/include/freetype2
else
ADD_INCLUDES = `xml2-config --cflags` 
endif

plugins_LTLIBRARIES = \
		libAutoSize-@VERSION@.la \
		libFitToLabels-@VERSION@.la \
		libMetricSizeMapping-@VERSION@.la

libFitToLabels_@VERSION@_la_SOURCES = FitToLabel.cpp 
else
plugins_LTLIBRARIES = \
		libAutoSize-@VERSION@.la \
		libMetricSizeMapping-@VERSION@.la
endif

libMetricSizeMapping_@VERSION@_la_SOURCES = MetricMapping.cpp 
libAutoSize_@VERSION@_la_SOURCES = AutoSize.cpp 
AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ ${ADD_INCLUDES}
