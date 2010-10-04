if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

EXTRA_DIST = CMakeLists.txt

if HAVE_TULIP_QT
plugins_LTLIBRARIES = libColorMapping-@VERSION@.la

pluginsdir = $(libdir)/tlp

libColorMapping_@VERSION@_la_SOURCES = ColorMapping.cpp DoubleStringsListRelationDialog.cpp
nodist_libColorMapping_@VERSION@_la_SOURCES = moc_DoubleStringsListRelationDialog.cpp

BUILT_SOURCES = DoubleStringsListRelationDialogData.h

dist_noinst_DATA = DoubleStringsListRelationDialog.ui

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @QT_INCLUDES@ @GL_INCLUDES@
AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ 
libColorMapping_@VERSION@_la_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@


moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_* *Data.*
endif
