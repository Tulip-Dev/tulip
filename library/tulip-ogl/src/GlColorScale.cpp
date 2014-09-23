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

#include <map>

#include <tulip/GlColorScale.h>
#include <tulip/ColorScale.h>
#include <tulip/GlPolyQuad.h>

using namespace std;

namespace tlp {

GlColorScale::GlColorScale(ColorScale *colorScale, const Coord &baseCoord, const float length,
                           const float thickness, Orientation orientation) :
  colorScale(colorScale), baseCoord(baseCoord), length(length),
  thickness(thickness), colorScalePolyQuad(NULL),orientation(orientation) {
  colorScale->addListener(this);
  updateDrawing();
}

GlColorScale::~GlColorScale() {
  delete colorScalePolyQuad;
}

void GlColorScale::setColorScale(ColorScale * scale) {
  colorScale->removeListener(this);
  colorScale = scale;
  colorScale->addListener(this);
  updateDrawing();
}

void GlColorScale::treatEvent(const Event &evt) {
  if (dynamic_cast<ColorScale *>(evt.sender()) && evt.type() == Event::TLP_MODIFICATION)
    updateDrawing();
}

void GlColorScale::updateDrawing() {
  delete colorScalePolyQuad;

  colorScalePolyQuad = new GlPolyQuad();

  if (!colorScale->colorScaleInitialized()) {
    colorScalePolyQuad->setOutlined(true);
    colorScalePolyQuad->setOutlineWidth(2);
  }

  map<float, Color> colorMap = colorScale->getColorMap();
  Coord currentMin, currentMax;

  for (map<float, Color>::iterator colorMapIt = colorMap.begin(); colorMapIt != colorMap.end(); ++colorMapIt) {
    if (orientation == Vertical) {
      currentMin.set(baseCoord.getX() - thickness / 2, baseCoord.getY() + colorMapIt->first * length);
      currentMax.set(baseCoord.getX() + thickness / 2, baseCoord.getY() + colorMapIt->first * length);
    }
    else {
      currentMin.set(baseCoord.getX() + colorMapIt->first * length, baseCoord.getY() - thickness / 2, 0);
      currentMax.set(baseCoord.getX() + colorMapIt->first * length, baseCoord.getY() + thickness / 2, 0);
    }

    colorScalePolyQuad->addQuadEdge(currentMin, currentMax,colorMapIt->second);
  }

  boundingBox = colorScalePolyQuad->getBoundingBox();
}

void GlColorScale::draw(float lod, Camera* camera) {
  if (colorScalePolyQuad != NULL) {
    colorScalePolyQuad->draw(lod, camera);
  }
}

Color GlColorScale::getColorAtPos(Coord pos) {
  float position = 0;

  if (orientation == GlColorScale::Vertical) {
    position = (pos.getY() - baseCoord.getY()) / length;
  }
  else {
    position = (pos.getX() - baseCoord.getX()) / length;
  }

  //Ensure to always have a valid position.
  if(position<0) {
    position=0;
  }

  if(position>1) {
    position=1;
  }

  return colorScale->getColorAtPos(position);
}

void GlColorScale::translate(const Coord &move) {
  if (colorScalePolyQuad != NULL) {
    colorScalePolyQuad->translate(move);
    baseCoord += move;
    boundingBox = colorScalePolyQuad->getBoundingBox();
  }
}

}
