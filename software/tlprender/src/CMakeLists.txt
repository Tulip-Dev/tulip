
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR})


########### next target ###############

set(@CONFIGURE_tlprender@_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

kde4_add_executable(@CONFIGURE_tlprender@ ${@CONFIGURE_tlprender@_SRCS})

target_link_libraries(@CONFIGURE_tlprender@ ${KDE4_KDECORE_LIBS})

install(TARGETS @CONFIGURE_tlprender@ ${INSTALL_TARGETS_DEFAULT_ARGS} COMPONENT tlprender)


########### next target ###############

set(tlprender_SRCS
    tlprender.cpp
    Shape.h
    Shape.cpp
    GlHTMLFeedBackBuilder.h
    GlHTMLFeedBackBuilder.cpp
    ImageWriter.h
    ImageWriter.cpp
    $(PNG)
    $(JPEG))

if(KDE4_BUILD_TESTS)

kde4_add_executable(tlprender ${tlprender_SRCS})

target_link_libraries(tlprender ${KDE4_KDECORE_LIBS} OSMesa)

endif(KDE4_BUILD_TESTS)

########### install files ###############




#original Makefile.am contents follow:

#if WIN32
#ADDLDFLAGS = -lfreetype -lxml2
#else
#ADDLDFLAGS = `freetype-config --libs` `xml2-config --libs`
#endif
#
#bin_PROGRAMS = @CONFIGURE_tlprender@
#EXTRA_PROGRAMS = tlprender
#INCLUDES = -I$(srcdir)../include @GL_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
#
#tlprender_LDFLAGS =  $(top_builddir)/library/tulip-ogl/src/libtulip-ogl.la  @FTGL_LDFLAGS@ @LIBGLE_LDFLAGS@ `ls $(top_builddir)/thirdparty/libgpugraph/*.lo` @TULIP_LDFLAGS@ @GL_LDFLAGS@ -lOSMesa @LIB_GL@ ${ADDLDFLAGS} @LIB_IMAGES_LDFLAGS@
#
#if PNG
#PNG = PNGWriter.h PNGWriter.cpp
#else
#PNG = 
#endif
#if JPEG
#JPEG = JPEGWriter.h JPEGWriter.cpp
#else
#JPEG =
#endif
#
#tlprender_SOURCES = \
#	tlprender.cpp \
#	Shape.h \
#	Shape.cpp \
#	GlHTMLFeedBackBuilder.h \
#	GlHTMLFeedBackBuilder.cpp \
#	ImageWriter.h \
#	ImageWriter.cpp $(PNG) $(JPEG)
