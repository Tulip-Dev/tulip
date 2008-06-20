if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = libParallelCoordinates-@VERSION@.la

pluginsdir = $(libdir)/tlp/view

libParallelCoordinates_@VERSION@_la_SOURCES = \
			ParallelView.h \
			ParallelCoordinatesWidget.h \
			ParallelCoordinatesView.h \
			ParallelCoordinatesGraphProxy.h \
			ParallelAxis.h \
			ParallelTools.h \
			ViewConfigDialog.h \
			ItemsDialogList.h \
			NominalParallelAxis.h \
			QuantitativeParallelAxis.h \
			GlNodeGlyph.h \
			GlCylinder.h \
			ParallelView.cpp \
			ParallelCoordinatesWidget.cpp \
			ParallelCoordinatesView.cpp \
			ParallelCoordinatesGraphProxy.cpp \
			ParallelAxis.cpp \
			ParallelTools.cpp \
			ViewConfigDialog.cpp \
			ItemsDialogList.cpp \
			NominalParallelAxis.cpp \
			GlNodeGlyph.cpp \
			GlCylinder.cpp

nodist_libParallelCoordinates_@VERSION@_la_SOURCES = \
			moc_ParallelViewData.cpp \
			moc_ParallelView.cpp \
			moc_ParallelCoordinatesWidget.cpp \
			moc_ParallelCoordinatesView.cpp \
			moc_ParallelCoordinatesGraphProxy.cpp \
			moc_ParallelAxis.cpp \
			moc_ParallelTools.cpp \
			moc_ViewConfigDialog.cpp \
			moc_ItemsDialogList.cpp \
			moc_NominalParallelAxis.cpp 

BUILT_SOURCES = \
			ParallelViewData.h

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_OGL_LDFLAGS@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_* *Data.*

