#include <GL/gl.h>
#include <cmath>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Glyph.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/** This glyph draws a transparent cube using the "viewBorderColor" node
 * property value to draw its edges.
 */
class CubeOutLinedTransparent : public Glyph {
public:
  CubeOutLinedTransparent(GlyphContext *gc=NULL);
  virtual ~CubeOutLinedTransparent();
  virtual void draw(node n);
  virtual Coord getAnchor(const Coord & vector) const;

private:
  void drawCubeSimple(GLenum);
};

GLYPHPLUGIN(CubeOutLinedTransparent, "3D - Cube OutLined Transparent", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", 9);

//===================================================================================
CubeOutLinedTransparent::CubeOutLinedTransparent(GlyphContext *gc): Glyph(gc) {
}

CubeOutLinedTransparent::~CubeOutLinedTransparent() {
}

void CubeOutLinedTransparent::draw(node n) {
  if(GlDisplayListManager::getInst().beginNewDisplayList("CubeOutLinedTransparent_cube")) {
    drawCubeSimple(GL_LINE_LOOP);
    GlDisplayListManager::getInst().endNewDisplayList();
  }
  
  tlp::setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    if (GlTextureManager::getInst().activateTexture(texturePath+texFile))
      setMaterial(Color(255,255,255,(glGraphInputData->elementColor->getNodeValue(n))[3]));
  }
  //glCallList(LList);
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
  glColor3ub(c[0],c[1],c[2]);
   GlDisplayListManager::getInst().callDisplayList("CubeOutLinedTransparent_cube");
  glEnable(GL_LIGHTING);
}


Coord CubeOutLinedTransparent::getAnchor(const Coord & vector ) const {
  float x,y,z, fmax;
  vector.get(x,y,z);
  fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));
  if( fmax > 0.0f )
    return vector * (0.5f/fmax);
  else
    return vector;
}

void CubeOutLinedTransparent::drawCubeSimple(GLenum type) {
  /* front face */
  glBegin(type);
  glVertex3f(-0.5f, -0.5f, 0.5f); 
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glEnd();
  /* back face */
  glBegin(type);
  glVertex3f(-0.5f, -0.5f, -0.5f); 
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glEnd();
  /* right face */
  glBegin(type);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glEnd();
  /* left face */
  glBegin(type);
  glVertex3f(-0.5f, -0.5f, 0.5f); 
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glEnd();
  /* top face */
  glBegin(type);
  glVertex3f(0.5f, 0.5f, 0.5f); 
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glEnd();
  /* bottom face */
  glBegin(type);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glEnd();
}
/*@}*/
