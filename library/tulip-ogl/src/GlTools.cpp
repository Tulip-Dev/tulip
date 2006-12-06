#include <GL/glu.h>
#include <tulip/Rectangle.h>
#include "tulip/GlTools.h"
#include <iostream>

using namespace std;
namespace tlp {
  //====================================================
  void glTest(string message) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      cerr << "[OpenGL ERROR] : " << message << endl;
      cerr << "=============> : " << gluErrorString(error) <<  endl;
      assert (error == GL_NO_ERROR);
    }
  }
  //====================================================
  void setColor(const Color &c) {
    glColor3ubv((unsigned char *) &c);
  }
  //====================================================
  void setMaterial(const Color &c) {
    float colorMat[4];
    colorMat[0] = ((float)c[0])/255.0;
    colorMat[1] = ((float)c[1])/255.0;
    colorMat[2] = ((float)c[2])/255.0;
    colorMat[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorMat);
  }
  //====================================================
  bool projectToScreen(const Coord &obj, 
		       const MatrixGL &transform,
		       const Vector<int, 4> &viewport,
		       Vector<float, 2> &result) {
    GLfloat winw;
    //  [objx, objy, objz, 1] * transform;
    winw = 
      transform[0][3] * obj[0] + 
      transform[1][3] * obj[1] +  
      transform[2][3] * obj[2] + 
      transform[3][3];
    if ( fabs(winw) < 1E-6 ) {
      cerr << "unprojectable" << endl;
      return false;
    }
    //    cerr << winw << endl;
    result[0] = ( (transform[0][0] * obj[0] + 
		   transform[1][0] * obj[1] +  
		   transform[2][0] * obj[2] + 
		   transform[3][0] )
		  /winw * 0.5 + 0.5 ) * float(viewport[2]) + float(viewport[0]);
    result[1] = ( (transform[0][1] * obj[0] + 
		   transform[1][1] * obj[1] +  
		   transform[2][1] * obj[2] + 
		   transform[3][1] )
		  /winw * 0.5 + 0.5) * float(viewport[3]) + float(viewport[1]);
    return true;
  }
  //====================================================
  double segmentSize(const Coord &u, const Coord &v, 
		     const tlp::MatrixGL &transform, 
		     const Vector<int, 4> &viewport) {
    Vector<float,2> p1, p2;
    projectToScreen(u, transform, viewport, p1);
    projectToScreen(v, transform, viewport, p2);
    return sqr(p1[0]-p2[0]) + sqr(p1[1]-p2[1]);
  }
  //====================================================
  double segmentVisible(const Coord &u, const Coord &v, 
			const tlp::MatrixGL &transform, 
			const Vector<int, 4> &viewport) {
    //    cerr << __PRETTY_FUNCTION__ << endl;
    Vector<float,2> p1, p2;
    projectToScreen(u, transform, viewport, p1);
    projectToScreen(v, transform, viewport, p2);
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
      return -size;
    }
    else 
      return size;
  }
  //====================================================
  GLfloat projectSize(const Coord& position, const Size &_size, 
		      const MatrixGL &projectionMatrix, const MatrixGL &modelviewMatrix, 
		      const Vector<int, 4> &viewport) {
    double max = -1;
    float  nSize = _size.norm(); //Enclosing bounding box

    MatrixGL translate;
    translate.fill(0);
    for (unsigned int i = 0; i<4; ++i)
      translate[i][i] = 1;
    for (unsigned int i = 0; i<3; ++i)
      translate[3][i] = position[i];

    MatrixGL tmp(translate * modelviewMatrix);
    //MatrixGL tmp(modelviewMatrix);
    tmp[0][0] = nSize; tmp[0][1] = 0;     tmp[0][2] = 0;     
    tmp[1][0] = 0;     tmp[1][1] = 0;     tmp[1][2] = 0;     
    tmp[2][0] = 0;     tmp[2][1] = 0;     tmp[2][2] = 0;     

    //    cerr << modelviewMatrix << endl;
    //    cerr << projectionMatrix << endl;

    tmp = tmp * projectionMatrix;
    tmp.transpose();
    Vector<float, 4> vect1;
    vect1.fill(0);
    vect1[0] = 0.5;
    vect1[3] = 1.0;
    Vector<float, 4> proj1 =  tmp * vect1;
    Vector<float, 4> vect2;
    vect2.fill(0);
    vect2[3] = 1.0;
    Vector<float, 4> proj2 =  tmp * vect2;
    //cerr << proj1 << endl;
    //cerr << proj2 << endl;
    float x1 = (proj1[0]/proj1[3] * 0.5 + 0.5 ) * viewport[2];
    float x2 = (proj2[0]/proj2[3] * 0.5 + 0.5 ) * viewport[2];
    //    cerr << x1 << "," << x2 << endl;
    float width = fabs(x1 - x2);
    float size = sqr(2. * width);
    //    cerr << "size :" << size << endl;
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
    
    //    cerr << r1 << endl;
    //    cerr << r2 << endl;
    
    if (!r1.intersect(r2)) {
      //      cerr << "not visible" << endl;
      size *= -1.0;
    }
    
    //    Vector<float, 2> upleft;
    //    r1[0] = 
    //    cerr << size << endl;
    return size;
    
    //    cerr << "s: " << nSize << " => " << minx << "," << miny << "/" << maxx << "," << maxy << endl;
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
}
