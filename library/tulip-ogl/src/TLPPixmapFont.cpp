#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
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


FTGlyph* TLPPixmapFont::MakeGlyph(FT_GlyphSlot ftGlyph) {
  if( ftGlyph) {
    TLPPixmapGlyph* tempGlyph = new TLPPixmapGlyph(ftGlyph);
    return tempGlyph;
  }
  return NULL;
}

#include <iostream>
void TLPPixmapFont::Render( const char* string) {   
  glPushAttrib( GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
  glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);

  glEnable(GL_BLEND);
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable( GL_TEXTURE_2D);

  GLfloat ftglColour[4];
  glGetFloatv( GL_CURRENT_RASTER_COLOR, ftglColour);
  glPixelTransferf(GL_RED_SCALE, ftglColour[0]);
  glPixelTransferf(GL_GREEN_SCALE, ftglColour[1]);
  glPixelTransferf(GL_BLUE_SCALE, ftglColour[2]);
  glPixelTransferf(GL_ALPHA_SCALE, ftglColour[3]);

  FTFont::Render( string);

  glPopClientAttrib();
  glPopAttrib();
}


void TLPPixmapFont::Render( const wchar_t* string) {   
  glPushAttrib( GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
  glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);

  glEnable(GL_BLEND);
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable( GL_TEXTURE_2D);

  GLfloat ftglColour[4];
  glGetFloatv( GL_CURRENT_RASTER_COLOR, ftglColour);
  ftglColour[3] = 1.0;  
  glPixelTransferf(GL_RED_SCALE, ftglColour[0]);
  glPixelTransferf(GL_GREEN_SCALE, ftglColour[1]);
  glPixelTransferf(GL_BLUE_SCALE, ftglColour[2]);
  glPixelTransferf(GL_ALPHA_SCALE, ftglColour[3]);

  FTFont::Render( string);

  glPopClientAttrib();
  glPopAttrib();

}


