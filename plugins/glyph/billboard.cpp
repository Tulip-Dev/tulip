/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
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
/// A 2D glyph.
/**
 * This glyph draws a textured square using the "viewTexture"
 * node property value. If this property has no value, the square is
 *  then colored using the "viewColor" node property value.
 * It is unsensitive to any axis rotation and so always remains displayed
 * in the same position.
 */
class Billboard : public Glyph {
public:
  Billboard(GlyphContext *gc=NULL);
  virtual ~Billboard();
  virtual void draw(node n,float lod);
  virtual Coord getAnchor(const Coord &vector) const;

protected:
  void drawBillboard();
};

GLYPHPLUGIN(Billboard, "2D - Billboard", "Gerald Gainant", "08/03/2004", "Textured billboard", "1.0", 7);

//===================================================================================
Billboard::Billboard(GlyphContext *gc): Glyph(gc){
}
//========================================================
Billboard::~Billboard() {
}
//========================================================
void Billboard::draw(node n,float) {
  if(GlDisplayListManager::getInst().beginNewDisplayList("Billboard_billboard")) {
    drawBillboard();
    GlDisplayListManager::getInst().endNewDisplayList();
  }

  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
  
  
  // setup orientation
  float mdlM[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mdlM );
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  Size sz(1,1,1);
  if( glGraphInputData->elementSize )
    sz = glGraphInputData->elementSize->getNodeValue(n);
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
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  GlDisplayListManager::getInst().callDisplayList("Billboard_billboard");
  glDisable(GL_ALPHA_TEST);
  glPopMatrix();
  GlTextureManager::getInst().desactivateTexture();
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
  glEnd();
}
//========================================================
/*@}*/
