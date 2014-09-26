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
#include <cmath>

#include <tulip/GlLines.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

//=============================================================
void GlLines::glDrawLine(const Coord &startPoint, const Coord &endPoint, const double width, const unsigned int stippleType,
                         const Color &startColor, const Color &endColor,
                         const bool, const double, const double) {
  GlLines::glEnableLineStipple(stippleType);
  glLineWidth(width);
  glBegin(GL_LINES);
  setColor(startColor);
  glVertex3f(startPoint[0],startPoint[1],startPoint[2]);
  setColor(endColor);
  glVertex3f(endPoint[0],endPoint[1],endPoint[2]);
  glEnd();
  GlLines::glDisableLineStipple(stippleType);
}
//=============================================================
void GlLines::glDrawCurve(const Coord &startPoint, const vector<Coord> &bends, const Coord &endPoint, const double width,
                          const unsigned int stippleType, const Color &startColor, const Color &endColor,
                          const bool arrow, const double arrowWidth, const double arrowHeight) {
  if (bends.empty()) {
    glDrawLine(startPoint,endPoint,width,stippleType,startColor,endColor,arrow,arrowWidth,arrowHeight);
    return;
  }

  GlLines::glEnableLineStipple(stippleType);
  glLineWidth(width);

  GLfloat *colorStart = startColor.getGL();
  colorStart[3] = 1.0;
  GLfloat *colorEnd = endColor.getGL();
  colorEnd[3] = 1.0;
  GLfloat colorDelta[4];

  for (int i=0; i<4; i++)
    colorDelta[i] = (colorEnd[i]-colorStart[i])/(bends.size()+2);

  glBegin(GL_LINE_STRIP);
  setColor(colorStart);
  glVertex3f(startPoint[0],startPoint[1],startPoint[2]);

  for (int i=0; i<4; i++)
    colorStart[i] += colorDelta[i];

  for (unsigned int i = 0; i < bends.size(); i++) {
    setColor(colorStart);
    glVertex3f(bends[i][0],bends[i][1],bends[i][2]);

    for (int i=0; i<4; i++)
      colorStart[i] += colorDelta[i];
  }

  setColor(colorEnd);
  glVertex3f(endPoint[0],endPoint[1],endPoint[2]);
  glEnd();
  delete [] colorStart;
  delete [] colorEnd;
  GlLines::glDisableLineStipple(stippleType);
}
//=============================================================
void GlLines::glDrawBezierCurve(const Coord &startPoint,const vector<Coord> &bends,const Coord &endPoint,
                                unsigned int steps,const double width,const unsigned int stippleType,
                                const Color &startColor,const Color &endColor,
                                const bool arrow, const double arrowWidth, const double arrowHeight) {
  if (bends.empty()) {
    glDrawLine(startPoint,endPoint,width,stippleType,startColor,endColor,arrow,arrowWidth,arrowHeight);
    return;
  }

  GlLines::glEnableLineStipple(stippleType);
  glLineWidth(width);
  GLfloat *bendsCoordinates=buildCurvePoints(startPoint,bends,endPoint);
  GLfloat *colorStart=startColor.getGL();
  colorStart[3] = 1.0;
  GLfloat *colorEnd=endColor.getGL();
  colorEnd[3] = 1.0;
  GLfloat colorDelta[4];

  for (int i=0; i<4; i++)
    colorDelta[i]=(colorEnd[i]-colorStart[i])/(steps);

  delete [] colorEnd;
  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, bends.size()+2, bendsCoordinates);
  glEnable(GL_MAP1_VERTEX_3);

  glBegin(GL_LINE_STRIP);

  for (unsigned int i = 0; i <= steps; i++) {
    setColor(colorStart);
    glEvalCoord1f((GLfloat) i/steps);

    for (unsigned int j=0; j<4; j++)
      colorStart[j]+=colorDelta[j];
  }

  glEnd();

  glDisable(GL_MAP1_VERTEX_3);
  delete [] bendsCoordinates;
  delete [] colorStart;
  GlLines::glDisableLineStipple(stippleType);
}
//=============================================================
void GlLines::glDrawSplineCurve(const Coord &startPoint,const vector<Coord> &bends,const Coord &endPoint,
                                unsigned int steps,const double width,const unsigned int stippleType,
                                const Color &startColor,const Color &endColor,const bool arrow,const double arrowWidth,
                                const double arrowHeight) {
  if (bends.empty()) {
    glDrawLine(startPoint,endPoint,width,stippleType,startColor,endColor,arrow,arrowWidth,arrowHeight);
    return;
  }

  GlLines::glEnableLineStipple(stippleType);
  glLineWidth(width);

  GLfloat *colorStart=startColor.getGL();
  colorStart[3] = 1.0;
  GLfloat *colorEnd=endColor.getGL();
  colorEnd[3] = 1.0;
  GLfloat colorDelta[4];

  for (int i=0; i<4; i++)
    colorDelta[i]=(colorEnd[i]-colorStart[i])/(steps*(bends.size()+1));

  delete [] colorEnd;

  Coord p1next;
  Coord p0,p1,p2,p3,p4;
  //Drawing first curve's segment
  {
    p0=startPoint;
    p3=bends[0];

    if (bends.size()>1) p4=bends[1];
    else p4=endPoint;

    Coord v03 = p3-p0;
    Coord v34 = p4-p3;
    v03=  v03/v03.norm();
    v34 = v34/v34.norm();
    p1 = v03+p0;
    double scalaireTmp=v03.dotProduct(v34) / (v03.norm()*v34.norm());

    if ((scalaireTmp<0.99999) && (scalaireTmp>-0.9999999)) {
      Coord vP2=(((v03^v34)^(v34-v03)));
      vP2=vP2/vP2.norm();
      Coord vP1=(((v03^v34)^(v03-v34)));
      vP1=vP1/vP1.norm();
      p2=vP2+p3;
      p1next=vP1+p3;
    }
    else {
      v34=v34/v34.norm();
      p2=p3-v34;
      p1next=p3+v34;
    }

    GLfloat *bendsCoordinates=buildCurvePoints(p0,p1,p2,p3);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, bendsCoordinates);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);

    for (unsigned int i = 0; i <= steps; i++) {
      setColor(colorStart);
      glEvalCoord1f((GLfloat) i/steps);

      for (unsigned int j=0; j<4; j++)
        colorStart[j]+=colorDelta[j];
    }

    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
    delete [] bendsCoordinates;
  }

  for (unsigned int i=1; i<bends.size(); ++i) {
    p0=bends[i-1];
    p1=p1next;
    p3=bends[i];

    if ((i+1)<bends.size()) p4=bends[i+1];
    else p4=endPoint;

    Coord v03=p3-p0;
    Coord v34=p4-p3;
    double scalaireTmp=v03.dotProduct(v34)/(v03.norm()*v34.norm());

    if ((scalaireTmp<0.99999) && (scalaireTmp>-0.9999999)) {
      v03=v03/v03.norm();
      v34=v34/v34.norm();
      Coord vP2=(((v03^v34)^(v34-v03)));
      vP2=vP2/vP2.norm();
      Coord vP1=(((v03^v34)^(v03-v34)));
      vP1=vP1/vP1.norm();
      p2=vP2+p3;
      p1next=vP1+p3;
    }
    else {
      v34=v34/v34.norm();
      p2=p3-v34;
      p1next=p3+v34;
    }

    GLfloat *bendsCoordinates=buildCurvePoints(p0,p1,p2,p3);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, bendsCoordinates);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);

    for (unsigned int i = 0; i <= steps; i++) {
      setColor(colorStart);
      glEvalCoord1f((GLfloat) i/steps);

      for (unsigned int j=0; j<4; j++)
        colorStart[j]+=colorDelta[j];
    }

    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
    delete [] bendsCoordinates;
  }

  //Drawing last curve's segment
  {
    p0=bends[bends.size()-1];
    p1=p1next;
    p3=endPoint;
    p2=p0-p3;
    p2=p2/p2.norm()+p3;
    GLfloat *bendsCoordinates=buildCurvePoints(p0,p1,p2,p3);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, bendsCoordinates);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);

    for (unsigned int i = 0; i <= steps; i++) {
      setColor(colorStart);
      glEvalCoord1f((GLfloat) i/steps);

      for (unsigned int j=0; j<4; j++)
        colorStart[j]+=colorDelta[j];
    }

    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
    delete [] bendsCoordinates;
  }
  delete [] colorStart;
  GlLines::glDisableLineStipple(stippleType);
}
//=============================================================
void GlLines::glDrawSpline2Curve(const Coord &startPoint,const vector<Coord> &bends,const Coord &endPoint,
                                 unsigned int steps,const double width,const unsigned int stippleType,
                                 const Color &startColor,const Color &endColor,const bool arrow,const double arrowWidth,
                                 const double arrowHeight) {
  if (bends.empty()) {
    glDrawLine(startPoint,endPoint,width,stippleType,startColor,endColor,arrow,arrowWidth,arrowHeight);
    return;
  }

  vector<Coord> tmpVect(bends.size()*3+2);
  int tmpVectIdx=0;
  Coord p1next;
  Coord p0,p1,p2,p3,p4;
  //Drawing first curve's segment
  {
    p0=startPoint;
    p3=bends[0];

    if (bends.size()>1) p4=bends[1];
    else p4=endPoint;

    Coord v03=p3-p0;
    Coord v34=p4-p3;
    double scalaireTmp=v03.dotProduct(v34)/(v03.norm()*v34.norm());

    if ((scalaireTmp<0.99999) && (scalaireTmp>-0.9999999)) {
      v03=v03/v03.norm();
      v34=v34/v34.norm();
      p1=v03+p0;
      Coord vP2=(((v03^v34)^(v34-v03)));
      vP2=vP2/vP2.norm();
      Coord vP1=(((v03^v34)^(v03-v34)));
      vP1=vP1/vP1.norm();
      p2=vP2+p3;
      p1next=vP1+p3;
    }
    else {
      v34=v34/v34.norm();
      p2=p3-v34;
      p1next=p3+v34;
    }

    tmpVect[tmpVectIdx++]=p1;
    tmpVect[tmpVectIdx++]=p2;
  }

  for (unsigned int i=1; i<bends.size(); ++i) {
    p0=bends[i-1];
    p1=p1next;
    p3=bends[i];

    if ((i+1)<bends.size()) p4=bends[i+1];
    else p4=endPoint;

    Coord v03=p3-p0;
    Coord v34=p4-p3;
    double scalaireTmp=v03.dotProduct(v34)/(v03.norm()*v34.norm());

    if ((scalaireTmp<0.99999) && (scalaireTmp>-0.9999999)) {
      v03=v03/v03.norm();
      v34=v34/v34.norm();
      Coord vP2=(((v03^v34)^(v34-v03)));
      vP2=vP2/vP2.norm();
      Coord vP1=(((v03^v34)^(v03-v34)));
      vP1=vP1/vP1.norm();
      p2=vP2+p3;
      p1next=vP1+p3;
    }
    else {
      v34=v34/v34.norm();
      p2=p3-v34;
      p1next=p3+v34;
    }

    tmpVect[tmpVectIdx++]=p0;
    tmpVect[tmpVectIdx++]=p1;
    tmpVect[tmpVectIdx++]=p2;
  }

  //Drawing last curve's segment
  {
    p0=bends[bends.size()-1];
    p1=p1next;
    p3=endPoint;
    p2=p0-p3;
    p2=p2/p2.norm()+p3;
    tmpVect[tmpVectIdx++]=p0;
    tmpVect[tmpVectIdx++]=p1;
    tmpVect[tmpVectIdx++]=p2;
  }
  GlLines::glDrawBezierCurve(startPoint,tmpVect,endPoint,
                             steps,width,stippleType,
                             startColor,endColor,arrow,arrowWidth,
                             arrowHeight);
}
//=============================================================
void GlLines::glDrawPoint(const Coord &p) {
  glPointSize(5.0);
  setColor(Color(255,255,0,255));
  glBegin(GL_POINTS);
  glVertex3f(p[0],p[1],p[2]);
  glEnd();
}
//=============================================================
void GlLines::glDisableLineStipple(unsigned int stippleType) {
  if (stippleType>0) glDisable(GL_LINE_STIPPLE);
}
//=============================================================
void GlLines::glEnableLineStipple(unsigned int stippleType) {
  if (stippleType>0) {
    glEnable(GL_LINE_STIPPLE);

    switch (stippleType) {
    case 1:
      glLineStipple (1, 0x0101);  /*  dotted  */
      break;

    case 2:
      glLineStipple (1, 0x00FF);  /*  dashed  */
      break;

    case 3:
      glLineStipple (1, 0x1C47);  /*  dash/dot/dash  */
      break;

    default:
      glDisable(GL_LINE_STIPPLE);
      tlp::warning() << "unrecognizedStippleType" << endl;
      break;
    }
  }
}
//=============================================================
GLfloat *GlLines::buildCurvePoints(const Coord &startPoint,const  vector<Coord> &bends,const Coord &endPoint) {
  GLfloat *bendsCoordinates=new GLfloat[(bends.size()+2)*3];
  bendsCoordinates[0]=startPoint[0];
  bendsCoordinates[1]=startPoint[1];
  bendsCoordinates[2]=startPoint[2];
  vector<Coord>::const_iterator it;
  int i = 1;

  for (it=bends.begin(); it!=bends.end(); ++it) {
    bendsCoordinates[i*3  ] = (*it)[0];
    bendsCoordinates[i*3+1] = (*it)[1];
    bendsCoordinates[i*3+2] = (*it)[2];
    i++;
  }

  bendsCoordinates[i*3]  = endPoint[0];
  bendsCoordinates[i*3+1]= endPoint[1];
  bendsCoordinates[i*3+2]= endPoint[2];
  return bendsCoordinates;
}
//=============================================================
GLfloat *GlLines::buildCurvePoints(const Coord &p0,const Coord &p1,const Coord &p2,const Coord &p3) {
  GLfloat *bendsCoordinates= new GLfloat[4*3];

  for (unsigned int i = 0; i < 3; ++i) {
    bendsCoordinates[i]   =p0[i];
    bendsCoordinates[i+3] =p1[i];
    bendsCoordinates[i+6] =p2[i];
    bendsCoordinates[i+9] =p3[i];
  }

  return bendsCoordinates;
}
