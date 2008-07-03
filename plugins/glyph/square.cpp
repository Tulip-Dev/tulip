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
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 2D glyph.
/**
 * This glyph draws a textured square using the "viewTexture" node
 * property value. If this property has no value, the square
 * is then colored using the "viewColor" node property value.
 */
class Square : public Glyph {
public:
  Square(GlyphContext *gc=NULL);
  virtual ~Square();
  virtual void draw(node n,float lod);
  virtual Coord getAnchor(const Coord &vector) const;

protected:
  void drawSquare();
  void drawSquareBorder();
};
//=====================================================
GLYPHPLUGIN(Square, "2D - Square", "David Auber", "09/07/2002", "Textured square", "1.0", 4);
//===================================================================================
Square::Square(GlyphContext *gc): Glyph(gc) {
}
//=====================================================
Square::~Square() {
}
//=====================================================
void Square::draw(node n,float lod) {
  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  if(GlDisplayListManager::getInst().beginNewDisplayList("Square_square")) {
    drawSquare();
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  if(GlDisplayListManager::getInst().beginNewDisplayList("Square_squareborder")) {
    drawSquareBorder();
    GlDisplayListManager::getInst().endNewDisplayList();
  }

  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }

  GlDisplayListManager::getInst().callDisplayList("Square_square");

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
      double lineWidth = borderWidth->getNodeValue(n);
      if (lineWidth < 1e-6) glLineWidth (1e-6); //no negative borders
      else glLineWidth (lineWidth);
    }
    
    glDisable(GL_LIGHTING);
    glColor4ub(c[0],c[1],c[2],c[3]);
    GlDisplayListManager::getInst().callDisplayList("Square_squareborder");
    glEnable(GL_LIGHTING);
  }
}
//=====================================================
Coord Square::getAnchor(const Coord &vector) const {
  Coord v( vector );
  float x,y,z, fmax;
  v.get(x,y,z);
  v.setZ( 0.0f );
  fmax = std::max(fabsf(x), fabsf(y));
  if( fmax > 0.0f )
    return v * (0.5f/fmax);
  else
    return v;
}
//=====================================================
void Square::drawSquare() {
  glBegin(GL_QUADS);
  /* front face */
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-0.5f, -0.5f); 
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(0.5f, -0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-0.5f, 0.5f);
  /* back face */
  glNormal3f(0.0f, 0.0f,-1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(-0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(-0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0.5f, -0.5f);
  glEnd();
}
//=====================================================
void Square::drawSquareBorder() {
  glBegin(GL_LINE_LOOP);
  /* front face */
  glVertex2f(-0.5f, -0.5f); 
  glVertex2f(0.5f, -0.5f);
  glVertex2f(0.5f, 0.5f);
  glVertex2f(-0.5f, 0.5f);
  glEnd();
}
/*@}*/
