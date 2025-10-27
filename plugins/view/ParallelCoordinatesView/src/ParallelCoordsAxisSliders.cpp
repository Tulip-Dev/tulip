/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlMainWidget.h>

#include "ParallelCoordinatesView.h"
#include "ParallelTools.h"
#include "ParallelCoordsAxisSliders.h"
#include "ParallelAxis.h"

#include <QMouseEvent>

using namespace std;

namespace tlp {

const Color redColor(255, 0, 0);
const Color blackColor(0, 0, 0);
const Color greenColor(0, 255, 0);
const Color blueColor(14, 241, 212);
const Color orangeColor(255, 123, 0);
const Color lightGreen(197, 255, 0);

AxisSlider::AxisSlider(const sliderType type, const Coord &sliderCoord, const float halfWidth,
                       const float halfHeight, const Color &sliderColor, const Color &labelColor,
                       const float rotationAngle)
    : type(type), sliderComposite(new GlComposite()), sliderCoord(sliderCoord),
      rotationAngle(rotationAngle) {

  float n = (type == TOP_SLIDER) ? 1.0f : -1.0f;

  Coord arrowPolygonVertice1(sliderCoord);
  Coord arrowPolygonVertice2(sliderCoord.getX() + halfWidth / 2.0f,
                             sliderCoord.getY() + n * (halfHeight / 2.0f));
  Coord arrowPolygonVertice3(sliderCoord.getX() + halfWidth / 4.0f,
                             sliderCoord.getY() + n * (halfHeight / 2.0f));
  Coord arrowPolygonVertice4(sliderCoord.getX() + halfWidth / 4.0f,
                             sliderCoord.getY() + n * halfHeight);
  Coord arrowPolygonVertice5(sliderCoord.getX() - halfWidth / 4.0f,
                             sliderCoord.getY() + n * halfHeight);
  Coord arrowPolygonVertice6(sliderCoord.getX() - halfWidth / 4.0f,
                             sliderCoord.getY() + n * (halfHeight / 2.0f));
  Coord arrowPolygonVertice7(sliderCoord.getX() - halfWidth / 2.0f,
                             sliderCoord.getY() + n * (halfHeight / 2.0f));

  Coord sliderPolygonVertice2(sliderCoord.getX() + 1.5f * halfWidth,
                              sliderCoord.getY() + n * halfHeight);
  Coord sliderPolygonVertice3(sliderCoord.getX() + 1.5f * halfWidth,
                              sliderCoord.getY() + 2.5f * n * halfHeight);
  Coord sliderPolygonVertice4(sliderCoord.getX() - 1.5f * halfWidth,
                              sliderCoord.getY() + 2.5f * n * halfHeight);
  Coord sliderPolygonVertice1(sliderCoord.getX() - 1.5f * halfWidth,
                              sliderCoord.getY() + n * halfHeight);

  vector<Coord> arrowPolygonCoords;
  arrowPolygonCoords.reserve(7);
  vector<Color> arrowPolygonColors(7, sliderColor);

  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice1));
  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice2));
  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice3));
  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice4));
  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice5));
  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice6));
  arrowPolygonCoords.push_back(std::move(arrowPolygonVertice7));

  sliderQuad = new GlQuad(sliderPolygonVertice1, sliderPolygonVertice2, sliderPolygonVertice3,
                          sliderPolygonVertice4, sliderColor);
  sliderQuad->setTextureName(SLIDER_TEXTURE_NAME);
  sliderComposite->addGlEntity(sliderQuad, "slider quad");

  vector<Coord> sliderPolygonCoords;
  sliderPolygonCoords.reserve(4);
  vector<Color> sliderPolygonColors(4, sliderColor);

  sliderPolygonCoords.push_back(std::move(sliderPolygonVertice1));
  sliderPolygonCoords.push_back(std::move(sliderPolygonVertice2));
  sliderPolygonCoords.push_back(std::move(sliderPolygonVertice3));
  sliderPolygonCoords.push_back(std::move(sliderPolygonVertice4));

  sliderPolygon =
      new GlPolygon(sliderPolygonCoords, sliderPolygonColors, sliderPolygonColors, false, true);
  sliderComposite->addGlEntity(sliderPolygon, "slider polygon");

  arrowPolygon =
      new GlPolygon(arrowPolygonCoords, arrowPolygonColors, arrowPolygonColors, true, false);
  sliderComposite->addGlEntity(arrowPolygon, "arrow polygon");

  sliderLabel = new GlLabel(Coord(sliderCoord.getX(), sliderCoord.getY() + n * 1.75f * halfHeight),
                            Size(3.0f * halfWidth, 1.5f * halfHeight), labelColor);
  sliderComposite->addGlEntity(sliderLabel, "slider label");
}

