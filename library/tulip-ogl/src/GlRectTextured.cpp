#include "tulip/GlRectTextured.h"

#include <GL/gl.h>

#include <tulip/Color.h>
#include "tulip/GlTools.h"

#include "tulip/GlTextureManager.h"

using namespace std;

namespace tlp {
  GlRectTextured::GlRectTextured(const Coord &topLeft,
				 const Coord &bottomRight,
				 const string& textureName)
    :topLeft(topLeft),bottomRight(bottomRight),textureName(textureName) 
  {
    GlTextureManager::getInst().loadTexture(textureName);
  }

  BoundingBox GlRectTextured::getBoundingBox() {
    BoundingBox bb;
    bb.check(topLeft);
    bb.check(bottomRight);
    return bb;
  }

  void GlRectTextured::draw(float lod) {
    glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3ub(255,255,255);
    if(GlTextureManager::getInst().activateTexture(textureName))
      setMaterial(Color(255,255,255,0));
    glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(topLeft[0], topLeft[1], topLeft[2]);				// Top Left
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(bottomRight[0], topLeft[1], topLeft[2]);			// Top Right
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(bottomRight[0], bottomRight[1], topLeft[2]);			// Bottom Right
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(topLeft[0], bottomRight[1], topLeft[2]);			// Bottom Left
    glEnd();
    GlTextureManager::getInst().desactivateTexture();
  }
}
