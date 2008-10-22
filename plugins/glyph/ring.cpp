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
 * This glyph draws a textured disc with a circular hole using the
 * "viewTexture" node property value.
 * If this property has no value, the ring
 * is then colored using the "viewColor" node property value.
 */
class Ring : public Glyph {
public:
  Ring(GlyphContext *gc=NULL);
  virtual ~Ring();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual string getName() {return string("Ring");}
  virtual void draw(node n,float lod);

protected:
  void drawRing();
  void drawRingBorder();
};
//=====================================================
GLYPHPLUGIN(Ring, "2D - Ring", "David Auber", "09/07/2002", "Textured Ring", "1.0", 15);
//===================================================================================
Ring::Ring(GlyphContext *gc): Glyph(gc) {
}
//=====================================================
Ring::~Ring() {
}
//=====================================================
void Ring::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0.15,0.15,0);
  boundingBox.second=Coord(0.85,0.85,1);
}
//=====================================================
void Ring::draw(node n,float lod) {
  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  if(GlDisplayListManager::getInst().beginNewDisplayList("Ring_ring")) {
    drawRing();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  if(GlDisplayListManager::getInst().beginNewDisplayList("Ring_ringborder")) {
    drawRingBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
  
  GlDisplayListManager::getInst().callDisplayList("Ring_ring");

  GlTextureManager::getInst().desactivateTexture();
    
  if(lod>20) {
    ColorProperty *borderColor = glGraphInputData->getGraph()->getProperty<ColorProperty>("viewBorderColor");
    DoubleProperty *borderWidth = 0;
    if (glGraphInputData->getGraph()->existProperty ("viewBorderWidth"))
      borderWidth = glGraphInputData->getGraph()->getProperty<DoubleProperty>("viewBorderWidth");
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
    GlDisplayListManager::getInst().callDisplayList("Ring_ringborder");
    glEnable(GL_LIGHTING);
  }
}
//=====================================================
void Ring::drawRing() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);  
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.2f, 0.5f, 30, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.2f, 0.5f, 30, 1);
  gluDeleteQuadric(quadratic);
}
//=====================================================
void Ring::drawRingBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI/30.0;
  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.5*cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
  glBegin(GL_LINE_LOOP);
  alpha = M_PI / 2.;
  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.2*cos(alpha), 0.2 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
}
//=====================================================
/*@}*/
