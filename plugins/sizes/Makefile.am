if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

if WIN32
ADD_INCLUDES = -I/mingw/include/freetype2
else
ADD_INCLUDES = `xml2-config --cflags` 
endif

plugins_LTLIBRARIES = \
		libAutoSize-@VERSION@.la \
		libFitToLabels-@VERSION@.la \
		libMetricSizeMapping-@VERSION@.la 

pluginsdir = $(libdir)/tlp

libMetricSizeMapping_@VERSION@_la_SOURCES = MetricMapping.cpp 
libAutoSize_@VERSION@_la_SOURCES = AutoSize.cpp 
libFitToLabels_@VERSION@_la_SOURCES = FitToLabel.cpp 

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_OGL_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @QT_INCLUDES@ ${ADD_INCLUDES}
UIC = @UIC@ -L @TULIP_QTDESIGNERPLUGIN@
MOC = @MOC@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<


moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.cpp: %.ui %Data.h
	$(UIC) -o $@ -impl $*Data.h $<

%Data.h: %.ui
	$(UIC) -o $@ $<


clean-generic:
	rm -f moc_*
