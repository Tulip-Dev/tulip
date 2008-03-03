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
/// A 2D glyph
/**
 * This glyph draws a textured hexagone using the "viewTexture"
 * node property value. If this property has no value, the hexagone
 * is then colored using the "viewColor" node property value.
 */
class Hexagone : public Glyph {
public:
  Hexagone(GlyphContext *gc=NULL);
  virtual ~Hexagone();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual void draw(node n);

protected:
  void drawHexagone();
  void drawHexagoneBorder();
};
//=====================================================
GLYPHPLUGIN(Hexagone, "2D - Hexagone", "David Auber", "09/07/2002", "Textured Hexagone", "1.0", 13);
//===================================================================================
Hexagone::Hexagone(GlyphContext *gc): Glyph(gc) {
}
//=====================================================
Hexagone::~Hexagone() {
  
}
//=====================================================
void Hexagone::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0.15,0.15,0);
  boundingBox.second=Coord(0.85,0.85,1);
}
//=====================================================
void Hexagone::draw(node n) {
  if(GlDisplayListManager::getInst().beginNewDisplayList("Hexagone_hexagone")) {
    drawHexagone();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  if(GlDisplayListManager::getInst().beginNewDisplayList("Hexagone_hexagoneborder")) {
    drawHexagoneBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
  
  GlDisplayListManager::getInst().callDisplayList("Hexagone_hexagone");
    
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
  GlDisplayListManager::getInst().callDisplayList("Hexagone_hexagoneborder");
  glEnable(GL_LIGHTING);
}
//=====================================================
void Hexagone::drawHexagone() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);  
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 6, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 6, 1);
  gluDeleteQuadric(quadratic);
}

void Hexagone::drawHexagoneBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI/6.0;
  for (unsigned int i = 0; i < 6; ++i) {
    glVertex3f(0.5*cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
}
/*@}*/
