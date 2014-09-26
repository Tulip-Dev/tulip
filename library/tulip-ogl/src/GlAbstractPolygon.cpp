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

#include <tulip/GlAbstractPolygon.h>
#include <tulip/GlTools.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlXMLTools.h>

using namespace std;

namespace tlp {

GlAbstractPolygon::GlAbstractPolygon():
  polygonMode(POLYGON),
  filled(true),
  outlined(true),
  lighting(true),
  invertYTexture(true),
  textureName(""),
  outlineSize(1.),
  hideOutlineLod(0.),
  indices(NULL),
  auxIndices(NULL),
  texArray(NULL),
  generated(false) {
}
//=====================================================
GlAbstractPolygon::~GlAbstractPolygon() {
  clearGenerated();
}
//=====================================================
GlAbstractPolygon::PolygonMode GlAbstractPolygon::getPolygonMode() {
  return polygonMode;
}
//=====================================================
void GlAbstractPolygon::setPolygonMode(PolygonMode mode) {
  polygonMode=mode;
}
//=====================================================
bool GlAbstractPolygon::getFillMode() {
  return filled;
}
//=====================================================
void GlAbstractPolygon::setFillMode(const bool filled) {
  this->filled = filled;
}
//=====================================================
bool GlAbstractPolygon::getOutlineMode() {
  return outlined;
}
//=====================================================
void GlAbstractPolygon::setOutlineMode(const bool outlined) {
  this->outlined = outlined;
}
//=====================================================
bool GlAbstractPolygon::getLightingMode() {
  return lighting;
}
//=====================================================
void GlAbstractPolygon::setLightingMode(const bool lighting) {
  this->lighting = lighting;
}
//=====================================================
string GlAbstractPolygon::getTextureName() {
  return textureName;
}
//=====================================================
void GlAbstractPolygon::setTextureName(const string &name) {
  textureName=name;
}
//=====================================================
float GlAbstractPolygon::getOutlineSize() {
  return outlineSize;
}
//=====================================================
void GlAbstractPolygon::setOutlineSize(float size) {
  outlineSize=size;
}
//=====================================================
Color GlAbstractPolygon::getFillColor(unsigned int i) {
  if(fillColors.size()<i)
    fillColors.resize(i,fillColors.back());

  return fillColors[i];
}
//=====================================================
void GlAbstractPolygon::setFillColor(unsigned int i, const Color &color) {
  if(fillColors.size()<i)
    fillColors.resize(i,fillColors.back());

  fillColors[i]=color;
  clearGenerated();
}
//=====================================================
void GlAbstractPolygon::setFillColor(const Color &color) {
  fillColors.clear();
  fillColors.push_back(color);
}
//=====================================================
Color GlAbstractPolygon::getOutlineColor(unsigned int i) {
  if(outlineColors.size()<i)
    outlineColors.resize(i,outlineColors.back());

  return outlineColors[i];
}
//=====================================================
void GlAbstractPolygon::setOutlineColor(unsigned int i, const Color &color) {
  if(outlineColors.size()<i)
    outlineColors.resize(i,outlineColors.back());

  outlineColors[i]=color;
  clearGenerated();
}
//=====================================================
void GlAbstractPolygon::setOutlineColor(const Color &color) {
  outlineColors.clear();
  outlineColors.push_back(color);
}
//=====================================================
float GlAbstractPolygon::getHideOutlineLod() {
  return hideOutlineLod;
}
//=====================================================
void GlAbstractPolygon::setHideOutlineLod(float lod) {
  hideOutlineLod=lod;
}
//=====================================================
void GlAbstractPolygon::setInvertYTexture(bool invertYTexture) {
  this->invertYTexture = invertYTexture;
}
//=====================================================
void GlAbstractPolygon::draw(float lod,Camera *) {

  bool canUseVBO=OpenGlConfigManager::getInst().hasVertexBufferObject();

  glDisable(GL_CULL_FACE);

  if(lighting) {
    if(cameraIs3D()) {
      glEnable(GL_LIGHTING);
      glEnable(GL_COLOR_MATERIAL);
    }
    else {
      glDisable(GL_LIGHTING);
    }
  }
  else {
    glDisable(GL_LIGHTING);
  }

  if(!generated) {
    Coord normal;

    if(filled) {
      // Normal compute
      vector<Coord> normalPoints;
      normalPoints.push_back(points[0]);

      for(size_t i=1; i<points.size() && normalPoints.size()<3; ++i) {
        bool find=false;

        for(size_t j=0; j<normalPoints.size(); ++j) {
          if(normalPoints[j]==points[i]) {
            find=true;
            break;
          }
        }

        if(!find)
          normalPoints.push_back(points[i]);
      }

      assert(normalPoints.size()==3);

      if(normalPoints.size()!=3)
        return;

      // Ok we have a valid filled polygon
      normal=normalPoints[0]-normalPoints[1];
      normal^=(normalPoints[2]-normalPoints[1]);
      normal/=normal.norm();

      if(normal[2]<0)
        normal=Coord(-normal[0],-normal[1],-normal[2]);
    }

    size_t size=points.size();

    // Create arrays
    indices=new GLubyte[size];
    texArray=new GLfloat[size*2];

    if(polygonMode==QUAD_STRIP) {
      auxIndices=new GLubyte[size];
    }

    // Expand vector
    if(filled) {
      normalArray.resize(size,normal);
      assert(!fillColors.empty());

      if(fillColors.size()!=1)
        fillColors.resize(size,fillColors.back());
    }

    if(outlined) {
      assert(outlineColors.size());

      if(outlineColors.size()!=1)
        outlineColors.resize(size,outlineColors.back());
    }

    // Compute texture coord array and indice array
    for(size_t i=0; i<size; ++i) {
      if(filled) {
        texArray[i*2]=(points[i][0]-boundingBox[0][0])/(boundingBox[1][0]-boundingBox[0][0]);

        if(!invertYTexture)
          texArray[i*2+1]=(points[i][1]-boundingBox[0][1])/(boundingBox[1][1]-boundingBox[0][1]);
        else
          texArray[i*2+1]=1-(points[i][1]-boundingBox[0][1])/(boundingBox[1][1]-boundingBox[0][1]);
      }

      indices[i]=i;

      if(polygonMode==QUAD_STRIP) {
        if(i<size/2) {
          auxIndices[i]=i*2;
          auxIndices[i+size/2]=size-(i*2+1);
        }
      }
    }

    if(canUseVBO) {

      // Generate buffers
      glGenBuffers(7,buffers);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*size,&points[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[5]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*size, indices, GL_STATIC_DRAW);

      if(polygonMode==QUAD_STRIP) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[6]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*size, auxIndices, GL_STATIC_DRAW);
      }

      if(filled) {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*size,&normalArray[0], GL_STATIC_DRAW);

        if(fillColors.size()!=1) {
          glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
          glBufferData(GL_ARRAY_BUFFER, sizeof(GLubyte)*4*size,&fillColors[0], GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*size,texArray, GL_STATIC_DRAW);
      }

      if(outlined && outlineColors.size()!=1) {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLubyte)*4*size,&outlineColors[0], GL_STATIC_DRAW);
      }

      delete[] indices;
      delete[] auxIndices;
      indices=NULL;
      auxIndices=NULL;
      delete[] texArray;
      texArray=NULL;
      normalArray.clear();
    }

    generated=true;
  }



  // Coord array
  glEnableClientState(GL_VERTEX_ARRAY);

  if(canUseVBO) {
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), BUFFER_OFFSET(0));
  }
  else {
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &points[0]);
  }



  // Fill
  if (filled) {
    // Normal array
    glEnableClientState(GL_NORMAL_ARRAY);

    if(canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), BUFFER_OFFSET(0));
    }
    else {
      glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), &normalArray[0]);
    }

    if(fillColors.size()!=1) {
      // fillColor array
      glEnableClientState(GL_COLOR_ARRAY);

      if(canUseVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glColorPointer(4,GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), BUFFER_OFFSET(0));
      }
      else {
        glColorPointer(4,GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &fillColors[0]);
      }
    }
    else {
      setMaterial(fillColors[0]);
    }

    // texture Array
    if(textureName!="") {
      GlTextureManager::getInst().activateTexture(textureName);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      if(canUseVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
        glTexCoordPointer(2,GL_FLOAT, 2*sizeof(GLfloat), BUFFER_OFFSET(0));
      }
      else {
        glTexCoordPointer(2,GL_FLOAT, 2*sizeof(GLfloat), texArray);
      }
    }

    GLenum fillMode = GL_POLYGON;

    switch(polygonMode) {
    case(POLYGON): {
      fillMode=GL_POLYGON;
      break;
    }

    case(QUAD_STRIP): {
      fillMode=GL_QUAD_STRIP;
      break;
    }
    }

    // Draw
    if(canUseVBO) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[5]);
      glDrawElements(fillMode, points.size(), GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

    }
    else {
      glDrawElements(fillMode, points.size(), GL_UNSIGNED_BYTE, indices);
    }

    // Disable
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    if(textureName!="") {
      GlTextureManager::getInst().desactivateTexture();
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }



  // Outline
  if(outlined && outlineSize!=0) {
    if((outlineSize<1 && lod>=hideOutlineLod) || (lod>=(hideOutlineLod/outlineSize))) {
      glDisable(GL_LIGHTING);

      glLineWidth(outlineSize);

      if(outlineColors.size()!=1) {
        // outlineColor
        glEnableClientState(GL_COLOR_ARRAY);

        if(canUseVBO) {
          glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
          glColorPointer(4,GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), BUFFER_OFFSET(0));
        }
        else {
          glColorPointer(4,GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &outlineColors[0]);
        }
      }
      else {
        glColor4ub(outlineColors[0][0],outlineColors[0][1],outlineColors[0][2],outlineColors[0][3]);
      }

      // Draw
      if(polygonMode!=QUAD_STRIP) {
        if(canUseVBO) {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[5]);
          glDrawElements(GL_LINE_LOOP, points.size(), GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
        }
        else {
          glDrawElements(GL_LINE_LOOP, points.size(), GL_UNSIGNED_BYTE, indices);
        }
      }
      else {
        if(canUseVBO) {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[6]);
          glDrawElements(GL_LINE_LOOP, points.size(), GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
        }
        else {
          glDrawElements(GL_LINE_LOOP, points.size(), GL_UNSIGNED_BYTE, auxIndices);
        }
      }

      // Disable
      glDisableClientState(GL_COLOR_ARRAY);

      glEnable(GL_LIGHTING);
    }
  }

  glDisableClientState(GL_VERTEX_ARRAY);

  if(canUseVBO) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }



  glTest(__PRETTY_FUNCTION__);
}
//===========================================================
void GlAbstractPolygon::translate(const Coord& vec) {
  boundingBox.translate(vec);

  for(vector<Coord>::iterator it = points.begin(); it!=points.end(); ++it) {
    (*it) += vec;
  }

  clearGenerated();
}
//===========================================================
void GlAbstractPolygon::scale(const Size& factor) {
  boundingBox.scale(factor);

  for(vector<Coord>::iterator it = points.begin(); it!=points.end(); ++it) {
    (*it) *= factor;
  }

  clearGenerated();
}
//===========================================================
void GlAbstractPolygon::getXML(string &outString) {

  GlXMLTools::createProperty(outString, "type", "GlPolygon","GlEntity");

  getXMLOnlyData(outString);

}
//===========================================================
void GlAbstractPolygon::getXMLOnlyData(string &outString) {

  GlXMLTools::getXML(outString,"points",points);
  GlXMLTools::getXML(outString,"fillColors",fillColors);
  GlXMLTools::getXML(outString,"outlineColors",outlineColors);
  GlXMLTools::getXML(outString,"filled",filled);
  GlXMLTools::getXML(outString,"outlined",outlined);
  GlXMLTools::getXML(outString,"textureName",textureName);
  GlXMLTools::getXML(outString,"outlineSize",outlineSize);
}
//============================================================
void GlAbstractPolygon::setWithXML(const string &inString, unsigned int &currentPosition) {

  points.clear();
  GlXMLTools::setWithXML(inString,currentPosition,"points",points);
  fillColors.clear();
  GlXMLTools::setWithXML(inString,currentPosition,"fillColors",fillColors);
  outlineColors.clear();
  GlXMLTools::setWithXML(inString,currentPosition,"outlineColors",outlineColors);
  GlXMLTools::setWithXML(inString,currentPosition,"filled",filled);
  GlXMLTools::setWithXML(inString,currentPosition,"outlined",outlined);
  GlXMLTools::setWithXML(inString,currentPosition,"textureName",textureName);
  GlXMLTools::setWithXML(inString,currentPosition,"outlineSize",outlineSize);

  for(vector<Coord>::iterator it= points.begin(); it!=points.end(); ++it) {
    boundingBox.expand(*it);
  }
}
//============================================================
void GlAbstractPolygon::setPoints(const vector<Coord> &points) {
  assert(points.size()<=256);
  assert(points.size() >= 3);

  this->points=points;
  recomputeBoundingBox();
}
//============================================================
void GlAbstractPolygon::setPoint(unsigned int index, const Coord &point) {
  assert(points.size()>index);

  this->points[index]=point;
  recomputeBoundingBox();
}
//============================================================
void GlAbstractPolygon::setFillColors(const std::vector<Color> &colors) {
  fillColors=colors;
}
//============================================================
void GlAbstractPolygon::setOutlineColors(const std::vector<Color> &colors) {
  outlineColors=colors;
}
//============================================================
void GlAbstractPolygon::clearGenerated() {
  delete[] indices;
  indices=NULL;
  delete[] auxIndices;
  auxIndices=NULL;
  delete[] texArray;
  texArray=NULL;
  normalArray.clear();

  if(generated)
    if(OpenGlConfigManager::getInst().hasVertexBufferObject()) {
      glDeleteBuffers(7,buffers);
    }

  generated=false;
}
//============================================================
void GlAbstractPolygon::recomputeBoundingBox() {
  boundingBox=BoundingBox();

  for(vector<Coord>::iterator it=points.begin(); it!=points.end(); ++it) {
    boundingBox.expand(*it);
  }
}
}
