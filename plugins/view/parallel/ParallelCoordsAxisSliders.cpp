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


#include <tulip/TlpTools.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlMainWidget.h>

#include "ParallelCoordinatesView.h"
#include "ParallelTools.h"
#include "ParallelCoordsAxisSliders.h"

using namespace std;

namespace tlp {

const Color redColor(255,0,0);
const Color blackColor(0,0,0);
const Color greenColor(0,255,0);
const Color blueColor(14,241,212);
const Color orangeColor(255,123,0);


AxisSlider::AxisSlider(const sliderType type, const Coord &sliderCoord, const float halfWidth, const float halfHeight,
		const Color &sliderColor, const Color &labelColor, const float rotationAngle) :
			type(type), sliderComposite(new GlComposite()), sliderCoord(sliderCoord), rotationAngle(rotationAngle) {

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
	sliderComposite->addGlEntity(sliderQuad, "slider quad");

	sliderPolygonCoords.push_back(sliderPolygonVertice1);
	sliderPolygonColors.push_back(sliderColor);
	sliderPolygonCoords.push_back(sliderPolygonVertice2);
	sliderPolygonColors.push_back(sliderColor);
	sliderPolygonCoords.push_back(sliderPolygonVertice3);
	sliderPolygonColors.push_back(sliderColor);
	sliderPolygonCoords.push_back(sliderPolygonVertice4);
	sliderPolygonColors.push_back(sliderColor);

	sliderPolygon = new GlPolygon(sliderPolygonCoords, sliderPolygonColors, sliderPolygonColors, false, true);
	sliderComposite->addGlEntity(sliderPolygon, "slider polygon");

	arrowPolygon = new GlPolygon(arrowPolygonCoords, arrowPolygonColors, arrowPolygonColors, true, false);
	sliderComposite->addGlEntity(arrowPolygon, "arrow polygon");

	sliderLabel = new GlLabel(Coord(sliderCoord.getX(), sliderCoord.getY()+ n * 1.75 * halfHeight), Coord(3 * halfWidth, 1.5 * halfHeight), labelColor);
	sliderComposite->addGlEntity(sliderLabel, "slider label");
}

AxisSlider::~AxisSlider() {
	sliderComposite->reset(true);
	delete sliderComposite;
}

void AxisSlider::setSliderFillColor(const Color &color) {
	sliderQuad->setColor(color);
	for (unsigned int i = 0 ; i < 7 ; ++i) {
		arrowPolygon->fcolor(i) = color;
	}
}

void AxisSlider::setSliderOutlineColor(const Color &color) {
	for (unsigned int i = 0 ; i < 4 ; ++i) {
		sliderPolygon->ocolor(i) = color;
	}
}

void AxisSlider::draw(float lod,Camera *camera) {
	glEnable(GL_LIGHTING);
	if (rotationAngle != 0) {
		glPushMatrix();
		glRotatef(rotationAngle, 0.0, 0.0, 1.0);
	}
	arrowPolygon->draw(lod, camera);
	sliderQuad->draw(lod, camera);
	sliderLabel->draw(lod, camera);
	glDisable(GL_LIGHTING);
	sliderPolygon->draw(lod, camera);
	if (rotationAngle != 0) {
		glPopMatrix();
	}
}

BoundingBox AxisSlider::getBoundingBox() {
	GlBoundingBoxSceneVisitor glBBSV(NULL);
	sliderComposite->acceptVisitor(&glBBSV);
	BoundingBox slidersBoundingBox = glBBSV.getBoundingBox();
	if (rotationAngle != 0) {
		Coord bbBL = slidersBoundingBox.first;
		Coord bbTR = slidersBoundingBox.second;
		Coord bbBR = Coord(slidersBoundingBox.second.getX(), slidersBoundingBox.first.getY());
		Coord bbTL = Coord(slidersBoundingBox.first.getX(), slidersBoundingBox.second.getY());
		rotateVector(bbBL, rotationAngle, Z_ROT);
		rotateVector(bbTR, rotationAngle, Z_ROT);
		rotateVector(bbBR, rotationAngle, Z_ROT);
		rotateVector(bbTL, rotationAngle, Z_ROT);
		slidersBoundingBox = BoundingBox();
		slidersBoundingBox.check(bbBL);
		slidersBoundingBox.check(bbTR);
		slidersBoundingBox.check(bbBR);
		slidersBoundingBox.check(bbTL);
	}
	return slidersBoundingBox;
}

void AxisSlider::translate(const Coord &move) {
	sliderCoord += move;
	sliderComposite->translate(move);
}

ParallelCoordsAxisSliders::ParallelCoordsAxisSliders() :
	currentGraph(NULL), selectedAxis(NULL), selectedSlider(NULL),
	axisSliderDragStarted(false), slidersRangeDragStarted(false),
	lastAxisHeight(0), lastNbAxis(0), multiFilteringActivated(false), selectionLayer(new GlLayer("sliders selection layer")) {}

ParallelCoordsAxisSliders::~ParallelCoordsAxisSliders() {
	deleteGlSliders();
	delete selectionLayer;
}

void ParallelCoordsAxisSliders::setView(View *view) {
	InteractorComponent::setView(view);
	parallelView = (ParallelCoordinatesView *) view;
	initOrUpdateSliders();
}

bool ParallelCoordsAxisSliders::compute(GlMainWidget *glMainWidget) {
	initOrUpdateSliders();
	return true;
}

void ParallelCoordsAxisSliders::initOrUpdateSliders() {
	vector<ParallelAxis *> allAxis = parallelView->getAllAxis();

	if (axisSlidersMap.size() == 0) {
		parallelView->updateAxisSlidersPosition();
		buildGlSliders(allAxis);
		parallelView->refresh();
		return;
	}

	if ((lastAxisHeight != 0 && allAxis.size() > 0 && lastAxisHeight != allAxis[0]->getAxisHeight()) ||
		(lastNbAxis != 0 && lastNbAxis != allAxis.size()) || (currentGraph != parallelView->getGraph())) {
		deleteGlSliders();
		if (currentGraph != NULL && currentGraph != parallelView->getGraph()) {
			vector<ParallelAxis *>::iterator it;
			for (it = allAxis.begin() ; it != allAxis.end() ; ++it) {
				(*it)->resetSlidersPosition();
			}
		}
		buildGlSliders(allAxis);
		selectedSlider = NULL;
		selectedAxis = NULL;
		lastSelectedAxis.clear();
		parallelView->refresh();
	}

	lastNbAxis = allAxis.size();
	currentGraph = parallelView->getGraph();

	map<ParallelAxis *, vector<AxisSlider *> >::iterator it;
	for (it = axisSlidersMap.begin() ; it != axisSlidersMap.end() ; ++it) {
		ParallelAxis *axis = it->first;
		axisSlidersMap[axis][TOP_SLIDER]->setRotationAngle(axis->getRotationAngle());
		axisSlidersMap[axis][BOTTOM_SLIDER]->setRotationAngle(axis->getRotationAngle());
	}
}

bool ParallelCoordsAxisSliders::eventFilter(QObject *widget, QEvent *e) {

	GlMainWidget *glWidget = (GlMainWidget *) widget;
	selectionLayer->setCamera(glWidget->getScene()->getLayer("Main")->getCamera());

	initOrUpdateSliders();

	if (!parallelView->hasHighlightedElts()) {
		lastSelectedAxis.clear();
	}

	if (e->type() == QEvent::MouseMove) {
		QMouseEvent *me = (QMouseEvent *) e;
		int x = glWidget->width() - me->x();
		int y = me->y();
		Coord screenCoords((double) x, (double) y, 0);
		Coord sceneCoords = glWidget->getScene()->getCamera()->screenTo3DWorld(screenCoords);
		if (!axisSliderDragStarted && !slidersRangeDragStarted) {
			selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
			if (selectedAxis != NULL) {
				if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
					rotateVector(sceneCoords, -(selectedAxis->getRotationAngle()), Z_ROT);
				}
				selectedSlider = getSliderUnderPointer(glWidget, selectedAxis, me->x(), me->y());
				pointerBetweenSliders = (sceneCoords.getY() < axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY()) &&
										(sceneCoords.getY() > axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
			} else {
				pointerBetweenSliders = false;
			}
		} else if (selectedAxis != NULL && selectedSlider != NULL && axisSliderDragStarted) {

			float minY, maxY;
			if (!multiFilteringActivated) {
				minY = selectedAxis->getBaseCoord().getY();
				maxY = selectedAxis->getBaseCoord().getY() + selectedAxis->getAxisHeight();
			} else {
				minY = slidersYBoundaries[selectedAxis].first;
				maxY = slidersYBoundaries[selectedAxis].second;
			}

			if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
				rotateVector(sceneCoords, -(selectedAxis->getRotationAngle()), Z_ROT);
			}

			if (sceneCoords.getY() < minY) {
				sceneCoords = Coord(selectedAxis->getBaseCoord().getX(), minY, 0);
			} else if (sceneCoords.getY() > maxY) {
				sceneCoords = Coord(selectedAxis->getBaseCoord().getX(), maxY, 0);
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
			if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
				float rotAngle = selectedAxis->getRotationAngle();
				if (rotAngle < -90 && rotAngle > -270) {
					dy *= -1;
				} else if (rotAngle == -90) {
					dy = xClick - x;
				} else if (rotAngle == -270) {
					dy = x - xClick;
				}
			}
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
			xClick = x;
		}
		parallelView->refresh();
		return true;
	} else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
		if (selectedSlider != NULL && !axisSliderDragStarted) {
			axisSliderDragStarted = true;
			parallelView->refresh();
			return true;
		} else if (selectedAxis != NULL && pointerBetweenSliders && !multiFilteringActivated && !slidersRangeDragStarted) {
			slidersRangeDragStarted = true;
			slidersRangeLength = axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY() -
								 axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY();
			yClick = ((QMouseEvent *) e)->y();
			xClick = ((QMouseEvent *) e)->x();
			return true;
		}
	} else if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
		if (selectedAxis != NULL && (axisSliderDragStarted || slidersRangeDragStarted)) {
			axisSliderDragStarted = false;
			slidersRangeDragStarted = false;
			Observable::holdObservers();
			parallelView->updateWithAxisSlidersRange(selectedAxis, multiFilteringActivated);
			updateSlidersYBoundaries();
			Observable::unholdObservers();
			selectedSlider = NULL;
			if (!multiFilteringActivated) {
				lastSelectedAxis.clear();
			}
			lastSelectedAxis.push_back(selectedAxis);;
			selectedAxis = NULL;
			parallelView->refresh();
			return true;
		}
	} else if (e->type() == QEvent::KeyPress && ((QKeyEvent *) e)->key() == Qt::Key_Control) {
		multiFilteringActivated = true;
		updateSlidersYBoundaries();
		parallelView->refresh();
	} else if (e->type() == QEvent::KeyRelease && ((QKeyEvent *) e)->key() == Qt::Key_Control) {
		multiFilteringActivated = false;
		parallelView->refresh();
	}

	return false;

}

