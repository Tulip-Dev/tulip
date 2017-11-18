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

#include <GL/glew.h>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/DrawingTools.h>
#include <tulip/OpenGlConfigManager.h>

using namespace std;
using namespace tlp;

namespace tlp {

static vector<Coord> ringVertices;
static vector<Vec2f> ringTexCoords;
static vector<unsigned short> ringIndices;
static vector<unsigned short> ringOutlineIndices;
static vector<unsigned int> buffers;

static void drawRing() {
  if (ringVertices.empty()) {
    const unsigned int numberOfSides = 30;
    ringVertices = computeRegularPolygon(30, Coord(0, 0, 0), Size(0.5, 0.5));
    vector<Coord> tmp = computeRegularPolygon(numberOfSides, Coord(0, 0, 0), Size(0.25, 0.25));
    ringVertices.insert(ringVertices.end(), tmp.begin(), tmp.end());

    for (unsigned int i = 0; i < numberOfSides - 1; ++i) {
      ringIndices.push_back(i);
      ringIndices.push_back(i + 1);
      ringIndices.push_back(numberOfSides + i);

      ringIndices.push_back(numberOfSides + i);
      ringIndices.push_back(i + 1);
      ringIndices.push_back(numberOfSides + i + 1);

      ringOutlineIndices.push_back(i);
      ringOutlineIndices.push_back(i + 1);

      ringOutlineIndices.push_back(numberOfSides + i);
      ringOutlineIndices.push_back(numberOfSides + i + 1);
    }

    ringIndices.push_back(numberOfSides - 1);
    ringIndices.push_back(0);
    ringIndices.push_back(numberOfSides + numberOfSides - 1);

    ringIndices.push_back(numberOfSides);
    ringIndices.push_back(0);
    ringIndices.push_back(numberOfSides + numberOfSides - 1);

    ringOutlineIndices.push_back(numberOfSides - 1);
    ringOutlineIndices.push_back(0);

    ringOutlineIndices.push_back(numberOfSides + numberOfSides - 1);
    ringOutlineIndices.push_back(numberOfSides);

    for (size_t i = 0; i < ringVertices.size(); ++i) {
      ringTexCoords.push_back(Vec2f(ringVertices[i][0] + 0.5, ringVertices[i][1] + 0.5));
    }

    buffers.resize(4);
    glGenBuffers(4, &buffers[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, ringVertices.size() * 3 * sizeof(float), &ringVertices[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, ringTexCoords.size() * 2 * sizeof(float), &ringTexCoords[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ringIndices.size() * sizeof(unsigned short),
                 &ringIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ringOutlineIndices.size() * sizeof(unsigned short),
                 &ringOutlineIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  glEnableClientState(GL_VERTEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glDrawElements(GL_TRIANGLES, ringIndices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void drawRingBorder() {
  glEnableClientState(GL_VERTEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
  glDrawElements(GL_LINES, ringOutlineIndices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  glDisableClientState(GL_VERTEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void drawGlyph(const Color &glyphColor, const string &texture, const string &texturePath,
                      double borderWidth, const Color &borderColor, float lod) {

  setMaterial(glyphColor);

  if (texture != "") {
    GlTextureManager::getInst().activateTexture(texturePath + texture);
  }

  drawRing();

  GlTextureManager::getInst().desactivateTexture();

  if (lod > 20 && borderWidth > 0) {
    glLineWidth(borderWidth);
    glDisable(GL_LIGHTING);
    setColor(borderColor);
    drawRingBorder();
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
class Ring : public Glyph {
public:
  GLYPHINFORMATION("2D - Ring", "David Auber", "09/07/2002", "Textured Ring", "1.0",
                   NodeShape::Ring)
  Ring(const tlp::PluginContext *context = NULL);
  virtual ~Ring();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox, node);
  virtual string getName() {
    return string("Ring");
  }
  virtual void draw(node n, float lod);
};
PLUGIN(Ring)
Ring::Ring(const tlp::PluginContext *context) : Glyph(context) {}
Ring::~Ring() {}
void Ring::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
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

class EERing : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Ring extremity", "David Auber", "09/07/2002",
                   "Textured Ring for edge extremities", "1.0", EdgeExtremityShape::Ring)

  EERing(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) {
    glDisable(GL_LIGHTING);
    drawGlyph(glyphColor, edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
              edgeExtGlGraphInputData->parameters->getTexturePath(),
              edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), borderColor, lod);
  }
};
PLUGIN(EERing)

} // end of namespace tlp
