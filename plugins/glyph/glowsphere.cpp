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
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/GlSphere.h>
#include <tulip/GlRect.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GlSphere *sphere = NULL;
static GlRect *rect = NULL;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere using the "viewColor" node property value.
 */
class GlowSphere : public Glyph {
public:
  GLYPHINFORMATION("3D - Glow Sphere", "Patrick Mary", "24/01/2012", "Glow Sphere", "1.0",
                   NodeShape::GlowSphere)
  GlowSphere(const tlp::PluginContext *context = NULL) : Glyph(context) {}
  virtual ~GlowSphere() {}
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox, node);
  virtual void draw(node n, float lod);
  static void drawGlyph(const Color &glyphColor, const Size &glyphSize, const string &texture,
                        const string &texturePath);
};

PLUGIN(GlowSphere)

void GlowSphere::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, -0.35f);
  boundingBox[1] = Coord(0.35f, 0.35f, 0.35f);
}

void GlowSphere::drawGlyph(const Color &glyphColor, const Size &glyphSize, const string &texture,
                           const string &texturePath) {

  // draw a sphere
  if (!sphere) {
    sphere = new GlSphere(Coord(0, 0, 0), 0.5);
  }

  sphere->setColor(glyphColor);
  sphere->setTexture(texturePath + texture);
  sphere->draw(0, 0);

  // draw a glow ring around
  // setup its orientation to ensure it is drawn is the screen's plane
  if (!rect) {
    rect = new GlRect(Coord(0, 0, 0), 2., 2, Color(0, 0, 0, 255), Color(0, 0, 0, 255));
    rect->setOutlineMode(false);
  }

  Color ringColor(glyphColor);
  // semi transparent
  ringColor.setA(128);
  rect->setFillColor(ringColor);
  rect->setTextureName(TulipBitmapDir + "radialGradientTexture.png");

  float mdlM[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, mdlM);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  float nx = glyphSize.getW();
  float ny = glyphSize.getH();
  float nz = glyphSize.getD();
  mdlM[0] = nx;
  mdlM[5] = ny;
  mdlM[10] = nz;
  mdlM[1] = mdlM[2] = 0.0f;
  mdlM[4] = mdlM[6] = 0.0f;
  mdlM[8] = mdlM[9] = 0.0f;
  glLoadMatrixf(mdlM);
  glStencilMask(0x00);
  glDepthMask(GL_FALSE);
  rect->draw(0, 0);
  glStencilMask(0xFF);
  glDepthMask(GL_TRUE);
  glPopMatrix();
}

void GlowSphere::draw(node n, float /* lod */) {
  drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
            glGraphInputData->getElementSize()->getNodeValue(n),
            glGraphInputData->getElementTexture()->getNodeValue(n),
            glGraphInputData->parameters->getTexturePath());
}

class EEGlowSphere : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Glow Sphere extremity", "Patrick Mary", "24/01/2012",
                   "Glow Sphere for edge extremities", "1.0", EdgeExtremityShape::GlowSphere)
  EEGlowSphere(const tlp::PluginContext *context = NULL) : EdgeExtremityGlyph(context) {}
  virtual ~EEGlowSphere() {}
  virtual void draw(edge e, node n, const Color &glyphColor, const Color & /* borderColor */,
                    float /* lod */) {
    glDisable(GL_LIGHTING);
    GlowSphere::drawGlyph(glyphColor, edgeExtGlGraphInputData->getElementSize()->getNodeValue(n),
                          edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
                          edgeExtGlGraphInputData->parameters->getTexturePath());
  }
};

PLUGIN(EEGlowSphere)

} // end of namespace tlp