AxisSlider::~AxisSlider() {
  sliderComposite->reset(true);
  delete sliderComposite;
}

void AxisSlider::setSliderFillColor(const Color &color) {
  sliderQuad->setColor(color);
  arrowPolygon->setFillColor(color);
}

void AxisSlider::setSliderOutlineColor(const Color &color) {
  sliderPolygon->setOutlineColor(color);
}

void AxisSlider::draw(float lod, Camera *camera) {
  glEnable(GL_LIGHTING);

  if (rotationAngle != 0.0f) {
    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
  }

  arrowPolygon->draw(lod, camera);
  sliderQuad->draw(lod, camera);
  sliderLabel->draw(lod, camera);
  glDisable(GL_LIGHTING);
  sliderPolygon->draw(lod, camera);

  if (rotationAngle != 0.0f) {
    glPopMatrix();
  }
}

BoundingBox AxisSlider::getBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSV(nullptr);
  sliderComposite->acceptVisitor(&glBBSV);
  BoundingBox &&slidersBoundingBox = glBBSV.getBoundingBox();

  if (rotationAngle != 0.0f) {
    Coord bbBL(slidersBoundingBox[0]);
    Coord bbTR(slidersBoundingBox[1]);
    Coord bbBR(slidersBoundingBox[1][0], slidersBoundingBox[0][1]);
    Coord bbTL(slidersBoundingBox[0][0], slidersBoundingBox[1][1]);
    rotateVector(bbBL, rotationAngle, Z_ROT);
    rotateVector(bbTR, rotationAngle, Z_ROT);
    rotateVector(bbBR, rotationAngle, Z_ROT);
    rotateVector(bbTL, rotationAngle, Z_ROT);
    slidersBoundingBox.init(bbBL);
    slidersBoundingBox.expand(bbTR, true);
    slidersBoundingBox.expand(bbBR, true);
    slidersBoundingBox.expand(bbTL, true);
  }

  return std::move(slidersBoundingBox);
}

void AxisSlider::translate(const Coord &move) {
  sliderCoord += move;
  sliderComposite->translate(move);
}

ParallelCoordsAxisSliders::ParallelCoordsAxisSliders()
    : parallelView(nullptr), currentGraph(nullptr), selectedAxis(nullptr), selectedSlider(nullptr),
      axisSliderDragStarted(false), pointerBetweenSliders(false), slidersRangeDragStarted(false),
      slidersRangeLength(0), xClick(0), yClick(0), lastAxisHeight(0), lastNbAxis(0),
      highlightedEltsSetOperation(ParallelCoordinatesDrawing::NONE),
      selectionLayer(new GlLayer("sliders selection layer")) {}

ParallelCoordsAxisSliders::~ParallelCoordsAxisSliders() {
  deleteGlSliders();
  delete selectionLayer;
}

void ParallelCoordsAxisSliders::viewChanged(View *view) {
  if (view == nullptr) {
    parallelView = nullptr;
    return;
  }

  parallelView = static_cast<ParallelCoordinatesView *>(view);
  initOrUpdateSliders();
}

bool ParallelCoordsAxisSliders::compute(GlMainWidget *) {
  initOrUpdateSliders();
  return true;
}

void ParallelCoordsAxisSliders::initOrUpdateSliders() {
  vector<ParallelAxis *> &&allAxis = parallelView->getAllAxis();

  if (axisSlidersMap.empty()) {
    parallelView->updateAxisSlidersPosition();
    buildGlSliders(allAxis);
    parallelView->refresh();
    return;
  }

  if ((lastAxisHeight != 0 && !allAxis.empty() && lastAxisHeight != allAxis[0]->getAxisHeight()) ||
      (lastNbAxis != 0 && lastNbAxis != allAxis.size()) ||
      (currentGraph != parallelView->getGraphProxy()->getGraph())) {
    deleteGlSliders();

    if (currentGraph != nullptr && currentGraph != parallelView->getGraphProxy()->getGraph()) {
      for (auto pa : allAxis) {
        pa->resetSlidersPosition();
      }
    }

    buildGlSliders(allAxis);
    selectedSlider = nullptr;
    selectedAxis = nullptr;
    lastSelectedAxis.clear();
    parallelView->refresh();
  }

  lastNbAxis = allAxis.size();
  currentGraph = parallelView->getGraphProxy()->getGraph();

  map<ParallelAxis *, vector<AxisSlider *>>::iterator it;

  for (auto &it : axisSlidersMap) {
    ParallelAxis *axis = it.first;
    axisSlidersMap[axis][TOP_SLIDER]->setRotationAngle(axis->getRotationAngle());
    axisSlidersMap[axis][BOTTOM_SLIDER]->setRotationAngle(axis->getRotationAngle());
  }
}

