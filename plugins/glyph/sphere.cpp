#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>

#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/GlTools.h>


using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a textured sphere using the "viewTexture" node
 * property value. If this property has no value, the sphere
 * is then colored using the "viewColor" node property value.
 */
class Sphere : public Glyph {
public:
  Sphere(GlyphContext *gc=NULL);
  virtual ~Sphere();
  virtual void draw(node n);

private:
  GLuint LList;
  bool listOk;
};

GLYPHPLUGIN(Sphere, "3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1", "1", 2);

//=========================================================================================
Sphere::Sphere(GlyphContext *gc): Glyph(gc),listOk(false) {
}

Sphere::~Sphere(){
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 1);
}

void Sphere::draw(node n) {
  tlp::setMaterial(glGraph->elementColor->getNodeValue(n));
  string texFile = glGraph->elementTexture->getNodeValue(n);
  if (texFile != "") {
    if (glGraph->activateTexture(texFile))
      setMaterial(Color(255,255,255,255));
  }
  if (!listOk) {
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);  
    LList = glGenLists( 1 );
    glNewList( LList, GL_COMPILE ); 
    gluSphere(quadratic, 0.5f, 30, 30);
    glEndList();
    gluDeleteQuadric(quadratic);
    listOk=true;
  }
  assert(glIsList(LList));
  glCallList(LList);
}
/*@}*/
