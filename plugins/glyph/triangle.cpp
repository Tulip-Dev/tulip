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
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Graph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

class Triangle : public Glyph {
public:
  Triangle(GlyphContext *gc=NULL);
  virtual ~Triangle();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual void draw(node n);

protected:
  void drawTriangle();
  void drawTriangleBorder();
};
//=====================================================
GLYPHPLUGIN(Triangle, "2D - Triangle", "David Auber", "09/07/2002", "Textured Triangle", "1.0", 11);
//===================================================================================
Triangle::Triangle(GlyphContext *gc): Glyph(gc){
}
//=====================================================
Triangle::~Triangle() {
}
//=====================================================
void Triangle::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0.25,0,0);
  boundingBox.second=Coord(0.75,0.5,1);
}
//=====================================================
void Triangle::draw(node n) {
  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  if(GlDisplayListManager::getInst().beginNewDisplayList("Triangle_triangle")) {
    drawTriangle();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  if(GlDisplayListManager::getInst().beginNewDisplayList("Triangle_triangleborder")) {
    drawTriangleBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
  
  GlDisplayListManager::getInst().callDisplayList("Triangle_triangle");

  ColorProperty *borderColor = glGraphInputData->getGraph()->getProperty<ColorProperty>("viewBorderColor");
  DoubleProperty *borderWidth = 0;
  if (glGraphInputData->getGraph()->existProperty ("viewBorderWidth"))
    borderWidth = glGraphInputData->getGraph()->getProperty<DoubleProperty>("viewBorderWidth");
  GlTextureManager::getInst().desactivateTexture();
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
  GlDisplayListManager::getInst().callDisplayList("Triangle_triangleborder");
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
