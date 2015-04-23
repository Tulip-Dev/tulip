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

#include "GlLabelledColorScale.h"

#include <tulip/GlRect.h>
#include <tulip/GlPolyQuad.h>

#include <sstream>

using namespace std;
using namespace tlp;

GlLabelledColorScale::GlLabelledColorScale(Coord position, Size size,
    ColorScale* colorScale, double minValue, double maxValue, bool colorScaleAtTop) :
  GlComposite(), position(position), size(size), minValue(minValue), maxValue(maxValue),
  colorScaleAtTop(colorScaleAtTop) {

  buildComposite(colorScale);
}

void GlLabelledColorScale::buildComposite(ColorScale* colorScale) {
  float labelHeightSize = size.getH() * 0.5;
  float scaleLength = size.getW();
  float scaleThickness = (1 - 0.5) * size.getH();
  Size valueLabelSize(size.getW() / 3, labelHeightSize, 0);

  Coord minValueLabelPosition;
  Coord maxValueLabelPosition;
  Coord scalePosition;

  if (colorScaleAtTop) {
    minValueLabelPosition.set(position.getX()+valueLabelSize.getW()/2, position.getY() + labelHeightSize / 2, 0);
    maxValueLabelPosition.set((position.getX() + size.getW()) - valueLabelSize.getW()/2,
                              minValueLabelPosition.getY(), 0);
    scalePosition.set(position.getX(), position.getY() + labelHeightSize + (scaleThickness / 2));
  }
  else {
    minValueLabelPosition.set(position.getX()+valueLabelSize.getW()/2, position.getY() + size.getH() - labelHeightSize
                              / 2, 0);
    maxValueLabelPosition.set((position.getX() + size.getW()) - valueLabelSize.getW()/2,
                              minValueLabelPosition.getY(), 0);
    scalePosition.set(position.getX(), position.getY() + (scaleThickness / 2));
  }

  minLabel = new GlLabel(minValueLabelPosition, valueLabelSize, tlp::Color(0, 0, 0), true);
  ostringstream oss;
  oss << minValue;
  minLabel->setText(oss.str());
  addGlEntity(minLabel, "minLabel");

  maxLabel = new GlLabel(maxValueLabelPosition, valueLabelSize, tlp::Color(0, 0, 0), true);
  oss.str("");
  oss << maxValue;
  maxLabel->setText(oss.str());
  addGlEntity(maxLabel, "maxLabel");

  glColorScale = new GlColorScale(colorScale, scalePosition, scaleLength, scaleThickness,
                                  GlColorScale::Horizontal);
  glColorScale->getColorScalePolyQuad()->setOutlined(true);
  glColorScale->getColorScalePolyQuad()->setOutlineColor(Color(0, 0, 0));

  addGlEntity(glColorScale, "scale");
}

GlLabelledColorScale::~GlLabelledColorScale() {
  reset(true);
}

void GlLabelledColorScale::setMinValue(double value) {
  ostringstream oss;
  oss << value;
  minLabel->setText(oss.str());
  minValue = value;
}
void GlLabelledColorScale::setMaxValue(double value) {
  ostringstream oss;
  oss << value;
  maxLabel->setText(oss.str());
  maxValue = value;
}

void GlLabelledColorScale::setColorScale(tlp::ColorScale* cs) {
  glColorScale->setColorScale(cs);
}
BoundingBox GlLabelledColorScale::getColorScaleBoundingBox() {
  return BoundingBox(Coord(position.getX(), position.getY() + size.getH() * 0.5), Coord(
                       position.getX() + size.getW(), position.getY() + size.getH()));
}

void GlLabelledColorScale::setPosition(tlp::Coord nPosition) {

  Coord mouvement = nPosition - position;

  glColorScale->translate(mouvement);
  minLabel->translate(mouvement);
  maxLabel->translate(mouvement);

  position = nPosition;
}

void GlLabelledColorScale::setSize(tlp::Size nSize) {

  ColorScale *cs = glColorScale->getColorScale();
  size = nSize;
  reset(true);
  buildComposite(cs);
}

void GlLabelledColorScale::addLayerParent(tlp::GlLayer *) {
}
