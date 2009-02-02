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

#include <tulip/tulipconf.h>
#include <tulip/GWInteractor.h>
#include <tulip/TlpTools.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

using namespace std;

namespace tlp {

static Color redColor(255,0,0);
static Color blackColor(0,0,0);
static Color greenColor(0,255,0);
static Color blueColor(14,241,212);

enum sliderType {TOP_SLIDER = 0, BOTTOM_SLIDER = 1};



class AxisSlider : public GlComposite {

public :

	AxisSlider(const sliderType type, const Coord &sliderCoord, const float halfWidth, const float halfHeight, const Color &sliderColor, const Color &labelColor) :
		type(type), sliderCoord(sliderCoord) {

		vector<Coord> sliderPolygonCoords;
		vector<Color> sliderPolygonColors;
		vector<Coord> arrowPolygonCoords;
		vector<Color> arrowPolygonColors;

		int n;

		if (type == TOP_SLIDER) {
			n = 1;
		} else {
			n = -1;
		}

		Coord arrowPolygonVertice1 = sliderCoord;
		Coord arrowPolygonVertice2 = Coord(sliderCoord.getX() + halfWidth / 2, sliderCoord.getY() + n * (halfHeight / 2));
		Coord arrowPolygonVertice3 = Coord(sliderCoord.getX() + halfWidth / 4, sliderCoord.getY() + n * (halfHeight / 2));
		Coord arrowPolygonVertice4 = Coord(sliderCoord.getX() + halfWidth / 4, sliderCoord.getY() + n * halfHeight);
		Coord arrowPolygonVertice5 = Coord(sliderCoord.getX() - halfWidth / 4, sliderCoord.getY() + n * halfHeight);
		Coord arrowPolygonVertice6 = Coord(sliderCoord.getX() - halfWidth / 4, sliderCoord.getY() + n * (halfHeight / 2));
		Coord arrowPolygonVertice7 = Coord(sliderCoord.getX() - halfWidth / 2, sliderCoord.getY() + n * (halfHeight / 2));

		Coord sliderPolygonVertice2 = Coord(sliderCoord.getX() + 1.5 * halfWidth, sliderCoord.getY() + n * halfHeight);
		Coord sliderPolygonVertice3 = Coord(sliderCoord.getX() + 1.5 * halfWidth, sliderCoord.getY() + 2.5 * n * halfHeight);
		Coord sliderPolygonVertice4 = Coord(sliderCoord.getX() - 1.5 * halfWidth, sliderCoord.getY() + 2.5 * n * halfHeight);
		Coord sliderPolygonVertice1 = Coord(sliderCoord.getX() - 1.5 * halfWidth, sliderCoord.getY() + n * halfHeight);

		arrowPolygonCoords.push_back(arrowPolygonVertice1);
		arrowPolygonColors.push_back(sliderColor);
		arrowPolygonCoords.push_back(arrowPolygonVertice2);
		arrowPolygonColors.push_back(sliderColor);
		arrowPolygonCoords.push_back(arrowPolygonVertice3);
		arrowPolygonColors.push_back(sliderColor);
		arrowPolygonCoords.push_back(arrowPolygonVertice4);
		arrowPolygonColors.push_back(sliderColor);
		arrowPolygonCoords.push_back(arrowPolygonVertice5);
		arrowPolygonColors.push_back(sliderColor);
		arrowPolygonCoords.push_back(arrowPolygonVertice6);
		arrowPolygonColors.push_back(sliderColor);
		arrowPolygonCoords.push_back(arrowPolygonVertice7);
		arrowPolygonColors.push_back(sliderColor);

		Coord sliderQuadCoords[4];
		sliderQuadCoords[0] = sliderPolygonVertice1;
		sliderQuadCoords[1] = sliderPolygonVertice2;
		sliderQuadCoords[2] = sliderPolygonVertice3;
		sliderQuadCoords[3] = sliderPolygonVertice4;

		sliderQuad = new GlQuad(sliderQuadCoords, sliderColor);
		sliderQuad->setTextureName(TulipBitmapDir + SLIDER_TEXTURE_NAME);
		addGlEntity(sliderQuad, "slider quad");

		sliderPolygonCoords.push_back(sliderPolygonVertice1);
		sliderPolygonColors.push_back(sliderColor);
		sliderPolygonCoords.push_back(sliderPolygonVertice2);
		sliderPolygonColors.push_back(sliderColor);
		sliderPolygonCoords.push_back(sliderPolygonVertice3);
		sliderPolygonColors.push_back(sliderColor);
		sliderPolygonCoords.push_back(sliderPolygonVertice4);
		sliderPolygonColors.push_back(sliderColor);

		sliderPolygon = new GlPolygon(sliderPolygonCoords, sliderPolygonColors, sliderPolygonColors, false, true);
		addGlEntity(sliderPolygon, "slider polygon");

		arrowPolygon = new GlPolygon(arrowPolygonCoords, arrowPolygonColors, arrowPolygonColors, true, false);
		addGlEntity(arrowPolygon, "arrow polygon");

		sliderLabel = new GlLabel(Coord(sliderCoord.getX(), sliderCoord.getY()+ n * 1.75 * halfHeight), Coord(3 * halfWidth, 1.5 * halfHeight), labelColor);
		addGlEntity(sliderLabel, "slider label");

		computeBoundingBox();
	}

