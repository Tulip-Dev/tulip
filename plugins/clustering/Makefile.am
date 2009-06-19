if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libStrengthClustering-@VERSION@.la \
	libHierarchicalClustering-@VERSION@.la \
	libEqualValueClustering-@VERSION@.la \
	libQuotientClustering-@VERSION@.la

pluginsdir = $(libdir)/tlp

libStrengthClustering_@VERSION@_la_SOURCES = StrengthClustering.h StrengthClustering.cpp
libHierarchicalClustering_@VERSION@_la_SOURCES = HierarchicalClustering.cpp  HierarchicalClustering.h
libEqualValueClustering_@VERSION@_la_SOURCES = EqualValueClustering.h EqualValueClustering.cpp
libQuotientClustering_@VERSION@_la_SOURCES = QuotientClustering.h QuotientClustering.cpp

INCLUDES = @TULIP_INCLUDE@

if HAVE_TULIP_QT
pluginsqt_LTLIBRARIES = libConvolutionClustering-@VERSION@.la

pluginsqtdir = $(libdir)/tlp

EXTRA_DIST = ConvolutionClusteringSetup.ui

libConvolutionClustering_@VERSION@_la_SOURCES = \
			ConvolutionClustering.h \
			ConvolutionClustering.cpp \
			ConvolutionClusteringSetup.cpp \
			ConvolutionClusteringSetup.h
nodist_libConvolutionClustering_@VERSION@_la_SOURCES = \
			moc_ConvolutionClusteringSetup.cpp

libConvolutionClustering_@VERSION@_la_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@

BUILT_SOURCES = ConvolutionClusteringSetupData.h moc_ConvolutionClusteringSetup.cpp 

AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES += @QT_INCLUDES@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_* *Data.*
endif

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@
