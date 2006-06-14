#include <GL/gl.h>
#include <GL/glu.h>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>

#include <tulip/Graph.h>
#include <tulip/Glyph.h>

#include <iostream>
using namespace std;
using namespace tlp;

class Sphere : public Glyph {
public:
  Sphere(GlyphContext *gc=NULL);
  virtual ~Sphere();
  virtual string getName() {return string("Sphere");}
  virtual void draw(node n);
  virtual void setLOD(int n);

private:
  GLuint LList;
  bool listOk;
};

GLYPHPLUGIN(Sphere, "3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1", "1", 2);

//=========================================================================================
Sphere::Sphere(GlyphContext *gc): Glyph(gc),listOk(false) {
  setLOD(30);
}

Sphere::~Sphere(){
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 1);
}

void Sphere::setLOD(int n) {
  LOD = ((n<0) ? 0 : ((n > 20) ? 20 : n));
  if (listOk) {glDeleteLists(LList, 1);listOk=false;};
}

void Sphere::draw(node n) {
  this->setMaterial(glGraph->elementColor->getNodeValue(n));
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
    gluSphere(quadratic, 0.5f, LOD, LOD);
    glEndList();
    gluDeleteQuadric(quadratic);
    listOk=true;
  }
  assert(glIsList(LList));
  glCallList(LList);
}


