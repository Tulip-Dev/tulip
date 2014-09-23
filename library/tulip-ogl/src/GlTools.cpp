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

#ifdef WIN32
#include <windows.h>
#ifndef GL_TABLE_TOO_LARGE
#ifdef GL_TABLE_TOO_LARGE_EXT
#define GL_TABLE_TOO_LARGE GL_TABLE_TOO_LARGE_EXT
#else
#define GL_TABLE_TOO_LARGE 0x8031
#endif
#endif
#endif

#include <tulip/Rectangle.h>
#include <tulip/GlTools.h>
#include <tulip/Matrix.h>
#include <tulip/BoundingBox.h>
#include <tulip/TulipException.h>
#include <iostream>
#include <string>
#include <sstream>
#include <climits>

using namespace std;
namespace tlp {

static char hullVertexTable[][7] = {
  {0,0,0,0,0,0,0},//0
  {4,0,4,7,3,0,0},//1
  {4,1,2,6,5,0,0},//2
  {0,0,0,0,0,0,0},//3
  {4,0,1,5,4,0,0},//4
  {6,0,1,5,4,7,3},//5
  {6,0,1,2,6,5,4},//6
  {0,0,0,0,0,0,0},//7
  {4,2,3,7,6,0,0},//8
  {6,4,7,6,2,3,0},//9
  {6,2,3,7,6,5,1},//10
  {0,0,0,0,0,0,0},//11
  {0,0,0,0,0,0,0},//12
  {0,0,0,0,0,0,0},//13
  {0,0,0,0,0,0,0},//14
  {0,0,0,0,0,0,0},//15
  {4,0,3,2,1,0,0},//16
  {6,0,4,7,3,2,1},//17
  {6,0,3,2,6,5,1},//18
  {0,0,0,0,0,0,0},//19
  {6,0,3,2,1,5,4},//20
  {6,2,1,5,4,7,3},//21
  {6,0,3,2,6,5,4},//22
  {0,0,0,0,0,0,0},//23
  {6,0,3,7,6,2,1},//24
  {6,0,4,7,6,2,1},//25
  {6,0,3,7,6,5,1},//26
  {0,0,0,0,0,0,0},//27
  {0,0,0,0,0,0,0},//28
  {0,0,0,0,0,0,0},//29
  {0,0,0,0,0,0,0},//30
  {0,0,0,0,0,0,0},//31
  {4,4,5,6,7,0,0},//32
  {6,4,5,6,7,3,0},//33
  {6,1,2,6,7,4,5},//34
  {0,0,0,0,0,0,0},//35
  {6,0,1,5,6,7,4},//36
  {6,0,1,5,6,7,3},//37
  {6,0,1,2,6,7,4},//38
  {0,0,0,0,0,0,0},//39
  {6,2,3,7,4,5,6},//40
  {6,0,4,5,6,2,3},//41
  {6,1,2,3,7,4,5}//42
};


// simple structure to embed an error code and its description
struct glErrorStruct {
  GLuint code;
  const std::string description;
};

// the known gl errors
static const struct glErrorStruct glErrorStructs[] = {
  {GL_NO_ERROR, "no error"},
  {GL_INVALID_ENUM, "invalid enumerant"},
  {GL_INVALID_VALUE, "invalid value"},
  {GL_INVALID_OPERATION, "invalid operation"},
  {GL_STACK_OVERFLOW, "stack overflow"},
  {GL_STACK_UNDERFLOW, "stack underflow"},
  {GL_OUT_OF_MEMORY, "out of memory"},
#ifdef GL_EXT_framebuffer_object
  {GL_INVALID_FRAMEBUFFER_OPERATION_EXT, "invalid framebuffer operation"},
#endif
  {GL_TABLE_TOO_LARGE, "table too large"},
  {UINT_MAX, "unknow error"} /* end of list indicator */
};

// the function to retrieve
const std::string& glGetErrorDescription(GLuint errorCode) {
  unsigned int i = 0;

  while(glErrorStructs[i].code != errorCode &&
        glErrorStructs[i].code != UINT_MAX)
    ++i;

  return glErrorStructs[i].description;
}

//====================================================
void glTest(const string &message) {
#ifndef NDEBUG
  unsigned int i = 1;
  GLenum error = glGetError();

  stringstream errorStream;
  bool haveError=false;

  while (error != GL_NO_ERROR) {
    haveError=true;

    if (i==1)
      errorStream << "[OpenGL ERROR] : " << message << endl;

    errorStream << "[" << i << "] ========> : " << glGetErrorDescription(error).c_str() <<  endl;
    error = glGetError();
    ++i;
  }

  if(haveError)
    throw tlp::TulipException(errorStream.str());

#else
  //fixes unused parameter warning in release mode
  (void)message;
#endif
}
//====================================================
void setColor(const Color &c) {
  glColor4ubv((unsigned char *) &c);
}
//====================================================
void setColor(GLfloat *c) {
  glColor4fv(c);
}
//====================================================
void setMaterial(const Color &c) {
  float colorMat[4];
  colorMat[0] = ((float)c[0])/255.0;
  colorMat[1] = ((float)c[1])/255.0;
  colorMat[2] = ((float)c[2])/255.0;
  colorMat[3] = ((float)c[3])/255.0;
  setColor(c);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorMat);
}
//====================================================
bool cameraIs3D() {
  return glIsEnabled(GL_LIGHT0);
}
//====================================================
Coord projectPoint(const Coord &obj,
                   const MatrixGL &transform,
                   const Vector<int, 4> &viewport) {
  Vector<float, 4> point;
  point[0] = obj[0];
  point[1] = obj[1];
  point[2] = obj[2];
  point[3] = 1.0f;
  point =  point * transform;
#ifndef NDEBUG

  if(fabs(point[3]) < 1E-6) {
    std::cerr << "Error in projectPoint with coord : "
              << obj
              << " and transform matrix : " << transform;
  }

#endif
  assert(fabs(point[3]) > 1E-6);
  Coord result(point[0], point[1], point[2]);
  result /= point[3];

  result[0] =  viewport[0]+ (1.0f + result[0]) * viewport[2] * 0.5f;
  result[1] =  viewport[1]+ (1.0f + result[1]) * viewport[3] * 0.5f;
  result[2] = (1.0f + result[2]) * 0.5f;
  return result;
}
//====================================================
Coord unprojectPoint(const Coord &obj,
                     const MatrixGL &invtransform,
                     const Vector<int, 4> &viewport) {
  Vector<float, 4> point;

  point[0] = (obj[0] - viewport[0]) / viewport[2] * 2.0f - 1.0f;
  point[1] = (obj[1] - viewport[1]) / viewport[3] * 2.0f - 1.0f;
  point[2] = 2.0f * obj[2] - 1.0f;
  point[3] = 1.0f;

  point = point * invtransform;
#ifndef NDEBUG

  if(fabs(point[3]) < 1E-6) {
    std::cerr << "Error in unprojectPoint with coord : " << obj << " and transform matrix : " << invtransform;
  }

#endif
  assert(fabs(point[3]) > 1E-6);

  Coord result(point[0], point[1], point[2]);
  result /= point[3];

  return result;
}
//====================================================
GLfloat projectSize(const Coord &position,const Size& size,
                    const MatrixGL &projectionMatrix, const MatrixGL &modelviewMatrix,
                    const Vector<int, 4> &viewport) {
  BoundingBox box;
  box.expand(position - size/2.f);
  box.expand(position + size/2.f);
  return projectSize(box, projectionMatrix, modelviewMatrix, viewport);
}
//====================================================
GLfloat projectSize(const BoundingBox &bb,
                    const MatrixGL &projectionMatrix, const MatrixGL &modelviewMatrix,
                    const Vector<int, 4> &viewport) {
  Coord bbSize(bb[1] - bb[0]);
  float  nSize = bbSize.norm(); //Enclosing bounding box

  MatrixGL translate;
  translate.fill(0);

  for (unsigned int i = 0; i<4; ++i)
    translate[i][i] = 1;

  for (unsigned int i = 0; i<3; ++i)
    translate[3][i] = bb[0][i] + bbSize[i]/2 ;

  MatrixGL tmp(translate * modelviewMatrix);

  tmp[0][0] = nSize;
  tmp[0][1] = 0;
  tmp[0][2] = 0;
  tmp[1][0] = 0;
  tmp[1][1] = 0;
  tmp[1][2] = 0;
  tmp[2][0] = 0;
  tmp[2][1] = 0;
  tmp[2][2] = 0;

  tmp *= projectionMatrix;

  Vector<float, 4> vect1;
  vect1[0] = 0.5;
  vect1[1] = 0;
  vect1[2] = 0;
  vect1[3] = 1.0;
  Vector<float, 4> proj1 =  vect1 * tmp;

  Vector<float, 4> vect2;
  vect2.fill(0);
  vect2[3] = 1.0;
  Vector<float, 4> proj2 =  vect2 * tmp;


  float x1 = (proj1[0]/proj1[3] * 0.5 + 0.5 ) * viewport[2];
  float x2 = (proj2[0]/proj2[3] * 0.5 + 0.5 ) * viewport[2];

  float width = fabs(x1 - x2);
  float size = pow(2. * width, 2);

  // Test of visibily
  x2 += viewport[0];
  float y2 = (proj2[1]/proj2[3] * 0.5 + 0.5) * viewport[3] + viewport[1];
  Vector<float, 2> upleft;
  upleft[0] = x2 - width;
  upleft[1] = y2 - width;
  Vector<float, 2> downright;
  downright[0] = x2 + width;
  downright[1] = y2 + width;
  Rectangle<float> r1;
  r1[0] = upleft;
  r1[1] = downright;

  Vector<float, 2> upleftV;
  upleftV[0] = viewport[0];
  upleftV[1] = viewport[1];

  Vector<float, 2> downrightV;
  downrightV[0] = viewport[0] + viewport[2];
  downrightV[1] = viewport[1] + viewport[3];

  Rectangle<float> r2;
  r2[0] = upleftV;
  r2[1] = downrightV;

  if (!r1.intersect(r2)) {
    size *= -1.0;
  }

  return size;
}
//====================================================
float calculateAABBSize(const BoundingBox& bb,const Coord& eye,const Matrix<float, 4>& transformMatrix,const Vector<int, 4>& globalViewport,const Vector<int, 4>& currentViewport) {
  BoundingBox bbTmp(bb);
  Coord src[8];
  Coord dst[8];
  int pos;
  int num;

  for(int i=0; i<3; i++) {
    if(bbTmp[0][i]>bbTmp[1][i]) {
      float tmp = bbTmp[0][i];
      bbTmp[0][i] = bbTmp[1][i];
      bbTmp[1][i] = tmp;
    }
  }

  bbTmp.getCompleteBB(src);
  pos = ((eye[0] < src[0][0])   )
        + ((eye[0] > src[6][0]) << 1)
        + ((eye[1] < src[0][1]) << 2)
        + ((eye[1] > src[6][1]) << 3)
        + ((eye[2] < src[0][2]) << 4)
        + ((eye[2] > src[6][2]) << 5);
  assert(pos<=42);

  //If pos==0 : camera are inside the entity so we return a arbitrary lod
  if(pos==0)
    return 10.;

  num=hullVertexTable[pos][0];

  if(num==0)
    return -1;

  for(int i=0; i<num; i++) {
    dst[i] = projectPoint(src[(int)hullVertexTable[pos][i+1]],transformMatrix,globalViewport);
    dst[i][1] = globalViewport[1] + globalViewport[3] - (dst[i][1] - globalViewport[1]);
  }

  bool inScreen=false;
  float bbBox[4] = {0.f, 0.f, 0.f, 0.f};

  for(int i=0; i<num; i++) {
    if((dst[i][0]>= currentViewport[0]) && (dst[i][0]<=currentViewport[0]+currentViewport[2]) && (dst[i][1] >= currentViewport[1]) && (dst[i][1]<=currentViewport[1]+currentViewport[3])) {
      inScreen=true;
    }

    if(i==0) {
      bbBox[0]= dst[i][0];
      bbBox[2]= dst[i][0];
      bbBox[1]= dst[i][1];
      bbBox[3]= dst[i][1];
    }
    else {
      if(dst[i][0]<bbBox[0])
        bbBox[0]= dst[i][0];

      if(dst[i][0]>bbBox[2])
        bbBox[2]= dst[i][0];

      if(dst[i][1]<bbBox[1])
        bbBox[1]= dst[i][1];

      if(dst[i][1]>bbBox[3])
        bbBox[3]= dst[i][1];
    }

    if(bbBox[0] < currentViewport[0]+currentViewport[2] && bbBox[2]>currentViewport[0] && bbBox[1]<currentViewport[1]+currentViewport[3] && bbBox[3]>currentViewport[1]) {
      inScreen=true;
    }
  }

  if(!inScreen) {
    return -1;
  }
  else {
    return sqrt((bbBox[2]-bbBox[0])*(bbBox[2]-bbBox[0])+(bbBox[3]-bbBox[1])*(bbBox[3]-bbBox[1])) * 2;
  }
}
//====================================================
float calculate2DLod(const BoundingBox& bb,const Vector<int, 4>&,const Vector<int, 4>&) {
  return (bb[1][0] - bb[0][0]) * (bb[1][1] - bb[0][1]);
}
}
