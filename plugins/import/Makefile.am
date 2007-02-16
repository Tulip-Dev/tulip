if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = \
	libplanar.la \
	libgrid.la \
	libdot.la \
	libgml.la \
	libfilesystem.la \
	libwebimport.la \
	librandomgraph.la \
	librandomsimplegraph.la \
	libadjacencymatrix.la \
	libcompletegraph.la \
	librandomtree.la \
	librandomtreegeneral.la \
	libcompletetree.la \
	libsmallworld.la 

pluginsdir = $(libdir)/tlp/plugins/import

BUILT_SOURCES =  moc_WebImport.cpp

libplanar_la_SOURCES = PlanarGraph.cpp
libdot_la_SOURCES = dotImport.cpp dotImportCLUT.h dotImportLexer.h dotImportParser.h 
libgrid_la_SOURCES = Grid.cpp
libgml_la_SOURCES = GMLImport.cpp GMLParser.h
librandomgraph_la_SOURCES = RandomGraph.cpp
librandomsimplegraph_la_SOURCES = RandomSimpleGraph.cpp
libadjacencymatrix_la_SOURCES = AdjacencyMatrixImport.cpp
libcompletegraph_la_SOURCES = CompleteGraph.cpp
librandomtree_la_SOURCES = RandomTree.cpp
librandomtreegeneral_la_SOURCES = RandomTreeGeneral.cpp
libcompletetree_la_SOURCES = CompleteTree.cpp
libsmallworld_la_SOURCES = SmallWorldGraph.cpp
libfilesystem_la_SOURCES = FileSystem.cpp
libwebimport_la_SOURCES = WebImport.cpp WebImport.h
nodist_libwebimport_la_SOURCES = moc_WebImport.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @QT_INCLUDES@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

