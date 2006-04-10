if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = 	libspanningdag.la \
			libinducedsubgraph.la \
			libreachable.la \
			libspanningtree.la \
			libloopselection.la \
			libkruskal.la \
			libmultipleselection.la

pluginsdir = $(libdir)/tlp/plugins/selection
libspanningdag_la_SOURCES = SpanningDagSelection.cpp SpanningDagSelection.h
libreachable_la_SOURCES = ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp
libspanningtree_la_SOURCES = SpanningTreeSelection.cpp SpanningTreeSelection.h
libinducedsubgraph_la_SOURCES = InducedSubGraphSelection.cpp InducedSubGraphSelection.h
libloopselection_la_SOURCES = LoopSelection.cpp LoopSelection.h
libmultipleselection_la_SOURCES = MultipleEdgeSelection.cpp MultipleEdgeSelection.h
libkruskal_la_SOURCES = Kruskal.cpp Kruskal.h

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@

INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@

UIC = LD_LIBRARY_PATH=$(top_builddir)/library/tulip/src/.libs:$(top_builddir)/library/tulip-ogl/src/.libs:$(top_builddir)/library/tulip-qt/src/.libs:$$LD_LIBRARY_PATH @UIC@ -L @TULIP_QTDESIGNERPLUGIN@
MOC = @MOC@


CLEANFILES = $(BUILT_SOURCES)

moc_%.cpp: %.h 
	$(MOC) -o $@ $<


moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.cpp: %.ui %Data.h
	$(UIC) -o $@ -impl $*Data.h $<

%Data.h: %.ui
	$(UIC) -o $@ $<
