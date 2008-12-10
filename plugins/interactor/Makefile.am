if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = libParallelCoordsElementShowInfos-@VERSION@.la \
					  libParallelCoordsElementsSelector-@VERSION@.la \
					  libParallelCoordsElementDeleter-@VERSION@.la \
					  libParallelCoordsElementHighlighter-@VERSION@.la \
					  libParallelCoordsAxisSwapper-@VERSION@.la \
					  libParallelCoordsAxisSliders-@VERSION@.la \
					  libParallelCoordsAxisBoxPlot-@VERSION@.la 

libParallelCoordsElementShowInfos_@VERSION@_la_SOURCES = ParallelCoordsElementShowInfos.cpp 

libParallelCoordsElementsSelector_@VERSION@_la_SOURCES = ParallelCoordsElementsSelector.cpp \
							 ParallelCoordsGlEntitiesSelector.h \ 
							 ParallelCoordsGlEntitiesSelector.cpp

libParallelCoordsElementDeleter_@VERSION@_la_SOURCES = ParallelCoordsElementDeleter.cpp 	

libParallelCoordsElementHighlighter_@VERSION@_la_SOURCES = ParallelCoordsElementHighlighter.cpp \
							   ParallelCoordsGlEntitiesSelector.h \ 
							   ParallelCoordsGlEntitiesSelector.cpp
							    																						
libParallelCoordsAxisSwapper_@VERSION@_la_SOURCES = ParallelCoordsAxisSwapper.cpp 

libParallelCoordsAxisSliders_@VERSION@_la_SOURCES = ParallelCoordsAxisSliders.cpp 

libParallelCoordsAxisBoxPlot_@VERSION@_la_SOURCES = ParallelCoordsAxisBoxPlot.cpp

pluginsdir = $(libdir)/tlp/interactors

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -L../view/parallel -lParallelCoordinates-@VERSION@ @QT_LDFLAGS@ @LIB_QT@

INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@ @QT_INCLUDES@ -I/usr/include/libxml2
