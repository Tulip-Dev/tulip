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
#include <unordered_map>
#include <GL/glew.h>

#include <tulip/BoundingBox.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlTools.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TulipIconicFont.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

struct FontIcon {

  string fontFile;
  unsigned int iconCodePoint;
  GLuint renderingDataBuffer;
  GLuint indicesBuffer;
  unsigned int nbVertices;
  unsigned int nbIndices;
  unsigned int nbOutlineIndices;
  BoundingBox boundingBox;

public:
  FontIcon()
      : iconCodePoint(0), renderingDataBuffer(0), indicesBuffer(0), nbVertices(0), nbIndices(0),
        nbOutlineIndices(0) {}

  FontIcon(const std::string &iconName)
      : fontFile(TulipIconicFont::getTTFLocation(iconName)),
        iconCodePoint(TulipIconicFont::getIconCodePoint(iconName)), renderingDataBuffer(0),
        indicesBuffer(0), nbVertices(0), nbIndices(0), nbOutlineIndices(0) {}

  ~FontIcon() {
    if (renderingDataBuffer != 0) {
      glDeleteBuffers(1, &renderingDataBuffer);
    }

    if (indicesBuffer != 0) {
      glDeleteBuffers(1, &indicesBuffer);
    }
  }

  void render(const tlp::Color &fillColor, const tlp::Color &outlineColor,
              const float outlineSize) {

    if (renderingDataBuffer == 0) {
      tesselateFontIcon(fontFile, iconCodePoint, renderingDataBuffer, indicesBuffer, nbVertices,
                        nbIndices, nbOutlineIndices, boundingBox);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, renderingDataBuffer);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(nbVertices * 3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    setMaterial(fillColor);
    glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if (outlineSize > 0) {
      setMaterial(outlineColor);
      glLineWidth(outlineSize);
      glDrawElements(GL_LINES, nbOutlineIndices, GL_UNSIGNED_SHORT,
                     BUFFER_OFFSET(nbIndices * sizeof(unsigned short)));
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  const BoundingBox &getBoundingBox() const {
    return boundingBox;
  }
};

static FontIcon defaultFontIcon;
static unordered_map<string, FontIcon> fontIcons;

static FontIcon &getFontIcon(const string &iconName) {
  if (iconName.empty() || !TulipIconicFont::isIconSupported(iconName)) {
    // initialization of defaultFontIcon is delayed
    if (defaultFontIcon.iconCodePoint == 0) {
      static const std::string defaultIconName = "fa-question-circle";
      defaultFontIcon.iconCodePoint = TulipIconicFont::getIconCodePoint(defaultIconName);
      defaultFontIcon.fontFile = TulipIconicFont::getTTFLocation(defaultIconName);
    }
    return defaultFontIcon;
  }
  auto it = fontIcons.find(iconName);
  if (it == fontIcons.end())
    it = fontIcons.emplace(iconName, iconName).first;
  return it->second;
}

static void drawIcon(FontIcon &fontIcon, const Color &color, const Color &outlineColor,
                     const float outlineSize, const string &texture) {
  if (!texture.empty()) {
    GlTextureManager::activateTexture(texture);
  }

  fontIcon.render(color, outlineColor, outlineSize);

  GlTextureManager::deactivateTexture();
}

class FontIconGlyph : public Glyph {

public:
  GLYPHINFORMATION("2D - Icon", "Antoine Lambert", "26/02/2015", "Icon", "2.0", NodeShape::Icon)

  FontIconGlyph(const tlp::PluginContext *context = nullptr) : Glyph(context) {}

  ~FontIconGlyph() override {}

  void draw(node n, float) override {
    const tlp::Color &nodeColor = glGraphInputData->getElementColor()->getNodeValue(n);
    const tlp::Color &nodeBorderColor = glGraphInputData->getElementBorderColor()->getNodeValue(n);
    float nodeBorderWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);
    const string &nodeTexture = glGraphInputData->parameters->getTexturePath() +
                                glGraphInputData->getElementTexture()->getNodeValue(n);

    drawIcon(getNodeFontIcon(n), nodeColor, nodeBorderColor, nodeBorderWidth, nodeTexture);
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox, node n) override {
    boundingBox = getNodeFontIcon(n).getBoundingBox();
  }

private:
  FontIcon &getNodeFontIcon(node n) {
    StringProperty *viewIcon = glGraphInputData->getElementIcon();
    const string &iconName = viewIcon->getNodeValue(n);
    return getFontIcon(iconName);
  }
};

PLUGIN(FontIconGlyph)

class EEFontIconGlyph : public EdgeExtremityGlyph {

public:
  GLYPHINFORMATION("2D - Icon extremity", "Antoine Lambert", "02/03/2015",
                   "Icon for edge extremities", "2.0", EdgeExtremityShape::Icon)

  EEFontIconGlyph(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node n, const Color &glyphColor, const Color &borderColor, float) override {
    StringProperty *viewIcon = edgeExtGlGraphInputData->getElementIcon();
    const string &iconName = viewIcon->getEdgeValue(e);

    string edgeTexture = edgeExtGlGraphInputData->parameters->getTexturePath() +
                         edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);
    float borderWidth = edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e);

    // apply some rotation before rendering the icon in order
    // to visually encode the edge direction
    if (edgeExtGlGraphInputData->getGraph()->source(e) == n) {
      // anchor the bottom of the icon to the source node
      glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    } else {
      // anchor the top of the icon to the target node
      glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    }

    // icon must be mirrored along its Y axis to get a correct rendering
    glScalef(-1.0f, 1.0f, 1.0f);

    drawIcon(getFontIcon(iconName), glyphColor, borderColor, borderWidth, edgeTexture);
  }
};

PLUGIN(EEFontIconGlyph)
