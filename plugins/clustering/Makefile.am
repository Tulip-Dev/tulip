if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libstrength.la \
	libhierarchical.la \
	libconvol.la \
	libequalvalues.la \
	libquotientclustering.la

pluginsdir = $(libdir)/tlp/plugins/clustering

EXTRA_DIST = ConvolutionClusteringSetup.ui

libstrength_la_SOURCES = StrengthClustering.h StrengthClustering.cpp
libhierarchical_la_SOURCES = HierarchicalClustering.cpp  HierarchicalClustering.h
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

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
UIC = @UIC@ -L @TULIP_QTDESIGNERPLUGIN@
MOC = @MOC@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.cpp: %.ui %Data.h
	$(UIC) -o $@ -impl $*Data.h $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_*
