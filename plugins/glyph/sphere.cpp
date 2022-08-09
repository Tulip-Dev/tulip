/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <GL/glew.h>

#include <iostream>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/GlSphere.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawGlyph(const Color &glyphColor, const string &texture, const string &texturePath) {
  static GlSphere sphere(Coord(0, 0, 0), 0.5);

  sphere.setColor(glyphColor);
  sphere.setTexture(texturePath + texture);

  sphere.draw(0, nullptr);
}

/** \addtogroup glyph */

/// A 3D glyph.
/**
 * This glyph draws a textured sphere using the "viewTexture" node
 * property value. If this property has no value, the sphere
 * is then colored using the "viewColor" node property value.
 */
class Sphere : public NoShaderGlyph {
public:
  GLYPHINFORMATION("3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1.0",
                   NodeShape::Sphere)
  Sphere(const tlp::PluginContext *context = nullptr);
  ~Sphere() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
};

PLUGIN(Sphere)

//=========================================================================================
Sphere::Sphere(const tlp::PluginContext *context) : NoShaderGlyph(context) {}

Sphere::~Sphere() {}

void Sphere::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, -0.35f);
  boundingBox[1] = Coord(0.35f, 0.35f, 0.35f);
}

void Sphere::draw(node n, float) {
  drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
            glGraphInputData->getElementTexture()->getNodeValue(n),
            glGraphInputData->parameters->getTexturePath());
}

class EESphere : public EdgeExtremityGlyph {
  GLYPHINFORMATION("3D - Sphere extremity", "Bertrand Mathieu", "09/07/2002",
                   "Textured sphere for edge extremities", "1.0", EdgeExtremityShape::Sphere)
public:
  EESphere(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}
  ~EESphere() override {}
  void draw(edge e, node, const Color &glyphColor, const Color &, float) override {
    glEnable(GL_LIGHTING);
    drawGlyph(glyphColor, edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
              edgeExtGlGraphInputData->parameters->getTexturePath());
  }
};
PLUGIN(EESphere)

} // end of namespace tlp
