if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = libGMLExport-@VERSION@.la
pluginsdir = $(libdir)/tlp

libGMLExport_@VERSION@_la_SOURCES = GMLExport.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