	~AxisSlider() {
		reset(true);
	}

	void setSliderFillColor(const Color &color) {
		sliderQuad->setColor(color);
		for (unsigned int i = 0 ; i < 7 ; ++i) {
			arrowPolygon->fcolor(i) = color;
		}
	}

	void setSliderOutlineColor(const Color &color) {
		for (unsigned int i = 0 ; i < 4 ; ++i) {
			sliderPolygon->ocolor(i) = color;
		}
	}

	void setSliderLabel(const string &label) {
		sliderLabel->setText(label);
	}

	void draw(float lod,Camera *camera) {
		glEnable(GL_LIGHTING);
		arrowPolygon->draw(lod, camera);
		sliderQuad->draw(lod, camera);
		sliderLabel->draw(lod, camera);
		glDisable(GL_LIGHTING);
		sliderPolygon->draw(lod, camera);
	}

	void computeBoundingBox() {
		GlBoundingBoxSceneVisitor glBBSV(NULL);
		acceptVisitor(&glBBSV);
		boundingBox = glBBSV.getBoundingBox();
	}

	Coord getSliderCoord() const {
		return sliderCoord;
	}

	void translate(const Coord &move) {
		sliderCoord += move;
		GlComposite::translate(move);
	}

	void moveToCoord(const Coord &coord) {
		translate(coord - sliderCoord);
	}

	sliderType getSliderType() const {
		return type;
	}

	Color getColor() {
		return arrowPolygon->fcolor(0);
	}

private :

	sliderType type;

	GlQuad *sliderQuad;
	GlPolygon *sliderPolygon;
	GlPolygon *arrowPolygon;
	GlLabel *sliderLabel;
	Coord sliderCoord;
};

class ParallelCoordsAxisSliders : public Interactor {

public :

	ParallelCoordsAxisSliders() : selectedAxis(NULL), lastSelectedAxis(NULL), selectedSlider(NULL), axisSliderDragStarted(false), drawSliders(true), slidersRangeDragStarted(false), lastAxisHeight(0), lastNbAxis(0) {}
	~ParallelCoordsAxisSliders() { deleteGlSliders();}
	bool eventFilter(QObject *, QEvent *);
	bool draw(GlMainWidget *glMainWidget);
	Interactor *clone() { return new ParallelCoordsAxisSliders(); }

private :

	AxisSlider *getSliderUnderPointer(ParallelAxis *axis, Coord pointerSceneCoords);
	void updateOtherAxisSliders();
	void buildGlSliders(vector<ParallelAxis *> axis);
	void deleteGlSliders();

