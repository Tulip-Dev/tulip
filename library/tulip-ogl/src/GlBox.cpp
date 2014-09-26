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

#include <tulip/GlBox.h>
#include <tulip/GlXMLTools.h>
#include <tulip/GlTools.h>
#include <tulip/GlTextureManager.h>
#include <tulip/OpenGlConfigManager.h>

using namespace std;

namespace tlp {

//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
static GLfloat cubeTexArrays[] = {
  //***
  1.0f, 1.0f, // v0
  0.0f, 1.0f, // v1
  0.0f, 0.0f, // v2
  1.0f, 0.0f, // v3
  //***
  1.0f, 1.0f, // v0
  0.0f, 1.0f, // v3
  0.0f, 0.0f, // v4
  1.0f, 0.0f, // v5
  //***
  1.0f, 1.0f, // v0
  0.0f, 1.0f, // v5
  0.0f, 0.0f, // v6
  1.0f, 0.0f, // v1
  //***
  1.0f, 1.0f, // v1
  0.0f, 1.0f, // v6
  0.0f, 0.0f, // v7
  1.0f, 0.0f, // v2
  //***
  1.0f, 1.0f, // v7
  0.0f, 1.0f, // v4
  0.0f, 0.0f, // v3
  1.0f, 0.0f, // v2
  //***
  1.0f, 1.0f, // v4
  0.0f, 1.0f, // v7
  0.0f, 0.0f, // v6
  1.0f, 0.0f
}; // v5

static GLfloat cubeNormalArrays[] = {
  //***
  0.0f, 0.0f, 1.0f, // v0
  0.0f, 0.0f, 1.0f, // v1
  0.0f, 0.0f, 1.0f, // v2
  0.0f, 0.0f, 1.0f, // v3
  //***
  1.0f, 0.0f, 0.0f, // v0
  1.0f, 0.0f, 0.0f, // v3
  1.0f, 0.0f, 0.0f, // v4
  1.0f, 0.0f, 0.0f, // v5
  //***
  0.0f, 1.0f, 0.0f, // v0
  0.0f, 1.0f, 0.0f, // v5
  0.0f, 1.0f, 0.0f, // v6
  0.0f, 1.0f, 0.0f, // v1
  //***
  -1.0f, 0.0f, 0.0f, // v1
  -1.0f, 0.0f, 0.0f, // v6
  -1.0f, 0.0f, 0.0f, // v7
  -1.0f, 0.0f, 0.0f, // v2
  //***
  0.0f, -1.0f, 0.0f, // v7
  0.0f, -1.0f, 0.0f, // v4
  0.0f, -1.0f, 0.0f, // v3
  0.0f, -1.0f, 0.0f, // v2
  //***
  0.0f, 0.0f, -1.0f, // v4
  0.0f, 0.0f, -1.0f, // v7
  0.0f, 0.0f, -1.0f, // v6
  0.0f, 0.0f, -1.0f
}; // v5

static GLfloat cubeCoordArrays[] = {
  //***
  0.5f, 0.5f, 0.5f, // v0
  -0.5f, 0.5f, 0.5f, // v1
  -0.5f, -0.5f, 0.5f, // v2
  0.5f, -0.5f, 0.5f, // v3
  //***
  0.5f, 0.5f, 0.5f, // v0
  0.5f, -0.5f, 0.5f, // v3
  0.5f, -0.5f, -0.5f, // v4
  0.5f, 0.5f, -0.5f, // v5
  //***
  0.5f, 0.5f, 0.5f, // v0
  0.5f, 0.5f, -0.5f, // v5
  -0.5f, 0.5f, -0.5f, // v6
  -0.5f, 0.5f, 0.5f, // v1
  //***
  -0.5f, 0.5f, 0.5f, // v1
  -0.5f, 0.5f, -0.5f, // v6
  -0.5f, -0.5f, -0.5f, // v7
  -0.5f, -0.5f, 0.5f, // v2
  //***
  -0.5f, -0.5f, -0.5f, // v7
  0.5f, -0.5f, -0.5f, // v4
  0.5f, -0.5f, 0.5f, // v3
  -0.5f, -0.5f, 0.5f, // v2
  //***
  0.5f, -0.5f, -0.5f, // v4
  -0.5f, -0.5f, -0.5f, // v7
  -0.5f, 0.5f, -0.5f, // v6
  0.5f, 0.5f, -0.5f
}; // v5

static GLubyte cubeIndices[] = {
  0,1,2,3,
  4,5,6,7,
  8,9,10,11,
  12,13,14,15,
  16,17,18,19,
  20,21,22,23
};

static GLubyte cubeOutlineIndices[] = {
  0,1,1,2,2,3,3,0,
  20,21,21,22,22,23,23,20,
  0,23,1,22,2,21,3,20
};

//===========================================================
//===========================================================
//===========================================================
//===========================================================
//===========================================================

GlBox::GlBox() {
  // no default constructor :)
}
//===========================================================
GlBox::GlBox(const Coord& position, const Size &size, const Color& fillColor, const Color &outlineColor,bool filled, bool outlined,const string &textureName,float outlineSize)
  :position(position),size(size),filled(filled),outlined(outlined),textureName(textureName),outlineSize(outlineSize),newCubeCoordArrays(NULL),generated(false) {
  if(filled)
    fillColors.push_back(fillColor);

  if(outlined)
    outlineColors.push_back(outlineColor);

  boundingBox.expand(position-size/2.f);
  boundingBox.expand(position+size/2.f);
}
//===========================================================
GlBox::~GlBox() {
  clearGenerated();
}
//===========================================================
void GlBox::draw(float lod,Camera *) {

  bool canUseVBO=OpenGlConfigManager::getInst().hasVertexBufferObject();

  if(canUseVBO) {
    if(!generated) {
      GLfloat newCubeCoordArrays[72];

      for(unsigned int i=0; i<24; ++i) {
        newCubeCoordArrays[i*3]=cubeCoordArrays[i*3]*size[0]+position[0];
        newCubeCoordArrays[i*3+1]=cubeCoordArrays[i*3+1]*size[1]+position[1];
        newCubeCoordArrays[i*3+2]=cubeCoordArrays[i*3+2]*size[2]+position[2];
      }

      glGenBuffers(5,buffers);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(newCubeCoordArrays),newCubeCoordArrays, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormalArrays),cubeNormalArrays, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexArrays),cubeTexArrays, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeOutlineIndices), cubeOutlineIndices, GL_STATIC_DRAW);
      generated=true;
    }
  }
  else {
    if(!generated) {
      newCubeCoordArrays= new GLfloat[72];

      for(unsigned int i=0; i<24; ++i) {
        newCubeCoordArrays[i*3]=cubeCoordArrays[i*3]*size[0]+position[0];
        newCubeCoordArrays[i*3+1]=cubeCoordArrays[i*3+1]*size[1]+position[1];
        newCubeCoordArrays[i*3+2]=cubeCoordArrays[i*3+2]*size[2]+position[2];
      }

      generated=true;
    }
  }

  glEnable(GL_LIGHTING);

  glEnableClientState(GL_VERTEX_ARRAY);

  if(canUseVBO) {
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), BUFFER_OFFSET(0));
  }
  else {
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), newCubeCoordArrays);
  }

  if(filled) {
    setMaterial(fillColors[0]);

    glEnableClientState(GL_NORMAL_ARRAY);

    if(canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), BUFFER_OFFSET(0));
    }
    else {
      glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), cubeNormalArrays);
    }

    if(textureName!="") {
      GlTextureManager::getInst().activateTexture(textureName);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      if(canUseVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glTexCoordPointer(2,GL_FLOAT, 2*sizeof(GLfloat), BUFFER_OFFSET(0));
      }
      else {
        glTexCoordPointer(2,GL_FLOAT, 2*sizeof(GLfloat), cubeTexArrays);
      }
    }

    if(canUseVBO) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
      glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    }
    else {
      glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
    }

    glDisableClientState(GL_NORMAL_ARRAY);

    if(textureName!="") {
      GlTextureManager::getInst().desactivateTexture();
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

  if(outlined && outlineSize!=0) {
    if((outlineSize<1 && lod>=20) || (lod>(20/outlineSize))) {
      glDisable(GL_LIGHTING);

      glColor4ub(outlineColors[0][0],outlineColors[0][1],outlineColors[0][2],outlineColors[0][3]);
      glLineWidth(outlineSize);

      if(canUseVBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
      }
      else {
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, cubeOutlineIndices);
      }

      glEnable(GL_LIGHTING);
    }
  }

  glDisableClientState(GL_VERTEX_ARRAY);

  if(canUseVBO) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}
