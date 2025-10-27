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
#include <tulip/Glyph.h>
#include <tulip/GlBox.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;

namespace tlp {

/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value. The "viewBorderColor"
 * node property value is used to draw its edges.
 */
class CubeOutLined : public NoShaderGlyph {
public:
  GLYPHINFORMATION("3D - Cube OutLined", "David Auber", "09/07/2002", "Textured cubeOutLined",
                   "1.0", NodeShape::CubeOutlined)
  CubeOutLined(tlp::PluginContext *context);
  ~CubeOutLined() override;
  void draw(node n, float lod) override;
  Coord getAnchor(const Coord &vector) const override;
};

PLUGIN(CubeOutLined)

//===================================================================================
CubeOutLined::CubeOutLined(tlp::PluginContext *context) : NoShaderGlyph(context) {}

CubeOutLined::~CubeOutLined() {}

void CubeOutLined::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!textureName.empty())
    textureName = glGraphInputData->parameters->getTexturePath() + textureName;

  GlBox::draw(glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementBorderColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod);
}

Coord CubeOutLined::getAnchor(const Coord &vector) const {
  return GlBox::getAnchor(vector);
}

} // namespace tlp