void ParallelCoordsAxisSliders::buildGlSliders(vector<ParallelAxis *> axis) {
	vector<ParallelAxis *>::iterator it;
	for (it = axis.begin() ; it != axis.end() ; ++it) {
		ParallelAxis *axis = *it;
		lastAxisHeight = axis->getAxisHeight();
		float sliderMetricRef = axis->getAxisHeight() / 60;
		AxisSlider *axisTopSlider = new AxisSlider(TOP_SLIDER, axis->getTopSliderCoord(), 2.5 * sliderMetricRef, 2 * sliderMetricRef, redColor, axis->getAxisColor(), axis->getRotationAngle());
		AxisSlider *axisBottomSlider = new AxisSlider(BOTTOM_SLIDER, axis->getBottomSliderCoord(), 2.5 * sliderMetricRef, 2 * sliderMetricRef, redColor, axis->getAxisColor(), axis->getRotationAngle());
		axisSlidersMap[axis].push_back(axisTopSlider);
		axisSlidersMap[axis].push_back(axisBottomSlider);
		selectionLayer->addGlEntity(axisTopSlider, getStringFromNumber(axisTopSlider));
		selectionLayer->addGlEntity(axisBottomSlider, getStringFromNumber(axisBottomSlider));
	}
}

void ParallelCoordsAxisSliders::deleteGlSliders() {
	map<ParallelAxis *, vector<AxisSlider *> >::iterator it;
	for (it = axisSlidersMap.begin() ; it != axisSlidersMap.end() ; ++it) {
		ParallelAxis *axis = it->first;
		selectionLayer->deleteGlEntity(axisSlidersMap[axis][TOP_SLIDER]);
		selectionLayer->deleteGlEntity(axisSlidersMap[axis][BOTTOM_SLIDER]);
		delete axisSlidersMap[axis][TOP_SLIDER];
		delete axisSlidersMap[axis][BOTTOM_SLIDER];
	}
	axisSlidersMap.clear();
}

