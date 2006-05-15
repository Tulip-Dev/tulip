#ifndef     __TLPPixmapGlyph__
#define     __TLPPixmapGlyph__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <FTGL.h>
#ifdef _WIN32
#include "../../../thirdparty/ftgl/FTGlyph.h"
#else
#include <FTGlyph.h>
#endif

namespace tlp {

/**
 * TLPPixmapGlyph is a specialisation of FTGlyph for creating pixmaps.
 * 
 * @see FTGlyphContainer
 *
 */
class  FTGL_EXPORT TLPPixmapGlyph : public FTGlyph {
    public:
        /**
         * Constructor
         *
         * @param glyph The Freetype glyph to be processed
         */
        TLPPixmapGlyph(FT_GlyphSlot glyph);

        /**
         * Destructor
         */
        virtual ~TLPPixmapGlyph();

        /**
         * Renders this glyph at the current pen position.
         *
         * @param pen   The current pen position.
         * @return      The advance distance for this glyph.
         */
        virtual FTPoint& Render( const FTPoint& pen);
        
        // attributes

    private:
        /**
         * The width of the glyph 'image'
         */
        int destWidth;

        /**
         * The height of the glyph 'image'
         */
        int destHeight;
        
        /**
         * Vector from the pen position to the topleft corner of the pixmap
         */
        FTPoint pos;
        
        /**
         * Pointer to the 'image' data
         */
        unsigned char* data;
};

}


#endif  //  __TLPPixmapGlyph__
