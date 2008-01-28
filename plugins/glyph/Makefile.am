if WIN32
DLL_FLAGS = -no-undefined
DLL_TULIP_GL = -DDLL_TULIP_GL
ADDLDFLAGS = -lxml2 
else
ADDLDFLAGS = `xml2-config --libs`
ADD_INCLUDES = `xml2-config --cflags` 
if MACOSX
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif
endif

plugins_LTLIBRARIES = \
	libCube-@VERSION@.la \
	libCubeOutlinedTransparent-@VERSION@.la \
	libSphere-@VERSION@.la \
	libCone-@VERSION@.la \
	libSquare-@VERSION@.la \
	libTriangle-@VERSION@.la \
	libRing-@VERSION@.la \
	libCircle-@VERSION@.la \
	libPentagone-@VERSION@.la \
	libHexagone-@VERSION@.la \
	libBillboard-@VERSION@.la \
	libCylinder-@VERSION@.la \
	libHalfCylinder-@VERSION@.la \
	libSquareBorderTextured-@VERSION@.la
#	libSquareBorder-@VERSION@.la

pluginsdir = $(libdir)/tlp/glyphs

libCircle_@VERSION@_la_SOURCES = circle.cpp
libHexagone_@VERSION@_la_SOURCES = hexagone.cpp
libTriangle_@VERSION@_la_SOURCES = triangle.cpp
libPentagone_@VERSION@_la_SOURCES = pentagone.cpp
libSquare_@VERSION@_la_SOURCES = square.cpp
libRing_@VERSION@_la_SOURCES = ring.cpp
libBillboard_@VERSION@_la_SOURCES = billboard.cpp
libCube_@VERSION@_la_SOURCES = cube.cpp 
libCubeOutlinedTransparent_@VERSION@_la_SOURCES = cubeoutlinedtransparent.cpp 
libSphere_@VERSION@_la_SOURCES = sphere.cpp 
libCone_@VERSION@_la_SOURCES = cone.cpp 
libCylinder_@VERSION@_la_SOURCES = cylinder.cpp 
libHalfCylinder_@VERSION@_la_SOURCES = halfcylinder.cpp 
libSquareBorderTextured_@VERSION@_la_SOURCES = squarebordertextured.cpp

AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @LIB_GL@ @GL_LDFLAGS@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
INCLUDES = $(ADD_INCLUDES) @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@
