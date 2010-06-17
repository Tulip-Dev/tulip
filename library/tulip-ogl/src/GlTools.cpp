/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <tulip/Rectangle.h>
#include "tulip/GlTools.h"
#include "tulip/Matrix.h"
#include "tulip/BoundingBox.h"
#include <iostream>

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

  //====================================================
  void glTest(string message) {
    unsigned int i = 1;
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
      if (i==1) cerr << "[OpenGL ERROR] : " << message << endl;
      cerr << "[" << i << "] ========> : " << gluErrorString(error) <<  endl;
      assert (error == GL_NO_ERROR);
      error = glGetError();
      ++i;
    }
  }
  //====================================================
  void setColor(const Color &c) {
    glColor4ubv((unsigned char *) &c);
  }
  //====================================================
  void setColor(GLfloat *c){
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
    /*if(colorMat[3]<255)
      glDepthMask ( GL_FALSE ) ;
    else
      glDepthMask ( GL_TRUE ) ;*/
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
    point[3] = 1;
    point =  point * transform;// * transform;
#ifndef NDEBUG
    if(fabs(point[3]) < 1E-6){
      std::cerr << "Error in projectPoint with coord : " << obj << " and transform matrix : " << transform << std::endl;
    }
#endif
    assert(fabs(point[3]) > 1E-6);
    //    cerr << winw << endl;
    Coord result(point[0], point[1], point[2]);
    result /= point[3];
    result *= 0.5;
    result += 0.5;

    result[0] = result[0] * viewport[2] + viewport[0];
    result[1] = result[1] * viewport[3] + viewport[1];
    return result;
  }
  //====================================================
  Coord unprojectPoint(const Coord &obj,
		       const MatrixGL &invtransform,
		       const Vector<int, 4> &viewport) {
    Vector<float, 4> point;
    point[0] = obj[0];
    point[1] = obj[1];
    point[2] = obj[2];

    point[0] = (point[0] - viewport[0]) / viewport[2];
    point[1] = (point[1] - viewport[1]) / viewport[3];

    point *= 2.;
    point -= 1.;

    point[3] = 1.;

    point = point * invtransform;
#ifndef NDEBUG
    if(fabs(point[3]) < 1E-6){
      std::cerr << "Error in unprojectPoint with coord : " << obj << " and transform matrix : " << invtransform << std::endl;
    }
#endif
    assert(fabs(point[3]) > 1E-6);

    Coord result(point[0], point[1], point[2]);
    result /= point[3];

    return result;
  }
  //====================================================
  double segmentSize(const Coord &u, const Coord &v,
		     const tlp::MatrixGL &transform,
		     const Vector<int, 4> &viewport) {
    Coord p1 = projectPoint(u, transform, viewport);
    Coord p2 = projectPoint(v, transform, viewport);
    return sqr(p1[0]-p2[0]) + sqr(p1[1]-p2[1]);
  }
  //====================================================
  double segmentVisible(const Coord &u, const Coord &v,
			const tlp::MatrixGL &transform,
			const Vector<int, 4> &viewport) {
    //    cerr << __PRETTY_FUNCTION__ << endl;
    Coord p1 = projectPoint(u, transform, viewport);
    Coord p2 = projectPoint(v, transform, viewport);
    GLfloat minx = viewport[0];
    GLfloat miny = viewport[1];
    GLfloat maxx = minx + viewport[2];
    GLfloat maxy = miny + viewport[3];
    //    cerr << p1 << endl;
    //    cerr << p2 << endl;
    double size = sqr(p1[0]-p2[0]) + sqr(p1[1]-p2[1]);
    if (( (p1[0]<minx && p2[0]<minx) ||
	  (p1[1]<miny && p2[1]<miny) ||
	  (p1[0]>maxx && p2[0]>maxx) ||
	  (p1[1]>maxy && p2[1]>maxy) ) ) {
      //      cerr << "not visible" << endl;
      //      cerr << p1 << " *** " << p2 << endl;
      return -size;
    }
    else
      return size;
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
    //MatrixGL tmp(modelviewMatrix);
    tmp[0][0] = nSize; tmp[0][1] = 0;     tmp[0][2] = 0;
    tmp[1][0] = 0;     tmp[1][1] = 0;     tmp[1][2] = 0;
    tmp[2][0] = 0;     tmp[2][1] = 0;     tmp[2][2] = 0;

    //    cerr << modelviewMatrix << endl;
    //    cerr << projectionMatrix << endl;

    tmp *= projectionMatrix;

    Vector<float, 4> vect1;
    vect1[0] = 0.5; vect1[1] = 0;
    vect1[2] = 0;   vect1[3] = 1.0;
    Vector<float, 4> proj1 =  vect1 * tmp;

    Vector<float, 4> vect2;
    vect2.fill(0);
    vect2[3] = 1.0;
    Vector<float, 4> proj2 =  vect2 * tmp;


    float x1 = (proj1[0]/proj1[3] * 0.5 + 0.5 ) * viewport[2];
    float x2 = (proj2[0]/proj2[3] * 0.5 + 0.5 ) * viewport[2];

    float width = fabs(x1 - x2);
    float size = sqr(2. * width);

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

    //    cerr << "s: " << nSize << " => " << minx << "," << miny << "/" << maxx << "," << maxy << endl;
  }
  //====================================================
  float calculateAABBSize(const BoundingBox& bb,const Coord& eye,const Matrix<float, 4>& transformMatrix,const Vector<int, 4>& globalViewport,const Vector<int, 4>& currentViewport) {
    //float lod=0.;
    BoundingBox bbTmp(bb);
    Coord src[8];
    Coord dst[8];
    int pos;
    int num;

    for(int i=0;i<3;i++) {
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
    num=hullVertexTable[pos][0];
    if(num==0)
      return -1;
    for(int i=0;i<num;i++) {
      dst[i] = projectPoint(src[(int)hullVertexTable[pos][i+1]],transformMatrix,globalViewport);
      dst[i][1] = globalViewport[3] - dst[i][1];
    }
    bool inScreen=false;
    float bbBox[4];
    for(int i=0;i<num;i++) {
      if((dst[i][0]>= currentViewport[0]) && (dst[i][0]<=currentViewport[0]+currentViewport[2]) && (dst[i][1] >= currentViewport[1]) && (dst[i][1]<=currentViewport[1]+currentViewport[3])){
        inScreen=true;
      }
      if(i==0){
        bbBox[0]= dst[i][0];bbBox[2]= dst[i][0];bbBox[1]= dst[i][1];bbBox[3]= dst[i][1];
      }else{
        if(dst[i][0]<bbBox[0])
          bbBox[0]= dst[i][0];
        if(dst[i][0]>bbBox[2])
          bbBox[2]= dst[i][0];
        if(dst[i][1]<bbBox[1])
          bbBox[1]= dst[i][1];
        if(dst[i][1]>bbBox[3])
          bbBox[3]= dst[i][1];
      }
      if(bbBox[0]<currentViewport[0]+currentViewport[2] && bbBox[2]>currentViewport[0] && bbBox[1]<currentViewport[1]+currentViewport[3] && bbBox[3]>currentViewport[1]){
        inScreen=true;
      }
    }

    if(!inScreen){
      return -1;
    }else{
      return sqrt((bbBox[2]-bbBox[0])*(bbBox[2]-bbBox[0])+(bbBox[3]-bbBox[1])*(bbBox[3]-bbBox[1])) * 2;
    }
  }
  //====================================================
  float calculate2DLod(const BoundingBox& bb,const Vector<int, 4>& globalViewport,const Vector<int, 4>& currentViewport) {
    Coord first(bb[0]);
    Coord second (bb[1]);
    first[1]  = globalViewport[3]-bb[1][1];
    second[1] = globalViewport[3]-bb[0][1];

    if(!(first[0]<currentViewport[0]+currentViewport[2] && second[0]>currentViewport[0] && first[1]<currentViewport[1]+currentViewport[3] && second[1]>currentViewport[1])){
      return -1.;
    }else{
      return 1.;
    }
  }
  //====================================================
  void solidCone() {
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);
    glTranslatef(0.0f, 0.0f, -1.0f);
    gluQuadricOrientation(quadratic, GLU_INSIDE);
    gluDisk(quadratic, 0.0f, 0.5f, 8, 1);
    gluQuadricOrientation(quadratic, GLU_OUTSIDE);
    gluCylinder(quadratic, 0.5f, 0.0f, 1.0f, 8, 1);
    gluDeleteQuadric(quadratic);
  }
  //====================================================
  void cube(GLenum type) {
    //  GLenum type = GL_LINE_LOOP;
    /* front face */
    glBegin(type);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    /* back face */
    glBegin(type);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    /* right face */
    glBegin(type);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();
    /* left face */
    glBegin(type);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
    /* top face */
    glBegin(type);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    /* bottom face */
    glBegin(type);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
  }

  //=======================================================
  //Calcul 3D,
  //=======================================================
  //Calcul de la matrice de transformation pour
  //le positionnement des flèches sur les arètes
  MatrixGL makeArrowMatrix(const Coord &A, const Coord &B) {
    MatrixGL matrix;

    //Vecteur AB
    Vector<float, 3> vAB;
    //Vecteur V
    Vector<float, 3> vV;
  //Vecteur W
    Vector<float, 3> vW;

    vAB = B - A;
    float nAB; //|AB|
    nAB = vAB.norm();
    if (fabs(nAB) > 1E-6)
      vAB /= nAB;

  //vAB * vV = xAB * xV + yAB*yV + zAB * zV = |AB| * |V| * cos(alpha) = 0;
    if (fabs(vAB[2]) < 1E-6) {
      vV[0] = 0; vV[1] = 0; vV[2] = 1.0;
    }
    else
    if (fabs(vAB[1]) < 1E-6) {
      vV[0] = 0; vV[1] = 1.0; vV[2] = 0;
    }
    else {
      vV[0] = 0;
      vV[1] = 1./vAB[1];
      vV[2] = -1./vAB[2];
      vV /= vV.norm();
    }

    vW = vAB ^ vV;
    float nW = vW.norm();
    if (fabs(nW) > 1E-6)
      vW /= nW;

    for (unsigned int i = 0; i < 3; ++i) {
      matrix[0][i] = vW[i];
      matrix[1][i] = vV[i];
      matrix[2][i] = vAB[i];
      matrix[3][i] = B[i];
    }
    matrix[0][3]=0;
    matrix[1][3]=0;
    matrix[2][3]=0;
    matrix[3][3]=1;

    return matrix;
  }
}
