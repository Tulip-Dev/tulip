if WIN32
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = 

pluginsdir = $(libdir)/tlp/plugins/string

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @TULIP_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
INCLUDES = @TULIP_INCLUDE@ @QT_INCLUDES@
