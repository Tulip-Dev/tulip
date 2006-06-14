if WIN32
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
	libcube.la \
	libcubeoutlinedtransparent.la \
	libsphere.la \
	libcone.la \
	libsquare.la \
	libtriangle.la \
	libring.la \
	libcircle.la \
	libpentagone.la \
	libhexagone.la \
	libbillboard.la \
	libcylinder.la \
	libhalfcylinder.la \
	libsquarebordertextured.la
#	libsquareborder.la

pluginsdir = $(libdir)/tlp/plugins/glyph

libcircle_la_SOURCES = circle.cpp
libhexagone_la_SOURCES = hexagone.cpp
libtriangle_la_SOURCES = triangle.cpp
libpentagone_la_SOURCES = pentagone.cpp
libsquare_la_SOURCES = square.cpp
libring_la_SOURCES = ring.cpp
libbillboard_la_SOURCES = billboard.cpp
libcube_la_SOURCES = cube.cpp 
libcubeoutlinedtransparent_la_SOURCES = cubeoutlinedtransparent.cpp 
libsphere_la_SOURCES = sphere.cpp 
libcone_la_SOURCES = cone.cpp 
libcylinder_la_SOURCES = cylinder.cpp 
libhalfcylinder_la_SOURCES = halfcylinder.cpp 

libsquarebordertextured_la_SOURCES = squarebordertextured.cpp
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @LIB_GL@ @GL_LDFLAGS@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@
