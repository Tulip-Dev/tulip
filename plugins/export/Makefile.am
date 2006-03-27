if WIN32
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = libtlp.la libgml.la
pluginsdir = $(libdir)/tlp/plugins/export

libtlp_la_SOURCES = TLPExport.cpp
libgml_la_SOURCES = GMLExport.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
