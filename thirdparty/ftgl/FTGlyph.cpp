#include    "FTGlyph.h"


FTGlyph::FTGlyph( FT_Glyph glyph)
:   advance(0.0f),
    err(0)  
{
    if( glyph)
    {
        bBox = FTBBox( glyph);
        advance = static_cast<float>( glyph->advance.x) / 65536.0f;
    }
}


FTGlyph::~FTGlyph()
{}
