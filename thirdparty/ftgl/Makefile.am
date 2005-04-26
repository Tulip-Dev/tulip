noinst_LTLIBRARIES = libftgl.la

if WIN32
FREETYPE_INCLUDE = -I/mingw/include/freetype2
FREETYPELDFLAGS = -lfreetype
else
FREETYPE_INCLUDE = `freetype-config --cflags`
FREETYPELDFLAGS = `freetype-config --libs`
endif 

AM_CXXFLAGS = -fPIC
INCLUDES = @GL_INCLUDES@ ${FREETYPE_INCLUDE}
AM_LDFLAGS = ${FREETYPELDFLAGS} @GL_LDFLAGS@ @LIB_GL@ 
			   
EXTRA_DIST = 	COPYING.txt \
		README.txt \
		TODO.txt \
		license.txt \
		HISTORY.txt

libftgl_la_SOURCES = \
	FTBBox.h \
	FTBitmapGlyph.h \
	FTCharmap.h \
	FTCharToGlyphIndexMap.h \
	FTContour.h \
	FTExtrdGlyph.h \
	FTFace.h \
	FTFont.h \
	FTGLBitmapFont.h \
	FTGLExtrdFont.h \
	FTGL.h \
	FTGLOutlineFont.h \
	FTGLPixmapFont.h \
	FTGLPolygonFont.h \
	FTGLTextureFont.h \
	FTGlyphContainer.h \
	FTGlyph.h \
	FTLibrary.h \
	FTList.h \
	FTOutlineGlyph.h \
	FTPixmapGlyph.h \
	FTPoint.h \
	FTPolyGlyph.h \
	FTSize.h \
	FTTextureGlyph.h \
	FTVector.h \
	FTVectoriser.h \
	FTBitmapGlyph.cpp	\
	FTCharmap.cpp		\
	FTContour.cpp		\
	FTExtrdGlyph.cpp	\
	FTFace.cpp		\
	FTFont.cpp		\
	FTGLBitmapFont.cpp	\
	FTGLExtrdFont.cpp	\
	FTGLOutlineFont.cpp	\
	FTGLPixmapFont.cpp	\
	FTGLPolygonFont.cpp	\
	FTGLTextureFont.cpp	\
	FTGlyph.cpp		\
	FTGlyphContainer.cpp	\
	FTLibrary.cpp		\
	FTOutlineGlyph.cpp	\
	FTPixmapGlyph.cpp	\
	FTPoint.cpp		\
	FTPolyGlyph.cpp		\
	FTSize.cpp		\
	FTTextureGlyph.cpp	\
	FTVectoriser.cpp
