#include "FTFace.h"
#include "FTCharmap.h"


FTCharmap::FTCharmap( FTFace* face)
:   ftFace( *(face->Face())),
    err(0)
{
    if( !ftFace->charmap)
    {
        err = FT_Set_Charmap( ftFace, ftFace->charmaps[0]);
    }
    
    ftEncoding = ftFace->charmap->encoding;
}


FTCharmap::~FTCharmap()
{
    charMap.clear();
}


bool FTCharmap::CharMap( FT_Encoding encoding)
{
    if( ftEncoding == encoding)
    {
        return true;
    }
    
    err = FT_Select_Charmap( ftFace, encoding );
    
    if( !err)
    {
        ftEncoding = encoding;
    }
    else
    {
        ftEncoding = ft_encoding_none;
    }
        
    charMap.clear();
    return !err;
}


unsigned int FTCharmap::CharIndex( unsigned int characterCode )
{
    const CharacterMap::GlyphIndex *result = charMap.find( characterCode);
    if( !result)
    {
        return 0;
    }
    else
    {
        return *result;
    }
}


void FTCharmap::InsertIndex( const unsigned int characterCode, const unsigned int containerIndex)
{
    charMap.insert( characterCode, containerIndex);
}


unsigned int FTCharmap::GlyphIndex( const unsigned int characterCode)
{
    unsigned int glyphIndex = FT_Get_Char_Index( ftFace, characterCode);
    return glyphIndex;
}
