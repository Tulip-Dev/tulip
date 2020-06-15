SET(VERSION_SERIES 2)
SET(VERSION_MAJOR 3)
SET(VERSION_MINOR 0)
SET(FTGL_SOVERSION 1)

ADD_DEFINITIONS(-DPACKAGE_VERSION="${VERSION_SERIES}.${VERSION_MAJOR}.${VERSION_MINOR}")

IF(WIN32)
  ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
  IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFTGL_LIBRARY")
  ENDIF()
ENDIF(WIN32)

SET(ftgl_SRCS
    FTBuffer.cpp
    FTCharmap.cpp
    FTCharmap.h
    FTCharToGlyphIndexMap.h
    FTCleanup.cpp
    FTCleanup.h
    FTContour.cpp
    FTContour.h
    FTFace.cpp
    FTFace.h
    FTGL.cpp
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

SET(ftgl_headers
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
    FTGL/FTGLTriangleExtractorFont.h
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
    FTGlyph/FTTriangleExtractorGlyph.cpp
    FTGlyph/FTTriangleExtractorGlyphImpl.h
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
    FTFont/FTTriangleExtractorFont.cpp
    FTFont/FTTriangleExtractorFontImpl.h
)

SET(ftlayout_SRCS
    FTLayout/FTLayout.cpp
    FTLayout/FTLayoutImpl.h
    FTLayout/FTLayoutGlue.cpp
    FTLayout/FTSimpleLayout.cpp
    FTLayout/FTSimpleLayoutImpl.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${Tess2Include} ${FREETYPE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})

ADD_LIBRARY(${FTGLLibrary} STATIC ${ftgl_SRCS} ${ftgl_headers_SRCS} ${ftglyph_SRCS} ${ftfont_SRCS} ${ftlayout_SRCS} $<TARGET_OBJECTS:${Tess2Library}>)
# allow the object files to be used to build shared libs
SET_TARGET_PROPERTIES(${FTGLLibrary} PROPERTIES POSITION_INDEPENDENT_CODE ON)
TARGET_LINK_LIBRARIES(${FTGLLibrary} ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARY} ${GLEW_LIBRARY})
# Prevent link issues with MSVC when freetype is dynamically linked
# against zlib and libpng
IF(MSVC)
  TARGET_LINK_LIBRARIES(${FTGLLibrary} ${ZLIB_LIBRARY} ${PNG_LIBRARY})
ENDIF(MSVC)
