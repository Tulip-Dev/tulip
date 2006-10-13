if WIN32
DLL_FLAGS = -no-undefined
DLL_TULIP_GL = -DDLL_TULIP_GL
ADDLDFLAGS = -lfreetype -lxml2 
ADD_INCLUDES = -I/mingw/include/freetype2
RELEASE = ${VERSION}
else
ADDLDFLAGS = `freetype-config --libs` `xml2-config --libs`
ADD_INCLUDES = `freetype-config --cflags` `xml2-config --cflags` 
RELEASE = @TULIP_VERSION@ -version-info 0:0:0
endif

lib_LTLIBRARIES = libtulip-ogl.la
INCLUDES = ${DLL_TULIP_GL} @TULIP_OGL_INCLUDE@ -D_REENTRANT @GL_INCLUDES@  @TULIP_INCLUDE@ @LIBGLE_INCLUDE@ @FTGL_INCLUDE@ ${ADD_INCLUDES}
# MAC PORT
if MACOSX
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @LIB_IMAGES_LDFLAGS@ `ls $(top_builddir)/thirdparty/ftgl/*.lo` `ls $(top_builddir)/thirdparty/gle/*.lo` ${ADDLDFLAGS}
else
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @LIB_IMAGES_LDFLAGS@ @FTGL_LDFLAGS@ ${ADDLDFLAGS}
endif

libtulip_ogl_la_LDFLAGS = -export-dynamic -release ${RELEASE}
if !MACOSX
libtulip_ogl_la_LIBADD = @LIBGLE_LDFLAGS@
endif

libtulip_ogl_la_SOURCES = \
	GlTools.cpp \
	Curves.cpp \
	Hline.cpp \
	Parser.cpp \
	Paragraph.cpp \
	Renderer.cpp \
        GlRenderer.cpp \
	TextRenderer.cpp \
	Table.cpp \
	Context.cpp \
	Document.cpp \
	ObservableGlGraph.cpp \
	Glyph.cpp \
	CubeOutLined.cpp \
	Camera.cpp \
        GlGraph.cpp \
        GlGraphSelection.cpp \
        GlGraphDrawGraph.cpp \
        GlGraphMouse.cpp \
	GlGraphNavigate.cpp \
        GlGraphMakeObject.cpp \
	GlGraphTextureProxy.cpp \
        GlGraphSetGet.cpp \
        GlGraphInit.cpp \
	GlAugmentedDisplay.cpp \
	GlADPoint.cpp \
	GlADAxisPoint.cpp \
	GlADLine.cpp \
	GlADQuad.cpp \
	GlADBox.cpp \
	GlADGrid.cpp \
	GlADRenderOptions.cpp \
	GlADComposite.cpp \
	GlHud.cpp \
	GlHudLine.cpp \
	GlHudRect.cpp \
	GlHudCircle.cpp \
	GlHudPolygon.cpp \
	Bezier.cpp \
	Spline.cpp \
        GlLines.cpp \
	TLPPixmapFont.cpp \
	TLPPixmapGlyph.cpp \
        EpsFunction.cpp

