
#include <cmath>

#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>

#include <GL/glew.h>

#include "tulip/OpenGlConfigManager.h"
#include "tulip/Glyph.h"
#include "tulip/Graph.h"
#include "tulip/GlTools.h"
#include "tulip/GlTextureManager.h"
#include "tulip/GlDisplayListManager.h"
#include "tulip/GlGraphRenderingParameters.h"

using namespace std;

namespace tlp {
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
    virtual void draw(node n,float lod);
    virtual Coord getAnchor(const Coord & vector) const;

  private:
    void drawCube(GLenum);
    void drawCubeSimple(GLenum);
  };

  GLYPHPLUGIN(CubeOutLined, "3D - Cube OutLined", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", 1);
/*@}*/

  //===================================================================================
  CubeOutLined::CubeOutLined(GlyphContext *gc): Glyph(gc) {
  }

  CubeOutLined::~CubeOutLined() {
  }

  //    v6----- v5
  //   /|      /|
  //  v1------v0|
  //  | |     | |
  //  | |v7---|-|v4
  //  |/      |/
  //  v2------v3
  static GLfloat cubeInterleavedArrays[] = {
      //***
      1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, // v0
      0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, // v1
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, // v2
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, // v3
      //***
      1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, // v0
      0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, // v3
      0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, // v4
      1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, // v5
      //***
      1.0f, 1.0f, 0.0f,1.0f,0.0f,0.5f,0.5f,0.5f, // v0
      0.0f, 1.0f, 0.0f,1.0f,0.0f,0.5f,0.5f,-0.5f, // v5
      0.0f, 0.0f, 0.0f,1.0f,0.0f,-0.5f,0.5f,-0.5f, // v6
      1.0f, 0.0f, 0.0f,1.0f,0.0f,-0.5f,0.5f,0.5f, // v1
      //***
      1.0f, 1.0f, -1.0f,0.0f,0.0f,-0.5f,0.5f,0.5f, // v1
      0.0f, 1.0f, -1.0f,0.0f,0.0f,-0.5f,0.5f,-0.5f, // v6
      0.0f, 0.0f, -1.0f,0.0f,0.0f,-0.5f,-0.5f,-0.5f, // v7
      1.0f, 0.0f, -1.0f,0.0f,0.0f,-0.5f,-0.5f,0.5f, // v2
      //***
      1.0f, 1.0f, 0.0f,-1.0f,0.0f,-0.5f,-0.5f,-0.5f, // v7
      0.0f, 1.0f, 0.0f,-1.0f,0.0f,0.5f,-0.5f,-0.5f, // v4
      0.0f, 0.0f, 0.0f,-1.0f,0.0f,0.5f,-0.5f,0.5f, // v3
      1.0f, 0.0f, 0.0f,-1.0f,0.0f,-0.5f,-0.5f,0.5f, // v2
      //***
      1.0f, 1.0f, 0.0f,0.0f,-1.0f,0.5f,-0.5f,-0.5f, // v4
      0.0f, 1.0f, 0.0f,0.0f,-1.0f,-0.5f,-0.5f,-0.5f, // v7
      0.0f, 0.0f, 0.0f,0.0f,-1.0f,-0.5f,0.5f,-0.5f, // v6
      1.0f, 0.0f, 0.0f,0.0f,-1.0f,0.5f,0.5f,-0.5f}; // v5

  static GLfloat cubeInterleavedArraysNoTex[] = {
        //***
        0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, // v0
        0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, // v1
        0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, // v2
        0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, // v3
        //***
        1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, // v0
        1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, // v3
        1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, // v4
        1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, // v5
        //***
        0.0f,1.0f,0.0f,0.5f,0.5f,0.5f, // v0
        0.0f,1.0f,0.0f,0.5f,0.5f,-0.5f, // v5
        0.0f,1.0f,0.0f,-0.5f,0.5f,-0.5f, // v6
        0.0f,1.0f,0.0f,-0.5f,0.5f,0.5f, // v1
        //***
        -1.0f,0.0f,0.0f,-0.5f,0.5f,0.5f, // v1
        -1.0f,0.0f,0.0f,-0.5f,0.5f,-0.5f, // v6
        -1.0f,0.0f,0.0f,-0.5f,-0.5f,-0.5f, // v7
        -1.0f,0.0f,0.0f,-0.5f,-0.5f,0.5f, // v2
        //***
        0.0f,-1.0f,0.0f,-0.5f,-0.5f,-0.5f, // v7
        0.0f,-1.0f,0.0f,0.5f,-0.5f,-0.5f, // v4
        0.0f,-1.0f,0.0f,0.5f,-0.5f,0.5f, // v3
        0.0f,-1.0f,0.0f,-0.5f,-0.5f,0.5f, // v2
        //***
        0.0f,0.0f,-1.0f,0.5f,-0.5f,-0.5f, // v4
        0.0f,0.0f,-1.0f,-0.5f,-0.5f,-0.5f, // v7
        0.0f,0.0f,-1.0f,-0.5f,0.5f,-0.5f, // v6
        0.0f,0.0f,-1.0f,0.5f,0.5f,-0.5f}; // v5

  static GLubyte cubeIndices[] = {
      0,1,2,3,
      4,5,6,7,
      8,9,10,11,
      12,13,14,15,
      16,17,18,19,
      20,21,22,23};

  static GLubyte cubeOutlineIndices[] = {
    0,1,1,2,2,3,3,0,
    20,21,21,22,22,23,23,20,
    0,23,1,22,2,21,3,20
  };

  static GLuint buffers[] = {0,0,0,0};

  void CubeOutLined::draw(node n, float lod) {
    glEnable(GL_LIGHTING);

    bool canUseGlew=OpenGlConfigManager::getInst().canUseGlew();

    if(canUseGlew){
      if(buffers[0]==0){
        glGenBuffers(4,buffers);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeInterleavedArrays),cubeInterleavedArrays, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeInterleavedArraysNoTex),cubeInterleavedArraysNoTex, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeOutlineIndices), cubeOutlineIndices, GL_STATIC_DRAW);
      }
    }else{
      if(GlDisplayListManager::getInst().beginNewDisplayList("CubeOutLined_cube")) {
        drawCube(GL_QUADS);
        GlDisplayListManager::getInst().endNewDisplayList();
      }
      if(GlDisplayListManager::getInst().beginNewDisplayList("CubeOutLined_outline")) {
        drawCubeSimple(GL_LINE_LOOP);
        GlDisplayListManager::getInst().endNewDisplayList();
      }
    }


    setMaterial(glGraphInputData->elementColor->getNodeValue(n));
    const string& texFile = glGraphInputData->elementTexture->getNodeValue(n);
    if (texFile.size() != 0) {
      const string& texturePath=glGraphInputData->parameters->getTexturePath();
      GlTextureManager::getInst().activateTexture(texturePath+texFile);
    }

    if(canUseGlew){
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      if(texFile.size() == 0){
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glInterleavedArrays(GL_N3F_V3F, 0, BUFFER_OFFSET(0));
      }else{
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glInterleavedArrays(GL_T2F_N3F_V3F, 0, BUFFER_OFFSET(0));
      }

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
      glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    }else{
      GlDisplayListManager::getInst().callDisplayList("CubeOutLined_cube");
    }

    GlTextureManager::getInst().desactivateTexture();

    DoubleProperty *borderWidth = 0;
    double lineWidth;
    if (glGraphInputData->getGraph()->existProperty ("viewBorderWidth"))
      borderWidth = glGraphInputData->getGraph()->getProperty<DoubleProperty>("viewBorderWidth");
    if (borderWidth == 0)
      lineWidth=2.;
    else {
      const double& lineWidthTmp = borderWidth->getNodeValue (n);
      if (lineWidthTmp < 1e-6)
	lineWidth=1e-6; //no negative borders
      else
	lineWidth=lineWidthTmp;
    }


    if((lineWidth<1 && lod>=20) || (lod>(20/lineWidth))) {
      ColorProperty *borderColor = glGraphInputData->getGraph()->getProperty<ColorProperty>("viewBorderColor");
      const Color& c = borderColor->getNodeValue(n);
      //  setMaterial(c);
      glLineWidth(lineWidth);
      glDisable(GL_LIGHTING);
      glColor4ub(c[0],c[1],c[2],c[3]);
      if(canUseGlew){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
      }else{
        GlDisplayListManager::getInst().callDisplayList("CubeOutLined_outline");
      }
      glEnable(GL_LIGHTING);
    }

    if(canUseGlew){
      glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
      if(texFile.size() != 0){
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      }
      glDisableClientState(GL_NORMAL_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
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
}
