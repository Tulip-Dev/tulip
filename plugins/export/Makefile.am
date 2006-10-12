DLL_FLAGS = -no-undefined

plugins_LTLIBRARIES = libgml.la
pluginsdir = $(libdir)/tlp/plugins/export

libgml_la_SOURCES = GMLExport.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
