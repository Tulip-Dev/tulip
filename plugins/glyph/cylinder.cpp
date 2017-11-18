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
#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/DrawingTools.h>
#include <tulip/OpenGlConfigManager.h>

using namespace std;
using namespace tlp;

namespace tlp {

class CylinderBase {

public:
  void drawCylinder(float cylinderHeight = 1.0, float dz = 0) {
    if (cylinderVertices.empty()) {
      const unsigned int numberOfSides = 30;
      cylinderVertices = computeRegularPolygon(numberOfSides, Coord(0, 0, -cylinderHeight / 2 + dz),
                                               Size(0.5, 0.5));
      cylinderVertices.push_back(Coord(0, 0, -cylinderHeight / 2 + dz));
      vector<Coord> tmp = computeRegularPolygon(numberOfSides, Coord(0, 0, cylinderHeight / 2 + dz),
                                                Size(0.5, 0.5));
      cylinderVertices.insert(cylinderVertices.end(), tmp.begin(), tmp.end());
      cylinderVertices.push_back(Coord(0, 0, cylinderHeight / 2 + dz));

      for (size_t i = 0; i < cylinderVertices.size(); ++i) {
        cylinderTexCoords.push_back(
            Vec2f(cylinderVertices[i][0] + 0.5, cylinderVertices[i][1] + 0.5));
      }

      size_t startIdx = cylinderVertices.size();

      tmp = computeRegularPolygon(numberOfSides, Coord(0, 0, -cylinderHeight / 2 + dz),
                                  Size(0.5, 0.5));
      cylinderVertices.insert(cylinderVertices.end(), tmp.begin(), tmp.end());
      tmp = computeRegularPolygon(numberOfSides, Coord(0, 0, cylinderHeight / 2 + dz),
                                  Size(0.5, 0.5));
      cylinderVertices.insert(cylinderVertices.end(), tmp.begin(), tmp.end());

      for (size_t i = startIdx; i < cylinderVertices.size(); ++i) {
        cylinderTexCoords.push_back(
            Vec2f(cylinderVertices[i][0] + 0.5, cylinderVertices[i][2] + 0.5));
      }

      for (unsigned int i = 0; i < numberOfSides - 1; ++i) {
        cylinderIndices.push_back(numberOfSides);
        cylinderIndices.push_back(i + 1);
        cylinderIndices.push_back(i);
      }

      cylinderIndices.push_back(numberOfSides);
      cylinderIndices.push_back(0);
      cylinderIndices.push_back(numberOfSides - 1);

      for (unsigned int i = 0; i < numberOfSides - 1; ++i) {
        cylinderIndices.push_back(numberOfSides + 1 + i);
        cylinderIndices.push_back(numberOfSides + 1 + i + 1);
        cylinderIndices.push_back(2 * numberOfSides);
      }

      cylinderIndices.push_back(2 * numberOfSides);
      cylinderIndices.push_back(numberOfSides + 1);
      cylinderIndices.push_back(numberOfSides + 1 + numberOfSides - 1);

      for (unsigned int i = 0; i < numberOfSides - 1; ++i) {
        cylinderIndices.push_back(startIdx + i);
        cylinderIndices.push_back(startIdx + i + 1);
        cylinderIndices.push_back(startIdx + numberOfSides + i);

        cylinderIndices.push_back(startIdx + i + 1);
        cylinderIndices.push_back(startIdx + numberOfSides + i + 1);
        cylinderIndices.push_back(startIdx + numberOfSides + i);
      }

      cylinderIndices.push_back(startIdx + numberOfSides - 1);
      cylinderIndices.push_back(startIdx);
      cylinderIndices.push_back(startIdx + numberOfSides);

      cylinderIndices.push_back(startIdx + numberOfSides);
      cylinderIndices.push_back(startIdx + numberOfSides + numberOfSides - 1);
      cylinderIndices.push_back(startIdx + numberOfSides - 1);

      cylinderNormals = computeNormals(cylinderVertices, cylinderIndices);

      buffers.resize(4);
      glGenBuffers(4, &buffers[0]);

      glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
      glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * 3 * sizeof(float),
                   &cylinderVertices[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, cylinderNormals.size() * 3 * sizeof(float), &cylinderNormals[0],
                   GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
      glBufferData(GL_ARRAY_BUFFER, cylinderTexCoords.size() * 2 * sizeof(float),
                   &cylinderTexCoords[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(unsigned short),
                   &cylinderIndices[0], GL_STATIC_DRAW);
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
    glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

private:
  vector<Coord> cylinderVertices;
  vector<Coord> cylinderNormals;
  vector<Vec2f> cylinderTexCoords;
  vector<unsigned short> cylinderIndices;
  vector<unsigned int> buffers;
};

/** \addtogroup glyph */
/// A 3D glyph.
/**
 * This glyph draws an textured cylinder using the "viewTexture" node
 * property value. If this property has no value, the cylinder is then colored
 * using the "viewColor" node property value.
 */
class Cylinder : public Glyph, public CylinderBase {
public:
  GLYPHINFORMATION("3D - Cylinder", "Bertrand Mathieu", "31/07/2002", "Textured Cylinder", "1.0",
                   NodeShape::Cylinder)
  Cylinder(const tlp::PluginContext *context = NULL);
  virtual ~Cylinder();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox, node);
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord &vector) const;
};
PLUGIN(Cylinder)

Cylinder::Cylinder(const tlp::PluginContext *context) : Glyph(context) {}
Cylinder::~Cylinder() {}
void Cylinder::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 1);
}
void Cylinder::draw(node n, float) {

  setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!texFile.empty()) {
    string texturePath = glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath + texFile);
  }

  drawCylinder();

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
    z = -0.5f; // z = z >? -0.5f;

  if (z > +0.5f)
    z = +0.5f; // z = z <? +0.5f;

  return Coord(x, y, z);
}

