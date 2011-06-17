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
	spaceBetweenAxisGrads = axisLength / (axisGradsLabels.size() - 1);
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
		GlLabel *graduationLabel = NULL;
		float labelWidth;
		if (axisOrientation == HORIZONTAL_AXIS) {
			labelWidth = spaceBetweenAxisGrads;
			if (maxGraduationLabelWidth != 0 && labelWidth > maxGraduationLabelWidth) {
				labelWidth = maxGraduationLabelWidth;
			}
			labelHeight = labelWidth / 3.;
			axisGraduation->addPoint(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() + axisGradsWidth / 2), axisColor);
			axisGraduation->addPoint(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() - axisGradsWidth / 2), axisColor);
			if (axisGradsPosition == LEFT_OR_BELOW) {
				graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() - axisGradsWidth / 2 - labelHeight / 2),
                                                                                          Size(labelWidth, labelHeight), axisColor);
			} else if (axisGradsPosition == RIGHT_OR_ABOVE) {
				graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY() + axisGradsWidth / 2 + labelHeight / 2),
                                                                                          Size(labelWidth, labelHeight), axisColor);
			}
		} else if (axisOrientation == VERTICAL_AXIS) {
			labelHeight = spaceBetweenAxisGrads * MAGIG_FACTOR;
			labelWidth = axisGradsLabels[i].length() * (labelHeight / 2.);

			if (maxGraduationLabelWidth != 0 && labelWidth > maxGraduationLabelWidth) {
				labelWidth = maxGraduationLabelWidth;
			}

			axisGraduation->addPoint(Coord(axisBaseCoord.getX() - axisGradsWidth / 2., axisBaseCoord.getY() + i * spaceBetweenAxisGrads), axisColor);
			axisGraduation->addPoint(Coord(axisBaseCoord.getX() + axisGradsWidth / 2., axisBaseCoord.getY() + i * spaceBetweenAxisGrads), axisColor);
			if (axisGradsPosition == LEFT_OR_BELOW) {
				graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() - axisGradsWidth / 2. - labelWidth / 2. , axisBaseCoord.getY() + i * spaceBetweenAxisGrads),
                                                                                          Size(labelWidth, labelHeight), axisColor);
			} else if (axisGradsPosition == RIGHT_OR_ABOVE) {
				graduationLabel = new GlLabel(Coord(axisBaseCoord.getX() + axisGradsWidth / 2. + labelWidth / 2. , axisBaseCoord.getY() + i * spaceBetweenAxisGrads),
                                                                                          Size(labelWidth, labelHeight), axisColor);
			}
		}
		oss.str("");
		oss << axisName << " axis grad " << ++gradsCpt;
		gradsComposite->addGlEntity(axisGraduation, oss.str());
		oss.str("");
		oss << axisName << " axis grad label" << ++gradsCpt;
		graduationLabel->setText(axisGradsLabels[i]);
		graduationLabel->setStencil(1);
		gradsComposite->addGlEntity(graduationLabel, oss.str());
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
	} else if (axisOrientation == HORIZONTAL_AXIS) {
		for (unsigned int i = 1 ; i <= nbLineSegments ; ++i) {
			axisLine->addPoint(Coord(axisBaseCoord.getX() + i * step, axisBaseCoord.getY()), axisColor);
		}
	}
	axisLine->setStencil(1);
	axisLine->setLineWidth(2.0);
	axisLinesComposite->addGlEntity(axisLine, axisName + " axis");
}

Coord GlAxis::computeCaptionCenter() {
	Coord captionCenter;
	if (axisOrientation == VERTICAL_AXIS) {
		if (captionPosition == RIGHT_OR_ABOVE) {
			captionCenter = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + captionOffset + captionHeight / 2);
		} else {
			captionCenter = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() - captionOffset - captionHeight / 2);
		}
	} else if (axisOrientation == HORIZONTAL_AXIS) {
		if (captionPosition == RIGHT_OR_ABOVE) {
			captionCenter = Coord(axisBaseCoord.getX() + axisLength + captionOffset + (captionWidth / 2.), axisBaseCoord.getY());
		} else {
			captionCenter = Coord(axisBaseCoord.getX() - captionOffset - (captionWidth / 2.) , axisBaseCoord.getY());
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
        GlRect *captionLabelInnerFrame = new GlRect(Coord(labelBB[0][0] - 1, labelBB[1][1] + 1),
                                                    Coord(labelBB[1][0] + 1, labelBB[0][1] - 1),
                                                    axisColor, axisColor, false, true);
        for (unsigned int i = 0 ; i < 4 ; ++i) {
            captionLabelInnerFrame->setOutlineColor(i, axisColor);
        }
        captionComposite->addGlEntity(captionLabelInnerFrame, "caption inner frame" + captionText);

        GlRect *captionLabelOuterFrame = new GlRect(Coord(labelBB[0][0] - 2, labelBB[1][1] + 2),
                                                    Coord(labelBB[1][0] + 2, labelBB[0][1] - 2),
                                                    axisColor, axisColor, false, true);
        for (unsigned int i = 0 ; i < 4 ; ++i) {
            captionLabelOuterFrame->setOutlineColor(i, axisColor);
        }
        captionComposite->addGlEntity(captionLabelOuterFrame, "caption outer frame" + captionText);
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