bool ParallelCoordsAxisSliders::eventFilter(QObject *widget, QEvent *e) {

  GlMainWidget *glWidget = static_cast<GlMainWidget *>(widget);

  if (glWidget == nullptr)
    return false;

  selectionLayer->setSharedCamera(&glWidget->getScene()->getLayer("Main")->getCamera());

  initOrUpdateSliders();

  if (!parallelView->hasHighlightedElts()) {
    lastSelectedAxis.clear();
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *me = static_cast<QMouseEvent *>(e);
    int x = glWidget->width() - me->pos().x();
    int y = me->pos().y();
    Coord screenCoords(x, y, 0.0f);
    Coord sceneCoords(glWidget->getScene()->getLayer("Main")->getCamera().viewportTo3DWorld(
        glWidget->screenToViewport(screenCoords)));

    if (!axisSliderDragStarted && !slidersRangeDragStarted) {
      selectedAxis = parallelView->getAxisUnderPointer(me->pos().x(), me->pos().y());

      if (selectedAxis != nullptr) {
        if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
          rotateVector(sceneCoords, -(selectedAxis->getRotationAngle()), Z_ROT);
        }

        selectedSlider =
            getSliderUnderPointer(glWidget, selectedAxis, me->pos().x(), me->pos().y());
        pointerBetweenSliders =
            (sceneCoords.getY() <
             axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY()) &&
            (sceneCoords.getY() >
             axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
      } else {
        pointerBetweenSliders = false;
      }
    } else if (selectedAxis != nullptr && selectedSlider != nullptr && axisSliderDragStarted) {

      float minY, maxY;

      if (highlightedEltsSetOperation != ParallelCoordinatesDrawing::INTERSECTION) {
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
        sceneCoords = Coord(selectedAxis->getBaseCoord().getX(), minY, 0.0f);
      } else if (sceneCoords.getY() > maxY) {
        sceneCoords = Coord(selectedAxis->getBaseCoord().getX(), maxY, 0.0f);
      }

      if (selectedSlider != nullptr && selectedSlider->getSliderType() == TOP_SLIDER) {
        if (sceneCoords.getY() < selectedAxis->getBottomSliderCoord().getY()) {
          sceneCoords = selectedAxis->getBottomSliderCoord();
        }

        selectedAxis->setTopSliderCoord(
            Coord(selectedAxis->getBaseCoord().getX(), sceneCoords.getY()));
      } else {
        if (sceneCoords.getY() > selectedAxis->getTopSliderCoord().getY()) {
          sceneCoords = selectedAxis->getTopSliderCoord();
        }

        selectedAxis->setBottomSliderCoord(
            Coord(selectedAxis->getBaseCoord().getX(), sceneCoords.getY()));
      }
    } else if (selectedAxis != nullptr && slidersRangeDragStarted) {
      int dy = yClick - y;

      if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
        float rotAngle = selectedAxis->getRotationAngle();

        if (rotAngle < -90.0f && rotAngle > -270.0f) {
          dy *= -1;
        } else if (rotAngle == -90.0f) {
          dy = xClick - x;
        } else if (rotAngle == -270.0f) {
          dy = x - xClick;
        }
      }

      if ((axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY() + dy) >
          (selectedAxis->getBaseCoord().getY() + selectedAxis->getAxisHeight())) {
        selectedAxis->setBottomSliderCoord(
            selectedAxis->getBaseCoord() +
            Coord(0.0f, selectedAxis->getAxisHeight() - slidersRangeLength));
        selectedAxis->setTopSliderCoord(selectedAxis->getBaseCoord() +
                                        Coord(0.0f, selectedAxis->getAxisHeight()));
      } else if ((axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY() + dy) <
                 (selectedAxis->getBaseCoord().getY())) {
        selectedAxis->setBottomSliderCoord(selectedAxis->getBaseCoord());
        selectedAxis->setTopSliderCoord(selectedAxis->getBaseCoord() +
                                        Coord(0.0f, slidersRangeLength));
      } else {
        selectedAxis->setBottomSliderCoord(
            axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord() + Coord(0.0f, dy));
        selectedAxis->setTopSliderCoord(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord() +
                                        Coord(0.0f, dy));
      }

      yClick = y;
      xClick = x;
    }

    parallelView->refresh();
    return true;
  } else if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *me = static_cast<QMouseEvent *>(e);

    if (me->button() == Qt::LeftButton && selectedSlider != nullptr && !axisSliderDragStarted) {
      axisSliderDragStarted = true;
      parallelView->refresh();
      return true;
    } else if (selectedAxis != nullptr && pointerBetweenSliders &&
               highlightedEltsSetOperation != ParallelCoordinatesDrawing::INTERSECTION &&
               !slidersRangeDragStarted) {
      QMouseEvent *me = static_cast<QMouseEvent *>(e);
      slidersRangeDragStarted = true;
      slidersRangeLength = axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY() -
                           axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY();
      yClick = me->pos().y();
      xClick = me->pos().x();
      return true;
    }
  } else if (e->type() == QEvent::MouseButtonRelease) {
    QMouseEvent *me = static_cast<QMouseEvent *>(e);

    if (me->button() == Qt::LeftButton && selectedAxis != nullptr &&
        (axisSliderDragStarted || slidersRangeDragStarted)) {
      axisSliderDragStarted = false;
      slidersRangeDragStarted = false;
      Observable::holdObservers();
      parallelView->updateWithAxisSlidersRange(selectedAxis, highlightedEltsSetOperation);
      updateSlidersYBoundaries();
      Observable::unholdObservers();
      selectedSlider = nullptr;

      if (highlightedEltsSetOperation == ParallelCoordinatesDrawing::NONE) {
        lastSelectedAxis.clear();
      }

      lastSelectedAxis.push_back(selectedAxis);
      ;
      selectedAxis = nullptr;
      parallelView->refresh();
      return true;
    }
  } else if (e->type() == QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(e);

    if (ke->key() == Qt::Key_Control) {
      highlightedEltsSetOperation = ParallelCoordinatesDrawing::INTERSECTION;
      updateSlidersYBoundaries();
    } else if (ke->key() == Qt::Key_Shift) {
      highlightedEltsSetOperation = ParallelCoordinatesDrawing::UNION;
    }

    parallelView->refresh();
    return true;
  } else if (e->type() == QEvent::KeyRelease) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(e);

    if (ke->key() == Qt::Key_Control || ke->key() == Qt::Key_Shift) {
      highlightedEltsSetOperation = ParallelCoordinatesDrawing::NONE;
    }

    parallelView->refresh();
    return true;
  }

  return false;
}

