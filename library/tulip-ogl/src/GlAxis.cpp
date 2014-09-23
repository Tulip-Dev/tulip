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

#include <sstream>

#include <tulip/GlLine.h>
#include <tulip/GlLabel.h>
#include <tulip/GlRect.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

#include <tulip/GlAxis.h>

using namespace std;

namespace tlp {

GlAxis::GlAxis(const std::string &axisName, const Coord &axisBaseCoord, const float axisLength,
               const AxisOrientation &axisOrientation, const Color &axisColor)  :
  axisName(axisName), axisBaseCoord(axisBaseCoord), axisLength(axisLength),
  axisOrientation(axisOrientation), axisColor(axisColor),
  captionText(axisName), captionOffset(0),
  axisLinesComposite(new GlComposite()), captionComposite(new GlComposite()),
  gradsComposite(new GlComposite()), captionSet(false), maxCaptionWidth(0),
  maxGraduationLabelWidth(axisLength / 8.) {
  buildAxisLine();
  addGlEntity(captionComposite, "caption composite");
  addGlEntity(gradsComposite, "grads composite");
  addGlEntity(axisLinesComposite, "axis lines composite");
  axisGradsWidth = (DEFAULT_GRAD_WIDTH * axisLength) / 200.;
}

GlAxis::~GlAxis() {
  reset(true);
}

void GlAxis::setAxisGraduations(const std::vector<std::string> &axisGradsLabels, const LabelPosition &axisGradsPosition) {
  this->axisGradsPosition=axisGradsPosition;

  if (axisGradsLabels.size() > 1) {
    spaceBetweenAxisGrads = axisLength / (axisGradsLabels.size() - 1);
  }
  else {
    spaceBetweenAxisGrads = axisLength;
  }

  gradsLabelsVector.clear();

  if (captionOffset == 0) {
    captionOffset = axisLength / 10.;
  }

  gradsComposite->reset(true);
  ostringstream oss;
  unsigned int gradsCpt = 0;

  for (unsigned int i = 0 ; i < axisGradsLabels.size() ; ++i) {
    GlLine *axisGraduation = new GlLine();
    axisGraduation->setStencil(1);
    axisGraduation->setLineWidth(2.0);
    GlLabel *graduationLabel = new GlLabel();
    float labelWidth;

    graduationLabel->setText(axisGradsLabels[i]);
    graduationLabel->setColor(axisColor);

    BoundingBox textBoundingBox=graduationLabel->getTextBoundingBox();
    labelHeight = spaceBetweenAxisGrads/2.;

    if(labelHeight>50)
      labelHeight=50;

    labelWidth = (labelHeight/(textBoundingBox[1][1]-textBoundingBox[0][1]))*(textBoundingBox[1][0]-textBoundingBox[0][0]);

    if (axisOrientation == HORIZONTAL_AXIS) {


      axisGraduation->addPoint(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() + axisGradsWidth / 2), axisColor);
      axisGraduation->addPoint(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() - axisGradsWidth / 2), axisColor);

      if (axisGradsPosition == LEFT_OR_BELOW) {
        graduationLabel->setPosition(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads - cos(M_PI/4.)*(labelWidth/2.), axisBaseCoord.getY() - axisGradsWidth / 2 - cos(M_PI/4.)*(labelWidth/2.) - 2));
        graduationLabel->setSize(Size(labelWidth, labelHeight));
        graduationLabel->rotate(0,0,45);
      }
      else if (axisGradsPosition == RIGHT_OR_ABOVE) {
        graduationLabel->setPosition(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads + cos(M_PI/4.)*(labelWidth/2.), axisBaseCoord.getY() + axisGradsWidth / 2 + cos(M_PI/4.)*(labelWidth/2.) + 2));
        graduationLabel->setSize(Size(labelWidth, labelHeight));
        graduationLabel->rotate(0,0,45);
      }
    }
    else if (axisOrientation == VERTICAL_AXIS) {

      axisGraduation->addPoint(Coord(axisBaseCoord.getX() - axisGradsWidth / 2., axisBaseCoord.getY() + i * spaceBetweenAxisGrads), axisColor);
      axisGraduation->addPoint(Coord(axisBaseCoord.getX() + axisGradsWidth / 2., axisBaseCoord.getY() + i * spaceBetweenAxisGrads), axisColor);

      if (axisGradsPosition == LEFT_OR_BELOW) {
        graduationLabel->setPosition(Coord(axisBaseCoord.getX() - axisGradsWidth / 2. - labelWidth / 2. - 2, axisBaseCoord.getY() + i * spaceBetweenAxisGrads));
        graduationLabel->setSize(Size(labelWidth, labelHeight));
      }
      else if (axisGradsPosition == RIGHT_OR_ABOVE) {
        graduationLabel->setPosition(Coord(axisBaseCoord.getX() + axisGradsWidth / 2. + labelWidth / 2. + 2, axisBaseCoord.getY() + i * spaceBetweenAxisGrads));
        graduationLabel->setSize(Size(labelWidth, labelHeight));
      }
    }

    oss.str("");
    oss << axisName << " axis grad " << ++gradsCpt;
    gradsComposite->addGlEntity(axisGraduation, oss.str());
    oss.str("");
    oss << axisName << " axis grad label" << ++gradsCpt;

    graduationLabel->setStencil(1);

    gradsComposite->addGlEntity(graduationLabel, oss.str());
    gradsLabelsVector.push_back(graduationLabel);
  }

  computeBoundingBox();
}

