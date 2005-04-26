#include    "FTGlyphContainer.h"
#include    "FTGlyph.h"
#include    "FTFace.h"
#include    "FTCharmap.h"


FTGlyphContainer::FTGlyphContainer( FTFace* f)
:   face(f),
    charMap(0),
    err(0)
{
    glyphs.push_back( NULL);
    charMap = new FTCharmap( face);
}


FTGlyphContainer::~FTGlyphContainer()
{
    GlyphVector::iterator glyphIterator;
    for( glyphIterator = glyphs.begin(); glyphIterator != glyphs.end(); ++glyphIterator)
    {
        delete *glyphIterator;
    }
    
    glyphs.clear();
}


bool FTGlyphContainer::CharMap( FT_Encoding encoding)
{
    bool result = charMap->CharMap( encoding);
    err = charMap->Error();
    return result;
}


unsigned int FTGlyphContainer::GlyphIndex( const unsigned int characterCode) const
{
    return charMap->GlyphIndex( characterCode);
}


void FTGlyphContainer::Add( FTGlyph* tempGlyph, const unsigned int characterCode)
{
    charMap->InsertIndex( characterCode, glyphs.size());
    glyphs.push_back( tempGlyph);
}


const FTGlyph* const FTGlyphContainer::Glyph( const unsigned int characterCode) const
{
    unsigned int index = charMap->CharIndex( characterCode);
    return glyphs[index];
}


FTBBox FTGlyphContainer::BBox( const unsigned int characterCode) const
{
    return glyphs[charMap->CharIndex( characterCode)]->BBox();
}


float FTGlyphContainer::Advance( const unsigned int characterCode, const unsigned int nextCharacterCode)
{
    unsigned int left = charMap->GlyphIndex( characterCode);
    unsigned int right = charMap->GlyphIndex( nextCharacterCode);
    
    float width = face->KernAdvance( left, right).x;
    width += glyphs[charMap->CharIndex( characterCode)]->Advance();
    
    return width;
}


FTPoint FTGlyphContainer::Render( const unsigned int characterCode, const unsigned int nextCharacterCode, FTPoint penPosition)
{
    FTPoint kernAdvance;
    float advance = 0;
    
    unsigned int left = charMap->GlyphIndex( characterCode);
    unsigned int right = charMap->GlyphIndex( nextCharacterCode);
    
    kernAdvance = face->KernAdvance( left, right);
        
    if( !face->Error())
    {
        advance = glyphs[charMap->CharIndex( characterCode)]->Render( penPosition);
    }
    
    kernAdvance.x = advance + kernAdvance.x;
//  kernAdvance.y = advance.y + kernAdvance.y;
    return kernAdvance;
}
