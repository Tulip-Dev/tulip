/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/Graph.h>
#include <tulip/GlRect.h>

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

  static GlRect *rect;
};

GlRect* Billboard::rect=0;

GLYPHPLUGIN(Billboard, "2D - Billboard", "Gerald Gainant", "08/03/2004", "Textured billboard", "1.0", 7);

//===================================================================================
Billboard::Billboard(GlyphContext *gc): Glyph(gc){
	if(!rect)
		rect = new GlRect(Coord(0,0,0),Size(1,1,0),Color(0,0,0,255),Color(0,0,0,255));
}
//========================================================
Billboard::~Billboard() {
}
//========================================================
void Billboard::draw(node n,float lod) {

  rect->setFillColor(glGraphInputData->getElementColor()->getNodeValue(n));
  rect->setOutlineColor(glGraphInputData->getElementBorderColor()->getNodeValue(n));

  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    rect->setTextureName(texturePath+texFile);
  }else{
    rect->setTextureName("");
  }
  double borderWidth=glGraphInputData->getElementBorderWidth()->getNodeValue(n);
  if (borderWidth < 1e-6)
    borderWidth=1e-6;
  rect->setOutlineSize(borderWidth);
  
  // setup orientation
  float mdlM[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mdlM );
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  Size sz(1,1,1);
  if( glGraphInputData->getElementSize() )
    sz = glGraphInputData->getElementSize()->getNodeValue(n);
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
  rect->draw(lod,NULL);
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
/*@}*/
