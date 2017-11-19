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
#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/GlLabel.h>
#include <tulip/GlGraphRenderingParameters.h>

using namespace std;
using namespace tlp;

static GlLabel label;

void drawTree(const Color &color, const Color &outlineColor, const float outlineSize,
              const std::string &texture) {
  label.setFontNameSizeAndColor(TulipFontAwesome::getFontAwesomeTrueTypeFileLocation(), 18, color);
  label.setPosition(Coord(0, 0, 0));
  label.setSize(Size(1, 1, 0));
  label.setUseLODOptimisation(false);
  label.setScaleToSize(true);
  label.setOutlineColor(outlineColor);
  label.setOutlineSize(outlineSize);
  label.setTextureName(texture);
  label.setText(TulipFontAwesome::getFontAwesomeIconUtf8String(TulipFontAwesome::Tree));
  label.draw(100);
}

/** \addtogroup glyph */

/// A 2D glyph.
/** This glyph draws a christmas tree special for christmas release of 2008
 */
class ChristmasTree : public Glyph {
public:
  GLYPHINFORMATION("2D - ChristmasTree", "Morgan Mathiaut", "16/12/2008", "Christmas tree", "1.0",
                   NodeShape::ChristmasTree)
  ChristmasTree(const tlp::PluginContext *context = nullptr);
  void draw(node n, float lod) override;
};
PLUGIN(ChristmasTree)

ChristmasTree::ChristmasTree(const tlp::PluginContext *context) : Glyph(context) {}
void ChristmasTree::draw(node n, float) {
  const tlp::Color &nodeColor = glGraphInputData->getElementColor()->getNodeValue(n);
  const tlp::Color &nodeBorderColor = glGraphInputData->getElementBorderColor()->getNodeValue(n);
  float nodeBorderWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);
  const std::string &nodeTexture = glGraphInputData->parameters->getTexturePath() +
                                   glGraphInputData->getElementTexture()->getNodeValue(n);

  drawTree(nodeColor, nodeBorderColor, nodeBorderWidth, nodeTexture);
}
