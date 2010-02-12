noinst_LTLIBRARIES = libftgl.la

if WIN32
FREETYPE_INCLUDE = -I/mingw/include/freetype2
FREETYPELDFLAGS = -lfreetype
else
FREETYPE_INCLUDE = `freetype-config --cflags`
FREETYPELDFLAGS = `freetype-config --libs`
endif 

EXTRA_DIST = AUTHORS BUGS COPYING INSTALL NEWS README CMakeLists.txt

libftgl_la_SOURCES = \
    FTBuffer.cpp \
    FTCharmap.cpp \
    FTCharmap.h \
    FTCharToGlyphIndexMap.h \
    FTContour.cpp \
    FTContour.h \
    FTFace.cpp \
    FTFace.h \
    FTGlyphContainer.cpp \
    FTGlyphContainer.h \
    FTInternals.h \
    FTLibrary.cpp \
    FTLibrary.h \
    FTList.h \
    FTPoint.cpp \
    FTSize.cpp \
    FTSize.h \
    FTVector.h \
    FTVectoriser.cpp \
    FTVectoriser.h \
    FTUnicode.h \
    $(ftglyph_sources) \
    $(ftfont_sources) \
    $(ftlayout_sources) \
    $(ftgl_headers) \
    $(NULL)

INCLUDES = @GL_INCLUDES@ ${FREETYPE_INCLUDE}
AM_LDFLAGS = ${FREETYPELDFLAGS} @GL_LDFLAGS@ @LIB_GL@

ftgl_headers = config.h \
    FTGL/ftgl.h \
    FTGL/FTBBox.h \
    FTGL/FTBuffer.h \
    FTGL/FTPoint.h \
    FTGL/FTGlyph.h \
    FTGL/FTBitmapGlyph.h \
    FTGL/FTBufferGlyph.h \
    FTGL/FTExtrdGlyph.h \
    FTGL/FTOutlineGlyph.h \
    FTGL/FTPixmapGlyph.h \
    FTGL/FTPolyGlyph.h \
    FTGL/FTTextureGlyph.h \
    FTGL/FTFont.h \
    FTGL/FTGLBitmapFont.h \
    FTGL/FTBufferFont.h \
    FTGL/FTGLExtrdFont.h \
    FTGL/FTGLOutlineFont.h \
    FTGL/FTGLPixmapFont.h \
    FTGL/FTGLPolygonFont.h \
    FTGL/FTGLTextureFont.h \
    FTGL/FTLayout.h \
    FTGL/FTSimpleLayout.h \
    ${NULL}

ftglyph_sources = \
    FTGlyph/FTGlyph.cpp \
    FTGlyph/FTGlyphImpl.h \
    FTGlyph/FTGlyphGlue.cpp \
    FTGlyph/FTBitmapGlyph.cpp \
    FTGlyph/FTBitmapGlyphImpl.h \
    FTGlyph/FTBufferGlyph.cpp \
    FTGlyph/FTBufferGlyphImpl.h \
    FTGlyph/FTExtrudeGlyph.cpp \
    FTGlyph/FTExtrudeGlyphImpl.h \
    FTGlyph/FTOutlineGlyph.cpp \
    FTGlyph/FTOutlineGlyphImpl.h \
    FTGlyph/FTPixmapGlyph.cpp \
    FTGlyph/FTPixmapGlyphImpl.h \
    FTGlyph/FTPolygonGlyph.cpp \
    FTGlyph/FTPolygonGlyphImpl.h \
    FTGlyph/FTTextureGlyph.cpp \
    FTGlyph/FTTextureGlyphImpl.h \
    $(NULL)

ftfont_sources = \
    FTFont/FTFont.cpp \
    FTFont/FTFontImpl.h \
    FTFont/FTFontGlue.cpp \
    FTFont/FTBitmapFont.cpp \
    FTFont/FTBitmapFontImpl.h \
    FTFont/FTBufferFont.cpp \
    FTFont/FTBufferFontImpl.h \
    FTFont/FTExtrudeFont.cpp \
    FTFont/FTExtrudeFontImpl.h \
    FTFont/FTOutlineFont.cpp \
    FTFont/FTOutlineFontImpl.h \
    FTFont/FTPixmapFont.cpp \
    FTFont/FTPixmapFontImpl.h \
    FTFont/FTPolygonFont.cpp \
    FTFont/FTPolygonFontImpl.h \
    FTFont/FTTextureFont.cpp \
    FTFont/FTTextureFontImpl.h \
    $(NULL)

ftlayout_sources = \
    FTLayout/FTLayout.cpp \
    FTLayout/FTLayoutImpl.h \
    FTLayout/FTLayoutGlue.cpp \
    FTLayout/FTSimpleLayout.cpp \
    FTLayout/FTSimpleLayoutImpl.h \
    $(NULL)

NULL =
