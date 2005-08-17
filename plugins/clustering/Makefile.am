if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
	libstrength.la \
	libhierarchical.la \
	libarity.la \
	libsegment.la \
	libaritysegment.la \
	libconvol.la \
	libequalvalues.la \
	libquotientclustering.la  \
	libplaneclustering.la

pluginsdir = $(libdir)/tlp/plugins/clustering

EXTRA_DIST = ConvolutionClusteringSetup.ui

libstrength_la_SOURCES = StrengthClustering.h StrengthClustering.cpp
libhierarchical_la_SOURCES = HierarchicalClustering.cpp  HierarchicalClustering.h
libarity_la_SOURCES = ArityRecClustering.h ArityRecClustering.cpp TableArity.h
libsegment_la_SOURCES = SegmentRecClustering.h SegmentRecClustering.cpp TableSegment.h
libaritysegment_la_SOURCES = AritySegmentRecClustering.h AritySegmentRecClustering.cpp TableAritySegment.h
BUILT_SOURCES = \
			ConvolutionClusteringSetupData.cpp \
			moc_ConvolutionClusteringSetupData.cpp \
			moc_ConvolutionClusteringSetup.cpp 
libconvol_la_SOURCES = 	ConvolutionClustering.h \
			ConvolutionClustering.cpp \
			ConvolutionClusteringSetup.cpp \
			ConvolutionClusteringSetup.h
nodist_libconvol_la_SOURCES = \
			ConvolutionClusteringSetupData.cpp \
			moc_ConvolutionClusteringSetupData.cpp \
			moc_ConvolutionClusteringSetup.cpp
libequalvalues_la_SOURCES = EqualValueClustering.h EqualValueClustering.cpp
libquotientclustering_la_SOURCES = QuotientClustering.h QuotientClustering.cpp
libplaneclustering_la_SOURCES = PlaneClustering.h PlaneClustering.cpp

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


clean:
	rm -f moc_*
