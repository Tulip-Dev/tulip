set(ftgl_STAT_SRCS
    FTBBox.h
    FTBitmapGlyph.h
    FTCharmap.h
    FTCharToGlyphIndexMap.h
    FTContour.h
    FTExtrdGlyph.h
    FTFace.h
    FTFont.h
    FTGLBitmapFont.h
    FTGLExtrdFont.h
    FTGL.h
    FTGLOutlineFont.h
    FTGLPixmapFont.h
    FTGLPolygonFont.h
    FTGLTextureFont.h
    FTGlyphContainer.h
    FTGlyph.h
    FTLibrary.h
    FTList.h
    FTOutlineGlyph.h
    FTPixmapGlyph.h
    FTPoint.h
    FTPolyGlyph.h
    FTSize.h
    FTTextureGlyph.h
    FTVector.h
    FTVectoriser.h
    FTBitmapGlyph.cpp
    FTCharmap.cpp
    FTContour.cpp
    FTExtrdGlyph.cpp
    FTFace.cpp
    FTFont.cpp
    FTGLBitmapFont.cpp
    FTGLExtrdFont.cpp
    FTGLOutlineFont.cpp
    FTGLPixmapFont.cpp
    FTGLPolygonFont.cpp
    FTGLTextureFont.cpp
    FTGlyph.cpp
    FTGlyphContainer.cpp
    FTLibrary.cpp
    FTOutlineGlyph.cpp
    FTPixmapGlyph.cpp
    FTPoint.cpp
    FTPolyGlyph.cpp
    FTSize.cpp
    FTTextureGlyph.cpp
    FTVectoriser.cpp)

INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIR_freetype2})

ADD_LIBRARY(ftgl STATIC ${ftgl_STAT_SRCS})

#			   
#EXTRA_DIST = 	COPYING.txt \
#		README.txt \
#		TODO.txt \
#		license.txt \
#		HISTORY.txt
#
