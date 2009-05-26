noinst_LTLIBRARIES = libftgl.la

if WIN32
FREETYPE_INCLUDE = -I/mingw/include/freetype2
FREETYPELDFLAGS = -lfreetype
else
FREETYPE_INCLUDE = `freetype-config --cflags`
FREETYPELDFLAGS = `freetype-config --libs`
endif 

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

libftgl_la_CPPFLAGS = @GL_INCLUDES@ ${FREETYPE_INCLUDE}
libftgl_la_LDFLAGS = ${FREETYPELDFLAGS} @GL_LDFLAGS@ @LIB_GL@

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
