/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <vector>
#include <string.h>
#include <GL/glew.h>
#include <tulip/Vector.h>
#include <tulip/Size.h>
#include <tulip/TlpTools.h>
#include "tulip/Curves.h"
#include "tulip/GlTextureManager.h"


#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


using namespace std;
using namespace tlp;

namespace tlp {
  //================================================
  inline float sqrnorm(const Coord &v) {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  }
  //================================================
  float lineLength(const vector<Coord> &line) {
    float result = 0;
    for (unsigned int i = 1; i < line.size(); ++i) {
      result += sqrnorm(line[i-1] - line[i]);
    }
    return result;
  }
  //================================================
  vector<Color> getColors(const vector<Coord> &line, const Color &c1, const Color &c2) {
    tlp::Vector<float, 4> _c1, _c2;
    for (unsigned int i=0; i<4; ++i) {
      _c1[i] = c1[i];
      _c2[i] = c2[i];
    }
    vector<Color> result(line.size());
    result[0] = c1;
    result[line.size()-1] = c2;
    _c2 -= _c1;
    _c2 /= lineLength(line);
    for (unsigned int i = 1; i < line.size() - 1; ++i) {
      float delta = sqrnorm(line[i-1] - line[i]);
      _c1 += _c2 * delta;
      result[i] = Color((unsigned char)_c1[0], (unsigned char)_c1[1], (unsigned char)_c1[2], (unsigned char)_c1[3]);
    }
    return result;
  }
  //================================================
  vector<float> getSizes(const vector<Coord> &line, float s1, float s2) {
    vector<float> result(line.size());
    result[0] = s1;
    result[line.size()-1] = s2;
    s2 -= s1;
    s2 /= lineLength(line);
    for (unsigned int i = 1; i < line.size() - 1; ++i) {
      float delta = sqrnorm(line[i-1] - line[i]);
      s1 += s2 * delta;
      result[i] = s1;
    }
    return result;
  }
  //===============================================
  struct CurvePoints {
    GLfloat *data;
    unsigned int size;
    CurvePoints(unsigned int size):size(size) {
      data = new GLfloat[2*size*3];
    }
    void addPoint(){
      GLfloat *newData=new GLfloat[2*(size+1)*3];
      memcpy(newData,data,sizeof(GLfloat)*size*3);
      memcpy(newData+((size+1)*3),data+(size*3),sizeof(GLfloat)*size*3);
      delete[] data;
      data=newData;
      ++size;
    }
    Coord& operator()(unsigned int i, unsigned int j) {
      return (Coord&)data[j*size*3+i*3];
    }
  };
  //================================================
  GLfloat* buildCurvePoints (const vector<Coord> &vertices,
			     const vector<float> &sizes,
			     const Coord &startN,
			     const Coord &endN,
			     unsigned int &resultSize,
			     vector<unsigned int> *dec){
    unsigned int resultDec=0;
    bool inversion=false;
    CurvePoints result(vertices.size());
    //start point
    Coord xu = startN - vertices[0];
    xu /= xu.norm();
    Coord xv = Coord(0,0,1.);
    Coord dir = xu^xv;
    if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
    result(0,0) = vertices[0] - dir*sizes[0];
    result(0,1) = vertices[0] + dir*sizes[0];
    //============
    for(unsigned int i=1; i< vertices.size() - 1; ++i) {
      Coord xu = vertices[i-1] - vertices[i];
      Coord xv = vertices[i+1] - vertices[i];
      float n_xu = xu.norm();
      float n_xv = xv.norm();
      xu /= n_xu;
      xv /= n_xv;


      Coord bi_xu_xv = xu+xv;
      if(bi_xu_xv ==Coord(0,0,0)) {
        //two point at the same coord
        result(i+resultDec,0)=result(i+resultDec-1,0);
        result(i+resultDec,1)=result(i+resultDec-1,1);
        continue;
      }
      bi_xu_xv /= bi_xu_xv.norm();
      float newSize=sizes[i];
      Coord u=vertices[i-1]-vertices[i];
      Coord v=vertices[i+1]-vertices[i];
      float angle=M_PI-acos((u[0]*v[0]+u[1]*v[1]+u[2]*v[2])/(u.norm()*v.norm()));
      if(isnan(angle))
        angle=0;
      newSize=newSize/cos(angle/2.);

      if(angle<M_PI/2+M_PI/4) {
        //normal form
        if ((xu^xv)[2] > 0) {
          result(i+resultDec,0 + inversion) = vertices[i] + bi_xu_xv*newSize;
          result(i+resultDec,1 - inversion) = vertices[i] - bi_xu_xv*newSize;
        } else {
          result(i+resultDec,0 + inversion) = vertices[i] - bi_xu_xv*newSize;
          result(i+resultDec,1 - inversion) = vertices[i] + bi_xu_xv*newSize;
        }

      }else{
        //broken form
        Coord vectUnit(-bi_xu_xv[1],bi_xu_xv[0],bi_xu_xv[2]);

        if(!(newSize>u.norm() || newSize>v.norm() || fabs(angle-M_PI)<1E-5)) {
          result.addPoint();
          if(dec)
            dec->push_back(i);
          if ((xu^xv)[2] > 0) {
            result(i+resultDec,0 + inversion) = vertices[i] + bi_xu_xv*newSize;
            result(i+resultDec,1 - inversion) = vertices[i] - vectUnit*sizes[1];
            result(i+resultDec+1,0 + inversion) = vertices[i] + bi_xu_xv*newSize;
            result(i+resultDec+1,1 - inversion) = vertices[i] + vectUnit*sizes[i];
          }else{
            result(i+resultDec,1 - inversion) = vertices[i] + bi_xu_xv*newSize;
            result(i+resultDec,0 + inversion) = vertices[i] + vectUnit*sizes[i];
            result(i+resultDec+1,1 - inversion) = vertices[i] + bi_xu_xv*newSize;
            result(i+resultDec+1,0 + inversion) = vertices[i] - vectUnit*sizes[i];
          }
          ++resultDec;
        }else{
          if ((xu^xv)[2] > 0) {
            result(i+resultDec,0 + inversion) = vertices[i] + vectUnit*sizes[1];
            result(i+resultDec,1 - inversion) = vertices[i] - vectUnit*sizes[1];
            inversion=!inversion;
          }else{
            result(i+resultDec,1 - inversion) = vertices[i] - vectUnit*sizes[i];
            result(i+resultDec,0 + inversion) = vertices[i] + vectUnit*sizes[i];
            inversion=!inversion;
          }
        }
      }
    }
    //end point
    xu = endN - vertices[vertices.size()-1];
    xu /= xu.norm();
    xv = Coord(0,0,-1);
    dir = xu^xv;
    if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
    result(vertices.size()-1+resultDec,0 + inversion) = vertices[vertices.size()-1] - dir*sizes[vertices.size()-1];
    result(vertices.size()-1+resultDec,1 - inversion) = vertices[vertices.size()-1] + dir*sizes[vertices.size()-1];
    resultSize=vertices.size()+resultDec;
    return result.data;
  }
  //==============================================
  vector<Coord> splineCurve(const vector<Coord> &vertices) {
    vector<Coord> curve;
    curve.push_back(vertices[0]);
    for(unsigned int i=1; i< vertices.size() - 1; ++i) {
      Coord xu = vertices[i-1]-vertices[i];
      Coord xv = vertices[i+1]-vertices[i];
      if ((xu^xv).norm() < 1E-3) continue;
      float n_xu = xu.norm();
      float n_xv = xv.norm();
      xu /= n_xu;
      xv /= n_xv;
      Coord bi_xu_xv = xu+xv;
      bi_xu_xv /= bi_xu_xv.norm();
      Coord tgt_xu_xv = xu^xv;
      tgt_xu_xv /= tgt_xu_xv.norm();
      Coord dir = tgt_xu_xv ^ bi_xu_xv;
      dir /= dir.norm();
      curve.push_back(vertices[i] - (dir * (n_xu/5.0f)));
      curve.push_back(vertices[i]);
      curve.push_back(vertices[i] + (dir * (n_xv/5.0f)));
    }
    curve.push_back(vertices.back());
    return curve;
  }
  bool visible(const Coord &startPoint,const std::vector<Coord> &bends, const Coord &endPoint,
	       const MatrixGL &transformMatrix, const Vector<int, 4> &viewport) {
    if (bends.size() == 0)
      return segmentVisible(startPoint, endPoint, transformMatrix, viewport) > 0.;

    //first point
    if (segmentVisible(startPoint, bends[0], transformMatrix, viewport)>0.)
      return true;
    for (unsigned int i=1; i<bends.size(); ++i)
      if (segmentVisible(bends[i-1], bends[i], transformMatrix, viewport)>0.)
	return true;
    if (segmentVisible(endPoint, bends.back(), transformMatrix, viewport)>0.)
      return true;
    return false;
  }

