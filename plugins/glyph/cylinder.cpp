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

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawCylinder() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);
  glTranslatef(0.0f, 0.0f, -0.5f);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 10, 10);
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluCylinder(quadratic, 0.5f, 0.5f, 1.0f, 10, 10);
  glTranslatef(0.0f, 0.0f, 1.0f);
  gluDisk(quadratic, 0.0f, 0.5f, 10, 10);
  gluDeleteQuadric(quadratic);

}

/** \addtogroup glyph */
/// A 3D glyph.
/**
 * This glyph draws an textured cylinder using the "viewTexture" node
 * property value. If this property has no value, the cylinder is then colored
 * using the "viewColor" node property value.
 */
class Cylinder: public Glyph {
public:
  GLYPHINFORMATION("3D - Cylinder", "Bertrand Mathieu", "31/07/2002", "Textured Cylinder", "1.0", NodeShape::Cylinder)
  Cylinder(const tlp::PluginContext* context = NULL);
  virtual ~Cylinder();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord &vector) const;
};
PLUGIN(Cylinder)

Cylinder::Cylinder(const tlp::PluginContext* context) :
  Glyph(context) {
}
Cylinder::~Cylinder() {
}
void Cylinder::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 1);
}
void Cylinder::draw(node n, float) {
  if (GlDisplayListManager::getInst().beginNewDisplayList("Cylinder_cylinder")) {
    drawCylinder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }

  setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (texFile != "") {
    string texturePath = glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath + texFile);
  }

  GlDisplayListManager::getInst().callDisplayList("Cylinder_cylinder");
  GlTextureManager::getInst().desactivateTexture();
}
Coord Cylinder::getAnchor(const Coord &vector) const {
  Coord anchor = vector;
  float x, y, z, n;
  anchor.get(x, y, z);
  n = sqrt(x * x + y * y);

  if (n == 0.0f)
    return anchor;

  n = 0.5f / n;
  x *= n;
  y *= n;
  z *= n;

  if (z < -0.5f)
    z = -0.5f; //z = z >? -0.5f;

  if (z > +0.5f)
    z = +0.5f; //z = z <? +0.5f;

  return Coord(x, y, z);
}

class EECylinder: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Cylinder extremity", "Bertrand Mathieu", "31/07/2002", "Textured Cylinder for edge extremities", "1.0", EdgeExtremityShape::Cylinder)

  EECylinder(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
  }

  void draw(edge, node n, const Color& glyphColor, const Color&, float) {
    glEnable(GL_LIGHTING);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    if (GlDisplayListManager::getInst().beginNewDisplayList("Cylinder_cylinder")) {
      drawCylinder();
      GlDisplayListManager::getInst().endNewDisplayList();
    }

    setMaterial(glyphColor);
    string texFile = edgeExtGlGraphInputData->getElementTexture()->getNodeValue(n);

    if (texFile != "") {
      string texturePath =
        edgeExtGlGraphInputData->parameters->getTexturePath();
      GlTextureManager::getInst().activateTexture(texturePath + texFile);
    }

    GlDisplayListManager::getInst().callDisplayList("Cylinder_cylinder");
    GlTextureManager::getInst().desactivateTexture();
  }
};

PLUGIN(EECylinder)

} // end of namespace tlp