//===========================================================
void GlBox::setSize(const Size& size) {
  this->size = size;

  boundingBox = BoundingBox();
  boundingBox.expand(position-size/2.f);
  boundingBox.expand(position+size/2.f);

  clearGenerated();
}
//===========================================================
void GlBox::setPosition(const Coord& position) {
  this->position = position;

  boundingBox = BoundingBox();
  boundingBox.expand(position-size/2.f);
  boundingBox.expand(position+size/2.f);

  clearGenerated();
}
//===========================================================
Size GlBox::getSize() const {
  return size;
}
//===========================================================
Color GlBox::getFillColor() const {
  return fillColors[0];
}
//===========================================================
void GlBox::setFillColor(const Color& color) {
  fillColors.clear();
  fillColors.push_back(color);
}
//===========================================================
Color GlBox::getOutlineColor() const {
  return outlineColors[0];
}
//===========================================================
void GlBox::setOutlineColor(const Color& color) {
  outlineColors.clear();
  outlineColors.push_back(color);
}
//===========================================================
float GlBox::getOutlineSize() const {
  return outlineSize;
}
//===========================================================
void GlBox::setOutlineSize(float size) {
  outlineSize=size;
}
//===========================================================
string GlBox::getTextureName() const {
  return textureName;
}
//===========================================================
void GlBox::setTextureName(const string& textureName) {
  this->textureName=textureName;
}
//===========================================================
void GlBox::translate(const Coord &mouvement) {
  boundingBox.translate(mouvement);

  position+=mouvement;

  clearGenerated();
}
//===========================================================
void GlBox::getXML(string &outString) {

  GlXMLTools::createProperty(outString,"type","GlBox","GlEntity");

  GlXMLTools::getXML(outString,"position",position);
  GlXMLTools::getXML(outString,"size",size);
  GlXMLTools::getXML(outString,"fillColors",fillColors);
  GlXMLTools::getXML(outString,"outlineColors",outlineColors);
  GlXMLTools::getXML(outString,"filled",filled);
  GlXMLTools::getXML(outString,"outlined",outlined);
  GlXMLTools::getXML(outString,"textureName",textureName);
  GlXMLTools::getXML(outString,"outlineSize",outlineSize);
}
//============================================================
void GlBox::setWithXML(const string &inString, unsigned int &currentPosition) {

  GlXMLTools::setWithXML(inString, currentPosition,"position",position);
  GlXMLTools::setWithXML(inString, currentPosition,"size",size);
  fillColors.clear();
  GlXMLTools::setWithXML(inString, currentPosition,"fillColors",fillColors);
  outlineColors.clear();
  GlXMLTools::setWithXML(inString, currentPosition,"outlineColors",outlineColors);
  GlXMLTools::setWithXML(inString, currentPosition,"filled",filled);
  GlXMLTools::setWithXML(inString, currentPosition,"outlined",outlined);
  GlXMLTools::setWithXML(inString, currentPosition,"textureName",textureName);
  GlXMLTools::setWithXML(inString, currentPosition,"outlineSize",outlineSize);

  boundingBox = BoundingBox();
  boundingBox.expand(position-size/2.f);
  boundingBox.expand(position+size/2.f);
}
//============================================================
void GlBox::clearGenerated() {
  delete[] newCubeCoordArrays;
  newCubeCoordArrays=NULL;

  if(OpenGlConfigManager::getInst().hasVertexBufferObject()) {
    if(generated)
      glDeleteBuffers(5,buffers);
  }

  generated=false;
}
}