void GlAxis::buildAxisLine() {
  GlLine *axisLine = new GlLine();
  axisLine->addPoint(axisBaseCoord, axisColor);
  const unsigned int nbLineSegments = 30;
  float step = axisLength / nbLineSegments;

  if (axisOrientation == VERTICAL_AXIS) {
    for (unsigned int i = 1 ; i <= nbLineSegments ; ++i) {
      axisLine->addPoint(Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + i * step), axisColor);
    }
  }
  else if (axisOrientation == HORIZONTAL_AXIS) {
    for (unsigned int i = 1 ; i <= nbLineSegments ; ++i) {
      axisLine->addPoint(Coord(axisBaseCoord.getX() + i * step, axisBaseCoord.getY()), axisColor);
    }
  }

  axisLine->setStencil(1);
  axisLine->setLineWidth(2.0);
  axisLinesComposite->addGlEntity(axisLine, axisName + " axis");
}

Coord GlAxis::computeCaptionCenter(const bool captionFrame) {
  Coord captionCenter;
  float captionHeight;

  if(captionFrame)
    captionHeight=baseCaptionHeight;
  else
    captionHeight=this->captionHeight;

  if (axisOrientation == VERTICAL_AXIS) {
    if (captionPosition == RIGHT) {
      captionCenter = Coord(axisBaseCoord.getX() + captionOffset + captionHeight / 2, axisBaseCoord.getY() + axisLength / 2);
    }
    else if(captionPosition == LEFT) {
      captionCenter = Coord(axisBaseCoord.getX() - captionOffset - captionHeight / 2, axisBaseCoord.getY() + axisLength / 2);
    }
    else if(captionPosition == BELOW) {
      captionCenter = Coord(axisBaseCoord.getX(), axisBaseCoord.getY()  - captionOffset - captionHeight / 2);
    }
    else {
      captionCenter = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + captionOffset + captionHeight / 2);
    }
  }
  else if (axisOrientation == HORIZONTAL_AXIS) {
    if (captionPosition == RIGHT) {
      captionCenter = Coord(axisBaseCoord.getX() + axisLength + captionOffset + captionWidth / 2, axisBaseCoord.getY());
    }
    else if(captionPosition == LEFT) {
      captionCenter = Coord(axisBaseCoord.getX() - captionOffset - captionWidth / 2, axisBaseCoord.getY());
    }
    else if(captionPosition == BELOW) {
      captionCenter = Coord(axisBaseCoord.getX() + axisLength / 2 , axisBaseCoord.getY() - captionOffset - captionHeight / 2);
    }
    else {
      captionCenter = Coord(axisBaseCoord.getX() + axisLength / 2 , axisBaseCoord.getY() + captionOffset + captionHeight / 2);
    }
  }

  return captionCenter;
}

void GlAxis::addCaption(const CaptionLabelPosition &captionPos, const float captionHeight, const bool frame,
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
  Coord captionCenter = computeCaptionCenter(captionFrame);
  addAxisCaption(captionCenter, captionFrame);
}

