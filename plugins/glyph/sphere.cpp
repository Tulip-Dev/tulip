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

#include <tulip/OpenGlConfigManager.h>

#include <iostream>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GLuint buffers[] = { 0, 0, 0 };

static GLfloat *vertex;
static GLfloat *texturesCoord;
static GLushort *indices;

static void generateBuffers(int space) {
  int vertexCount = (90 / space) * (360 / space) * 4;

  glGenBuffers(3, buffers);

  double PI = 3.1415926535897;
  vertex = new GLfloat[vertexCount * 3* 2 ];
  texturesCoord= new GLfloat[vertexCount*2*2];
  indices = new GLushort[vertexCount*2*2];

  int n;

  n = 0;

  for(float j = 0; j <= 90 - space; j+=space) {
    for(float i = 0; i <= 360 - space; i+=space) {
      indices[n]=n;
      indices[n+1]=n+1;
      indices[n+2]=n+2;
      indices[n+3]=n+3;
      indices[vertexCount*2-n]=n+vertexCount;
      indices[vertexCount*2-n-1]=n+vertexCount+1;
      indices[vertexCount*2-n-2]=n+vertexCount+2;
      indices[vertexCount*2-n-3]=n+vertexCount+3;

      vertex[n*3] = sin((i) / 180 * PI) * sin((j) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i) / 180 * PI) * sin((j) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-((i) / 360);
      texturesCoord[n*2+1] = (2 * j) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;

      vertex[n*3] = sin((i) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j + space) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-(i) / 360;
      texturesCoord[n*2+1] = (2 * (j + space)) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;

      vertex[n*3] = sin((i + space) / 180 * PI) * sin((j) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i + space) / 180 * PI) * sin((j) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-(i + space) / 360;
      texturesCoord[n*2+1] = (2 * j) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;

      vertex[n*3] = sin((i + space) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i + space) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j + space) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-(i + space) / 360;
      texturesCoord[n*2+1] = (2 * (j + space)) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;
    }
  }

  indices[vertexCount]=vertexCount*2-1;

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, vertexCount*3*2*sizeof(GLfloat),vertex, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, vertexCount*2*2*sizeof(GLfloat),texturesCoord, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount*2*sizeof(GLushort), indices, GL_STATIC_DRAW);
}
static void drawGlyph(const Color& glyphColor, const string& texture,
                      const string& texturePath, float) {
  bool canUseVBO = OpenGlConfigManager::getInst().hasVertexBufferObject();

  int space = 9;
  int vertexCount = (90 / space) * (360 / space) * 4;

  if (canUseVBO) {
    if (buffers[0] == 0) {
      generateBuffers(space);
    }
  }
  else {
    if (GlDisplayListManager::getInst().beginNewDisplayList("Sphere_sphere")) {
      GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluQuadricNormals(quadratic, GLU_SMOOTH);
      gluQuadricTexture(quadratic, GL_TRUE);
      gluSphere(quadratic, 0.5f, 30, 30);
      GlDisplayListManager::getInst().endNewDisplayList();
      gluDeleteQuadric(quadratic);
    }
  }

  tlp::setMaterial(glyphColor);

  if (texture.size() != 0) {
    GlTextureManager::getInst().activateTexture(texturePath + texture);
  }

  if (canUseVBO) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));

    if (texture.size() != 0) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glDrawElements(GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_SHORT,BUFFER_OFFSET(0));
    glDrawElements(GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_SHORT,BUFFER_OFFSET(vertexCount*sizeof(GLushort)));

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (texture.size() != 0)
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  else {
    GlDisplayListManager::getInst().callDisplayList("Sphere_sphere");
  }

  GlTextureManager::getInst().desactivateTexture();
}

/** \addtogroup glyph */

/// A 3D glyph.
/**
 * This glyph draws a textured sphere using the "viewTexture" node
 * property value. If this property has no value, the sphere
 * is then colored using the "viewColor" node property value.
 */
class Sphere: public Glyph {
public:
  GLYPHINFORMATION("3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1.0", NodeShape::Sphere)
  Sphere(const tlp::PluginContext *context = NULL);
  virtual ~Sphere();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n, float lod);
};

PLUGIN(Sphere)

//=========================================================================================
Sphere::Sphere(const tlp::PluginContext* context) :
  Glyph(context) {
}

Sphere::~Sphere() {
}

void Sphere::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, -0.35f);
  boundingBox[1] = Coord(0.35f, 0.35f, 0.35f);
}

void Sphere::draw(node n, float lod) {
  drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
            glGraphInputData->getElementTexture()->getNodeValue(n),
            glGraphInputData->parameters->getTexturePath(), lod);
}

class EESphere: public EdgeExtremityGlyph {
  GLYPHINFORMATION("3D - Sphere extremity", "Bertrand Mathieu", "09/07/2002", "Textured sphere for edge extremities", "1.0", EdgeExtremityShape::Sphere)
public:
  EESphere(const tlp::PluginContext *context): EdgeExtremityGlyph(context) {

  }
  virtual ~EESphere() {

  }
  virtual void draw(edge e, node, const Color& glyphColor,const Color&, float lod) {
    glEnable(GL_LIGHTING);
    drawGlyph(glyphColor,
              edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
              edgeExtGlGraphInputData->parameters->getTexturePath(), lod);
  }
};
PLUGIN(EESphere)

} // end of namespace tlp
