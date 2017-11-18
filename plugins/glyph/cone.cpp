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

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/DrawingTools.h>
#include <tulip/OpenGlConfigManager.h>

using namespace std;
using namespace tlp;

namespace tlp {

static vector<Coord> coneVertices;
static vector<Coord> coneNormals;
static vector<Vec2f> coneTexCoords;
static vector<unsigned short> coneIndices;
static vector<unsigned int> buffers;

static void drawCone() {
  if (coneVertices.empty()) {
    const unsigned int numberOfSides = 30;
    coneVertices = computeRegularPolygon(numberOfSides, Coord(0, 0, -0.5), Size(0.5, 0.5));
    coneVertices.push_back(Coord(0, 0, -0.5));
    coneVertices.push_back(Coord(0, 0, 0.5));

    for (size_t i = 0; i < coneVertices.size(); ++i) {
      coneTexCoords.push_back(Vec2f(coneVertices[i][0] + 0.5, coneVertices[i][1] + 0.5));
    }

    for (unsigned int i = 0; i < numberOfSides - 1; ++i) {
      coneIndices.push_back(numberOfSides);
      coneIndices.push_back(i + 1);
      coneIndices.push_back(i);
    }

    coneIndices.push_back(numberOfSides);
    coneIndices.push_back(0);
    coneIndices.push_back(numberOfSides - 1);

    for (unsigned int i = 0; i < numberOfSides - 1; ++i) {
      coneIndices.push_back(i);
      coneIndices.push_back(i + 1);
      coneIndices.push_back(numberOfSides + 1);
    }

    coneIndices.push_back(numberOfSides - 1);
    coneIndices.push_back(0);
    coneIndices.push_back(numberOfSides + 1);

    coneNormals = computeNormals(coneVertices, coneIndices);

    buffers.resize(4);
    glGenBuffers(4, &buffers[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, coneVertices.size() * 3 * sizeof(float), &coneVertices[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, coneNormals.size() * 3 * sizeof(float), &coneNormals[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, coneTexCoords.size() * 2 * sizeof(float), &coneTexCoords[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, coneIndices.size() * sizeof(unsigned short),
                 &coneIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
  glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
  glDrawElements(GL_TRIANGLES, coneIndices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/** \addtogroup glyph */

/// A 3D glyph.
/**
 * This glyph draws a textured cone using the "viewTexture" node
 * property value. If this property has no value, the cone is then colored
 * using the "viewColor" node property value.
 */
class Cone : public Glyph {
public:
  GLYPHINFORMATION("3D - Cone", "Bertrand Mathieu", "09/07/2002", "Textured cone", "1.0",
                   NodeShape::Cone)
  Cone(const tlp::PluginContext *context = NULL);
  virtual ~Cone();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox, node);
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord &vector) const;
};
PLUGIN(Cone)

Cone::Cone(const tlp::PluginContext *context) : Glyph(context) {}
Cone::~Cone() {}
void Cone::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.25, -0.25, 0);
  boundingBox[1] = Coord(0.25, 0.25, 0.5);
}
void Cone::draw(node n, float) {

  setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!texFile.empty()) {
    string texturePath = glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath + texFile);
  }

  drawCone();

  GlTextureManager::getInst().desactivateTexture();
}
Coord Cone::getAnchor(const Coord &vector) const {
  Coord anchor = vector;

  float x, y, z, n;
  anchor.get(x, y, z);
  n = sqrt(x * x + y * y);
  float vx0, vy0, vx1, vy1, x0, y0, x1, y1, px, py;
  x0 = 0;
  y0 = 0.5;
  vx0 = 0.5;
  vy0 = -1.0;
  x1 = 0;
  y1 = 0;
  vx1 = sqrt(x * x + y * y);
  vy1 = z;
  py = -1.0 * (vy1 * (vx0 / vy0 * y0 + x0 - x1) - vx1 * y1) / (vx1 - vy1 / vy0 * vx0);
  px = vx0 * (py - y0) / vy0 + x0;

  if (fabsf(py) > 0.5) {
    n = anchor.norm();
    py = n * 0.5 / fabsf(z);
    anchor.setX(x * py / n);
    anchor.setY(y * py / n);
    anchor.setZ(z * py / n);
  } else {
    anchor.setX(x * px / n);
    anchor.setY(y * px / n);
    anchor.setZ(py);
  }

  return anchor;
}

class EECone : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Cone extremity", "Bertrand Mathieu", "09/07/2002",
                   "Textured cone for edge extremities", "1.0", EdgeExtremityShape::Cone)

  EECone(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}
  virtual ~EECone() {}

  virtual void draw(edge e, node /*n*/, const Color &glyphColor, const Color & /*borderColor*/,
                    float /*lod*/) {
    glEnable(GL_LIGHTING);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    setMaterial(glyphColor);
    string texFile = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!texFile.empty()) {
      string texturePath = edgeExtGlGraphInputData->parameters->getTexturePath();
      GlTextureManager::getInst().activateTexture(texturePath + texFile);
    }

    drawCone();
    GlTextureManager::getInst().desactivateTexture();
  }
};

PLUGIN(EECone)

} // end of namespace tlp
