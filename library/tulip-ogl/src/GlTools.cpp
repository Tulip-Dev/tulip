#include <GL/glu.h>
#include "tulip/GlTools.h"
#include <iostream>

using namespace std;
namespace tlp {
  //====================================================
  bool glTest(){
    if (glGetError() != GL_NO_ERROR) {
      cerr << "[ERROR] Open GL ERROR : " << glGetError() << gluErrorString(glGetError()) <<  endl;
      return false;
    }
    return true;
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
  void multMatrix(const GLdouble *projectionMatrix, const GLdouble *modelviewMatrix, GLdouble *result){
    GLdouble sum_cell = 0;
    for(unsigned int i=0; i<4; ++i) { // (i,j) sur projection
      for(unsigned int l=0; l<4; ++l) { // (j,l) sur modelview
	for(unsigned int j=0; j<4; ++j) {
	  sum_cell += projectionMatrix[4*j+i] * modelviewMatrix[4*l+j];
	}
	result[4*l+i] = sum_cell;
	sum_cell = 0;
      }
    }
  }
  //====================================================
  bool projectToScreen(const GLdouble objx, const GLdouble objy, const GLdouble objz, 
		       const GLdouble *transform,
		       const GLint *viewport,
		       GLdouble &winx, GLdouble &winy){
    
    GLdouble winw;
    // transform * [objx, objy, objz, 1]
    winw = transform[3] * objx + transform[7] * objy +  transform[11] * objz + transform[15];
    if(winw == 0.0) return false;

    winx = ( (transform[0] * objx + transform[4] * objy +  transform[8] * objz + transform[12] )
	     /winw * 0.5 + 0.5 ) * viewport[2] + viewport[0];
    winy = ( (transform[1] * objx + transform[5] * objy +  transform[9] * objz + transform[13])
	     /winw * 0.5 + 0.5) * viewport[3] + viewport[1];
    
    return true;
  }
  //====================================================
  bool segmentVisible(const Coord &u, const Coord &v, const GLdouble *transform, const GLint *viewport) {
    GLdouble x1Scr,y1Scr;
    GLdouble x2Scr,y2Scr;
    projectToScreen(u[0], u[1], u[2], transform, viewport, x1Scr, y1Scr);
    projectToScreen(v[0], v[1], v[2], transform, viewport, x2Scr, y2Scr);
    GLdouble minx = viewport[0];
    GLdouble miny = viewport[1];
    GLdouble maxx = minx + viewport[2];
    GLdouble maxy = miny + viewport[3];
    if (!( (x1Scr<minx && x2Scr<minx) || 
	   (x1Scr>maxx && x2Scr>maxx) || 
	   (y1Scr<miny && y2Scr<miny) || 
	   (y1Scr>maxy && y2Scr>maxy) ) )
      return true;
    else 
      return false;
  }
  //====================================================
  GLdouble projectSize(const Coord& position, const Size &_size, const GLdouble *transformMatrix, const GLint *viewport) {

    bool project;  
    bool visible = false;
    GLdouble max = 0;
    GLdouble x1Scr,y1Scr;
    GLdouble x2Scr,y2Scr;
  

    Size size(_size/2.0);
    //  size /= 2.0;
    GLdouble minx = viewport[0];
    GLdouble miny = viewport[1];
    GLdouble maxx = minx + viewport[2];
    GLdouble maxy = miny + viewport[3];

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
}
