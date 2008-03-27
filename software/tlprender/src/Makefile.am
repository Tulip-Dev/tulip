bin_PROGRAMS = @CONFIGURE_tlprender@
EXTRA_PROGRAMS = tlprender
INCLUDES = -I$(srcdir)../include @GL_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@

#tlprender_LDFLAGS = -L. $(AM_LDFLAGS)
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
