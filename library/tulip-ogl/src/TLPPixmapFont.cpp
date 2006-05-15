#include    "tulip/TLPPixmapGlyph.h"
#include    "tulip/TLPPixmapFont.h"

using namespace tlp;

TLPPixmapFont::TLPPixmapFont( const char* fontname)
:   FTFont( fontname)
{}


TLPPixmapFont::TLPPixmapFont( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
:   FTFont( pBufferBytes, bufferSizeInBytes)
{}


TLPPixmapFont::~TLPPixmapFont()
{}


FTGlyph* TLPPixmapFont::MakeGlyph( unsigned int g) {
    FT_GlyphSlot ftGlyph = face.Glyph( g, FT_LOAD_NO_HINTING);
    if( ftGlyph) {
        TLPPixmapGlyph* tempGlyph = new TLPPixmapGlyph(ftGlyph);
        return tempGlyph;
    }
    err = face.Error();
    return NULL;
}


void TLPPixmapFont::Render( const char* string) {   
    glPushAttrib( GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable( GL_TEXTURE_2D);
    FTFont::Render( string);
    glPopAttrib();
}


void TLPPixmapFont::Render( const wchar_t* string) {   
    glPushAttrib( GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable( GL_TEXTURE_2D);
    FTFont::Render( string);
    glPopClientAttrib();
    glPopAttrib();
}


