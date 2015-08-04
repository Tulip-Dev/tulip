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
#include <GL/glew.h>

#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlLabel.h>
#include <tulip/TlpTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipFontAwesome.h>

using namespace std;
using namespace tlp;

static const string defaultIcon = "question-circle";

static GlLabel label;

void drawFontAwesomeIcon(const std::string &iconName, const Color &color, const Color &outlineColor,
                         const float outlineSize, const std::string &texture) {
  label.setFontNameSizeAndColor(TulipFontAwesome::getFontAwesomeTrueTypeFileLocation(), 18, color);
  label.setPosition(Coord(0,0,0));
  label.setSize(Size(1,1,0));
  label.setUseLODOptimisation(false);
  label.setScaleToSize(true);
  label.setOutlineColor(outlineColor);
  label.setOutlineSize(outlineSize);
  label.setTextureName(texture);
  label.setText(TulipFontAwesome::getFontAwesomeIconUtf8String(iconName));
  label.draw(100);
}

class FontAwesomeIcon : public Glyph {

public:

  GLYPHINFORMATION("2D - Font Awesome Icon", "Antoine Lambert", "26/02/2015", "Font Awesome Icon", "1.0", NodeShape::FontAwesomeIcon)

  FontAwesomeIcon(const tlp::PluginContext* context = NULL) : Glyph(context) {}

  ~FontAwesomeIcon() {}

  void draw(node n, float) {
    StringProperty *viewFontAwesomeIcon = glGraphInputData->getElementFontAwesomeIcon();
    string iconName = viewFontAwesomeIcon->getNodeValue(n);

    if (iconName.empty() || !TulipFontAwesome::isFontAwesomeIconSupported(iconName)) {
      iconName = defaultIcon;
    }

    const tlp::Color &nodeColor = glGraphInputData->getElementColor()->getNodeValue(n);
    const tlp::Color &nodeBorderColor = glGraphInputData->getElementBorderColor()->getNodeValue(n);
    float nodeBorderWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);
    const std::string &nodeTexture = glGraphInputData->getElementTexture()->getNodeValue(n);

    drawFontAwesomeIcon(iconName, nodeColor, nodeBorderColor, nodeBorderWidth, nodeTexture);
  }

};

PLUGIN(FontAwesomeIcon)

class EEFontAwesomeIcon : public EdgeExtremityGlyph {

public:

  GLYPHINFORMATION("2D - Font Awesome Icon extremity", "David Auber", "02/03/2015", "Font Awesome Icon for edge extremities", "1.1", EdgeExtremityShape::FontAwesomeIcon)

  EEFontAwesomeIcon(const tlp::PluginContext* context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color& glyphColor,const Color &borderColor, float) {
    StringProperty *viewFontAwesomeIcon = edgeExtGlGraphInputData->getElementFontAwesomeIcon();
    string iconName = viewFontAwesomeIcon->getEdgeValue(e);

    if (iconName.empty() || !TulipFontAwesome::isFontAwesomeIconSupported(iconName)) {
      iconName = defaultIcon;
    }

    string edgeTexture = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);
    float borderWidth = edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e);

    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawFontAwesomeIcon(iconName, glyphColor, borderColor, borderWidth, edgeTexture);
  }
};

PLUGIN(EEFontAwesomeIcon)
