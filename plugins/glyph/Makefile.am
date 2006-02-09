if WIN32
DLL_FLAGS = -no-undefined
endif

if CYGWIN
DLL_FLAGS = -no-undefined
endif

plugins_LTLIBRARIES = \
	libcube.la \
	libcubeoutlined.la \
	libsphere.la \
	libcone.la \
	libsquare.la \
	libbillboard.la \
	libteapot.la \
	libcylinder.la \
	libhalfcylinder.la \
	libsquarebordertextured.la
#	libsquareborder.la

pluginsdir = $(libdir)/tlp/plugins/glyph

libsquare_la_SOURCES = square.cpp
libbillboard_la_SOURCES = billboard.cpp
libcube_la_SOURCES = cube.cpp 
libcubeoutlined_la_SOURCES = cubeoutlined.cpp 
libsphere_la_SOURCES = sphere.cpp 
libcone_la_SOURCES = cone.cpp 
libteapot_la_SOURCES = teapot.cpp 
libcylinder_la_SOURCES = cylinder.cpp 
libhalfcylinder_la_SOURCES = halfcylinder.cpp 
#libsquareborder_la_SOURCES = squareborder.cpp
libsquarebordertextured_la_SOURCES = squarebordertextured.cpp
AM_LDFLAGS = ${DLL_FLAGS} -module -avoid-version @LIB_GL@ @GL_LDFLAGS@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@
