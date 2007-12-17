#include "tulip/GlSphere.h"

#include <GL/gl.h>

#include <tulip/Color.h>

#include "tulip/GlDisplayListManager.h"
#include "tulip/GlTextureManager.h"
#include "tulip/GlTools.h"

using namespace std;

namespace tlp {

  GlSphere::GlSphere(const Coord &position, float radius,const Color& color,float rotX,float rotY,float rotZ):position(position),radius(radius),rotX(rotX),rotY(rotY),rotZ(rotZ),color(color) { 
    boundingBox.first=Coord(position[0]-radius,position[1]-radius,position[2]-radius);
    boundingBox.second=Coord(position[0]+radius,position[1]+radius,position[2]+radius);
  }

  GlSphere::GlSphere(const Coord &position, float radius,const string& textureFile,int alpha,float rotX,float rotY,float rotZ):position(position),radius(radius),textureFile(textureFile),alpha(alpha),rotX(rotX),rotY(rotY),rotZ(rotZ) { 
    boundingBox.first=Coord(position[0]-radius,position[1]-radius,position[2]-radius);
    boundingBox.second=Coord(position[0]+radius,position[1]+radius,position[2]+radius);
  }

  void GlSphere::draw(float lod) {
    /*if(GlDisplayListManager::getInst().beginNewDisplayList("Sphere")) {
      GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluQuadricNormals(quadratic, GLU_SMOOTH);
      gluQuadricTexture(quadratic, GL_TRUE);  
      gluSphere(quadratic,radius, 30, 30);
      GlDisplayListManager::getInst().endNewDisplayList();
      gluDeleteQuadric(quadratic);
    }
    */    
    glPushMatrix();
    glTranslatef(position[0],position[1],position[2]);
    glRotatef(rotX,1,0,0);
    glRotatef(rotY,0,1,0);
    glRotatef(rotZ,0,0,1);
    glScalef(1.,1.,0.9977);
    if (textureFile != "") {
      GlTextureManager::getInst().activateTexture(textureFile);
	if (GlTextureManager::getInst().activateTexture(textureFile))
	  setMaterial(Color(255,255,255,alpha));
    }else{
      setMaterial(color);
    }
    glBegin(GL_QUADS);
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
    //GLUquadricObj *quadratic;
    //quadratic = gluNewQuadric();
      //gluQuadricNormals(quadratic, GLU_SMOOTH);
      //gluQuadricTexture(quadratic, GL_TRUE);  
      //gluSphere(quadratic,radius, 30, 30);
      //gluDeleteQuadric(quadratic);
    //GlDisplayListManager::getInst().callDisplayList("Sphere");
    GlTextureManager::getInst().desactivateTexture();
    glPopMatrix();
  }

}
