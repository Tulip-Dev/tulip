bin_PROGRAMS = @CONFIGURE_tlprender@
EXTRA_PROGRAMS = tlprender
INCLUDES = -I$(srcdir)../include @GL_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
AM_LDFLAGS = `xml2-config --libs` @LIB_GL@ @GL_LDFLAGS@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @LIB_IMAGES_LDFLAGS@

tlprender_LDFLAGS = -L.
tlprender_LDADD = -lOSMesa

if PNG
PNG = PNGWriter.h PNGWriter.cpp
else
PNG = 
endif
if JPEG
JPEG = JPEGWriter.h JPEGWriter.cpp
else
JPEG =
endif

tlprender_SOURCES = \
	tlprender.cpp \
	Shape.h \
	Shape.cpp \
	ImageWriter.h \
	ImageWriter.cpp $(PNG) $(JPEG)
