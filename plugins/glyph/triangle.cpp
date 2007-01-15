#include <cmath>

#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>

#include <tulip/Graph.h>
#include <tulip/GlGraph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

class Triangle : public Glyph {
public:
  Triangle(GlyphContext *gc=NULL);
  virtual ~Triangle();
  virtual void draw(node n);

protected:
  GLuint LList;
  bool listOk;
  void drawTriangle();
  void drawTriangleBorder();
};
//=====================================================
GLYPHPLUGIN(Triangle, "2D - Triangle", "David Auber", "09/07/2002", "Textured Triangle", "1", "1", 11);
//===================================================================================
Triangle::Triangle(GlyphContext *gc): Glyph(gc),listOk(false) {
}
//=====================================================
Triangle::~Triangle() {
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 2);
}
//=====================================================
void Triangle::draw(node n) {
  setMaterial(glGraph->elementColor->getNodeValue(n));
  string texFile = glGraph->elementTexture->getNodeValue(n);
  if (texFile != "") {
    if (glGraph->activateTexture(texFile))
      setMaterial(Color(255,255,255,0));
  }
  if (!listOk) {
    LList = glGenLists( 2 );
    glNewList( LList, GL_COMPILE ); 
    drawTriangle();
    glEndList();
    glNewList( LList + 1, GL_COMPILE ); 
    drawTriangleBorder();
    glEndList();
    listOk=true;
  }
  glCallList(LList);
  ColorProperty *borderColor = glGraph->getGraph()->getProperty<ColorProperty>("viewBorderColor");
  DoubleProperty *borderWidth = 0;
  if (glGraph->getRenderingParameters().getGraph()->existProperty ("viewBorderWidth"))
    borderWidth = glGraph->getRenderingParameters().getGraph()->getProperty<DoubleProperty>("viewBorderWidth");
  glGraph->desactivateTexture();
  Color c = borderColor->getNodeValue(n);
  //  setMaterial(c);
  if (borderWidth == 0) glLineWidth(2);
  else {
    double lineWidth = borderWidth->getNodeValue (n);
    if (lineWidth < 1e-6) glLineWidth (1e-6); //no negative borders
    else glLineWidth (lineWidth);
  }
  glDisable(GL_LIGHTING);
  glColor4ub(c[0],c[1],c[2],c[3]);
  glCallList(LList + 1);
  glEnable(GL_LIGHTING);
}
//=====================================================
void Triangle::drawTriangle() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);  
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 3, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 3, 1);
  gluDeleteQuadric(quadratic);
}
//=====================================================
void Triangle::drawTriangleBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI/3.0;
  for (unsigned int i = 0; i < 3; ++i) {
    glVertex3f(0.5*cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
}
//=====================================================