	map<ParallelAxis *, vector<AxisSlider *> > axisSlidersMap;
	ParallelAxis *selectedAxis;
	ParallelAxis *lastSelectedAxis;
	AxisSlider *selectedSlider;
	bool axisSliderDragStarted;
	bool drawSliders;
	bool pointerBetweenSliders;
	bool slidersRangeDragStarted;
	int slidersRangeLength;
	int yClick;
	float lastAxisHeight;
	unsigned int lastNbAxis;
};

INTERACTORPLUGIN(ParallelCoordsAxisSliders, "ParallelCoordsAxisSliders", "Tulip Team", "05/11/2008", "Parallel Coordinates Axis Sliders", "1.0");

bool ParallelCoordsAxisSliders::eventFilter(QObject *widget, QEvent *e) {

	GlMainWidget *glWidget = (GlMainWidget *) widget;
	ParallelCoordinatesView *parallelView = (ParallelCoordinatesView *) view;

	drawSliders = false;

	vector<ParallelAxis *> allAxis = parallelView->getAllAxis();

	if (axisSlidersMap.size() == 0) {
		parallelView->updateAxisSlidersPosition();
		buildGlSliders(allAxis);
		drawSliders = true;
		parallelView->refresh();
		drawSliders = false;
		return true;
	}


	if ((lastAxisHeight != 0 && allAxis.size() > 0 && lastAxisHeight != allAxis[0]->getAxisHeight()) ||
		(lastNbAxis != 0 && lastNbAxis != allAxis.size())) {
		deleteGlSliders();
		buildGlSliders(allAxis);
		drawSliders = true;
		selectedSlider = NULL;
		selectedAxis = NULL;
		lastSelectedAxis = NULL;
		parallelView->refresh();
		drawSliders = false;
		lastNbAxis = allAxis.size();
		return true;
	}

	lastNbAxis = allAxis.size();

	if (e->type() == QEvent::MouseMove) {
		QMouseEvent *me = (QMouseEvent *) e;
		int x = glWidget->width() - me->x();
		int y = me->y();
		Coord screenCoords((double) x, (double) y, 0);
		Coord sceneCoords = glWidget->getScene()->getCamera()->screenTo3DWorld(screenCoords);
		if (!axisSliderDragStarted && !slidersRangeDragStarted) {
			selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
			if (selectedAxis != NULL) {
				selectedSlider = getSliderUnderPointer(selectedAxis, sceneCoords);
				pointerBetweenSliders = (sceneCoords.getY() < axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY()) &&
										(sceneCoords.getY() > axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
			} else {
				pointerBetweenSliders = false;
			}
		} else if (selectedAxis != NULL && selectedSlider != NULL && axisSliderDragStarted) {

			if (sceneCoords.getY() < selectedAxis->getBaseCoord().getY()) {
				sceneCoords = selectedAxis->getBaseCoord();
			} else if (sceneCoords.getY() > (selectedAxis->getBaseCoord().getY() + selectedAxis->getAxisHeight())) {
				sceneCoords = selectedAxis->getBaseCoord() + Coord(0, selectedAxis->getAxisHeight());
			}

			if (selectedSlider != NULL && selectedSlider->getSliderType() == TOP_SLIDER) {
				if (sceneCoords.getY() < selectedAxis->getBottomSliderCoord().getY()) {
					sceneCoords = selectedAxis->getBottomSliderCoord();
				}
				selectedAxis->setTopSliderCoord(Coord(selectedAxis->getBaseCoord().getX(), sceneCoords.getY()));
			} else {
				if (sceneCoords.getY() > selectedAxis->getTopSliderCoord().getY()) {
					sceneCoords = selectedAxis->getTopSliderCoord();
				}
				selectedAxis->setBottomSliderCoord(Coord(selectedAxis->getBaseCoord().getX(), sceneCoords.getY()));
			}
		} else if (selectedAxis != NULL && slidersRangeDragStarted) {
			int dy = yClick - y;
			if ((axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY() + dy) > (selectedAxis->getBaseCoord().getY() + selectedAxis->getAxisHeight())) {
				selectedAxis->setBottomSliderCoord(selectedAxis->getBaseCoord() + Coord(0, selectedAxis->getAxisHeight() - slidersRangeLength));
				selectedAxis->setTopSliderCoord(selectedAxis->getBaseCoord() + Coord(0, selectedAxis->getAxisHeight()));
			} else if ((axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY() + dy) < (selectedAxis->getBaseCoord().getY())) {
				selectedAxis->setBottomSliderCoord(selectedAxis->getBaseCoord());
				selectedAxis->setTopSliderCoord(selectedAxis->getBaseCoord() + Coord(0, slidersRangeLength));
			} else {
				selectedAxis->setBottomSliderCoord(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord() + Coord(0, dy));
				selectedAxis->setTopSliderCoord(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord() + Coord(0, dy));
			}
			yClick = y;
		}
		drawSliders = true;
		parallelView->refresh();
		return true;
	} else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
		if (selectedSlider != NULL && !axisSliderDragStarted) {
			axisSliderDragStarted = true;
			drawSliders = true;
			parallelView->refresh();
			return true;
		} else if (selectedAxis != NULL && pointerBetweenSliders && !slidersRangeDragStarted) {
			slidersRangeDragStarted = true;
			slidersRangeLength = axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY() -
								 axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY();
			yClick = ((QMouseEvent *) e)->y();
			return true;
		}
	} else if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
		if (selectedAxis != NULL && (axisSliderDragStarted || slidersRangeDragStarted)) {
			axisSliderDragStarted = false;
			slidersRangeDragStarted = false;
			drawSliders = false;
			Observable::holdObservers();
			parallelView->updateWithAxisSlidersRange(selectedAxis);
			Observable::unholdObservers();
			selectedSlider = NULL;
			lastSelectedAxis = selectedAxis;
			selectedAxis = NULL;
			drawSliders = true;
			parallelView->refresh();
			return true;
		}
	}

