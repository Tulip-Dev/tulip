#include    "FTPixmapGlyph.h"

FTPixmapGlyph::FTPixmapGlyph( FT_Glyph glyph)
:   FTGlyph( glyph),
    destWidth(0),
    destHeight(0),
    data(0)
{
    // This function will always fail if the glyph's format isn't scalable????
    err = FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1);
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
    destWidth = srcWidth;
    destHeight = srcHeight;
    
    if( destWidth && destHeight)
    {
        data = new unsigned char[destWidth * destHeight * 4];
    
        // Get the current glColor.
        float ftglColour[4];
        glGetFloatv( GL_CURRENT_COLOR, ftglColour);

        unsigned char redComponent =   static_cast<unsigned char>( ftglColour[0] * 255.0f);
        unsigned char greenComponent = static_cast<unsigned char>( ftglColour[1] * 255.0f);
        unsigned char blueComponent =  static_cast<unsigned char>( ftglColour[2] * 255.0f);

        unsigned char* src = source->buffer;

        unsigned char* dest = data + ((destHeight - 1) * destWidth) * 4;
        size_t destStep = destWidth * 4 * 2;

        if( ftglColour[3] == 1.0f)
        {
            for( int y = 0; y < srcHeight; ++y)
            {
                for( int x = 0; x < srcWidth; ++x)
                {
                    *dest++ = redComponent;
                    *dest++ = greenComponent;
                    *dest++ = blueComponent;
                    *dest++ = *src++;
                }
                dest -= destStep;
            }
        }
        else
        {
            for( int y = 0; y < srcHeight; ++y)
            {
                for( int x = 0; x < srcWidth; ++x)
                {
                    *dest++ = redComponent;
                    *dest++ = greenComponent;
                    *dest++ = blueComponent;
                    *dest++ = static_cast<unsigned char>(ftglColour[3] * *src++);
                }
                dest -= destStep;
            }
        }

        destHeight = srcHeight;
    }

    pos.x = bitmap->left;
    pos.y = srcHeight - bitmap->top;
    
    // Is this the right place to do this?
    FT_Done_Glyph( glyph );
}


FTPixmapGlyph::~FTPixmapGlyph()
{
    delete [] data;
}


float FTPixmapGlyph::Render( const FTPoint& pen)
{
    if( data)
    {
        // Move the glyph origin
        glBitmap( 0, 0, 0.0f, 0.0f, pen.x + pos.x, pen.y - pos.y, (const GLubyte*)0);

        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);

        glDrawPixels( destWidth, destHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)data);
        
        // Restore the glyph origin
        glBitmap( 0, 0, 0.0f, 0.0f, -pen.x - pos.x, -pen.y + pos.y, (const GLubyte*)0);
    }

    return advance;
}