/** \addtogroup glyph */
/// A 3D glyph.
/**
 * This glyph draws an textured a half cylinder - a cylinder whose height
 * is half the one draws by the Cylinder glyph - using the "viewTexture"
 * node property value. If this property has no value, the half cylinder
 * is then colored using the "viewColor" node property value.
 */

//=================================================================================================
class HalfCylinder : public Glyph, public CylinderBase {
public:
  GLYPHINFORMATION("3D - Half Cylinder", "Auber David", "31/07/2002", "Textured HalfCylinder",
                   "1.0", NodeShape::HalfCylinder)
  HalfCylinder(const tlp::PluginContext *context = NULL);
  virtual ~HalfCylinder();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox, node);
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord &vector) const;

private:
};

PLUGIN(HalfCylinder)
//=================================================================================================
HalfCylinder::HalfCylinder(const tlp::PluginContext *context) : Glyph(context) {}
//=================================================================================================
HalfCylinder::~HalfCylinder() {}
//=====================================================
void HalfCylinder::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 1);
}
//=================================================================================================
void HalfCylinder::draw(node n, float) {

  setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!texFile.empty()) {
    string texturePath = glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath + texFile);
  }

  drawCylinder(0.5, 0.25);

  GlTextureManager::getInst().desactivateTexture();
}
//=================================================================================================
Coord HalfCylinder::getAnchor(const Coord &vector) const {
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

  if (z < 0.0f)
    z = 0.0f; // z = z >? 0.0f;

  if (z > 0.5f)
    z = 0.5f; // z = z <? 0.5f;

  return Coord(x, y, z);
}

class EECylinder : public EdgeExtremityGlyph, public CylinderBase {
public:
  GLYPHINFORMATION("3D - Cylinder extremity", "Bertrand Mathieu", "31/07/2002",
                   "Textured Cylinder for edge extremities", "1.0", EdgeExtremityShape::Cylinder)

  EECylinder(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge, node n, const Color &glyphColor, const Color &, float) {
    glEnable(GL_LIGHTING);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    setMaterial(glyphColor);
    string texFile = edgeExtGlGraphInputData->getElementTexture()->getNodeValue(n);

    if (!texFile.empty()) {
      string texturePath = edgeExtGlGraphInputData->parameters->getTexturePath();
      GlTextureManager::getInst().activateTexture(texturePath + texFile);
    }

    drawCylinder();

    GlTextureManager::getInst().desactivateTexture();
  }
};

PLUGIN(EECylinder)

} // end of namespace tlp
