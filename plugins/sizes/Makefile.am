if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
		libAutoSize.la \
		libfittolabels.la \
		libmetricmapping.la 

pluginsdir = $(libdir)/tlp/plugins/sizes

libmetricmapping_la_SOURCES = MetricMapping.cpp 
libAutoSize_la_SOURCES = AutoSize.cpp 
libfittolabels_la_SOURCES = FitToLabel.cpp 

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
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
