if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libPlanarGraph-@VERSION@.la \
	libGrid-@VERSION@.la \
	libDotImport-@VERSION@.la \
	libGMLImport-@VERSION@.la \
	libFileSystem-@VERSION@.la \
	libWebImport-@VERSION@.la \
	libRandomGraph-@VERSION@.la \
	libRandomSimpleGraph-@VERSION@.la \
	libAdjacencyMatrixImport-@VERSION@.la \
	libCompleteGraph-@VERSION@.la \
	libRandomTree-@VERSION@.la \
	libRandomTreeGeneral-@VERSION@.la \
	libCompleteTree-@VERSION@.la \
	libSmallWorldGraph-@VERSION@.la 

pluginsdir = $(libdir)/tlp

BUILT_SOURCES =  moc_WebImport.cpp

libPlanarGraph_@VERSION@_la_SOURCES = PlanarGraph.cpp
libDotImport_@VERSION@_la_SOURCES = dotImport.cpp dotImportCLUT.h dotImportLexer.h dotImportParser.h 
libGrid_@VERSION@_la_SOURCES = Grid.cpp
libGMLImport_@VERSION@_la_SOURCES = GMLImport.cpp GMLParser.h
libRandomGraph_@VERSION@_la_SOURCES = RandomGraph.cpp
libRandomSimpleGraph_@VERSION@_la_SOURCES = RandomSimpleGraph.cpp
libAdjacencyMatrixImport_@VERSION@_la_SOURCES = AdjacencyMatrixImport.cpp
libCompleteGraph_@VERSION@_la_SOURCES = CompleteGraph.cpp
libRandomTree_@VERSION@_la_SOURCES = RandomTree.cpp
libRandomTreeGeneral_@VERSION@_la_SOURCES = RandomTreeGeneral.cpp
libCompleteTree_@VERSION@_la_SOURCES = CompleteTree.cpp
libSmallWorldGraph_@VERSION@_la_SOURCES = SmallWorldGraph.cpp
libFileSystem_@VERSION@_la_SOURCES = FileSystem.cpp
libWebImport_@VERSION@_la_SOURCES = WebImport.cpp WebImport.h
nodist_libWebImport_@VERSION@_la_SOURCES = moc_WebImport.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @QT_INCLUDES@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

clean-generic:
	rm -f moc_*
