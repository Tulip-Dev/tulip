if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = libParallelCoordinates-@VERSION@.la

pluginsdir = $(libdir)/tlp/view

libParallelCoordinates_@VERSION@_la_SOURCES = \
			ParallelCoordinatesView.h \
			ParallelCoordinatesDrawing.h \
			ParallelCoordinatesGraphProxy.h \
			ParallelAxis.h \
			ParallelTools.h \
			ParallelCoordinatesConfigDialog.h \
			ItemsDialogList.h \
			NominalParallelAxis.h \
			QuantitativeParallelAxis.h \
			GlNodeGlyph.h \
			GlPolyQuad.h \
			AxisConfigDialogs.h \
			ParallelCoordinatesView.cpp \
			ParallelCoordinatesDrawing.cpp \
			ParallelCoordinatesGraphProxy.cpp \
			ParallelAxis.cpp \
			ParallelCoordinatesConfigDialog.cpp \
			ItemsDialogList.cpp \
			NominalParallelAxis.cpp \
			GlNodeGlyph.cpp \
			GlPolyQuad.cpp \
			ParallelResource.cpp \
			AxisConfigDialogs.cpp  \
			QuantitativeParallelAxis.cpp \
			Thread.h \
			Thread.cpp \
			GlProgressBar.h \
			GlProgressBar.cpp 

nodist_libParallelCoordinates_@VERSION@_la_SOURCES = \
			moc_ParallelCoordinatesView.cpp \
			moc_ParallelCoordinatesConfigDialog.cpp \
			moc_ItemsDialogList.cpp \
			moc_AxisConfigDialogs.cpp

BUILT_SOURCES = ParallelCoordinatesConfigDialogData.h 

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

dist_noinst_DATA = ParallelResource.qrc \
				   ParallelCoordinatesConfigDialog.ui	

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_OGL_LDFLAGS@
AM_CPPFLAGS= @QT_CPPFLAGS@ 
INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@ @QT_INCLUDES@  -I/usr/include/libxml2

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

ParallelResource.cpp: ParallelResource.qrc
	$(RCC) -name ParallelView -o $@ $<

clean-generic:
	rm -f moc_* *Data.*