AxisSlider *ParallelCoordsAxisSliders::getSliderUnderPointer(GlMainWidget *glWidget, ParallelAxis *axis, int x, int y) {
	vector<GlEntity *> pickedEntities;
	if (glWidget->selectGlEntities(x, y, pickedEntities, selectionLayer)) {
		for (unsigned int i = 0 ; i < pickedEntities.size() ; ++i) {
			vector<AxisSlider *>::iterator it;
			for (it = axisSlidersMap[axis].begin() ; it != axisSlidersMap[axis].end() ; ++it) {
				if ((*it) == pickedEntities[i]) {
					return (*it);
				}
			}
		}
	}
	return NULL;
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

	Camera *camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
	camera->initGl();

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
			} else if (std::find(lastSelectedAxis.begin(), lastSelectedAxis.end(), axis) != lastSelectedAxis.end()) {
				slider->setSliderFillColor(blueColor);
				slider->setSliderOutlineColor(blueColor);
			} else if (multiFilteringActivated) {
				slider->setSliderFillColor(orangeColor);
				slider->setSliderOutlineColor(orangeColor);
			} else {
				slider->setSliderFillColor(redColor);
				slider->setSliderOutlineColor(redColor);
			}
			if (axis->isVisible()) {
				glEnable(GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				slider->draw(0,0);
				glDisable(GL_BLEND);
			}
		}
	}

	if (selectedAxis != NULL && pointerBetweenSliders) {
		Coord quadCoords[4];
		quadCoords[0] = Coord(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getX() - 1.5 * selectedAxis->getAxisGradsWidth(), axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY());
		quadCoords[1] = Coord(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getX() + 1.5 * selectedAxis->getAxisGradsWidth(), axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY());
		quadCoords[2] = Coord(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getX() + 1.5 * selectedAxis->getAxisGradsWidth(), axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
		quadCoords[3] = Coord(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getX() - 1.5 * selectedAxis->getAxisGradsWidth(), axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
		GlQuad quad(quadCoords, axisSlidersMap[selectedAxis][TOP_SLIDER]->getColor() + Color(0,0,0,100));
		float rotAngle = selectedAxis->getRotationAngle();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		if (rotAngle != 0) {
			glPushMatrix();
			glRotatef(rotAngle, 0.0, 0.0, 1.0);
		}
		quad.draw(0,0);
		if (rotAngle != 0) {
			glPopMatrix();
		}
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
	}

	return true;
}

void ParallelCoordsAxisSliders::updateSlidersYBoundaries() {
	slidersYBoundaries.clear();
	map<ParallelAxis *, vector<AxisSlider *> >::iterator it;
	for (it = axisSlidersMap.begin() ; it != axisSlidersMap.end() ; ++it) {
		ParallelAxis *axis = it->first;
		slidersYBoundaries[axis].first = axis->getBottomSliderCoord().getY();
		slidersYBoundaries[axis].second = axis->getTopSliderCoord().getY();
	}
}

}
