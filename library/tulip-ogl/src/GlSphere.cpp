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

#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlSphere.h>
#include <tulip/GlXMLTools.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlTools.h>

using namespace std;

namespace tlp {

GlSphere::GlSphere(const Coord &position, float radius, const Color &color, float rotX, float rotY,
                   float rotZ)
    : position(position), radius(radius), color(color), rot(rotX, rotY, rotZ) {
  boundingBox[0] = Coord(position[0] - radius, position[1] - radius, position[2] - radius);
  boundingBox[1] = Coord(position[0] + radius, position[1] + radius, position[2] + radius);
}

GlSphere::GlSphere(const Coord &position, float radius, const string &textureFile, int alpha,
                   float rotX, float rotY, float rotZ)
    : position(position), radius(radius), color(255, 255, 255, alpha), textureFile(textureFile),
      rot(rotX, rotY, rotZ) {
  boundingBox[0] = Coord(position[0] - radius, position[1] - radius, position[2] - radius);
  boundingBox[1] = Coord(position[0] + radius, position[1] + radius, position[2] + radius);
}

GlSphere::~GlSphere() {
  if (!buffers.empty())
    glDeleteBuffers(3, &buffers[0]);
}

void GlSphere::generateBuffers(int space) {
  verticesCount = (90 / space) * (360 / space) * 4;

  buffers.resize(3);

  glGenBuffers(3, &buffers[0]);

  double PI = 3.1415926535897;
  vertices.resize(verticesCount * 3 * 2);
  texturesCoord.resize(verticesCount * 2 * 2);
  indices.resize(verticesCount * 2);

  int n = 0;

  for (float j = 0; j <= 90 - space; j += space) {
    for (float i = 0; i <= 360 - space; i += space) {
      indices[n] = n;
      indices[n + 1] = n + 1;
      indices[n + 2] = n + 2;
      indices[n + 3] = n + 3;

      if (n)
        indices[verticesCount * 2 - n] = n + verticesCount;

      indices[verticesCount * 2 - n - 1] = n + verticesCount + 1;
      indices[verticesCount * 2 - n - 2] = n + verticesCount + 2;
      indices[verticesCount * 2 - n - 3] = n + verticesCount + 3;

      vertices[n * 3] = sin((i) / 180 * PI) * sin((j) / 180 * PI) * radius;
      vertices[n * 3 + 1] = cos((i) / 180 * PI) * sin((j) / 180 * PI) * radius;
      vertices[n * 3 + 2] = -cos((j) / 180 * PI) * radius;
      vertices[(verticesCount + n) * 3] = vertices[n * 3];
      vertices[(verticesCount + n) * 3 + 1] = vertices[n * 3 + 1];
      vertices[(verticesCount + n) * 3 + 2] = -vertices[n * 3 + 2];
      texturesCoord[n * 2] = 1 - ((i) / 360);
      texturesCoord[n * 2 + 1] = (2 * j) / 360;
      texturesCoord[(verticesCount + n) * 2] = texturesCoord[n * 2];
      texturesCoord[(verticesCount + n) * 2 + 1] = -texturesCoord[n * 2 + 1];
      n++;

      vertices[n * 3] = sin((i) / 180 * PI) * sin((j + space) / 180 * PI) * radius;
      vertices[n * 3 + 1] = cos((i) / 180 * PI) * sin((j + space) / 180 * PI) * radius;
      vertices[n * 3 + 2] = -cos((j + space) / 180 * PI) * radius;
      vertices[(verticesCount + n) * 3] = vertices[n * 3];
      vertices[(verticesCount + n) * 3 + 1] = vertices[n * 3 + 1];
      vertices[(verticesCount + n) * 3 + 2] = -vertices[n * 3 + 2];
      texturesCoord[n * 2] = 1 - (i) / 360;
      texturesCoord[n * 2 + 1] = (2 * (j + space)) / 360;
      texturesCoord[(verticesCount + n) * 2] = texturesCoord[n * 2];
      texturesCoord[(verticesCount + n) * 2 + 1] = -texturesCoord[n * 2 + 1];
      n++;

      vertices[n * 3] = sin((i + space) / 180 * PI) * sin((j) / 180 * PI) * radius;
      vertices[n * 3 + 1] = cos((i + space) / 180 * PI) * sin((j) / 180 * PI) * radius;
      vertices[n * 3 + 2] = -cos((j) / 180 * PI) * radius;
      vertices[(verticesCount + n) * 3] = vertices[n * 3];
      vertices[(verticesCount + n) * 3 + 1] = vertices[n * 3 + 1];
      vertices[(verticesCount + n) * 3 + 2] = -vertices[n * 3 + 2];
      texturesCoord[n * 2] = 1 - (i + space) / 360;
      texturesCoord[n * 2 + 1] = (2 * j) / 360;
      texturesCoord[(verticesCount + n) * 2] = texturesCoord[n * 2];
      texturesCoord[(verticesCount + n) * 2 + 1] = -texturesCoord[n * 2 + 1];
      n++;

      vertices[n * 3] = sin((i + space) / 180 * PI) * sin((j + space) / 180 * PI) * radius;
      vertices[n * 3 + 1] = cos((i + space) / 180 * PI) * sin((j + space) / 180 * PI) * radius;
      vertices[n * 3 + 2] = -cos((j + space) / 180 * PI) * radius;
      vertices[(verticesCount + n) * 3] = vertices[n * 3];
      vertices[(verticesCount + n) * 3 + 1] = vertices[n * 3 + 1];
      vertices[(verticesCount + n) * 3 + 2] = -vertices[n * 3 + 2];
      texturesCoord[n * 2] = 1 - (i + space) / 360;
      texturesCoord[n * 2 + 1] = (2 * (j + space)) / 360;
      texturesCoord[(verticesCount + n) * 2] = texturesCoord[n * 2];
      texturesCoord[(verticesCount + n) * 2 + 1] = -texturesCoord[n * 2 + 1];
      n++;
    }
  }

  indices[verticesCount] = verticesCount * 2 - 1;

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, texturesCoord.size() * sizeof(float), &texturesCoord[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GlSphere::draw(float, Camera *) {

  if (buffers.empty()) {
    generateBuffers(9);
  }

  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  glPushMatrix();
  glTranslatef(position[0], position[1], position[2]);
  glRotatef(rot[0], 1, 0, 0);
  glRotatef(rot[1], 0, 1, 0);
  glRotatef(rot[2], 0, 0, 1);

  if (textureFile != "") {
    GlTextureManager::getInst().activateTexture(textureFile);
  }

  setMaterial(color);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
  glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));

  if (textureFile != "") {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glDrawElements(GL_TRIANGLE_STRIP, verticesCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glDrawElements(GL_TRIANGLE_STRIP, verticesCount, GL_UNSIGNED_SHORT,
                 BUFFER_OFFSET(verticesCount * sizeof(unsigned short)));

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  if (textureFile != "")
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GlTextureManager::getInst().desactivateTexture();
  glPopMatrix();
}
//===========================================================
void GlSphere::translate(const Coord &vec) {
  boundingBox.translate(vec);
  position += vec;
}
//===========================================================
void GlSphere::getXML(string &outString) {
  GlXMLTools::createProperty(outString, "type", "GlSphere", "GlEntity");

  GlXMLTools::getXML(outString, "position", position);
  GlXMLTools::getXML(outString, "radius", radius);
  GlXMLTools::getXML(outString, "color", color);
  GlXMLTools::getXML(outString, "textureFile", textureFile);
  GlXMLTools::getXML(outString, "rotation", rot);
}
//============================================================
void GlSphere::setWithXML(const string &inString, unsigned int &currentPosition) {

  GlXMLTools::setWithXML(inString, currentPosition, "position", position);
  GlXMLTools::setWithXML(inString, currentPosition, "radius", radius);
  GlXMLTools::setWithXML(inString, currentPosition, "color", color);
  GlXMLTools::setWithXML(inString, currentPosition, "textureFile", textureFile);
  GlXMLTools::setWithXML(inString, currentPosition, "rotation", rot);

  boundingBox[0] = Coord(position[0] - radius, position[1] - radius, position[2] - radius);
  boundingBox[1] = Coord(position[0] + radius, position[1] + radius, position[2] + radius);
}
}
