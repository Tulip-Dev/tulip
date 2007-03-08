#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>

#include <tulip/Glyph.h>
#include <tulip/Graph.h>
#include <tulip/GlGraph.h>
#include <tulip/GlTools.h>

using namespace std;

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {
#endif //DOXYGEN_NOTFOR_DEVEL
/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value. The "viewBorderColor"
 * node property value is used to draw its edges.
 */
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

  GLYPHPLUGIN(CubeOutLined, "3D - CubeOutLined", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", 1);
/*@}*/

  //===================================================================================
  CubeOutLined::CubeOutLined(GlyphContext *gc): Glyph(gc), LList(0), listOk(false) {
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
      drawCubeSimple(GL_LINE_LOOP);
      glEndList();

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
    ColorProperty *borderColor = glGraph->getRenderingParameters().getGraph()->getProperty<ColorProperty>("viewBorderColor");
    DoubleProperty *borderWidth = 0;
    if (glGraph->getRenderingParameters().getGraph()->existProperty ("viewBorderWidth"))
      borderWidth = glGraph->getRenderingParameters().getGraph()->getProperty<DoubleProperty>("viewBorderWidth");
    glGraph->desactivateTexture();
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
}
