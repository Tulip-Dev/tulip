#include <cmath>
#include <string>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <tulip/TulipPlugin.h>
#include <tulip/GlGraph.h>
#include <tulip/Glyph.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// - An implementation of square with variable border glyph.
/**
 * This glyph is an implementation of a square with a variable border.
 * The size of the border depend on the depth of the node, decreasing from the
 * root.
 *
 * AUTHOR:
 *
 * Julien Testut,
 * Antony Durand,
 * Pascal Ollier,
 * Yashvin Nababsing,
 * Sebastien Leclerc,
 * Ruchon Thibault,
 * Eric Dauchier,
 * University Bordeaux I France
 */

//====================================================================
class SquareBorder: public Glyph {
public:
  SquareBorder(GlyphContext* gc = NULL);
  virtual ~SquareBorder();
  virtual string getName() {
    return string("SquareBorder");
  }
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual void  draw(node n,float lod);
  virtual Coord getAnchor(const Coord& vector) const;

protected:
  GLuint LList;
  bool   listOk;
  void   drawSquare();

private:
  static float 	   borderProportion;
  const static float BORDER_COLOR[3];
  const static float MAX_BORDER_COEF;
  Graph*        tree;
  float calculateDepth(node n);
  float calculateBorderSize(node n);
};

//====================================================================
GLYPHPLUGIN(SquareBorder, "SquareBorder",
            "Julien Testut, Antony Durand, Pascal Ollier,"
            "Yashvin Nababsing, Sebastien Leclerc, Ruchon Thibault,"
            "Eric Dauchier",
            "09/07/2004", "Textured square22", "1.0", 100);

const float SquareBorder::MAX_BORDER_COEF = 0.8;
float SquareBorder::borderProportion = 0;
const float SquareBorder::BORDER_COLOR[3] = {0.f, 0.f, 0.f};

//====================================================================
SquareBorder::SquareBorder(GlyphContext* gc):Glyph(gc),listOk(false){
}

//====================================================================
SquareBorder::~SquareBorder() {
  if (listOk)
    if (glIsList(LList))
      glDeleteLists(LList, 1);
}
//=====================================================
void SquareBorder::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0,0,0);
  boundingBox.second=Coord(1,1,0);
}
//====================================================================
void SquareBorder::draw(node n,float lod) {
  tree = (*graph);
  borderProportion  = calculateBorderSize(n);
  Color borderColor = glGraph->elementColor->getNodeValue(n);
  setMaterial(borderColor);
  string texFile = glGraph->elementTexture->getNodeValue(n);

  if (texFile != "") {
    glGraph->activateTexture(texFile);
  }

  drawSquare();
}

//====================================================================
Coord SquareBorder::getAnchor(const Coord& vector) const {
  Coord v(vector);
  float x, y, z, fMax;
  v.get(x, y, z);
  v.setZ(0.0f);
  fMax = max(fabsf(x), fabsf(y));
  if (fMax > 0.0f)
    return v * (0.5f / fMax);
  else
    return v;
}

//====================================================================
void SquareBorder::drawSquare() {
  float borderCalc = borderProportion;
  float relatifBorderCalc = 0.5f * borderCalc;
  glPushAttrib(GL_LIGHTING_BIT);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BORDER_COLOR);

  glBegin(GL_TRIANGLE_STRIP);

  glNormal3f(0.0f, 0.0f, 1.0f);

  // front face
  glVertex2f(0.5f, 0.5f);
  glVertex2f(relatifBorderCalc, relatifBorderCalc);
  glVertex2f(0.5f, -0.5f);

  glVertex2f(relatifBorderCalc, relatifBorderCalc);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(0.5f, -0.5f);

  glVertex2f(0.5f, -0.5f);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-0.5f, -0.5f);

  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-0.5f, -0.5f);

  glVertex2f(-0.5f, -0.5f);
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-0.5f, 0.5f);

  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  glVertex2f(-0.5f, 0.5f);

  glVertex2f(-0.5f, 0.5f);
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  glVertex2f(0.5f, 0.5f);

  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  glVertex2f(relatifBorderCalc, relatifBorderCalc);
  glVertex2f(0.5f, 0.5f);

  glNormal3f(0.0f, 0.0f, -1.0f);

  // back face
  // -------------------------------------------------------
  glVertex2f(0.5f, 0.5f);
  glVertex2f(0.5f, -0.5f);
  glVertex2f(relatifBorderCalc, relatifBorderCalc);

  glVertex2f(relatifBorderCalc, relatifBorderCalc);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(0.5f, -0.5f);
  // ---------------------------------------------------------
  glVertex2f(0.5f, -0.5f);
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);

  glVertex2f(-0.5f, -0.5f);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  // ---------------------------------------------------------
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(-0.5f, 0.5f);

  glVertex2f(-0.5f, 0.5f);
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  // ---------------------------------------------------------
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  glVertex2f(-0.5f, 0.5f);
  glVertex2f(0.5f, 0.5f);

  glVertex2f(0.5f, 0.5f);
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  glVertex2f(relatifBorderCalc, relatifBorderCalc);

  glEnd();

  // ---------------------------------------------------------
  glPopAttrib();

  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex2f(relatifBorderCalc, relatifBorderCalc);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);

  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex2f(relatifBorderCalc, relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, relatifBorderCalc);
  glVertex2f(-relatifBorderCalc, -relatifBorderCalc);
  glVertex2f(relatifBorderCalc, -relatifBorderCalc);
  glEnd();
}
//====================================================================
float SquareBorder::calculateDepth(node n) {
  int             profondeur   = 0;
  Iterator<node>* itNode       = tree->getInNodes(n);
  float           currentIndeg = tree->indeg(n);
  while (currentIndeg != 0) {
    node currentNode = itNode->next();
    currentIndeg     = tree->indeg(currentNode);
    itNode           = tree->getInNodes(currentNode);
    profondeur++;
  }
  delete itNode;
  return profondeur;
}
//====================================================================
float SquareBorder::calculateBorderSize(node n) {
  float profondeur = calculateDepth(n);
  float nbNoeuds   = tree->numberOfNodes();
  if (nbNoeuds == 0)
    return 0;
  return (profondeur / nbNoeuds) * MAX_BORDER_COEF;
}
/*@}*/