void ParallelCoordsAxisSliders::buildGlSliders(const vector<ParallelAxis *> &axis) {
  for (auto pa : axis) {
    lastAxisHeight = pa->getAxisHeight();
    float sliderMetricRef = pa->getAxisHeight() / 60.0f;
    AxisSlider *axisTopSlider = new AxisSlider(
        TOP_SLIDER, pa->getTopSliderCoord(), 2.5f * sliderMetricRef, 2.0f * sliderMetricRef,
        redColor, pa->getAxisColor(), pa->getRotationAngle());
    AxisSlider *axisBottomSlider = new AxisSlider(
        BOTTOM_SLIDER, pa->getBottomSliderCoord(), 2.5f * sliderMetricRef, 2.0f * sliderMetricRef,
        redColor, pa->getAxisColor(), pa->getRotationAngle());
    axisSlidersMap[pa].push_back(axisTopSlider);
    axisSlidersMap[pa].push_back(axisBottomSlider);
    selectionLayer->addGlEntity(axisTopSlider, getStringFromNumber(axisTopSlider));
    selectionLayer->addGlEntity(axisBottomSlider, getStringFromNumber(axisBottomSlider));
  }
}

void ParallelCoordsAxisSliders::deleteGlSliders() {
  for (auto &it : axisSlidersMap) {
    ParallelAxis *axis = it.first;
    selectionLayer->deleteGlEntity(axisSlidersMap[axis][TOP_SLIDER]);
    selectionLayer->deleteGlEntity(axisSlidersMap[axis][BOTTOM_SLIDER]);
    delete axisSlidersMap[axis][TOP_SLIDER];
    delete axisSlidersMap[axis][BOTTOM_SLIDER];
  }

  axisSlidersMap.clear();
}

