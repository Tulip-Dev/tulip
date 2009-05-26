#ifndef     __TLPPixmapFont__
#define     __TLPPixmapFont__
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <FTGL/ftgl.h>
#include <FTGL/FTFont.h>

class FTGlyph;

namespace tlp {

/**
 * TLPPixmapFont is a specialisation of the FTFont class for handling
 * Pixmap (Grey Scale) fonts
 *
 * @see     FTFont
 */
class FTGL_EXPORT TLPPixmapFont : public FTFont
{
    public:
        /**
         * Open and read a font file. Sets Error flag.
         *
         * @param fontname  font file name.
         */
        TLPPixmapFont( const char* fontname);
        
        /**
         * Open and read a font from a buffer in memory. Sets Error flag.
         *
         * @param pBufferBytes  the in-memory buffer
         * @param bufferSizeInBytes  the length of the buffer in bytes
         */
        TLPPixmapFont( const unsigned char *pBufferBytes, size_t bufferSizeInBytes);
        
        /**
         * Destructor
         */
        ~TLPPixmapFont();
        
        /**
         * Renders a string of characters
         * 
         * @param string    'C' style string to be output.   
         */
        void Render( const char* string);
        
        /**
         * Renders a string of characters
         * 
         * @param string    wchar_t string to be output.     
         */
        void Render( const wchar_t* string);

    private:
        /**
         * Construct a FTPixmapGlyph.
         *
         * @param slot  A FreeType glyph slot
         * @return  An FTPixmapGlyph or <code>null</code> on failure.
         */
        inline virtual FTGlyph* MakeGlyph(FT_GlyphSlot slot);
        
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif  //  __TLPPixmapFont__

