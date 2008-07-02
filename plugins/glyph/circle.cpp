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

/** \addtogroup glyph */
/*@{*/
/// A 2D glyph.
/**
 * This glyph draws a textured disc using the "viewTexture" node
 * property value. If this property has no value, the disc is then colored
 * using the "viewColor" node property value.
 */
class Circle : public Glyph {
public:
  Circle(GlyphContext *gc=NULL);
  virtual ~Circle();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual void draw(node n);

protected:
  void drawCircle();
  void drawCircleBorder();
};
//=====================================================
GLYPHPLUGIN(Circle, "2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.0", 14);
//===================================================================================
Circle::Circle(GlyphContext *gc): Glyph(gc) {
}
//=====================================================
Circle::~Circle() {
}
//=====================================================
void Circle::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0.15,0.15,0);
  boundingBox.second=Coord(0.85,0.85,1);
}
//=====================================================
void Circle::draw(node n) {
  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
  if(GlDisplayListManager::getInst().beginNewDisplayList("Circle_circle")) {
    drawCircle();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  if(GlDisplayListManager::getInst().beginNewDisplayList("Circle_circleborder")) {
    drawCircleBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  setColor(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
  
  GlDisplayListManager::getInst().callDisplayList("Circle_circle");
    
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
  GlDisplayListManager::getInst().callDisplayList("Circle_circleborder");
  glEnable(GL_LIGHTING);
}
//=====================================================
void Circle::drawCircle() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);  
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 30, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 30, 1);
  gluDeleteQuadric(quadratic);
}

void Circle::drawCircleBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI/30.0;
  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.5*cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
}
/*@}*/
