//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include "ParallelCoordinatesView.h"
#include "ParallelTools.h"
#include "QuantitativeParallelAxis.h"

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <map>

#include <tulip/tulipconf.h>
#include <tulip/GWInteractor.h>
#include <tulip/TlpTools.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLines.h>
#include <tulip/GlSimpleEntity.h>

using namespace std;

namespace tlp {

const Color lightBlue(0, 217, 255, 100);
const Color darkBlue(0, 0, 255, 50);

class GlAxisBoxPlot : GlSimpleEntity {

public :

	GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color& fillColor, const Color& outlineColor) :
		axis(axis), boxWidth(5 * axis->getAxisGradWidth()), fillColor(fillColor), outlineColor(outlineColor),
		highlightRangeLowBound(NULL), highlightRangeHighBound(NULL) {}
	~GlAxisBoxPlot() {}

	void draw(float lod,Camera* camera) {
		bottomOutlierCoord = axis->getBottomOutlierCoord();
		firstQuartileCoord = axis->getFirstQuartileCoord();
		medianCoord = axis->getMedianCoord();
		thirdQuartileCoord = axis->getThirdQuartileCoord();
		topOutlierCoord = axis->getTopOutlierCoord();

		Coord interQuartileRangeBoxCoords[4];

		if (axis->hasAscendindOrder()) {
			boundingBox.check(Coord(bottomOutlierCoord.getX() - boxWidth / 2, bottomOutlierCoord.getY(), 0));
			boundingBox.check(Coord(topOutlierCoord.getX() + boxWidth / 2, topOutlierCoord.getY(), 0));

			interQuartileRangeBoxCoords[0] = Coord(thirdQuartileCoord.getX() - boxWidth / 2, thirdQuartileCoord.getY(), 0);
			interQuartileRangeBoxCoords[1] = Coord(thirdQuartileCoord.getX() + boxWidth / 2, thirdQuartileCoord.getY(), 0);
			interQuartileRangeBoxCoords[2] = Coord(firstQuartileCoord.getX() + boxWidth / 2, firstQuartileCoord.getY(), 0);
			interQuartileRangeBoxCoords[3] = Coord(firstQuartileCoord.getX() - boxWidth / 2, firstQuartileCoord.getY(), 0);
		} else {
			boundingBox.check(Coord(topOutlierCoord.getX() - boxWidth / 2, topOutlierCoord.getY(), 0));
			boundingBox.check(Coord(bottomOutlierCoord.getX() + boxWidth / 2, bottomOutlierCoord.getY(), 0));

			interQuartileRangeBoxCoords[0] = Coord(firstQuartileCoord.getX() - boxWidth / 2, firstQuartileCoord.getY(), 0);
			interQuartileRangeBoxCoords[1] = Coord(firstQuartileCoord.getX() + boxWidth / 2, firstQuartileCoord.getY(), 0);
			interQuartileRangeBoxCoords[2] = Coord(thirdQuartileCoord.getX() + boxWidth / 2, thirdQuartileCoord.getY(), 0);
			interQuartileRangeBoxCoords[3] = Coord(thirdQuartileCoord.getX() - boxWidth / 2, thirdQuartileCoord.getY(), 0);
		}

		GlQuad interQuartileRangeBox(interQuartileRangeBoxCoords, fillColor);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		interQuartileRangeBox.draw(lod, camera);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		GlLines::glDrawLine(interQuartileRangeBoxCoords[0], interQuartileRangeBoxCoords[1], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
		GlLines::glDrawLine(interQuartileRangeBoxCoords[1], interQuartileRangeBoxCoords[2], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
		GlLines::glDrawLine(interQuartileRangeBoxCoords[2], interQuartileRangeBoxCoords[3], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
		GlLines::glDrawLine(interQuartileRangeBoxCoords[3], interQuartileRangeBoxCoords[0], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);

		GlLines::glDrawLine(bottomOutlierCoord + Coord(-(boxWidth / 2), 0, 0), bottomOutlierCoord + Coord(boxWidth / 2, 0, 0), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
		GlLines::glDrawLine(medianCoord + Coord(-(boxWidth / 2), 0, 0), medianCoord + Coord(boxWidth / 2, 0, 0), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
		GlLines::glDrawLine(topOutlierCoord + Coord(-(boxWidth / 2), 0, 0), topOutlierCoord + Coord(boxWidth / 2, 0, 0), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);

		GlLines::glDrawLine(bottomOutlierCoord, firstQuartileCoord, 2, GlLines::TLP_DASHED, outlineColor, outlineColor);
		GlLines::glDrawLine(thirdQuartileCoord, topOutlierCoord, 2, GlLines::TLP_DASHED, outlineColor, outlineColor);

		drawLabel(bottomOutlierCoord, axis->getBottomOutlierStringValue());
		drawLabel(firstQuartileCoord, axis->getFirstQuartileStringValue());
		drawLabel(medianCoord, axis->getMedianStringValue());
		drawLabel(thirdQuartileCoord, axis->getThirdQuartileStringValue());
		drawLabel(topOutlierCoord, axis->getTopOutlierStringValue());

		if (highlightRangeLowBound != NULL && highlightRangeHighBound != NULL) {
			Coord highlightBoxCoords[4];
			highlightBoxCoords[0] = Coord(highlightRangeHighBound->getX() - boxWidth /2, highlightRangeHighBound->getY(), 0);
			highlightBoxCoords[1] = Coord(highlightRangeHighBound->getX() + boxWidth /2, highlightRangeHighBound->getY(), 0);
			highlightBoxCoords[2] = Coord(highlightRangeLowBound->getX() + boxWidth /2, highlightRangeLowBound->getY(), 0);
			highlightBoxCoords[3] = Coord(highlightRangeLowBound->getX() - boxWidth /2, highlightRangeLowBound->getY(), 0);
			Color outlineColorTranslucent(outlineColor);
			outlineColorTranslucent.setA(100);
			GlQuad highlightBox(highlightBoxCoords, outlineColorTranslucent);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
			glEnable(GL_LIGHTING);
			highlightBox.draw(lod, camera);
			glDisable(GL_BLEND);
			glDisable(GL_LIGHTING);
			highlightRangeLowBound = NULL;
			highlightRangeHighBound = NULL;
		}
	}

	void getXML(xmlNodePtr rootNode) {}

	void setWithXML(xmlNodePtr rootNode) {}

	void setHighlightRangeIfAny(Coord sceneCoords) {
		if (axis->hasAscendindOrder()) {
			if (sceneCoords.getY() < topOutlierCoord.getY() &&
				sceneCoords.getY() > thirdQuartileCoord.getY() &&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeLowBound = &thirdQuartileCoord;
				highlightRangeHighBound = &topOutlierCoord;
				axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, TOP_OUTLIER);

			} else if (sceneCoords.getY() < thirdQuartileCoord.getY() &&
					   sceneCoords.getY() > (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeLowBound = &medianCoord;
				highlightRangeHighBound = &thirdQuartileCoord;
				axis->setBoxPlotHighlightBounds(MEDIAN, THIRD_QUARTILE);

			} else if (sceneCoords.getY() > (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
					   sceneCoords.getY() < (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeLowBound = &firstQuartileCoord;
				highlightRangeHighBound = &thirdQuartileCoord;
				axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, THIRD_QUARTILE);

			} else if (sceneCoords.getY() > firstQuartileCoord.getY() &&
					   sceneCoords.getY() < (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY()))&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {
						   highlightRangeLowBound = &firstQuartileCoord;
						   highlightRangeHighBound = &medianCoord;
				axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, MEDIAN);
			} else if (sceneCoords.getY() > bottomOutlierCoord.getY() &&
					   sceneCoords.getY() < firstQuartileCoord.getY()&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeLowBound = &bottomOutlierCoord;
				highlightRangeHighBound = &firstQuartileCoord;
				axis->setBoxPlotHighlightBounds(BOTTOM_OUTLIER, FIRST_QUARTILE);

			} else {
				highlightRangeLowBound = NULL;
				highlightRangeHighBound = NULL;
				axis->setBoxPlotHighlightBounds(NO_VALUE, NO_VALUE);
			}
		} else {
			if (sceneCoords.getY() > topOutlierCoord.getY() &&
				sceneCoords.getY() < thirdQuartileCoord.getY()&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeHighBound = &thirdQuartileCoord;
				highlightRangeLowBound = &topOutlierCoord;
				axis->setBoxPlotHighlightBounds(TOP_OUTLIER, THIRD_QUARTILE);

			} else if (sceneCoords.getY() > thirdQuartileCoord.getY() &&
					   sceneCoords.getY() < (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeHighBound = &medianCoord;
				highlightRangeLowBound = &thirdQuartileCoord;
				axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, MEDIAN);

			} else if (sceneCoords.getY() < (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
					   sceneCoords.getY() > (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeHighBound = &firstQuartileCoord;
				highlightRangeLowBound = &thirdQuartileCoord;
				axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, FIRST_QUARTILE);

			} else if (sceneCoords.getY() < firstQuartileCoord.getY() &&
					   sceneCoords.getY() > (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY()))&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeHighBound = &firstQuartileCoord;
				highlightRangeLowBound = &medianCoord;
				axis->setBoxPlotHighlightBounds(MEDIAN, FIRST_QUARTILE);

			} else if (sceneCoords.getY() < bottomOutlierCoord.getY() &&
					   sceneCoords.getY() > firstQuartileCoord.getY()&&
					   sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
					   sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

				highlightRangeHighBound = &bottomOutlierCoord;
				highlightRangeLowBound = &firstQuartileCoord;
				axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, BOTTOM_OUTLIER);

			} else {
				highlightRangeLowBound = NULL;
				highlightRangeHighBound = NULL;
				axis->setBoxPlotHighlightBounds(NO_VALUE, NO_VALUE);
			}
		}
	}

private :

	void drawLabel(Coord position, string labelName) {
		float labelHeight = axis->getLabelHeight();

		float heightRef;
		if (axis->hasAscendindOrder()) {
			heightRef = topOutlierCoord.getY() - thirdQuartileCoord.getY();
		} else {
			heightRef = thirdQuartileCoord.getY() - topOutlierCoord.getY();
		}

		if (labelHeight > heightRef) {
			labelHeight = heightRef / 2;
		}

		float labelWidth = labelName.length() * (labelHeight / 2);

		if (labelName.length() == 1) {
			labelWidth *= 2;
		}

		GlLabel labelToDraw(Coord(position.getX() - boxWidth / 2 - labelWidth /2, position.getY(), 0),
							Coord(labelWidth, labelHeight), outlineColor);
		labelToDraw.setText(labelName);
		labelToDraw.draw(0, 0);
	}

	QuantitativeParallelAxis *axis;
	Coord bottomOutlierCoord;
	Coord firstQuartileCoord;
	Coord medianCoord;
	Coord thirdQuartileCoord;
	Coord topOutlierCoord;

	float boxWidth;
	Color fillColor, outlineColor;

	Coord *highlightRangeLowBound;
	Coord *highlightRangeHighBound;



};

class ParallelCoordsAxisBoxPlot : public Interactor {

public :

	ParallelCoordsAxisBoxPlot() : drawAxisBoxPlot(false), selectedAxis(NULL), lastNbAxis(0) {}
	~ParallelCoordsAxisBoxPlot() {deleteGlAxisPlot();}
	bool eventFilter(QObject *, QEvent *);
	bool draw(GlMainWidget *glMainWidget);
	Interactor *clone() { return new ParallelCoordsAxisBoxPlot(); }

private :
	void buildGlAxisPlot(vector<ParallelAxis *> currentAxis);
	void deleteGlAxisPlot();
	map<QuantitativeParallelAxis *, GlAxisBoxPlot *> axisBoxPlotMap;
	bool drawAxisBoxPlot;
	ParallelAxis *selectedAxis;
	unsigned int lastNbAxis;

};

INTERACTORPLUGIN(ParallelCoordsAxisBoxPlot, "ParallelCoordsAxisBoxPlot", "Tulip Team", "25/11/2008", "Parallel Coordinates Axis Box Plot", "1.0");

void ParallelCoordsAxisBoxPlot::buildGlAxisPlot(vector<ParallelAxis *> currentAxis) {
	for (unsigned int i = 0 ; i < currentAxis.size() ; ++i) {
		if (dynamic_cast<QuantitativeParallelAxis *>(currentAxis[i])) {
			QuantitativeParallelAxis *quantitativeAxis = (QuantitativeParallelAxis *) currentAxis[i];
			if (quantitativeAxis->getMedianStringValue() != "KO")
				axisBoxPlotMap[quantitativeAxis] = new GlAxisBoxPlot(quantitativeAxis, lightBlue, darkBlue);
		}
	}
}

void ParallelCoordsAxisBoxPlot::deleteGlAxisPlot() {
	map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;
	for (it = axisBoxPlotMap.begin() ; it != axisBoxPlotMap.end() ; ++it) {
		delete (it->second);
	}
	axisBoxPlotMap.clear();
}

bool ParallelCoordsAxisBoxPlot::eventFilter(QObject *widget, QEvent *e) {

	GlMainWidget *glWidget = (GlMainWidget *) widget;
	ParallelCoordinatesView *parallelView = (ParallelCoordinatesView *) view;

	vector<ParallelAxis *> allAxis = parallelView->getAllAxis();

	drawAxisBoxPlot = false;

	if (axisBoxPlotMap.size() == 0) {
		buildGlAxisPlot(allAxis);
		drawAxisBoxPlot = true;
		lastNbAxis = allAxis.size();
		parallelView->refresh();
		drawAxisBoxPlot = false;
		return true;
	}

	if (lastNbAxis != 0 && lastNbAxis != allAxis.size()) {
		deleteGlAxisPlot();
		buildGlAxisPlot(allAxis);
		drawAxisBoxPlot = true;
		selectedAxis = NULL;
		lastNbAxis = allAxis.size();
		parallelView->refresh();
		drawAxisBoxPlot = false;
		return true;
	}

	lastNbAxis = allAxis.size();

	if (e->type() == QEvent::MouseMove) {
		QMouseEvent *me = (QMouseEvent *) e;
		int x = glWidget->width() - me->x();
		int y = me->y();
		Coord screenCoords((double) x, (double) y, 0);
		Coord sceneCoords = glWidget->getScene()->getCamera()->screenTo3DWorld(screenCoords);
		selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
		if (selectedAxis != NULL && dynamic_cast<QuantitativeParallelAxis *>(selectedAxis)) {
			if (axisBoxPlotMap.find((QuantitativeParallelAxis *)selectedAxis) != axisBoxPlotMap.end())
				axisBoxPlotMap[(QuantitativeParallelAxis *)selectedAxis]->setHighlightRangeIfAny(sceneCoords);
		}
		drawAxisBoxPlot = true;
		parallelView->refresh();
		return true;
	}

	if (e->type() == QEvent::MouseButtonPress) {
		drawAxisBoxPlot = false;
		return false;
	}

	if (e->type() == QEvent::MouseButtonRelease) {
		if (selectedAxis != NULL && dynamic_cast<QuantitativeParallelAxis *>(selectedAxis)) {
			drawAxisBoxPlot = false;
			Observable::holdObservers();
			if (axisBoxPlotMap.find((QuantitativeParallelAxis *)selectedAxis) != axisBoxPlotMap.end())
				parallelView->highlightDataInAxisBoxPlotRange((QuantitativeParallelAxis *) selectedAxis);
			Observable::unholdObservers();
			selectedAxis = NULL;
			drawAxisBoxPlot = true;
			parallelView->refresh();
			return true;
		}
	}

	return false;
}

bool ParallelCoordsAxisBoxPlot::draw(GlMainWidget *glMainWidget) {

	if (!drawAxisBoxPlot) {
		return false;
	}

	map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;
	for (it = axisBoxPlotMap.begin(); it != axisBoxPlotMap.end(); ++it) {
		(it->second)->draw(0,0);
	}

	return true;
}

}
