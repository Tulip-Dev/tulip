SET (FTGL_MAJOR 2)
SET (FTGL_MINOR 1)

IF(MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFTGL_LIBRARY")
ENDIF()

SET(ftgl_SRCS
    FTBuffer.cpp 
    FTCharmap.cpp 
    FTCharmap.h 
    FTCharToGlyphIndexMap.h 
    FTContour.cpp 
    FTContour.h 
    FTFace.cpp 
    FTFace.h 
    FTGlyphContainer.cpp 
    FTGlyphContainer.h 
    FTInternals.h 
    FTLibrary.cpp 
    FTLibrary.h 
    FTList.h 
    FTPoint.cpp 
    FTSize.cpp 
    FTSize.h 
    FTVector.h 
    FTVectoriser.cpp 
    FTVectoriser.h 
    FTUnicode.h 
)

SET(ftgl_headers_SRCS
    FTGL/ftgl.h 
    FTGL/FTBBox.h 
    FTGL/FTBuffer.h 
    FTGL/FTPoint.h 
    FTGL/FTGlyph.h 
    FTGL/FTBitmapGlyph.h 
    FTGL/FTBufferGlyph.h 
    FTGL/FTExtrdGlyph.h 
    FTGL/FTOutlineGlyph.h 
    FTGL/FTPixmapGlyph.h 
    FTGL/FTPolyGlyph.h 
    FTGL/FTTextureGlyph.h 
    FTGL/FTFont.h 
    FTGL/FTGLBitmapFont.h 
    FTGL/FTBufferFont.h 
    FTGL/FTGLExtrdFont.h 
    FTGL/FTGLOutlineFont.h 
    FTGL/FTGLPixmapFont.h 
    FTGL/FTGLPolygonFont.h 
    FTGL/FTGLTextureFont.h 
    FTGL/FTLayout.h 
    FTGL/FTSimpleLayout.h
)

SET(ftglyph_SRCS
    FTGlyph/FTGlyph.cpp 
    FTGlyph/FTGlyphImpl.h 
    FTGlyph/FTGlyphGlue.cpp 
    FTGlyph/FTBitmapGlyph.cpp 
    FTGlyph/FTBitmapGlyphImpl.h 
    FTGlyph/FTBufferGlyph.cpp 
    FTGlyph/FTBufferGlyphImpl.h 
    FTGlyph/FTExtrudeGlyph.cpp 
    FTGlyph/FTExtrudeGlyphImpl.h 
    FTGlyph/FTOutlineGlyph.cpp 
    FTGlyph/FTOutlineGlyphImpl.h 
    FTGlyph/FTPixmapGlyph.cpp 
    FTGlyph/FTPixmapGlyphImpl.h 
    FTGlyph/FTPolygonGlyph.cpp 
    FTGlyph/FTPolygonGlyphImpl.h 
    FTGlyph/FTTextureGlyph.cpp 
    FTGlyph/FTTextureGlyphImpl.h 
)

SET(ftfont_SRCS
    FTFont/FTFont.cpp 
    FTFont/FTFontImpl.h 
    FTFont/FTFontGlue.cpp 
    FTFont/FTBitmapFont.cpp 
    FTFont/FTBitmapFontImpl.h 
    FTFont/FTBufferFont.cpp 
    FTFont/FTBufferFontImpl.h 
    FTFont/FTExtrudeFont.cpp 
    FTFont/FTExtrudeFontImpl.h 
    FTFont/FTOutlineFont.cpp 
    FTFont/FTOutlineFontImpl.h 
    FTFont/FTPixmapFont.cpp 
    FTFont/FTPixmapFontImpl.h 
    FTFont/FTPolygonFont.cpp 
    FTFont/FTPolygonFontImpl.h 
    FTFont/FTTextureFont.cpp 
    FTFont/FTTextureFontImpl.h 
)

SET(ftlayout_SRCS
    FTLayout/FTLayout.cpp 
    FTLayout/FTLayoutImpl.h 
    FTLayout/FTLayoutGlue.cpp 
    FTLayout/FTSimpleLayout.cpp 
    FTLayout/FTSimpleLayoutImpl.h 
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})

ADD_LIBRARY(${FTGLLibrary} SHARED ${ftgl_SRCS} ${ftgl_headers_SRCS} ${ftglyph_SRCS} ${ftfont_SRCS} ${ftlayout_SRCS})
TARGET_LINK_LIBRARIES(${FTGLLibrary} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${FREETYPE_LIBRARY})
SET_TARGET_PROPERTIES(${FTGLLibrary} PROPERTIES
  SOVERSION ${FTGL_MAJOR}
  VERSION ${FTGL_MAJOR}.${FTGL_MINOR})

INSTALL(TARGETS ${FTGLLibrary}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT ftgl
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT ftgl
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT ftgl)
