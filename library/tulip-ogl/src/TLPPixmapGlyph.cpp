#include    <FTBitmapGlyph.h>
#include    "tulip/TLPPixmapGlyph.h"

unsigned char outlineFont(unsigned x,unsigned y, unsigned srcWidth, 
		      unsigned srcHeight, unsigned char *src) {
  if (src[x+y*srcWidth]!=0) return src[x+y*srcWidth];
  if (x<srcWidth-1 && src[(x+1)+y*srcWidth]>0) return 128;
  if (x>0 && src[(x-1)+y*srcWidth]>0) return 128;
  if (y<srcHeight-1 && src[x+(y+1)*srcWidth]>0) return 128;
  if (y>0 && src[x+(y-1)*srcWidth]>0) return 128;
  return 0;
}

unsigned char outlineFont2(unsigned x,unsigned y, unsigned srcWidth, 
		      unsigned srcHeight, unsigned char *src) {
  if (src[x+y*srcWidth]==0) return 0;
  if (x==srcWidth-1) return 128;
  if (x==0) return 128;
  if (y==srcHeight-1) return 128;
  if (y==0) return 128;
  if (src[(x-1)+y*srcWidth]==0) return 128;
  if (src[(x+1)+y*srcWidth]==0) return 128;
  if (src[x+(y+1)*srcWidth]==0) return 128;
  if (src[x+(y-1)*srcWidth]==0) return 128;
  return 255;
}

TLPPixmapGlyph::TLPPixmapGlyph(FT_GlyphSlot glyphSlot)
  :FTGlyph(glyphSlot),
   destWidth(0),
   destHeight(0),
   data(0) {
  FT_Glyph glyph;
  FT_Get_Glyph(glyphSlot, &glyph);
  // This function will always fail if the glyph's format isn't scalable????
  err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
  if( err || ft_glyph_format_bitmap != glyph->format)
    {
      return;
    }

  FT_BitmapGlyph  bitmap = (FT_BitmapGlyph)glyph;
  FT_Bitmap*      source = &bitmap->bitmap;

  //check the pixel mode
  //ft_pixel_mode_grays
        
  int srcWidth = source->width;
  int srcHeight = source->rows;

  // FIXME What about dest alignment?
  //    destWidth = srcWidth;
  //    destHeight = srcHeight;

  destWidth = srcWidth+4;
  destHeight = srcHeight+4;    
    
  if( destWidth && destHeight)
    {

      data = new unsigned char[destWidth * destHeight * 4];
    
      // Get the current glColor.
      float ftglColour[4];
      glGetFloatv( GL_CURRENT_COLOR, ftglColour);

      unsigned char redComponent =   static_cast<unsigned char>( ftglColour[0] * 255.0f);
      unsigned char greenComponent = static_cast<unsigned char>( ftglColour[1] * 255.0f);
      unsigned char blueComponent =  static_cast<unsigned char>( ftglColour[2] * 255.0f);

      unsigned char* tmp = source->buffer;

      unsigned char* src  = new unsigned char[(destWidth)*(destHeight)];
      unsigned char* src1 = new unsigned char[(destWidth)*(destHeight)];

      for (unsigned i=0;i<(destWidth)*(destHeight);++i) src1[i]=0;

      for( int y = 0; y < srcHeight; ++y) 
	for( int x = 0; x < srcWidth; ++x)
	  src1[(x+2)+(y+2)*destWidth]=tmp[x+y*srcWidth];

      srcWidth+=4;
      srcHeight+=4;
      for( int y = 0; y < srcHeight; ++y) 
	for( int x = 0; x < srcWidth; ++x)
	  src[x+y*srcWidth]=outlineFont(x,y,srcWidth,srcHeight,src1);

      unsigned char* dest = data + ((destHeight - 1) * destWidth) * 4;
      size_t destStep = destWidth * 4 * 2;
       
      for( int y = 0; y < srcHeight; ++y)
	{
	  for( int x = 0; x < srcWidth; ++x)
	    {
	      unsigned char newVal=outlineFont(x,y,srcWidth,srcHeight,src);
	      if (newVal==128) {
		*dest++ = 255;
		*dest++ = 255;
		*dest++ = 255;
		*dest++ = 255;
	      } else
		if (newVal!=0) {
		  *dest++ = redComponent;
		  *dest++ = greenComponent;
		  *dest++ = blueComponent;
		  *dest++ = 255;
		} else
		  {
		    *dest++ = 255;
		    *dest++ = 255;
		    *dest++ = 255;
		    *dest++ = 0;
		  }
	    }
	  dest -= destStep;
	}
      delete [] src;
      delete [] src1;
      destHeight = srcHeight;
    }
  pos.X(bitmap->left - 2);
  pos.Y(srcHeight - bitmap->top - 2);
  bBox.lowerX-=2;
  bBox.lowerY-=2;
  bBox.upperX+=2;
  bBox.upperY+=2;
  advance += FTPoint(4, 4, 0); //advance+=4;
  // Is this the right place to do this?
  FT_Done_Glyph( glyph );
}


TLPPixmapGlyph::~TLPPixmapGlyph()
{
    delete [] data;
}


FTPoint& TLPPixmapGlyph::Render( const FTPoint& pen)
{
    if( data)
    {
        // Move the glyph origin
        glBitmap( 0, 0, 0.0, 0.0, pen.X() + pos.X(), pen.Y() - pos.Y(), (const GLubyte*)0);

        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);

        glDrawPixels( destWidth, destHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)data);
        
        // Restore the glyph origin
        glBitmap( 0, 0, 0.0, 0.0, -pen.X() - pos.X(), -pen.Y() + pos.Y(), (const GLubyte*)0);
    }

    return advance;
}
