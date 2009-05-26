#ifndef     __TLPPixmapGlyph__
#define     __TLPPixmapGlyph__
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <FTGL/ftgl.h>

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
         * @param mode  Render mode to display
         * @return      The advance distance for this glyph.
         */
        virtual FTPoint& Render( const FTPoint& pen, int mode);
        
        /**
         * Return the advance width for this glyph.
         *
         * @return  advance width.
         */
        virtual float Advance() const;

        /**
         * Return the bounding box for this glyph.
         *
         * @return  bounding box.
         */
        virtual const FTBBox& BBox() const;

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
        /**
         * The advance distance for this glyph
         */
        FTPoint advance;

        /**
         * The bounding box of this glyph.
         */
        FTBBox bBox;
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif  //  __TLPPixmapGlyph__