	return false;

}

void ParallelCoordsAxisSliders::buildGlSliders(vector<ParallelAxis *> axis) {
	vector<ParallelAxis *>::iterator it;
	for (it = axis.begin() ; it != axis.end() ; ++it) {
		ParallelAxis *axis = *it;
		lastAxisHeight = axis->getAxisHeight();
		float sliderMetricRef = axis->getAxisHeight() / 60;
		AxisSlider *axisTopSlider = new AxisSlider(TOP_SLIDER, axis->getTopSliderCoord(), 2.5 * sliderMetricRef, 2 * sliderMetricRef, redColor, axis->getAxisColor());
		AxisSlider *axisBottomSlider = new AxisSlider(BOTTOM_SLIDER, axis->getBottomSliderCoord(), 2.5 * sliderMetricRef, 2 * sliderMetricRef, redColor, axis->getAxisColor());
		axisSlidersMap[axis].push_back(axisTopSlider);
		axisSlidersMap[axis].push_back(axisBottomSlider);
	}
}

void ParallelCoordsAxisSliders::deleteGlSliders() {
	map<ParallelAxis *, vector<AxisSlider *> >::iterator it;
	for (it = axisSlidersMap.begin() ; it != axisSlidersMap.end() ; ++it) {
		ParallelAxis *axis = it->first;
		delete axisSlidersMap[axis][TOP_SLIDER];
		delete axisSlidersMap[axis][BOTTOM_SLIDER];
	}
	axisSlidersMap.clear();
}

