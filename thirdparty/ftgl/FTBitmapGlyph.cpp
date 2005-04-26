#include    <string>

#include "FTBitmapGlyph.h"

FTBitmapGlyph::FTBitmapGlyph( FT_Glyph glyph)
:   FTGlyph( glyph),
    destWidth(0),
    destHeight(0),
    data(0)
{
    err = FT_Glyph_To_Bitmap( &glyph, ft_render_mode_mono, 0, 1);
    if( err || ft_glyph_format_bitmap != glyph->format)
    {
        return;
    }

    FT_BitmapGlyph  bitmap = (FT_BitmapGlyph)glyph;
    FT_Bitmap*      source = &bitmap->bitmap;

    unsigned int srcWidth = source->width;
    unsigned int srcHeight = source->rows;
    unsigned int srcPitch = source->pitch;
    
    destWidth = srcWidth;
    destHeight = srcHeight;
    destPitch = srcPitch;    

    if( destWidth && destHeight)
    {
        data = new unsigned char[destPitch * destHeight];
        unsigned char* dest = data + (( destHeight - 1) * destPitch);

        unsigned char* src = source->buffer;

        for( unsigned int y = 0; y < srcHeight; ++y)
        {
            memcpy( dest, src, srcPitch);
            dest -= destPitch;
            src += srcPitch;
        }
    }
    
    pos.x = bitmap->left;
    pos.y = static_cast<int>(srcHeight) - bitmap->top;

    FT_Done_Glyph( glyph );
}


FTBitmapGlyph::~FTBitmapGlyph()
{
    delete [] data;
}


float FTBitmapGlyph::Render( const FTPoint& pen)
{
    if( data)
    {
        glBitmap( 0, 0, 0.0, 0.0, pen.x + pos.x, pen.y - pos.y, (const GLubyte*)0 );

        glPixelStorei( GL_UNPACK_ROW_LENGTH, destPitch * 8);
        glBitmap( destWidth, destHeight, 0.0f, 0.0, 0.0, 0.0, (const GLubyte*)data);

        glBitmap( 0, 0, 0.0, 0.0, -pen.x - pos.x, -pen.y + pos.y, (const GLubyte*)0 );
    }
    
    return advance;
}
