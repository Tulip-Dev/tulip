#include <cmath>

#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/Graph.h>
#include <tulip/GlGraph.h>
#include <tulip/GlTools.h>

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
  virtual void draw(node n);
  virtual Coord getAnchor(const Coord &vector) const;

protected:
  GLuint LList;
  bool listOk;
  void drawSquare();
  void drawSquareBorder();
};
//=====================================================
GLYPHPLUGIN(Square, "2D - Square", "David Auber", "09/07/2002", "Textured square", "1", "1", 4);
//===================================================================================
Square::Square(GlyphContext *gc): Glyph(gc),listOk(false) {
}
//=====================================================
Square::~Square() {
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 2);
}
//=====================================================
void Square::draw(node n) {
  setMaterial(glGraph->elementColor->getNodeValue(n));
  string texFile = glGraph->elementTexture->getNodeValue(n);
  if (texFile != "") {
    if (glGraph->activateTexture(texFile))
      setMaterial(Color(255,255,255,0));
  }
  if (!listOk) {
    LList = glGenLists( 2 );
    glNewList( LList, GL_COMPILE ); 
    drawSquare();
    glEndList();
    glNewList(LList + 1, GL_COMPILE ); 
    drawSquareBorder();
    glEndList();
    listOk=true;
  }
  glCallList(LList);
  ColorProperty *border = glGraph->getGraph()->getProperty<ColorProperty>("viewBorderColor");
  glGraph->desactivateTexture();
  Color c = border->getNodeValue(n);
  glLineWidth(2);
  glDisable(GL_LIGHTING);
  glColor4ub(c[0],c[1],c[2],c[3]);
  glCallList(LList + 1);
  glEnable(GL_LIGHTING);
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
