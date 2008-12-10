#include <cmath>

#include <string>

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
 * This glyph draws a textured pentagone using the "viewTexture"
 * node property value. If this property has no value, the pentagone
 * is then colored using the "viewColor" node property value.
 */
class Pentagone : public Glyph {
public:
  Pentagone(GlyphContext *gc=NULL);
  virtual ~Pentagone();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual void draw(node n,float lod);

protected:
  void drawPentagone();
  void drawPentagoneBorder();
};
//=====================================================
GLYPHPLUGIN(Pentagone, "2D - Pentagone", "David Auber", "09/07/2002", "Textured Pentagone", "1.0", 12);
//===================================================================================
Pentagone::Pentagone(GlyphContext *gc): Glyph(gc) {
}
//=====================================================
Pentagone::~Pentagone() {
}
//=====================================================
void Pentagone::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0.15,0.15,0);
  boundingBox.second=Coord(0.85,0.85,0);
}
//=====================================================
void Pentagone::draw(node n, float lod) {
  if(GlDisplayListManager::getInst().beginNewDisplayList("Pentagone_pentagone")) {
    drawPentagone();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  if(GlDisplayListManager::getInst().beginNewDisplayList("Pentagone_pentagoneborder")) {
    drawPentagoneBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }

  GlDisplayListManager::getInst().callDisplayList("Pentagone_pentagone");

  GlTextureManager::getInst().desactivateTexture();

  if(lod>20) {
  ColorProperty *borderColor = glGraphInputData->getGraph()->getProperty<ColorProperty>("viewBorderColor");
  DoubleProperty *borderWidth = 0;
  if (glGraphInputData->getGraph()->existProperty ("viewBorderWidth"))
    borderWidth = glGraphInputData->getGraph()->getProperty<DoubleProperty>("viewBorderWidth");
  if (borderWidth == 0) glLineWidth(2);
  else {
    double lineWidth = borderWidth->getNodeValue (n);
    if (lineWidth < 1e-6) glLineWidth (1e-6); //no negative borders
    else glLineWidth (lineWidth);
  }
  glDisable(GL_LIGHTING);
  setColor(borderColor->getNodeValue(n));
  GlDisplayListManager::getInst().callDisplayList("Pentagone_pentagoneborder");
  glEnable(GL_LIGHTING);
}
}

//=====================================================
void Pentagone::drawPentagone() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 5, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.0f, 0.5f, 5, 1);
  gluDeleteQuadric(quadratic);
}

void Pentagone::drawPentagoneBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI/5.0;
  for (unsigned int i = 0; i < 5; ++i) {
    glVertex3f(0.5*cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
}
/*@}*/
