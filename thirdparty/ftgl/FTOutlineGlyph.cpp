#include    "FTOutlineGlyph.h"
#include    "FTVectoriser.h"


FTOutlineGlyph::FTOutlineGlyph( FT_Glyph glyph)
:   FTGlyph( glyph),
    glList(0)
{
    if( ft_glyph_format_outline != glyph->format)
    {
        return;
    }

    FTVectoriser vectoriser( glyph);

    size_t numContours = vectoriser.ContourCount();
    if ( ( numContours < 1) || ( vectoriser.PointCount() < 3))
    {
        return;
    }

    glList = glGenLists(1);
    glNewList( glList, GL_COMPILE);
        for( unsigned int c = 0; c < numContours; ++c)
        {
            const FTContour* contour = vectoriser.Contour(c);
            
            glBegin( GL_LINE_LOOP);
                for( unsigned int p = 0; p < contour->PointCount(); ++p)
                {
                    glVertex2f( contour->Point(p).x / 64.0f, contour->Point(p).y / 64.0f);
                }
            glEnd();
        }
    glEndList();


    // discard glyph image (bitmap or not)
    FT_Done_Glyph( glyph); // Why does this have to be HERE
}


FTOutlineGlyph::~FTOutlineGlyph()
{
    glDeleteLists( glList, 1);
}


float FTOutlineGlyph::Render( const FTPoint& pen)
{
    if( glList)
    {
        glTranslatef( pen.x, pen.y, 0);
            glCallList( glList);
        glTranslatef( -pen.x, -pen.y, 0);
    }
    
    return advance;
}

