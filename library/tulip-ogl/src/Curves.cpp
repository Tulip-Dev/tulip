#include <vector>
#include <GL/gl.h>
#include <gle.h>
#include <tulip/Vector.h>
#include <tulip/Size.h>
#include "tulip/Curves.h"
#include "tulip/GlTools.h"


using namespace std;

namespace {
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
    Coord& operator()(unsigned int i, unsigned int j) {
      return (Coord&)data[j*size*3+i*3];
    }
  };
  //================================================
  GLfloat* buildCurvePoints (const vector<Coord> &vertices, 
			     const vector<float> &sizes,
			     const Coord &startN,
			     const Coord &endN,
			     GlGraph *glGraph) {

    //    Camera cam = glGraph->getCamera();
    /*
    vector<float> zVal(vvertices.size());
    vector<Coord> vertices(vvertices.size());
    for (unsigned int i = 0; i< vvertices.size(); ++i) {
      Coord tmp = vvertices[i];
      //   glGraph->worldTo2DScreen(tmp[0], tmp[1], tmp[2]);
      zVal[i] = tmp[2];
      tmp[2] = 0;
      vertices[i] = tmp;
    }
    */
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
      if ((xu^xv).norm() < 1E-3) {
	Coord xv = Coord(0,0,1);
	xu /= xu.norm();
	Coord dir = xu^xv;
	if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
	result(i,0) = vertices[i] - dir*sizes[i];
	result(i,1) = vertices[i] + dir*sizes[i];
      } 
      else {
	Coord bi_xu_xv = xu+xv;
	bi_xu_xv /= bi_xu_xv.norm();
	if ((xu^xv)[2] > 0) {
	  result(i,0) = vertices[i] + bi_xu_xv*sizes[i];
	  result(i,1) = vertices[i] - bi_xu_xv*sizes[i];
	} else {
	  result(i,0) = vertices[i] - bi_xu_xv*sizes[i];
	  result(i,1) = vertices[i] + bi_xu_xv*sizes[i];
	}
      }
    }
    //end point
    xu = endN - vertices[vertices.size()-1];
    xu /= xu.norm();    
    xv = Coord(0,0,-1);
    dir = xu^xv;
    if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
    result(vertices.size()-1,0) = vertices[vertices.size()-1] - dir*sizes[vertices.size()-1];
    result(vertices.size()-1,1) = vertices[vertices.size()-1] + dir*sizes[vertices.size()-1];
    //============
    /*
      for (unsigned int i = 0; i< vvertices.size(); ++i)
      for (unsigned int j = 0; j < 2; ++j) {
	Coord tmp = result(i,j);
	tmp[2] = zVal[i];
	//glGraph->screenTo3DWorld(tmp[0], tmp[1], tmp[2]);
	result(i,j) = tmp;
      }
    */
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
      curve.push_back(vertices[i] - (dir * (n_xu/5.0)));
      curve.push_back(vertices[i]);
      curve.push_back(vertices[i] + (dir * (n_xv/5.0)));
    }
    curve.push_back(vertices.back());
    return curve;
  }
  bool visible(const Coord &startPoint,const std::vector<Coord> &bends, const Coord &endPoint,
	       const GLdouble *transformMatrix, const GLint *viewportArray) {
    if (bends.size() == 0) 
      return segmentVisible(startPoint, endPoint, transformMatrix, viewportArray) > 0.;
    
    //first point
    if (segmentVisible(startPoint, bends[0], transformMatrix, viewportArray)>0.)
      return true;
    for (unsigned int i=1; i<bends.size(); ++i) 
      if (segmentVisible(bends[i-1], bends[i], transformMatrix, viewportArray)>0.)
	return true;
    if (segmentVisible(endPoint, bends.back(), transformMatrix, viewportArray)>0.)
      return true;
    return false;
  }
}

namespace tlp {  
  
