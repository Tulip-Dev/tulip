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
/** This glyph draws a transparent cube using the "viewBorderColor" node
 * property value to draw its edges.
 */
class CubeOutLinedTransparent : public Glyph {
public:
  GLYPHINFORMATION("3D - Cube OutLined Transparent", "David Auber", "09/07/2002",
                   "Textured cubeOutLined", "1.0", NodeShape::CubeOutlinedTransparent)
  CubeOutLinedTransparent(const tlp::PluginContext *context = nullptr);
  ~CubeOutLinedTransparent() override;
  void draw(node n, float lod) override;
  Coord getAnchor(const Coord &vector) const override;
};
PLUGIN(CubeOutLinedTransparent)

CubeOutLinedTransparent::CubeOutLinedTransparent(const tlp::PluginContext *context)
    : Glyph(context) {}

CubeOutLinedTransparent::~CubeOutLinedTransparent() {}

void CubeOutLinedTransparent::draw(node n, float lod) {
  GlBox::draw(Color(0, 0, 0, 0), glGraphInputData->getElementBorderColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n), "", lod);
}

Coord CubeOutLinedTransparent::getAnchor(const Coord &vector) const {
  return GlBox::getAnchor(vector);
}

class EECubeOutlinedTransparent : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Cube OutLined Transparent extremity", "David Auber", "09/07/2002",
                   "Textured cubeOutLined for edge extremities", "1.0",
                   EdgeExtremityShape::CubeOutlinedTransparent)

  EECubeOutlinedTransparent(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &fillColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!textureName.empty())
      textureName = edgeExtGlGraphInputData->parameters->getTexturePath() + textureName;

    glEnable(GL_LIGHTING);
    GlBox::draw(fillColor, borderColor,
                edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName,
                lod);
    glDisable(GL_LIGHTING);
  }
};
PLUGIN(EECubeOutlinedTransparent)

} // end of namespace tlp
