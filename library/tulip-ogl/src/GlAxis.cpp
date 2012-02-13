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

#include <sstream>

#include <tulip/GlLine.h>
#include <tulip/GlLabel.h>
#include <tulip/GlRect.h>
#include <tulip/GlQuad.h>
#include <tulip/GlPolyQuad.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlComplexPolygon.h>

#include <tulip/GlAxis.h>

using namespace std;

namespace tlp {

GlAxis::GlAxis(const std::string &axisName, const Coord &axisBaseCoord, const float axisLength,
               const AxisOrientation &axisOrientation, const Color &axisColor, const float axisWidth)  :
  axisName(axisName), axisBaseCoord(axisBaseCoord), axisLength(axisLength),
  axisOrientation(axisOrientation), axisColor(axisColor),
  captionText(axisName), captionOffset(0),
  axisLinesComposite(new GlComposite()), captionComposite(new GlComposite()),
  gradsComposite(new GlComposite()), captionSet(false), maxCaptionWidth(0),
  maxGraduationLabelWidth(axisLength / 8.f), axisWidth(axisWidth) {
  buildAxisLine();
  addGlEntity(captionComposite, "caption composite");
  addGlEntity(gradsComposite, "grads composite");
  addGlEntity(axisLinesComposite, "axis lines composite");
  axisGradsWidth = (DEFAULT_GRAD_WIDTH * axisLength) / 200.f;
}

GlAxis::~GlAxis() {
  reset(true);
}

void GlAxis::setAxisGraduations(const std::vector<std::string> &axisGradsLabels, const LabelPosition &axisGradsPosition) {
  spaceBetweenAxisGrads = axisLength / (axisGradsLabels.size() - 1);

  if (captionOffset == 0) {
    captionOffset = axisLength / 10.f;
  }

  gradsComposite->reset(true);
  ostringstream oss;
  unsigned int gradsCpt = 0;

  for (unsigned int i = 0 ; i < axisGradsLabels.size() ; ++i) {
    GlQuad *axisGraduation = new GlQuad();
    axisGraduation->setColor(axisColor);
    axisGraduation->setStencil(2);
    GlLabel *graduationLabel = NULL;
    float labelWidth;

    if (axisOrientation == HORIZONTAL_AXIS) {
      labelWidth = 0.8f * spaceBetweenAxisGrads;

      if (maxGraduationLabelWidth != 0 && labelWidth > maxGraduationLabelWidth) {
        labelWidth = maxGraduationLabelWidth;
      }

      labelHeight = labelWidth / 2.0f;
      axisGraduation->setPosition(0, Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads - axisWidth / 2, axisBaseCoord.getY() + axisGradsWidth / 2));
      axisGraduation->setPosition(1, Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads + axisWidth / 2, axisBaseCoord.getY() + axisGradsWidth / 2));
      axisGraduation->setPosition(2, Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads + axisWidth / 2, axisBaseCoord.getY() - axisGradsWidth / 2));
      axisGraduation->setPosition(3, Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads - axisWidth / 2, axisBaseCoord.getY() - axisGradsWidth / 2));

      if (axisGradsPosition == LEFT_OR_BELOW) {
        graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() - axisGradsWidth / 2 - labelHeight / 2),
                                      Size(labelWidth, labelHeight), axisColor);
      }
      else if (axisGradsPosition == RIGHT_OR_ABOVE) {
        graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() + axisGradsWidth / 2 + labelHeight / 2),
                                      Size(labelWidth, labelHeight), axisColor);
      }
    }
    else if (axisOrientation == VERTICAL_AXIS) {
      labelHeight = spaceBetweenAxisGrads * MAGIG_FACTOR;
      labelWidth = axisGradsLabels[i].length() * (labelHeight / 2.f);

      if (maxGraduationLabelWidth != 0 && labelWidth > maxGraduationLabelWidth) {
        labelWidth = maxGraduationLabelWidth;
      }

      axisGraduation->setPosition(0, Coord(axisBaseCoord.getX() - axisGradsWidth / 2.f, axisBaseCoord.getY() + i * spaceBetweenAxisGrads - axisWidth / 2));
      axisGraduation->setPosition(1, Coord(axisBaseCoord.getX() - axisGradsWidth / 2.f, axisBaseCoord.getY() + i * spaceBetweenAxisGrads + axisWidth / 2));
      axisGraduation->setPosition(2, Coord(axisBaseCoord.getX() + axisGradsWidth / 2.f, axisBaseCoord.getY() + i * spaceBetweenAxisGrads + axisWidth / 2));
      axisGraduation->setPosition(3, Coord(axisBaseCoord.getX() + axisGradsWidth / 2.f, axisBaseCoord.getY() + i * spaceBetweenAxisGrads - axisWidth / 2));

      if (axisGradsPosition == LEFT_OR_BELOW) {
        graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() - axisGradsWidth / 2.f - labelWidth / 2.f , axisBaseCoord.getY() + i * spaceBetweenAxisGrads),
                                      Size(labelWidth, labelHeight), axisColor);
      }
      else if (axisGradsPosition == RIGHT_OR_ABOVE) {
        graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() + axisGradsWidth / 2.f + labelWidth / 2.f , axisBaseCoord.getY() + i * spaceBetweenAxisGrads),
                                      Size(labelWidth, labelHeight), axisColor);
      }
    }

    oss.str("");
    oss << axisName << " axis grad " << ++gradsCpt;
    gradsComposite->addGlEntity(axisGraduation, oss.str());
    oss.str("");
    oss << axisName << " axis grad label" << ++gradsCpt;
    graduationLabel->setText(axisGradsLabels[i]);
    graduationLabel->setStencil(2);
    gradsComposite->addGlEntity(graduationLabel, oss.str());
  }

  computeBoundingBox();
}

