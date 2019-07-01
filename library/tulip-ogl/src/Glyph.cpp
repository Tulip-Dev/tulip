/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <tulip/BoundingBox.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/GlRect.h>

using namespace tlp;
using namespace std;

Glyph::Glyph(const tlp::PluginContext *context) : glGraphInputData(nullptr) {
  if (context != nullptr) {
    const GlyphContext *glyphContext = static_cast<const GlyphContext *>(context);
    glGraphInputData = glyphContext->glGraphInputData;
  }
}
//=============================================
Glyph::~Glyph() {}

//=============================================
void Glyph::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.5, -0.5, -0.5);
  boundingBox[1] = Coord(0.5, 0.5, 0.5);
}

//=============================================
void Glyph::getTextBoundingBox(BoundingBox &boundingBox, node n) {
    getIncludeBoundingBox(boundingBox, n);
}

//=============================================
Coord Glyph::getAnchor(const Coord &nodeCenter, const Coord &from, const Size &scale,
                       const double zRotation) const {
  Coord anchor = from - nodeCenter;

  if (anchor.getX() == 0.0f && anchor.getY() == 0.0f)
    return nodeCenter;

  if (scale.getW() == 0.0f || scale.getH() == 0.0f)
    return nodeCenter;

  if (zRotation != 0) {
    // unrotate
    Coord saveAnchor(anchor);
    double zRot = -2.0 * M_PI * zRotation / 360.0;
    anchor[0] = saveAnchor[0] * cos(zRot) - saveAnchor[1] * sin(zRot);
    anchor[1] = saveAnchor[0] * sin(zRot) + saveAnchor[1] * cos(zRot);
  }

  // unscale
  anchor.setX(anchor.getX() / scale.getW());
  anchor.setY(anchor.getY() / scale.getH());

  if (scale.getD() != 0.0f)
    anchor.setZ(anchor.getZ() / scale.getD());
  else
    anchor.setZ(0.0f);

  anchor = getAnchor(anchor);

  // rescale
  anchor.setX(anchor.getX() * scale.getW());
  anchor.setY(anchor.getY() * scale.getH());
  anchor.setZ(anchor.getZ() * scale.getD());

  if (zRotation != 0) {
    // rerotate
    Coord saveAnchor(anchor);
    double zRot = 2.0 * M_PI * zRotation / 360.0;
    anchor[0] = saveAnchor[0] * cos(zRot) - saveAnchor[1] * sin(zRot);
    anchor[1] = saveAnchor[0] * sin(zRot) + saveAnchor[1] * cos(zRot);
  }

  return nodeCenter + anchor;
}
//=================================================================
Coord Glyph::getAnchor(const Coord &vector) const {
  Coord anchor = vector;
  /* anchor must be on the surface of a sphere centered on nodecenter, radius is 0.5 */
  anchor *= 0.5f / anchor.norm();
  return anchor;
}
//=================================================================
void Glyph::drawRectInScreenPlane(GlRect &rect, const Size &size, bool disableMasks) {
  // setup rect orientation to ensure it is drawn is the screen plane
  float mdlM[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, mdlM);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  mdlM[0] = size.getW();
  mdlM[5] = size.getH();
  mdlM[10] = size.getD();
  mdlM[1] = mdlM[2] = 0.0f;
  mdlM[4] = mdlM[6] = 0.0f;
  mdlM[8] = mdlM[9] = 0.0f;
  glLoadMatrixf(mdlM);
  if (disableMasks) {
    glStencilMask(0x00);
    glDepthMask(GL_FALSE);
  }
  rect.draw(0, nullptr);
  if (disableMasks) {
    glStencilMask(0xFF);
    glDepthMask(GL_TRUE);
  }
  glPopMatrix();
}
