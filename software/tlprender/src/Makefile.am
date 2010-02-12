if WIN32
ADDLDFLAGS = -lfreetype -lxml2
else
ADDLDFLAGS = `freetype-config --libs` `xml2-config --libs`
endif

EXTRA_DIST = CMakeLists.txt

bin_PROGRAMS = tlprender
EXTRA_PROGRAMS = tlprender
INCLUDES = -I$(srcdir)../include @GL_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@

tlprender_LDFLAGS =  $(top_builddir)/library/tulip-ogl/src/libtulip-ogl.la  @FTGL_LDFLAGS@ @LIBGLE_LDFLAGS@ `ls $(top_builddir)/thirdparty/libgpugraph/*.lo` @TULIP_LDFLAGS@ @GL_LDFLAGS@ -lOSMesa @LIB_GL@ ${ADDLDFLAGS} @LIB_IMAGES_LDFLAGS@

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
	GlHTMLFeedBackBuilder.h \
	GlHTMLFeedBackBuilder.cpp \
	ImageWriter.h \
	ImageWriter.cpp $(PNG) $(JPEG)