void GlAxis::buildAxisLine() {
  GlPolyQuad *axisLine = new GlPolyQuad();


  const unsigned int nbLineSegments = 30;
  float step = axisLength / nbLineSegments;

  if (axisOrientation == VERTICAL_AXIS) {
    axisLine->addQuadEdge(axisBaseCoord - Coord(axisWidth/2,0), axisBaseCoord + Coord(axisWidth/2,0), axisColor);

    for (unsigned int i = 1 ; i <= nbLineSegments ; ++i) {
      axisLine->addQuadEdge(Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + i * step) - Coord(axisWidth/2,0), Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + i * step) + Coord(axisWidth/2,0), axisColor);
    }
  }
  else if (axisOrientation == HORIZONTAL_AXIS) {
    axisLine->addQuadEdge(axisBaseCoord - Coord(0, axisWidth/2), axisBaseCoord + Coord(0, axisWidth/2), axisColor);

    for (unsigned int i = 1 ; i <= nbLineSegments ; ++i) {
      axisLine->addQuadEdge(Coord(axisBaseCoord.getX() + i * step , axisBaseCoord.getY()) - Coord(0, axisWidth/2), Coord(axisBaseCoord.getX() + i * step, axisBaseCoord.getY()) + Coord(0, axisWidth/2), axisColor);
    }
  }

  axisLine->setStencil(2);
  axisLinesComposite->addGlEntity(axisLine, axisName + " axis");
}

Coord GlAxis::computeCaptionCenter() {
  Coord captionCenter;

  if (axisOrientation == VERTICAL_AXIS) {
    if (captionPosition == RIGHT_OR_ABOVE) {
      captionCenter = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + captionOffset + captionHeight / 2);
    }
    else {
      captionCenter = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() - captionOffset - captionHeight / 2.f);
    }
  }
  else if (axisOrientation == HORIZONTAL_AXIS) {
    if (captionPosition == RIGHT_OR_ABOVE) {
      captionCenter = Coord(axisBaseCoord.getX() + axisLength + captionOffset + (captionWidth / 2.f), axisBaseCoord.getY());
    }
    else {
      captionCenter = Coord(axisBaseCoord.getX() - captionOffset - (captionWidth / 2.f) , axisBaseCoord.getY());
    }
  }

  return captionCenter;
}

void GlAxis::addCaption(const LabelPosition &captionPos, const float captionHeight, const bool frame,
                        const float maxCapWidth, const float offset, const std::string caption) {
  if (caption != "")
    captionText = caption;

  captionPosition = captionPos;
  captionFrame = frame;

  if (maxCapWidth != 0)
    maxCaptionWidth = maxCapWidth;

  if (offset !=0) {
    captionOffset = offset;
  }

  computeCaptionSize(captionHeight);
  Coord captionCenter = computeCaptionCenter();
  addAxisCaption(captionCenter, captionFrame);
}

void GlAxis::addAxisCaption(const Coord &captionLabelCenter, const bool frame) {

  captionComposite->reset(true);
  captionSet = true;

  GlLabel *captionLabel = new GlLabel(captionLabelCenter, Size(captionWidth, captionHeight), axisColor);
  captionLabel->setText(captionText);
  captionComposite->addGlEntity(captionLabel, axisName + " axis caption");

  if (frame) {

    BoundingBox labelBB(captionLabel->getBoundingBox());
    vector<Coord> outer;
    outer.push_back(Coord(labelBB[0][0] - 3, labelBB[0][1] - 3));
    outer.push_back(Coord(labelBB[0][0] - 3, labelBB[1][1] + 3));
    outer.push_back(Coord(labelBB[1][0] + 3, labelBB[1][1] + 3));
    outer.push_back(Coord(labelBB[1][0] + 3, labelBB[0][1] - 3));
    vector<Coord> inner;
    inner.push_back(Coord(labelBB[0][0] - 1, labelBB[0][1] - 1));
    inner.push_back(Coord(labelBB[0][0] - 1, labelBB[1][1] + 1));
    inner.push_back(Coord(labelBB[1][0] + 1, labelBB[1][1] + 1));
    inner.push_back(Coord(labelBB[1][0] + 1, labelBB[0][1] - 1));

    vector<vector<Coord> > contours;
    contours.push_back(outer);
    contours.push_back(inner);

    GlComplexPolygon *frame = new GlComplexPolygon(contours, axisColor);
    frame->setOutlineMode(true);
    frame->setOutlineColor(axisColor);

    captionComposite->addGlEntity(frame, "caption outer frame" + captionText);
  }
}

void GlAxis::computeCaptionSize(float height) {
  captionHeight = height;
  captionWidth = height * captionText.size();

  if (maxCaptionWidth != 0 && captionWidth > maxCaptionWidth) {
    captionWidth = maxCaptionWidth;
  }
}

void GlAxis::translate(const Coord &c) {
  axisBaseCoord += c;
  GlComposite::translate(c);
  computeBoundingBox();
}

void GlAxis::updateAxis() {
  axisLinesComposite->reset(true);
  buildAxisLine();

  if (captionSet) {
    addCaption(captionPosition, captionHeight, captionFrame, maxCaptionWidth, captionOffset, captionText);
  }

  computeBoundingBox();
}

void GlAxis::computeBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSV(NULL);
  acceptVisitor(&glBBSV);
  boundingBox = glBBSV.getBoundingBox();
}

}

