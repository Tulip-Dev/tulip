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

#include "SOMPreviewComposite.h"

#include <tulip/ColorProperty.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

using namespace tlp;
using namespace std;

SOMPreviewComposite::SOMPreviewComposite(tlp::Coord position, tlp::Size size, const std::string& propertyName, tlp::ColorProperty* colorProperty,
    SOMMap *map, ColorScale* colorScale, double minValue, double maxValue) :
  propertyName(propertyName), currentPosition(position) {

  double spacing = 1;
  double labelSizeRate = 0.1;
  float labelHeightSize = size.getH() * labelSizeRate;
  double scaleSizeRate = 0.1;

  Coord frameTopLeft(position.getX(), position.getY() + size.getH(), 0);
  Coord frameBottomRight(position.getX() + size.getW(), position.getY(), 0);
  //Creating the frame.
  frame = new GlRect(frameTopLeft, frameBottomRight, Color(255, 255, 255, 0), Color(255, 255, 255, 0), true, true);
  addGlEntity(frame, "frame");

  tlp::Size labelSize(size.getW() - spacing * 2, labelHeightSize);
  tlp::Coord labelPosition(position.getX() + labelSize.getW() / 2, position.getY() + size.getW() - labelSize.getH() / 2);

  label = new tlp::GlLabel(labelPosition, labelSize, tlp::Color(0, 0, 0));
  label->setText(propertyName);
  addGlEntity(label, "label");

  Coord lColorScalePosition(position.getX() + spacing, position.getY(), 0);
  Size lColorScaleSize(size.getW() - 2 * spacing, labelHeightSize + size.getH() * scaleSizeRate, 0);

  lColorScale = new GlLabelledColorScale(lColorScalePosition, lColorScaleSize, colorScale, minValue, maxValue);
  addGlEntity(lColorScale, "scale");

  Size screenshotMaxSize(size.getW() - 2 * spacing, size.getH() - labelSize.getH() - spacing - lColorScaleSize.getH());
  Size screenshotSize = computeAspectRatio(map->getWidth(), map->getHeight(), screenshotMaxSize.getW(), screenshotMaxSize.getH());

  Coord screenshotCoord(position.getX() + spacing, position.getY() + spacing + lColorScaleSize.getH());

  Size diff = screenshotMaxSize - screenshotSize;

  if (diff.getW() > 0) {

    screenshotCoord.setX(screenshotCoord.getX() + diff.getW() / 2);
  }

  if (diff.getH() > 0) {
    screenshotCoord.setY(screenshotCoord.getY() + diff.getH() / 2);
  }

  mapComposite = new SOMMapElement(screenshotCoord, screenshotSize, map, colorProperty);
  addGlEntity(mapComposite, "view");
}

void SOMPreviewComposite::updateColors(tlp::ColorProperty *newColor) {
  mapComposite->updateColors(newColor);
}

SOMPreviewComposite::~SOMPreviewComposite() {
  reset(true);
}

void SOMPreviewComposite::setFrameColor(tlp::Color color) {
  frame->setOutlineColor(color);
}

Size SOMPreviewComposite::computeAspectRatio(unsigned int width, unsigned int height, float maxWidth, float maxHeight) {

  Size elementsSize;

  if (width > height) {
    elementsSize.setW(maxWidth);
    elementsSize.setH((elementsSize.getW() * height) / (float) width);
  }
  else {
    elementsSize.setH(maxHeight);
    elementsSize.setW((width * elementsSize.getH()) / height);
  }

  return elementsSize;
}

bool SOMPreviewComposite::isElement(GlEntity* entity) {
  deque<GlComposite*> compositeToExplore;
  //Search in the current composite and all internal composites to find the element.
  compositeToExplore.push_back(this);

  while (!compositeToExplore.empty()) {
    GlComposite *current = compositeToExplore.front();
    compositeToExplore.pop_front();

    map<string, GlSimpleEntity*> displays = current->getGlEntities();

    for (map<string, GlSimpleEntity*>::iterator itElements = displays.begin(); itElements != displays.end(); ++itElements) {
      if (itElements->second == entity) {
        return true;
      }

      //If the element is a composite add it to the list of composite to explore.
      GlComposite *composite = dynamic_cast<GlComposite*> (itElements->second);

      if (composite) {
        compositeToExplore.push_back(composite);
      }
    }
  }

  return false;
}