void GlAxis::addAxisCaption(const Coord &captionLabelCenter, const bool frame) {

  captionComposite->reset(true);
  captionSet = true;

  captionLabel = new GlLabel(captionLabelCenter, Size(captionWidth, captionHeight), axisColor);

  if (axisOrientation == VERTICAL_AXIS && (captionPosition == LEFT || captionPosition == RIGHT))
    captionLabel->rotate(0,0,90);

  captionLabel->setText(captionText);
  captionComposite->addGlEntity(captionLabel, axisName + " axis caption");

  if (frame) {
    captionLabel->setSize(Size(captionWidth, baseCaptionHeight));
    BoundingBox labelBB=captionLabel->getBoundingBox();
    GlRect *captionLabelInnerFrame = new GlRect(Coord(labelBB[0][0] - 1, labelBB[0][1] + baseCaptionHeight + 1),
        Coord(labelBB[0][0] + captionWidth + 1, labelBB[0][1] - 1),
        axisColor, axisColor, false, true);

    for (unsigned int i = 0 ; i < 4 ; ++i) {
      captionLabelInnerFrame->setOutlineColor(i, axisColor);
    }

    captionComposite->addGlEntity(captionLabelInnerFrame, "caption inner frame" + captionText);

    GlRect *captionLabelOuterFrame = new GlRect(Coord(labelBB[0][0] - 2, labelBB[0][1] + baseCaptionHeight + 2),
        Coord(labelBB[0][0] + captionWidth + 2, labelBB[0][1] - 2),
        axisColor, axisColor, false, true);

    for (unsigned int i = 0 ; i < 4 ; ++i) {
      captionLabelOuterFrame->setOutlineColor(i, axisColor);
    }

    captionComposite->addGlEntity(captionLabelOuterFrame, "caption outer frame" + captionText);
  }
}

void GlAxis::computeCaptionSize(float height) {
  GlLabel captionLabel;
  captionLabel.setText(captionText);
  BoundingBox boundingBox=captionLabel.getTextBoundingBox();
  captionHeight = height;
  captionWidth = height/(boundingBox[1][1]-boundingBox[0][1])*(boundingBox[1][0]-boundingBox[0][0]);
  baseCaptionHeight=captionHeight;
  float captionSizeFactor=captionWidth/captionHeight;

  if (maxCaptionWidth != 0 && captionWidth > maxCaptionWidth) {
    captionWidth = maxCaptionWidth;
    captionHeight = captionWidth / captionSizeFactor;
  }
}

void GlAxis::setCaptionHeight(float height,bool frame) {
  computeCaptionSize(height);
  Coord captionCenter = computeCaptionCenter(frame);
  captionLabel->setSize(Size(captionWidth, captionHeight));
  captionLabel->setPosition(captionCenter);
}

float GlAxis::getCaptionHeight() const {
  return captionHeight;
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
    addCaption(captionPosition, baseCaptionHeight, captionFrame, maxCaptionWidth, captionOffset, captionText);
  }

  computeBoundingBox();
}

void GlAxis::computeBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSV(NULL);
  acceptVisitor(&glBBSV);
  boundingBox = glBBSV.getBoundingBox();
}

void GlAxis::setGradsLabelsHeight(float height) {
  labelHeight = height;

  unsigned int i=0;

  for(vector<GlLabel*>::iterator it=gradsLabelsVector.begin(); it!=gradsLabelsVector.end(); ++it) {
    BoundingBox textBoundingBox=(*it)->getTextBoundingBox();
    float labelWidth = (labelHeight/(textBoundingBox[1][1]-textBoundingBox[0][1]))*(textBoundingBox[1][0]-textBoundingBox[0][0]);
    (*it)->setSize(Size(labelWidth,labelHeight));

    if (axisOrientation == HORIZONTAL_AXIS) {
      if (axisGradsPosition == LEFT_OR_BELOW)
        (*it)->setPosition(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() - axisGradsWidth / 2 - labelWidth / 2 - 2));
      else if (axisGradsPosition == RIGHT_OR_ABOVE)
        (*it)->setPosition(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() + axisGradsWidth / 2 + labelWidth / 2 + 2));
    }
    else if (axisOrientation == VERTICAL_AXIS) {
      if (axisGradsPosition == LEFT_OR_BELOW)
        (*it)->setPosition(Coord(axisBaseCoord.getX() - axisGradsWidth / 2. - labelWidth / 2. - 2, axisBaseCoord.getY() + i * spaceBetweenAxisGrads));
      else if (axisGradsPosition == RIGHT_OR_ABOVE)
        (*it)->setPosition(Coord(axisBaseCoord.getX() + axisGradsWidth / 2. + labelWidth / 2. + 2, axisBaseCoord.getY() + i * spaceBetweenAxisGrads));
    }

    ++i;
  }

  computeBoundingBox();
}

}