AxisSlider *ParallelCoordsAxisSliders::getSliderUnderPointer(ParallelAxis *axis, Coord pointerSceneCoords) {
	AxisSlider *slider = NULL;
	vector<AxisSlider *>::iterator it;
	for (it = axisSlidersMap[axis].begin() ; it != axisSlidersMap[axis].end() ; ++it) {
		(*it)->computeBoundingBox();
		BoundingBox axisSliderBB = (*it)->getBoundingBox();
		if (pointerSceneCoords.getX() >= axisSliderBB.first.getX() && pointerSceneCoords.getX() <= axisSliderBB.second.getX() &&
			pointerSceneCoords.getY() >= axisSliderBB.first.getY() && pointerSceneCoords.getY() <= axisSliderBB.second.getY()) {
			slider = *it;
			break;
		}
	}

	return slider;
}


void ParallelCoordsAxisSliders::updateOtherAxisSliders() {
	map<ParallelAxis *, vector<AxisSlider *> >::iterator it;
	for (it = axisSlidersMap.begin() ; it != axisSlidersMap.end() ; ++it) {
		ParallelAxis *axis = it->first;
		if (axis != selectedAxis) {
			axisSlidersMap[axis][TOP_SLIDER]->moveToCoord(axis->getTopSliderCoord());
			axisSlidersMap[axis][TOP_SLIDER]->setSliderLabel(axis->getTopSliderTextValue());
			axisSlidersMap[axis][BOTTOM_SLIDER]->moveToCoord(axis->getBottomSliderCoord());
			axisSlidersMap[axis][BOTTOM_SLIDER]->setSliderLabel(axis->getBottomSliderTextValue());
		}
	}
}

bool ParallelCoordsAxisSliders::draw(GlMainWidget *glMainWidget) {

	if (!drawSliders) {
		return false;
	}

	map<ParallelAxis *, vector<AxisSlider *> >::iterator it;
	vector<AxisSlider *>::iterator it2;
	for (it = axisSlidersMap.begin() ; it != axisSlidersMap.end() ; ++it) {
		ParallelAxis *axis = it->first;
		for (it2 = (it->second).begin() ; it2 != (it->second).end() ; ++it2) {
			AxisSlider *slider = *it2;
			if (slider->getSliderType() == TOP_SLIDER) {
				slider->moveToCoord(axis->getTopSliderCoord());
				slider->setSliderLabel(axis->getTopSliderTextValue());
			} else {
				slider->moveToCoord(axis->getBottomSliderCoord());
				slider->setSliderLabel(axis->getBottomSliderTextValue());
			}
			if (slider == selectedSlider && !axisSliderDragStarted) {
				slider->setSliderOutlineColor(axis->getAxisColor());
			} else if (slider == selectedSlider && axisSliderDragStarted) {
				slider->setSliderFillColor(greenColor);
				slider->setSliderOutlineColor(greenColor);
			} else if (slidersRangeDragStarted && axis == selectedAxis) {
				slider->setSliderFillColor(greenColor);
				slider->setSliderOutlineColor(greenColor);
			} else if (axis == lastSelectedAxis) {
				slider->setSliderFillColor(blueColor);
				slider->setSliderOutlineColor(blueColor);
			} else {
				slider->setSliderFillColor(redColor);
				slider->setSliderOutlineColor(redColor);
			}
			if (axis->isVisible()) {
				slider->draw(0,0);
			}
		}
	}

	if (selectedAxis != NULL && pointerBetweenSliders) {
		Coord quadCoords[4];
		quadCoords[0] = Coord(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getX() - 1.5 * selectedAxis->getAxisGradWidth(), axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY());
		quadCoords[1] = Coord(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getX() + 1.5 * selectedAxis->getAxisGradWidth(), axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY());
		quadCoords[2] = Coord(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getX() + 1.5 * selectedAxis->getAxisGradWidth(), axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
		quadCoords[3] = Coord(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getX() - 1.5 * selectedAxis->getAxisGradWidth(), axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
		GlQuad quad(quadCoords, axisSlidersMap[selectedAxis][TOP_SLIDER]->getColor() + Color(0,0,0,100));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		quad.draw(0,0);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
	}

	return true;
}

}
