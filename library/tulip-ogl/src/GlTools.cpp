#include <GL/glu.h>
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
  bool projectToScreen(const GLfloat objx, const GLfloat objy, const GLfloat objz, 
		       const MatrixGL &transform,
		       const GLint *viewport,
		       GLfloat &winx, GLfloat &winy){
    
    GLfloat winw;
    // transform * [objx, objy, objz, 1]
    winw = 
      transform[0][3] * objx + 
      transform[1][3] * objy +  
      transform[2][3] * objz + 
      transform[3][3];

    if ( fabs(winw) < 1E-6 ) return false;

    winx = ( (transform[0][0] * objx + 
	      transform[1][0] * objy +  
	      transform[2][0] * objz + 
	      transform[3][0] )
	     /winw * 0.5 + 0.5 ) * viewport[2] + viewport[0];
    winy = ( (transform[0][1] * objx + 
	      transform[1][1] * objy +  
	      transform[2][1] * objz + 
	      transform[3][1] )
	     /winw * 0.5 + 0.5) * viewport[3] + viewport[1];
    return true;
  }
  //====================================================
  double segmentSize(const Coord &u, const Coord &v, const tlp::MatrixGL &transform, const GLint *viewport) {
    GLfloat x1Scr,y1Scr;
    GLfloat x2Scr,y2Scr;
    projectToScreen(u[0], u[1], u[2], transform, viewport, x1Scr, y1Scr);
    projectToScreen(v[0], v[1], v[2], transform, viewport, x2Scr, y2Scr);
    return sqr(x1Scr-x2Scr) + sqr(y1Scr-y2Scr);
  }
  //====================================================
  double segmentVisible(const Coord &u, const Coord &v, const tlp::MatrixGL &transform, const GLint *viewport) {
    GLfloat x1Scr,y1Scr;
    GLfloat x2Scr,y2Scr;
    projectToScreen(u[0], u[1], u[2], transform, viewport, x1Scr, y1Scr);
    projectToScreen(v[0], v[1], v[2], transform, viewport, x2Scr, y2Scr);
    GLfloat minx = viewport[0];
    GLfloat miny = viewport[1];
    GLfloat maxx = minx + viewport[2];
    GLfloat maxy = miny + viewport[3];
    double size = sqr(x1Scr-x2Scr) + sqr(y1Scr-y2Scr);
    if (!( (x1Scr<minx && x2Scr<minx) || 
	   (x1Scr>maxx && x2Scr>maxx) || 
	   (y1Scr<miny && y2Scr<miny) || 
	   (y1Scr>maxy && y2Scr>maxy) ) )
      return size;
    else 
      return -size;
  }
  //====================================================
  GLfloat projectSize(const Coord& position, const Size &_size, const MatrixGL &transformMatrix, const GLint *viewport) {
    bool project;  
    bool visible = false;
    GLfloat max = 0;
    GLfloat x1Scr,y1Scr;
    GLfloat x2Scr,y2Scr;
  

    Size size(_size/2.0);
    //  size /= 2.0;
    GLfloat minx = viewport[0];
    GLfloat miny = viewport[1];
    GLfloat maxx = minx + viewport[2];
    GLfloat maxy = miny + viewport[3];

    //test x-axis
    if (project=(projectToScreen(position[0] - size[0], position[1], position[2],transformMatrix, viewport,x1Scr,y1Scr) &&
		 projectToScreen(position[0] + size[0], position[1], position[2],transformMatrix, viewport,x2Scr,y2Scr))) {
      if (!( (x1Scr<minx && x2Scr<minx) || 
	     (x1Scr>maxx && x2Scr>maxx) || 
	     (y1Scr<miny && y2Scr<miny) || 
	     (y1Scr>maxy && y2Scr>maxy) ) ) {
	visible = true;
      }
      max = std::max( max, sqr(x1Scr-x2Scr)+sqr(y1Scr-y2Scr));
    }

    //test Y-axis
    if (project=(projectToScreen(position[0], position[1] - size[1], position[2],transformMatrix, viewport,x1Scr,y1Scr) &&
		 projectToScreen(position[0], position[1] + size[1], position[2],transformMatrix, viewport,x2Scr,y2Scr))){
      if (!( (x1Scr<minx && x2Scr<minx) || 
	     (x1Scr>maxx && x2Scr>maxx) || 
	     (y1Scr<miny && y2Scr<miny) || 
	     (y1Scr>maxy && y2Scr>maxy) ) ) {

	visible = true;
      }
      max = std::max( max, sqr(x1Scr-x2Scr)+sqr(y1Scr-y2Scr));
    }
  
    //test z-axis
    if (project=(projectToScreen(position[0], position[1], position[2]- size[2],transformMatrix, viewport,x1Scr,y1Scr) &&
		 projectToScreen(position[0], position[1], position[2]+ size[2],transformMatrix, viewport,x2Scr,y2Scr))){
      if (!( (x1Scr<minx && x2Scr<minx) || 
	     (x1Scr>maxx && x2Scr>maxx) || 
	     (y1Scr<miny && y2Scr<miny) || 
	     (y1Scr>maxy && y2Scr>maxy) ) ) {

	visible = true;
      }
      max = std::max( max, sqr(x1Scr-x2Scr)+sqr(y1Scr-y2Scr));
    }
    
    if (!visible) return -max;
    return max;
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