AxisSlider *ParallelCoordsAxisSliders::getSliderUnderPointer(GlMainWidget *glWidget,
                                                             ParallelAxis *axis, int x, int y) {
  vector<SelectedEntity> pickedEntities;

  if (glWidget->pickGlEntities(x, y, pickedEntities, selectionLayer)) {
    for (auto &entity : pickedEntities) {
      for (auto slider : axisSlidersMap[axis]) {
        if (slider == entity.getSimpleEntity()) {
          return slider;
        }
      }
    }
  }

  return nullptr;
}

void ParallelCoordsAxisSliders::updateOtherAxisSliders() {
  for (auto &it : axisSlidersMap) {
    ParallelAxis *axis = it.first;

    if (axis != selectedAxis) {
      axisSlidersMap[axis][TOP_SLIDER]->moveToCoord(axis->getTopSliderCoord());
      axisSlidersMap[axis][TOP_SLIDER]->setSliderLabel(axis->getTopSliderTextValue());
      axisSlidersMap[axis][BOTTOM_SLIDER]->moveToCoord(axis->getBottomSliderCoord());
      axisSlidersMap[axis][BOTTOM_SLIDER]->setSliderLabel(axis->getBottomSliderTextValue());
    }
  }
}

bool ParallelCoordsAxisSliders::draw(GlMainWidget *glMainWidget) {

  Camera &camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
  camera.initGl();

  for (auto &it : axisSlidersMap) {
    ParallelAxis *axis = it.first;

    for (auto slider : it.second) {
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
      } else if (std::find(lastSelectedAxis.begin(), lastSelectedAxis.end(), axis) !=
                 lastSelectedAxis.end()) {
        slider->setSliderFillColor(blueColor);
        slider->setSliderOutlineColor(blueColor);
      } else if (highlightedEltsSetOperation == ParallelCoordinatesDrawing::INTERSECTION) {
        slider->setSliderFillColor(orangeColor);
        slider->setSliderOutlineColor(orangeColor);
      } else if (highlightedEltsSetOperation == ParallelCoordinatesDrawing::UNION) {
        slider->setSliderFillColor(lightGreen);
        slider->setSliderOutlineColor(lightGreen);
      } else {
        slider->setSliderFillColor(redColor);
        slider->setSliderOutlineColor(redColor);
      }

      if (axis->isVisible()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        slider->draw(0, &camera);
        glDisable(GL_BLEND);
      }
    }
  }

  if (selectedAxis != nullptr && pointerBetweenSliders) {
    Coord p1(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getX() -
                 1.5f * selectedAxis->getAxisGradsWidth(),
             axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY());
    Coord p2(axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getX() +
                 1.5f * selectedAxis->getAxisGradsWidth(),
             axisSlidersMap[selectedAxis][TOP_SLIDER]->getSliderCoord().getY());
    Coord p3(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getX() +
                 1.5f * selectedAxis->getAxisGradsWidth(),
             axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
    Coord p4(axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getX() -
                 1.5f * selectedAxis->getAxisGradsWidth(),
             axisSlidersMap[selectedAxis][BOTTOM_SLIDER]->getSliderCoord().getY());
    GlQuad quad(p1, p2, p3, p4,
                axisSlidersMap[selectedAxis][TOP_SLIDER]->getColor() + Color(0, 0, 0, 100));
    float rotAngle = selectedAxis->getRotationAngle();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
    glEnable(GL_LIGHTING);

    if (rotAngle != 0.0f) {
      glPushMatrix();
      glRotatef(rotAngle, 0.0f, 0.0f, 1.0f);
    }

    quad.draw(0, nullptr);

    if (rotAngle != 0.0f) {
      glPopMatrix();
    }

    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
  }

  return true;
}

void ParallelCoordsAxisSliders::updateSlidersYBoundaries() {
  slidersYBoundaries.clear();

  for (auto &it : axisSlidersMap) {
    ParallelAxis *axis = it.first;
    slidersYBoundaries[axis].first = axis->getBottomSliderCoord().getY();
    slidersYBoundaries[axis].second = axis->getTopSliderCoord().getY();
  }
}
} // namespace tlp
