if WIN32
ADDLDFLAGS = -lfreetype -lxml2
else
ADDLDFLAGS = `freetype-config --libs` `xml2-config --libs`
endif

bin_PROGRAMS = @CONFIGURE_tlprender@
EXTRA_PROGRAMS = tlprender
INCLUDES = -I$(srcdir)../include @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@

tlprender_LDFLAGS = @FTGL_LDFLAGS@ @LIBGLE_LDFLAGS@ `ls $(top_builddir)/thirdparty/libgpugraph/*.lo` @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @GL_LDFLAGS@ @LIB_GL@  @LIB_QT@ ${ADDLDFLAGS} @LIB_IMAGES_LDFLAGS@

tlprender_SOURCES = \
	tlprender.cpp
