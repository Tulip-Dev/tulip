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
			NominalParallelAxis.h \
			QuantitativeParallelAxis.h \
			GlNodeGlyph.h \
			AxisConfigDialogs.h \
			ParallelCoordinatesView.cpp \
			ParallelCoordinatesDrawing.cpp \
			ParallelCoordinatesGraphProxy.cpp \
			ParallelAxis.cpp \
			ParallelCoordinatesConfigDialog.cpp \
			NominalParallelAxis.cpp \
			GlNodeGlyph.cpp \
			AxisConfigDialogs.cpp  \
			QuantitativeParallelAxis.cpp \
			ParallelCoordsElementShowInfos.cpp \
			ParallelCoordsElementsSelector.cpp \
			ParallelCoordsGlEntitiesSelector.h \
			ParallelCoordsGlEntitiesSelector.cpp \
			ParallelCoordsElementDeleter.cpp \
			ParallelCoordsElementHighlighter.cpp \
			ParallelCoordsAxisSwapper.cpp \
			ParallelCoordsAxisSliders.cpp \
			ParallelCoordsAxisBoxPlot.cpp 

nodist_libParallelCoordinates_@VERSION@_la_SOURCES = \
			ParallelResource.cpp \
			moc_ParallelCoordinatesView.cpp \
			moc_ParallelCoordinatesConfigDialog.cpp \
			moc_AxisConfigDialogs.cpp
			

BUILT_SOURCES = ParallelCoordinatesConfigDialogData.h

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

dist_noinst_DATA = ParallelResource.qrc \
		   		   i_axis_boxplot.png i_axis_sliders.png \
		   		   i_axis_swapper.png i_element_highlighter.png \
		   		   ParallelCoordinatesConfigDialog.ui
		   	

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@  @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@ 
INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@ @QT_INCLUDES@  -I/usr/include/libxml2

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

ParallelResource.cpp: ParallelResource.qrc
	$(RCC) -name parallel -o $@ $<

clean-generic:
	rm -f moc_* *Data.*

