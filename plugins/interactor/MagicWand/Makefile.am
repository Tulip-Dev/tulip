if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

EXTRA_DIST = CMakeLists.txt

plugins_LTLIBRARIES = libInteractorMagicSelection-@VERSION@.la

pluginsdir = $(libdir)/tlp/interactor

libInteractorMagicSelection_@VERSION@_la_SOURCES = \
	InteractorMagicSelection.cpp \
	MagicSelectionInteractorConfigWidget.h \
	MagicSelectionInteractorConfigWidget.cpp \
    	MouseMagicWandSelector.cpp

nodist_libInteractorMagicSelection_@VERSION@_la_SOURCES = \
	moc_MagicSelectionInteractorConfigWidget.cpp

BUILT_SOURCES = MagicSelectionInteractorConfigWidgetData.h
 
dist_noinst_DATA = MagicSelectionInteractorConfigWidget.ui

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @QT_INCLUDES@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_* *Data.*


