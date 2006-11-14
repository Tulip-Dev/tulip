#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>

#include <tulip/Graph.h>
#include <tulip/GlGraph.h>
#include <tulip/Glyph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/// A 3D glyph.
/**
 * This glyph draws an textured a half cylinder - a cylinder whose height
 * is half the one draws by the Cylinder glyph - using the "viewTexture"
 * node property value. If this property has no value, the half cylinder
 * is then colored using the "viewColor" node property value.
 */
/*@{*/

//=================================================================================================
class HalfCylinder : public Glyph {
public:
  HalfCylinder(GlyphContext *gc=NULL);
  virtual ~HalfCylinder();
  virtual void draw(node n);
  virtual Coord getAnchor(const Coord &vector) const;

private:
  GLuint LList;
  bool listOk;
};

GLYPHPLUGIN(HalfCylinder, "3D - HalfCylinder", "Auber David", "31/07/2002", "Textured HalfCylinder", "1", "1", 10);
//=================================================================================================
HalfCylinder::HalfCylinder(GlyphContext *gc): Glyph(gc),listOk(false) {
}
//=================================================================================================
HalfCylinder::~HalfCylinder() {
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 1);
}
//=================================================================================================
void HalfCylinder::draw(node n) {
  setMaterial(glGraph->elementColor->getNodeValue(n));
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
    //    glTranslatef(0.0f, 0.0f, -0.0f);
    gluQuadricOrientation(quadratic, GLU_INSIDE);
    gluDisk(quadratic, 0.0f, 0.5f, 10, 10);
    gluQuadricOrientation(quadratic, GLU_OUTSIDE);
    gluCylinder(quadratic, 0.5f, 0.5f, 0.5f, 10, 10);  
    glTranslatef(0.0f, 0.0f,0.5f);
    gluDisk(quadratic, 0.0f, 0.5f, 10, 10);
    glEndList();
    gluDeleteQuadric(quadratic);
    listOk=true;
  }
  glCallList(LList);
}
//=================================================================================================
Coord HalfCylinder::getAnchor(const Coord &vector) const {
  Coord anchor = vector;
  float x,y,z,n;
  anchor.get(x,y,z);
  n = sqrt(x*x + y*y);
  if( n == 0.0f )
  	return anchor;
  n = 0.5f / n;
  x *= n;
  y *= n;
  z *= n;
  if (z < 0.0f) z = 0.0f; //z = z >? 0.0f;
  if (z > 0.5f) z = 0.5f; //z = z <? 0.5f;
  return Coord(x,y,z);
}
//=================================================================================================
/*@}*/