  void curveVisibility(const Coord &startPoint,const std::vector<Coord> &bends, const Coord &endPoint,
		       const Size &size, bool &drawPoly, bool &drawLine, const GLdouble *transformMatrix, const GLint *viewportArray) {
    Size tmp(size[0], size[0], size[0]);
    Size tmp2(size[1], size[1], size[1]);
    float s1 = projectSize(startPoint, tmp,  transformMatrix, viewportArray);
    float s2 = projectSize(endPoint,   tmp2, transformMatrix, viewportArray);
    //    cerr << startPoint<< "/" << endPoint << "/" << size << "/" << s1 << "/" << s2 << endl;
    drawLine = false;
    drawPoly = false;
    if (s1>0. || s2>0.) {
      drawLine = true;
      drawPoly = true;
    } else {
      if (visible(startPoint, bends, endPoint, transformMatrix, viewportArray)) {
	drawLine = true;
	drawPoly = true;
      }
    }
    if (fabs(s1)<2. && fabs(s2)<2.)
      drawPoly = false;
    if (fabs(s1)>2. && fabs(s2)>2.)
      drawLine = false;
  }
  //=============================================
  void polyLine(const vector<Coord> & vertices, 
		const vector<Color> & colors) {
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      glColor4ubv(((const GLubyte *)&colors[i]));
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
		GlGraph *glGraph) {
    GLfloat *points = buildCurvePoints(vertices, sizes, startN, endN, glGraph);
    glBegin(GL_QUAD_STRIP);
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      glColor4ubv(((const GLubyte *)&colors[i]));
      glVertex3fv(&points[i*3]);
      glVertex3fv(&points[i*3 + vertices.size()*3]);
    } 
    glEnd();
    delete [] points;
  }
  void polyQuad(const vector<Coord> &vertices, 
		const Color &c1, const Color &c2, 
		float s1, float s2,
		const Coord &startN, const Coord &endN,
		GlGraph *glGraph) {
    polyQuad(vertices, 
	     getColors(vertices, c1, c2), 
	     getSizes(vertices, s1, s2),
	     startN, endN, glGraph); 
  }
  //=============================================
  typedef gleDouble gleCoord[3];
  void polyCylinder(const vector<Coord> &vertices, 
		    const vector<Color> &colors, 
		    const vector<float> &sizes,
		    const Coord & startN, const Coord &endN) {

    
    gleCoord  *point_array  = new gleCoord [vertices.size() + 2];
    gleColor  *color_array  = new gleColor [vertices.size() + 2];
    gleDouble *radius_array = new gleDouble[vertices.size() + 2];
    
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      gleColor col;
      col[0] = (float)colors[i][0]/255.; 
      col[1] = (float)colors[i][1]/255.; 
      col[2] = (float)colors[i][2]/255.; 
      for (unsigned int j=0; j < 3; ++j) {
	color_array[i+1][j]   = col[j];
	point_array[i+1][j] = vertices[i][j];
      }
      radius_array[i+1] = sizes[i];
    }
    for (unsigned int j=0; j < 3; ++j) {
      point_array[0][j] = startN[j];
      point_array[vertices.size()+1][j] = endN[j];
    }
    glePolyCone(vertices.size()+2, point_array, color_array, radius_array);
  }
  void polyCylinder(const vector<Coord> &vertices, 
		    const Color &c1, const Color &c2, 
		    float s1, float s2,
		    const Coord &startN, const Coord &endN) {
    polyCylinder(vertices, 
		 getColors(vertices, c1, c2), 
		 getSizes(vertices, s1, s2),
		 startN, endN); 
  }
  //=============================================
  void bezierCylinder(const vector<Coord> &vertices, 
		      const Color &c1, const Color &c2, 
		      float s1, float s2,
		      const Coord &startN, const Coord &endN) {
    gleCoord  *point_array  = new gleCoord [30 + 2];
    gleColor  *color_array  = new gleColor [30 + 2];
    gleDouble *radius_array = new gleDouble[30 + 2];
    /*
      for (unsigned int i = 0; i < 30; ++i) {
      Bezier(double (&p)[3], const double (*points)[3], unsigned int size, double mu);
    }
    */
  }
  void bezierQuad(const vector<Coord> &vertices, 
		  const Color &c1, const Color &c2, 
		  float s1, float s2,
		  const Coord &startN, const Coord &endN,
		  GlGraph *glGraph) {
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
      bezierQuad(points, c1, colors[MAX_BENDS - 1], s1, sizes[MAX_BENDS - 1], startN, vertices[MAX_BENDS-1] + dir, glGraph);
      vector<Coord> newCurve(vertices.size()-(MAX_BENDS - 2));
      newCurve[0] = vertices[MAX_BENDS - 1];
      newCurve[1] = vertices[MAX_BENDS - 1] + dir;
      for (unsigned int i = MAX_BENDS; i<vertices.size(); ++i)
       newCurve[i-(MAX_BENDS) + 2] = vertices[i];
      bezierQuad(newCurve, colors[MAX_BENDS-1], c2, sizes[MAX_BENDS-1], s2, 
		 vertices[MAX_BENDS - 2], 
		 endN, glGraph);
      return;
    }

    unsigned int steps = 40;
    Vector<float, 4> color, delta;
    for(unsigned int i = 0; i<4; ++i) {
      color[i] = c1[i];
      delta[i] = float(c2[i]) - float(c1[i]);
    }
    delta /= steps;
    GLfloat *points    = buildCurvePoints(vertices, getSizes(vertices, s1, s2), startN, endN, glGraph);
    GLfloat *pointsIt  = points;
    glMap2f(GL_MAP2_VERTEX_3, 0., 1.0, 3, vertices.size() , 0.0, 1.0, 
	    vertices.size()*3, 2, pointsIt );
    glEnable(GL_MAP2_VERTEX_3);
    glBegin(GL_QUAD_STRIP);
    for (unsigned int i = 0; i <= steps; ++i) {
      glColor4ub((unsigned char)color[0], (unsigned char)color[1], 
		 (unsigned char)color[2], (unsigned char)color[3]); 
      glEvalCoord2f((GLfloat) i/steps,0);
      glColor4ub((unsigned char)color[0], (unsigned char)color[1],  //Need to be done, bug of opengl ???
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
      glColor4ub((unsigned char)color[0], (unsigned char)color[1],
		 (unsigned char)color[2], (unsigned char)color[3]); 
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
		  const Coord &startN, const Coord &endN, GlGraph *glGraph) {
    tlp::bezierQuad(splineCurve(vertices), c1, c2, s1, s2, startN, endN, glGraph);
  }
  void splineLine(const vector<Coord> &vertices, 
		  const Color &c1, const Color &c2) {
    tlp::bezierLine(splineCurve(vertices), c1, c2);
  }
}
