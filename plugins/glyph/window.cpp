/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#ifndef WINDOWGLYPH_H
#define WINDOWGLYPH_H

#include <tulip/Glyph.h>
#include <tulip/GlRect.h>
#include <tulip/GlLabel.h>
#include <tulip/GlPolyQuad.h>
#include <tulip/GlQuad.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipViewSettings.h>

using namespace tlp;
using namespace std;

namespace tlp {

class Window : public Glyph {
public:
  GLYPHINFORMATION("2D - Window", "David Auber", "28/05/2010", "Window with a title bar", "1.0", NodeShape::Window)
  Window(const tlp::PluginContext *context);
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void getTextBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord &vector) const;

private:
  Color _rectColor;
  Color _textColor;
  const float _borderWidth;
  GlPolyQuad _border;
  GlQuad _center;
  GlQuad _titleRec;
  BoundingBox _bb;
  BoundingBox _textbb;
};

/*
 *
0------------1 -0.5, 0.5      0.5, 0.5
|\          /|
| 4--------5 |
| |  TXT   | |
| 9--------8 |-0.5 + _borderWith   0.5, 0.5-bo
| |  FREE  | |
| 7--------6 |
|/          \|
3------------2  -0.5,-0.5        0.5, -0.5
*/

PLUGIN(Window)

Window::Window(const tlp::PluginContext* context):
  Glyph(context),
  _rectColor(205, 205, 205, 255),
  _textColor(205, 205, 205, 255),
  _borderWidth(0.02f),
  _border(TulipBitmapDir + "halfCylinderTexture.png") {

  const float textheight = 0.05f;

  Coord v[10];
  v[0].set(-0.5, 0.5, 0);
  v[1].set( 0.5, 0.5, 0);
  v[2].set( 0.5,-0.5, 0);
  v[3].set(-0.5,-0.5, 0);
  v[4].set(-0.5 + _borderWidth, 0.5 - _borderWidth, 0);
  v[5].set( 0.5 - _borderWidth, 0.5 - _borderWidth, 0);
  v[6].set( 0.5 - _borderWidth,-0.5 + _borderWidth, 0);
  v[7].set(-0.5 + _borderWidth,-0.5 + _borderWidth, 0);
  v[8].set( 0.5 - _borderWidth, 0.5 - _borderWidth - textheight *2.f, 0);
  v[9].set(-0.5 + _borderWidth, 0.5 - _borderWidth - textheight *2.f, 0);

  _bb.expand(v[6]);
  _bb.expand(v[9]);
  _bb.expand(v[8]);
  _bb.expand(v[7]);

  _border.addQuadEdge(v[0], v[4], _textColor);
  _border.addQuadEdge(v[1], v[5], _textColor);
  _border.addQuadEdge(v[2], v[6], _textColor);
  _border.addQuadEdge(v[3], v[7], _textColor);
  _border.addQuadEdge(v[0], v[4], _textColor);

  _titleRec.setPosition(0, v[4]);
  _titleRec.setPosition(1, v[5]);
  _titleRec.setPosition(2, v[8]);
  _titleRec.setPosition(3, v[9]);
  _titleRec.setTextureName(TulipBitmapDir + "titlebarGradient.png");


  _textbb.expand(v[4]);
  _textbb.expand(v[8]);
  _textbb.expand(v[5]);
  _textbb.expand(v[9]);

  _center.setPosition(0, v[9]);
  _center.setPosition(1, v[8]);
  _center.setPosition(2, v[6]);
  _center.setPosition(3, v[7]);


}
//=====================================================
void Window::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox = _bb;
}
//=====================================================
void Window::getTextBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox = _textbb;
}
//=====================================================
void Window::draw(node n, float lod) {
  ColorProperty* color = glGraphInputData->getElementColor();
  ColorProperty* colorBorder = glGraphInputData->getElementBorderColor();
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  _border.setColor(colorBorder->getNodeValue(n));
  _titleRec.setColor(colorBorder->getNodeValue(n));

  _center.setFillColor(color->getNodeValue(n));
  _center.setTextureName(textureName);
  _center.draw(lod, NULL);
  _titleRec.draw(lod, NULL);
  _border.draw(lod, NULL);
}
//=====================================================
Coord Window::getAnchor(const Coord &vector) const {
  Coord v(vector);
  float x, y, z, fmax;
  v.get(x, y, z);
  v.setZ(0.0f);
  fmax = std::max(fabsf(x), fabsf(y));

  if (fmax > 0.0f)
    return v * (0.5f / fmax);
  else
    return v;
}

} // end of namespace tlp

#endif // WINDOWGLYPH_H
