#include <cmath>

#include <string>
#include <GL/gl.h>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>

#include <tulip/Graph.h>
#include <tulip/GlGraph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

class Billboard : public Glyph {
public:
  Billboard(GlyphContext *gc=NULL);
  virtual ~Billboard();
  virtual void draw(node n);
  virtual Coord getAnchor(const Coord &vector) const;

protected:
  GLuint LList;
  bool listOk;
  void drawBillboard();
};

GLYPHPLUGIN(Billboard, "2D - Billboard", "Gerald Gainant", "08/03/2004", "Textured billboard", "1", "1", 7);

//===================================================================================
Billboard::Billboard(GlyphContext *gc): Glyph(gc),listOk(false) {
}
//========================================================
Billboard::~Billboard() {
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 1);
}
//========================================================
void Billboard::draw(node n) {
  setMaterial(glGraph->elementColor->getNodeValue(n));
  string texFile = glGraph->elementTexture->getNodeValue(n);
  if(texFile.size() && texFile != "") {
    if (glGraph->activateTexture(texFile)) {
//      setMaterial(Color(255,255,255,0));
    }
  }
  if (!listOk) {
    LList = glGenLists( 1 );
    glNewList( LList, GL_COMPILE ); 
    drawBillboard();
    glEndList();
    listOk=true;
  }

  // setup orientation
  float mdlM[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mdlM );
  glMatrixMode( GL_MODELVIEW_MATRIX );
  glPushMatrix();
  Size sz(1,1,1);
  if( glGraph->elementSize )
    sz = glGraph->elementSize->getNodeValue(n);
  float nx = sz.getW();
  float ny = sz.getH();
  float nz = sz.getD();
  mdlM[0]  = nx;
  mdlM[5]  = ny;
  mdlM[10] = nz;
  mdlM[1] = mdlM[2] = 0.0f;
  mdlM[4] = mdlM[6] = 0.0f;
  mdlM[8] = mdlM[9] = 0.0f;
  glLoadMatrixf( mdlM );
  glCallList(LList);
  glPopMatrix();
}
//========================================================
Coord Billboard::getAnchor(const Coord & vector ) const {
  Coord v( vector );
  float x,y,z, fmax;
  v.get(x,y,z);
  v.setZ( 0.0f );
  fmax = std::max(fabsf(x), fabsf(y));// fmax = fabsf(x) >? fabsf(y);
  if( fmax > 0.0f )
    return v * (0.5f/fmax);
  else
    return v;
}
//========================================================
void Billboard::drawBillboard() {
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-0.5f, -0.5f); 
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(0.5f, -0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-0.5f, 0.5f);
  glDisable(GL_ALPHA_TEST);
  glEnd();
}
//========================================================
