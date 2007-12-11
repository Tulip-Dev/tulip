#include "tulip/GlSphere.h"

#include <GL/gl.h>

#include <tulip/Color.h>

#include "tulip/GlDisplayListManager.h"
#include "tulip/GlTools.h"

using namespace std;

namespace tlp {

  GlSphere::GlSphere(const Coord &position, float radius):position(position),radius(radius) {
    
    boundingBox.first=Coord(position[0]-radius,position[1]-radius,position[2]-radius);
    boundingBox.second=Coord(position[0]+radius,position[1]+radius,position[2]+radius);
  }

  void GlSphere::draw(float lod) {
    if(GlDisplayListManager::getInst().beginNewDisplayList("Sphere")) {
      GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluQuadricNormals(quadratic, GLU_SMOOTH);
      gluQuadricTexture(quadratic, GL_TRUE);  
      gluSphere(quadratic,radius, 30, 30);
      GlDisplayListManager::getInst().endNewDisplayList();
      gluDeleteQuadric(quadratic);
    }
    
    glPushMatrix();
    glTranslatef(position[0],position[1],position[2]);
    setMaterial(Color(0,255,0,255));
    GlDisplayListManager::getInst().callDisplayList("Sphere");
    glPopMatrix();
  }

}
