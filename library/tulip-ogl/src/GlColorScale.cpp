/*
	  GlColorScale.cpp

   Created on: 18 févr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <map>
#include <iostream>

#include "tulip/GlColorScale.h"

using namespace std;

namespace tlp {

GlColorScale::GlColorScale(ColorScale *colorScale, const Coord &baseCoord, const float length,
						   const float thickness, Orientation orientation) :
  colorScale(colorScale), baseCoord(baseCoord), length(length),
  thickness(thickness), colorScalePolyQuad(NULL),orientation(orientation) {
  colorScale->addObserver(this);
  updateDrawing();
}

GlColorScale::~GlColorScale() {
  if (colorScalePolyQuad != NULL) {
    delete colorScalePolyQuad;
  }
  colorScale->removeObserver(this);
}

void GlColorScale::setColorScale(ColorScale * scale) {
  colorScale->removeObserver(this);
  colorScale = scale;
  colorScale->addObserver(this);
  updateDrawing();
}

void GlColorScale::update(std::set<Observable *>::iterator, std::set<Observable *>::iterator) {
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
      } else {
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
  if (orientation == GlColorScale::Vertical) {
    return colorScale->getColorAtPos((pos.getY() - baseCoord.getY()) / length);
  } else {
    return colorScale->getColorAtPos((pos.getX() - baseCoord.getX()) / length);
  }
}

void GlColorScale::translate(const Coord &move) {
	if (colorScalePolyQuad != NULL) {
		colorScalePolyQuad->translate(move);
		baseCoord += move;
		boundingBox = colorScalePolyQuad->getBoundingBox();
	}
}

}
