#include <cmath>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>

using namespace tlp;

TemplateFactory<GlyphFactory,Glyph, GlyphContext *> *GlyphFactory::factory;

Glyph::Glyph(GlyphContext *gc) {
  if (gc != 0) {
    glGraphInputData=gc->glGraphInputData;
  }
  else {
    glGraphInputData = 0;
  }
}
//=============================================
Glyph::~Glyph() {
}
//=============================================
Coord Glyph::getAnchor(const Coord &nodeCenter, const Coord &from, const Size &scale, const double zRotation) const {
  Coord anchor = from - nodeCenter;
  if( anchor.getX() == 0.0f && anchor.getY() == 0.0f && anchor.getZ() == 0.0f )
    return nodeCenter;
  if( scale.getW() == 0.0f || scale.getH() == 0.0f || scale.getD() == 0.0f )
    return nodeCenter;

  //unrotate
  Coord saveAnchor(anchor);
  double zRot =  - 2.0*M_PI * zRotation / 360.0;
  anchor[0] = saveAnchor[0]*cos(zRot) - saveAnchor[1]*sin(zRot);
  anchor[1] = saveAnchor[0]*sin(zRot) + saveAnchor[1]*cos(zRot);

  // unscale
  anchor.setX( anchor.getX() / scale.getW() );
  anchor.setY( anchor.getY() / scale.getH() );
  anchor.setZ( anchor.getZ() / scale.getD() );
  anchor = getAnchor( anchor );

  // rescale
  anchor.setX( anchor.getX() * scale.getW() );
  anchor.setY( anchor.getY() * scale.getH() );
  anchor.setZ( anchor.getZ() * scale.getD() );

  //rerotate
  saveAnchor = anchor;
  zRot = -zRot;
  anchor[0] = saveAnchor[0]*cos(zRot) - saveAnchor[1]*sin(zRot);
  anchor[1] = saveAnchor[0]*sin(zRot) + saveAnchor[1]*cos(zRot);

  return nodeCenter + anchor;
}
//=================================================================
Coord Glyph::getAnchor(const Coord &vector) const
{
  Coord anchor = vector;
  /* anchor must be on the surface of a sphere centered on nodecenter, radius is 0.5 */
  anchor *= 0.5f / anchor.norm();
  return anchor;
}
