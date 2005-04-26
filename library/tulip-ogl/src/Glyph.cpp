#include <cmath>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>

Glyph::Glyph(GlyphContext *gc)
{
  if (gc!=NULL) {
    superGraph=gc->superGraph;
    glGraph=gc->glGraph;
    LOF=gc->LOF;
    LOD=gc->LOD;
  }
  else {
    superGraph = NULL;
    glGraph = NULL;
    LOD=LOF=0;
  }
}

Glyph::~Glyph()
{
}

void Glyph::setLOF(int n) {
  LOF = ((n<0) ? 0 : ((n > 10) ? 10 : n));
}

int Glyph::getLOF() {return LOF;}

void Glyph::setLOD(int n) {
  LOD = ((n<0) ? 0 : ((n > 10) ? 10 : n));
}

int Glyph::getLOD() {return LOD;}

void Glyph::setMaterial(const Color &C) {
  float color[4] = { 0.0, 0.0, 0.0, 1.0 };
  color[0]=((float)C.getR())/255;
  color[1]=((float)C.getG())/255;
  color[2]=((float)C.getB())/255;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

Coord Glyph::getAnchor(const Coord &nodeCenter, const Coord &from, const Size &scale) const
{
   Coord anchor = from - nodeCenter;
   if( anchor.getX() == 0.0f && anchor.getY() == 0.0f && anchor.getZ() == 0.0f )
		return nodeCenter;
   if( scale.getW() == 0.0f || scale.getH() == 0.0f || scale.getD() == 0.0f )
		return nodeCenter;

   // unscale
   anchor.setX( anchor.getX() / scale.getW() );
   anchor.setY( anchor.getY() / scale.getH() );
   anchor.setZ( anchor.getZ() / scale.getD() );

   anchor = getAnchor( anchor );

   // rescale
   anchor.setX( anchor.getX() * scale.getW() );
   anchor.setY( anchor.getY() * scale.getH() );
   anchor.setZ( anchor.getZ() * scale.getD() );

   return nodeCenter + anchor;
}

Coord Glyph::getAnchor(const Coord &vector) const
{
  Coord anchor = vector;
  /* anchor must be on the surface of a sphere centered on nodecenter, radius is 0.5 */
  anchor *= 0.5f / anchor.norm();
  return anchor;
}
