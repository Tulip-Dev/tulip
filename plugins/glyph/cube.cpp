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
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlBox.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */

/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value.
 */
class Cube : public NoShaderGlyph {
public:
  GLYPHINFORMATION("3D - Cube", "Bertrand Mathieu", "09/07/2002", "Textured cube", "1.0",
                   NodeShape::Cube)
  Cube(const tlp::PluginContext *context = nullptr);
  ~Cube() override;
  void draw(node n, float lod) override;
  Coord getAnchor(const Coord &vector) const override;

protected:
};
PLUGIN(Cube)
Cube::Cube(const tlp::PluginContext *context) : NoShaderGlyph(context) {}
Cube::~Cube() {}
void Cube::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);
  if (!textureName.empty())
    textureName = textureName + glGraphInputData->parameters->getTexturePath();

  GlBox::draw(glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod);
}
Coord Cube::getAnchor(const Coord &vector) const {
  return GlBox::getAnchor(vector);
}

class EECube : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Cube extremity", "Bertrand Mathieu", "09/07/2002",
                   "Textured cube for edge extremities", "1.0", EdgeExtremityShape::Cube)

  EECube(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);
    if (!textureName.empty())
      textureName = textureName + edgeExtGlGraphInputData->parameters->getTexturePath();

    glEnable(GL_LIGHTING);
    GlBox::draw(glyphColor, borderColor,
                edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName,
                lod);
    glDisable(GL_LIGHTING);
  }
};

PLUGIN(EECube)

} // end of namespace tlp
