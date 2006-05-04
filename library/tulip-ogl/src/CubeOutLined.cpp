#include <GL/gl.h>
#include <cmath>

#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>

#include <tulip/Glyph.h>
#include <tulip/Graph.h>
#include <tulip/GlGraph.h>

using namespace std;

class CubeOutLined : public Glyph {
public:
  CubeOutLined(GlyphContext *gc=NULL);
  virtual ~CubeOutLined();
  virtual string getName() {return string("CubeOutLined");}
  virtual void draw(node n);
  virtual Coord getAnchor(const Coord & vector) const;

private:
  GLuint LList;
  bool listOk;
  void drawCube(GLenum);
  void drawCubeSimple(GLenum);
};

GLYPHPLUGIN(CubeOutLined, "CubeOutLined", "David Auber", "09/07/2002", "Textured cubeOutLined", "1", "1", 1);

//===================================================================================
CubeOutLined::CubeOutLined(GlyphContext *gc): Glyph(gc),listOk(false) {
}

CubeOutLined::~CubeOutLined() {
  if (listOk) {
    if (glIsList(LList)) glDeleteLists(LList, 2);
  }
}

void CubeOutLined::draw(node n) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (!listOk) {
    LList = glGenLists( 2 );
    glNewList( LList, GL_COMPILE );
    drawCube(GL_QUADS);
    glEndList();
    glNewList( LList + 1, GL_COMPILE );
    //    setMaterial(Color(0,0,0,255));
    drawCubeSimple(GL_LINE_LOOP);
    glEndList();
    GLenum error = glGetError();
    if ( error != GL_NO_ERROR)
      cerr << "Open GL Error : " << error << " in " << __PRETTY_FUNCTION__ << endl;
    listOk=true;
  }
  assert(glIsList(LList));
  setMaterial(glGraph->elementColor->getNodeValue(n));
  string texFile = glGraph->elementTexture->getNodeValue(n);
  if (texFile != "") {
    if (glGraph->activateTexture(texFile))
      setMaterial(Color(255,255,255,0));
  }
  glCallList(LList);
  ColorProperty *border = glGraph->getGraph()->getProperty<ColorProperty>("viewBorderColor");
  glGraph->desactivateTexture();
  Color c = border->getNodeValue(n);
  //  setMaterial(c);
  glLineWidth(2);
  glDisable(GL_LIGHTING);
  glColor3ub(c[0],c[1],c[2]);
  glCallList(LList + 1);  
  glEnable(GL_LIGHTING);
}


Coord CubeOutLined::getAnchor(const Coord & vector ) const {
  float x,y,z, fmax;
  vector.get(x,y,z);
  fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));
  if( fmax > 0.0f )
    return vector * (0.5f/fmax);
  else
    return vector;
}


void CubeOutLined::drawCube(GLenum type) {
  /* front face */
  glBegin(type);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f); 
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glEnd();
  /* back face */
  glBegin(type);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glEnd();
  /* right face */
  glBegin(type);
  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glEnd();
  /* left face */
  glBegin(type);
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glEnd();
  /* top face */
  glBegin(type);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, 0.5f, 0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glEnd();
  /* bottom face */
  glBegin(type);
  glNormal3f(0.0f, -1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glEnd();
}
void CubeOutLined::drawCubeSimple(GLenum type) {
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
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, 0.5f, 0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glEnd();
  /* bottom face */
  glBegin(type);
  glNormal3f(0.0f, -1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glEnd();
}