  vector<Coord> computeCleanVertices(const vector<Coord> &bends,
				     const Coord &startPoint, const Coord& endPoint,
				     Coord &startN, Coord &endN) {
    vector<Coord> result;
    if (bends.size() > 0) {
      result.push_back(startPoint);
      Coord lastPoint = bends[0];
      if ((startPoint - lastPoint).norm()> 1E-4)
	result.push_back(lastPoint);
      unsigned int i;
      for(i = 1; i < bends.size(); ++i) {
	Coord currentPoint = bends[i];
	if ((currentPoint - lastPoint).norm() > 1E-4) {
	  result.push_back(currentPoint);
	}
	lastPoint = currentPoint;
      }
      if ((endPoint - lastPoint).norm() > 1E-4) {
	lastPoint = endPoint;
	result.push_back(endPoint);
      }
      if (result.size() < 2) { //only one valid point for a line
	result.clear();
	return result;
      }
      //Adjust tangent direction
      if ((startN - startPoint).norm() < 1E-4) {
	startN = startPoint - (result[1] - startPoint);
      }
      if ((endN - lastPoint).norm()<1E-4) {
	endN = lastPoint + lastPoint - result[result.size()-2];
      }
      return result;
    } else {
      if ((startPoint - endPoint).norm()> 1E-4) {
	result.push_back(startPoint);
	result.push_back(endPoint);
	if ((startN - startPoint).norm() < 1E-4) {
	  startN = startPoint - (endPoint - startPoint);
	}
	if ((endN - endPoint).norm() < 1E-4) {
	  endN = endPoint + endPoint - startPoint;
	}
      }
      return result;
    }
  }
  //=============================================
  void curveVisibility(const Coord &startPoint,const std::vector<Coord> &bends, const Coord &endPoint,
		       const Size &size, bool &drawPoly, bool &drawLine, const MatrixGL &projectionMatrix,
		       const MatrixGL &modelviewMatrix, const Vector<int, 4> &viewport) {
    float s1 = projectSize(startPoint, Size(size[0], size[0], size[0]),
			   projectionMatrix, modelviewMatrix, viewport);
    float s2 = projectSize(endPoint, Size(size[1], size[1], size[1]),
			   projectionMatrix, modelviewMatrix, viewport);
    //    cerr << startPoint<< "/" << endPoint << "/" << size << "/" << s1 << "/" << s2 << endl;
    drawLine = drawPoly = (s1 > 0.) || (s2 > 0.) ||
      visible(startPoint, bends, endPoint,  modelviewMatrix * projectionMatrix, viewport);
    if (drawLine) {
      s1 = fabs(s1);
      s2 = fabs(s2);
      if (s1 < 2. && s2 < 2.)
	drawPoly = false;
      if (s1 > 2. && s2 > 2.)
	drawLine = false;
    }
  }
  //=============================================
  void polyLine(const vector<Coord> & vertices,
		const vector<Color> & colors) {
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      setColor(colors[i]);
      glVertex3fv((float *)&vertices[i]);
    }
    glEnd();
  }
  void polyLine(const vector<Coord> &vertices,
		const Color &c1, const Color &c2) {
    polyLine(vertices, getColors(vertices, c1, c2));
  }
  //=============================================
  void polyQuad(const vector<Coord> &vertices,
		const vector<Color> &colors,
		const vector<float> &sizes,
		const Coord & startN, const Coord &endN,
		bool colorInterpolate,const Color &borderColor,
		const string &textureName) {
    unsigned int size;
    vector<unsigned int> decTab;
    unsigned int dec=0;
    GLfloat *points = buildCurvePoints(vertices, sizes, startN, endN,size,&decTab);
    if(textureName!="") {
      GlTextureManager::getInst().activateTexture(textureName);
    }

    float length;
    glBegin(GL_QUAD_STRIP);
    for (unsigned int i = 0; i < size; ++i) {
      if(dec<decTab.size())
        if(i==decTab[dec])
          dec++;

      glColor4ubv(((const GLubyte *)&colors[i-dec]));

      if(i==0){
        length=0.;
        glTexCoord2f(0, 0.0f);
        glVertex3fv(&points[i*3]);
        glTexCoord2f(0, 1.0f);
        glVertex3fv(&points[i*3 + size*3]);
      }else{
        if(textureName!="") {
          Coord p1_0(points[i*3 - 3],points[i*3-2],points[i*3-1]);
          Coord p1_1(points[i*3],points[i*3+1],points[i*3+2]);
          Coord p2_0(points[i*3 + size*3 - 3],points[i*3 + size*3-2],points[i*3 + size*3-1]);
          Coord p2_1(points[i*3 + size*3],points[i*3 + size*3+1],points[i*3 + size*3+2]);
          length += ((p1_1+p2_1)/2.-(p1_0+p2_0)/2.).norm()/(p1_0-p2_0).norm();
        }

        glTexCoord2f(length, 0.0f);
        glVertex3fv(&points[i*3]);
        glTexCoord2f(length, 1.0f);
        glVertex3fv(&points[i*3 + size*3]);
      }
    }
    glEnd();

    if(textureName!="")
      GlTextureManager::getInst().desactivateTexture();
    
    dec=0;
    glBegin(GL_LINE_STRIP);
    if(!colorInterpolate)
      glColor4ubv(((const GLubyte *)&borderColor));
    for (unsigned int i = 0; i < size; ++i) {
      if(dec<decTab.size())
	if(i==decTab[dec])
	  dec++;
	if(colorInterpolate)
	  glColor4ubv(((const GLubyte *)&colors[i-dec]));
	glVertex3fv(&points[i*3]);
    }
    glEnd();
    dec=0;
    glBegin(GL_LINE_STRIP);
    if(!colorInterpolate)
      glColor4ubv(((const GLubyte *)&borderColor));
    for (unsigned int i = 0; i < size; ++i) {
      if(dec<decTab.size())
	if(i==decTab[dec])
	  dec++;
	if(colorInterpolate)
	  glColor4ubv(((const GLubyte *)&colors[i-dec]));
	glVertex3fv(&points[i*3 + size*3]);
    }
    glEnd();

    delete [] points;
  }
  //=============================================
  void polyQuad(const vector<Coord> &vertices,
		const vector<Color> &colors,
		const vector<float> &sizes,
		const Coord & startN, const Coord &endN,
		const string &textureName){
    polyQuad(vertices,colors,sizes,startN,endN,true,Color(0,0,0,0),textureName);
  }
  //=============================================
  void polyQuad(const vector<Coord> &vertices,
		const Color &c1, const Color &c2,
		float s1, float s2,
		const Coord &startN, const Coord &endN,
		bool colorInterpolate,const Color &borderColor,
		const string &textureName) {
    polyQuad(vertices,
	     getColors(vertices, c1, c2),
	     getSizes(vertices, s1, s2),
	     startN, endN,
	     colorInterpolate,borderColor,
	     textureName);
  }
  //=============================================
  void polyQuad(const vector<Coord> &vertices,
		const Color &c1, const Color &c2,
		float s1, float s2,
		const Coord &startN, const Coord &endN,
		const string &textureName) {
    polyQuad(vertices,
	     getColors(vertices, c1, c2),
	     getSizes(vertices, s1, s2),
	     startN, endN,textureName);
  }
  //=============================================
  void simpleQuad(const vector<Coord> &vertices,
        const Color &c1, const Color &c2,
        float s1, float s2,
        const Coord &startN, const Coord &endN, const Coord &lookDir,
        bool colorInterpolate,const Color &borderColor,
        const string &textureName) {

    vector<Color> colors=getColors(vertices, c1, c2);
    vector<float> sizes=getSizes(vertices, s1, s2);

    CurvePoints result(vertices.size());
    //start point
    Coord xu = startN - vertices[0];
    xu /= xu.norm();
    Coord dir = xu^lookDir;
    if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
    result(0,0) = vertices[0] - dir*s1;
    result(0,1) = vertices[0] + dir*s1;

    //============
    for(unsigned int i=1; i< vertices.size() - 1; ++i) {
      Coord xu = vertices[i-1] - vertices[i];
      Coord xv = vertices[i+1] - vertices[i];
      xu = xu ^ lookDir;
      xv = xv ^ (-lookDir);
      xu /= xu.norm();
      xv /= xv.norm();
      Coord xu_xv=xu+xv;
      xu_xv /= xu_xv.norm();


      float newSize=sizes[i];
      Coord u=vertices[i-1]-vertices[i];
      Coord v=vertices[i+1]-vertices[i];
      float angle=M_PI-acos((u[0]*v[0]+u[1]*v[1]+u[2]*v[2])/(u.norm()*v.norm()));
      if(isnan(angle))
        angle=0;
      newSize=newSize/cos(angle/2.);

      result(i,0) = vertices[i] - xu_xv*newSize;
      result(i,1) = vertices[i] + xu_xv*newSize;
    }
    //end point
    xu = endN - vertices[vertices.size()-1];
    xu = xu ^ lookDir;
    xu /= xu.norm();
    result(vertices.size()-1,1) = vertices[vertices.size()-1] - xu*sizes[vertices.size()-1];
    result(vertices.size()-1,0) = vertices[vertices.size()-1] + xu*sizes[vertices.size()-1];

    unsigned int size=vertices.size();
    GLfloat *points = result.data;

    if(textureName!="") {
    	glActiveTexture(GL_TEXTURE0);
    	glEnable(GL_TEXTURE_2D);
    	GlTextureManager::getInst().activateTexture(textureName);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
    	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
    }

    string newTextureName1=TulipBitmapDir+"cylinderTexture.png";
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    GlTextureManager::getInst().activateTexture(newTextureName1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    float length;
    glBegin(GL_QUAD_STRIP);
    for (unsigned int i = 0; i < size; ++i) {
      glColor4ubv(((const GLubyte *)&colors[i]));
      if(i==0){
        length=0.;
        glMultiTexCoord2f(GL_TEXTURE0,0, 1.0f);
        glMultiTexCoord2f(GL_TEXTURE1,0, 1.0f);
        glVertex3fv(&points[i*3]);
        glMultiTexCoord2f(GL_TEXTURE0,0, 0.0f);
        glMultiTexCoord2f(GL_TEXTURE1,0, 0.0f);
        glVertex3fv(&points[i*3 + size*3]);
      }else{
        Coord p1_0(points[i*3 - 3],points[i*3-2],points[i*3-1]);
        Coord p1_1(points[i*3],points[i*3+1],points[i*3+2]);
        Coord p2_0(points[i*3 + size*3 - 3],points[i*3 + size*3-2],points[i*3 + size*3-1]);
        Coord p2_1(points[i*3 + size*3],points[i*3 + size*3+1],points[i*3 + size*3+2]);

        length += ((p1_1+p2_1)/2.-(p1_0+p2_0)/2.).norm()/(p1_0-p2_0).norm();
        glMultiTexCoord2f(GL_TEXTURE0,length, 1.0f);
        glMultiTexCoord2f(GL_TEXTURE1,length, 1.0f);
        glVertex3fv(&points[i*3]);
        glMultiTexCoord2f(GL_TEXTURE0,length, 0.0f);
        glMultiTexCoord2f(GL_TEXTURE1,length, 0.0f);
        glVertex3fv(&points[i*3 + size*3]);
      }
    }
    glEnd();

    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    if(textureName!="") {
    	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    	GlTextureManager::getInst().desactivateTexture();
    }

    glBegin(GL_LINE_STRIP);
    if(!colorInterpolate)
      glColor4ubv(((const GLubyte *)&borderColor));
    for (unsigned int i = 0; i < size; ++i) {
      if(colorInterpolate)
        glColor4ubv(((const GLubyte *)&colors[i]));
      glVertex3fv(&points[i*3]);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    if(!colorInterpolate)
      glColor4ubv(((const GLubyte *)&borderColor));
    for (unsigned int i = 0; i < size; ++i) {
      if(colorInterpolate)
        glColor4ubv(((const GLubyte *)&colors[i]));
      glVertex3fv(&points[i*3 + size*3]);
    }
    glEnd();

    delete [] points;
  }
  //=============================================
  void bezierCylinder(const vector<Coord> &,
		      const Color &, const Color &,
		      float, float,
		      const Coord &, const Coord &) {
    /* gleCoord  *point_array  = new gleCoord [30 + 2];
    gleColor  *color_array  = new gleColor [30 + 2];
    gleDouble *radius_array = new gleDouble[30 + 2];

      for (unsigned int i = 0; i < 30; ++i) {
      Bezier(double (&p)[3], const double (*points)[3], unsigned int size, double mu);
    }
    */
  }
  void bezierQuad(const vector<Coord> &vertices,
		  const Color &c1, const Color &c2,
		  float s1, float s2,
		  const Coord &startN, const Coord &endN) {
    unsigned int MAX_BENDS = 8;
    if (vertices.size()>MAX_BENDS) {
      vector<float> sizes = getSizes(vertices, s1, s2);
      vector<Color> colors = getColors(vertices, c1, c2);
      vector<Coord> points(MAX_BENDS);
      for (unsigned int i=0;i<MAX_BENDS;++i)
	points[i]=vertices[i];
      Coord dir = vertices[MAX_BENDS - 1] - vertices[(MAX_BENDS - 2)];
      dir /= dir.norm();
      dir *= ((vertices[MAX_BENDS-1] - vertices[MAX_BENDS]).norm()/5.);
      bezierQuad(points, c1, colors[MAX_BENDS - 1], s1, sizes[MAX_BENDS - 1], startN, vertices[MAX_BENDS-1] + dir);
      vector<Coord> newCurve(vertices.size()-(MAX_BENDS - 2));
      newCurve[0] = vertices[MAX_BENDS - 1];
      newCurve[1] = vertices[MAX_BENDS - 1] + dir;
      for (unsigned int i = MAX_BENDS; i<vertices.size(); ++i)
       newCurve[i-(MAX_BENDS) + 2] = vertices[i];
      bezierQuad(newCurve, colors[MAX_BENDS-1], c2, sizes[MAX_BENDS-1], s2,
		 vertices[MAX_BENDS - 2],
		 endN);
      return;
    }

    unsigned int steps = 40;
    Vector<float, 4> baseColor, delta;
    for(unsigned int i = 0; i<4; ++i) {
      baseColor[i] = c1[i];
      delta[i] = float(c2[i]) - float(c1[i]);
    }
    delta /= steps;
    unsigned int size;
    GLfloat *points    = buildCurvePoints(vertices, getSizes(vertices, s1, s2), startN, endN,size);
    GLfloat *pointsIt  = points;
    glMap2f(GL_MAP2_VERTEX_3, 0., 1.0, 3, size , 0.0, 1.0,
	    size*3, 2, pointsIt );
    glEnable(GL_MAP2_VERTEX_3);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 0.0f, 1.0f);

    Vector<float, 4> color=baseColor;
    for (unsigned int i = 0; i <= steps; ++i) {
      glColor4ub((unsigned char)color[0], (unsigned char)color[1],
          (unsigned char)color[2], (unsigned char)color[3]);
      glTexCoord2f(0.0f, 0.0f);
      glEvalCoord2f((GLfloat) i/steps,0);
      glColor4ub((unsigned char)color[0], (unsigned char)color[1],  //Need to be done, bug of opengl ???
          (unsigned char)color[2], (unsigned char)color[3]);
      glTexCoord2f(1.0f, 1.0f);
      glEvalCoord2f((GLfloat) i/steps,1);
      color += delta;
    }
    glEnd();

    color=baseColor;
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i <= steps; ++i) {
      glColor4ub((unsigned char)color[0], (unsigned char)color[1],
          (unsigned char)color[2], (unsigned char)color[3]);
      glEvalCoord2f((GLfloat) i/steps,0);
      color += delta;
    }
    glEnd();
    color=baseColor;
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i <= steps; ++i) {
      glColor4ub((unsigned char)color[0], (unsigned char)color[1],
          (unsigned char)color[2], (unsigned char)color[3]);
      glEvalCoord2f((GLfloat) i/steps,1);
      color += delta;
    }
    glEnd();

    glDisable(GL_MAP2_VERTEX_3);
    delete [] points;
  }

  void bezierLine(const vector<Coord> &vertices,
		  const Color &c1, const Color &c2) {

    unsigned int MAX_BENDS = 8;
    if (vertices.size()>MAX_BENDS) {
      vector<Coord> points(MAX_BENDS);
      vector<Color> colors = getColors(vertices, c1, c2);
      for (unsigned int i=0;i<MAX_BENDS;++i)
	points[i]=vertices[i];
      bezierLine(points, c1, colors[MAX_BENDS - 1]);

      Coord dir = vertices[MAX_BENDS - 1] - vertices[(MAX_BENDS - 2)];
      dir /= dir.norm();
      dir *= ((vertices[MAX_BENDS-1] - vertices[MAX_BENDS]).norm()/5.);

      vector<Coord> newCurve(vertices.size()-(MAX_BENDS - 2));
      newCurve[0] = vertices[MAX_BENDS - 1];
      newCurve[1] = vertices[MAX_BENDS - 1] + dir;
      for (unsigned int i = MAX_BENDS; i<vertices.size(); ++i)
       newCurve[i-(MAX_BENDS) + 2] = vertices[i];
      bezierLine(newCurve, colors[MAX_BENDS-1], c2);
      return;
    }


    GLfloat *data = new GLfloat[vertices.size()*3];
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      *((Coord *)&data[i*3]) = vertices[i];
    }
    unsigned int steps = 40;
    Vector<float, 4> color, delta;
    for(unsigned int i = 0; i<4; ++i) {
      color[i] = c1[i];
      delta[i] = float(c2[i]) - float(c1[i]);
    }
    delta /= steps;
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, vertices.size(), data);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i <= steps; ++i) {
      setColor(Color((unsigned char)color[0], (unsigned char)color[1],(unsigned char)color[2], (unsigned char)color[3]));
      glEvalCoord1f((GLfloat) i/steps);
      color += delta;
    }
    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
    delete [] data;
  }
  //==================================================================
  void splineCylinder(const vector<Coord> &vertices,
		      const Color &c1, const Color &c2,
		      float s1, float s2,
		      const Coord &startN, const Coord &endN) {
    tlp::bezierCylinder(splineCurve(vertices), c1, c2, s1, s2, startN, endN);
  }
  void splineQuad(const vector<Coord> &vertices,
		  const Color &c1, const Color &c2,
		  float s1, float s2,
		  const Coord &startN, const Coord &endN) {
    tlp::bezierQuad(splineCurve(vertices), c1, c2, s1, s2, startN, endN);
  }
  void splineLine(const vector<Coord> &vertices,
		  const Color &c1, const Color &c2) {
    tlp::bezierLine(splineCurve(vertices), c1, c2);
  }
}
