if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

EXTRA_DIST = CMakeLists.txt

plugins_LTLIBRARIES = 	libSpanningDagSelection-@VERSION@.la \
			libInducedSubGraphSelection-@VERSION@.la \
			libReachableSubGraphSelection-@VERSION@.la \
			libSpanningTreeSelection-@VERSION@.la \
			libLoopSelection-@VERSION@.la \
			libKruskal-@VERSION@.la \
			libMultipleSelection-@VERSION@.la

pluginsdir = $(libdir)/tlp

libSpanningDagSelection_@VERSION@_la_SOURCES = SpanningDagSelection.cpp SpanningDagSelection.h
libReachableSubGraphSelection_@VERSION@_la_SOURCES = ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp
libSpanningTreeSelection_@VERSION@_la_SOURCES = SpanningTreeSelection.cpp SpanningTreeSelection.h
libInducedSubGraphSelection_@VERSION@_la_SOURCES = InducedSubGraphSelection.cpp InducedSubGraphSelection.h
libLoopSelection_@VERSION@_la_SOURCES = LoopSelection.cpp LoopSelection.h
libMultipleSelection_@VERSION@_la_SOURCES = MultipleEdgeSelection.cpp MultipleEdgeSelection.h
libKruskal_@VERSION@_la_SOURCES = Kruskal.cpp Kruskal.h

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@

INCLUDES = @TULIP_INCLUDE@

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
