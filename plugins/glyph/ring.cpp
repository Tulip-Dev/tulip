/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawRing() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.2f, 0.5f, 30, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.2f, 0.5f, 30, 1);
  gluDeleteQuadric(quadratic);
}
static void drawRingBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI / 30.0;

  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.5 * cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }

  glEnd();
  glBegin(GL_LINE_LOOP);
  alpha = M_PI / 2.;

  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.2 * cos(alpha), 0.2 * sin(alpha), 0.0);
    alpha += delta;
  }

  glEnd();
}
static void drawGlyph(const Color& glyohColor, const string& texture,
                      const string& texturePath, double borderWidth,
                      const Color& borderColor, float lod) {

  if (GlDisplayListManager::getInst().beginNewDisplayList("Ring_ring")) {
    drawRing();
    GlDisplayListManager::getInst().endNewDisplayList();
  }

  if (GlDisplayListManager::getInst().beginNewDisplayList("Ring_ringborder")) {
    drawRingBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }

  setMaterial(glyohColor);

  if (texture != "") {
    GlTextureManager::getInst().activateTexture(texturePath + texture);
  }

  GlDisplayListManager::getInst().callDisplayList("Ring_ring");

  GlTextureManager::getInst().desactivateTexture();

  if (lod > 20 && borderWidth > 0) {

    glLineWidth(borderWidth);

    glDisable(GL_LIGHTING);
    setColor(borderColor);
    GlDisplayListManager::getInst().callDisplayList("Ring_ringborder");
    glEnable(GL_LIGHTING);
  }
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured disc with a circular hole using the
 * "viewTexture" node property value.
 * If this property has no value, the ring
 * is then colored using the "viewColor" node property value.
 */
class Ring: public Glyph {
public:
  GLYPHINFORMATION("2D - Ring", "David Auber", "09/07/2002", "Textured Ring", "1.0", NodeShape::Ring)
  Ring(const tlp::PluginContext *context = NULL);
  virtual ~Ring();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual string getName() {
    return string("Ring");
  }
  virtual void draw(node n, float lod);
};
PLUGIN(Ring)
Ring::Ring(const tlp::PluginContext* context) :
  Glyph(context) {
}
Ring::~Ring() {
}
void Ring::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
void Ring::draw(node n, float lod) {
  drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
            glGraphInputData->getElementTexture()->getNodeValue(n),
            glGraphInputData->parameters->getTexturePath(),
            glGraphInputData->getElementBorderWidth()->getNodeValue(n),
            glGraphInputData->getElementBorderColor()->getNodeValue(n), lod);

}


class EERing: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Ring extremity", "David Auber", "09/07/2002", "Textured Ring for edge extremities", "1.0", EdgeExtremityShape::Ring)

  EERing(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
  }

  void draw(edge e, node, const Color& glyphColor, const Color &borderColor, float lod) {
    glDisable(GL_LIGHTING);
    drawGlyph(glyphColor,
              edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
              edgeExtGlGraphInputData->parameters->getTexturePath(),
              edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
              borderColor, lod);
  }
};
PLUGIN(EERing)

} // end of namespace tlp
