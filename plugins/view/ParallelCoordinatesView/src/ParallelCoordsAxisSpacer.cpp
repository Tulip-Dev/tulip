/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <tulip/GlLine.h>
#include <tulip/Camera.h>
#include <tulip/GlMainWidget.h>

#include "ParallelCoordsAxisSpacer.h"
#include "ParallelTools.h"
#include "ParallelAxis.h"
#include "ParallelCoordinatesView.h"

#include <QCursor>
#include <QMouseEvent>

using namespace std;

namespace tlp {

ParallelCoordsAxisSpacer::ParallelCoordsAxisSpacer()
    : parallelView(nullptr), selectedAxis(nullptr), x(0), y(0), dragStarted(false) {}

bool ParallelCoordsAxisSpacer::eventFilter(QObject *widget, QEvent *e) {

  GlMainWidget *glWidget = static_cast<GlMainWidget *>(widget);
  QMouseEvent *me = static_cast<QMouseEvent *>(e);

  if (e->type() == QEvent::MouseMove) {

    if (!dragStarted) {
      selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
      vector<ParallelAxis *> allAxis(parallelView->getAllAxis());

      if (selectedAxis == allAxis[0] && allAxis.size() > 1) {
        if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
          neighborsAxis = make_pair(allAxis[allAxis.size() - 1], allAxis[1]);
        } else {
          neighborsAxis = make_pair(static_cast<ParallelAxis *>(nullptr), allAxis[1]);
        }
      } else if (selectedAxis == allAxis[allAxis.size() - 1]) {
        if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
          neighborsAxis = make_pair(allAxis[allAxis.size() - 2], allAxis[0]);
        } else {
          neighborsAxis =
              make_pair(allAxis[allAxis.size() - 2], static_cast<ParallelAxis *>(nullptr));
        }
      }

      for (unsigned int i = 1; i < allAxis.size() - 1; ++i) {
        if (selectedAxis == allAxis[i]) {
          neighborsAxis = make_pair(allAxis[i - 1], allAxis[i + 1]);
        }
      }

      parallelView->refresh();
    } else if (selectedAxis != nullptr) {
      x = glWidget->width() - me->x();
      y = me->y();
      Coord screenCoords(x, y, 0.0f);
      Coord sceneCoords(glWidget->getScene()->getLayer("Main")->getCamera().viewportTo3DWorld(
          glWidget->screenToViewport(screenCoords)));

      if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
        float rotAngle = computeABACAngleWithAlKashi(Coord(0.0f, 0.0f, 0.0f),
                                                     Coord(0.0f, 50.0f, 0.0f), sceneCoords);
        float rotAngleLeft = neighborsAxis.first->getRotationAngle();

        if (rotAngleLeft <= 0.0f) {
          rotAngleLeft += 360.0f;
        }

        float rotAngleRight = neighborsAxis.second->getRotationAngle();

        if (rotAngleRight < 0.0f) {
          rotAngleRight += 360.0f;
        }

        if (sceneCoords.getX() < 0.0f) {
          if (((rotAngleRight > rotAngleLeft) && (rotAngle > 0.0f && rotAngle < rotAngleLeft)) ||
              (rotAngle > rotAngleRight && rotAngle < rotAngleLeft)) {
            selectedAxis->setRotationAngle(rotAngle);
          }
        } else {
          if (((rotAngleRight > rotAngleLeft) && (-rotAngle + 360.0f) > rotAngleRight &&
               (-rotAngle + 360.0f) < 360.0f) ||
              ((-rotAngle + 360.0f) > rotAngleRight && (-rotAngle + 360.0f) < rotAngleLeft)) {
            selectedAxis->setRotationAngle(-rotAngle);
          }
        }
      } else {
        Coord translationVector(sceneCoords.getX() - selectedAxis->getBaseCoord().getX(), 0.0f,
                                0.0f);
        BoundingBox axisBB(selectedAxis->getBoundingBox());
        axisBB.translate(translationVector);

        if ((neighborsAxis.first == nullptr ||
             axisBB[0][0] > neighborsAxis.first->getBoundingBox()[1][0]) &&
            (neighborsAxis.second == nullptr ||
             axisBB[1][0] < neighborsAxis.second->getBoundingBox()[0][0])) {
          selectedAxis->translate(translationVector);
        }
      }

      parallelView->draw();
    }

    return true;
  } else if (e->type() == QEvent::MouseButtonPress && me->button() == Qt::LeftButton) {
    if (selectedAxis != nullptr && !dragStarted) {
      dragStarted = true;
    }

    return true;

  } else if (e->type() == QEvent::MouseButtonRelease && me->button() == Qt::LeftButton) {
    if (selectedAxis != nullptr && dragStarted) {
      dragStarted = false;
      selectedAxis = nullptr;
      parallelView->draw();
      return true;
    }
  } else if (e->type() == QEvent::MouseButtonDblClick) {
    parallelView->resetAxisLayoutNextUpdate();
    parallelView->draw();
    return true;
  }

  return false;
}

bool ParallelCoordsAxisSpacer::draw(GlMainWidget *glMainWidget) {
  if (selectedAxis != nullptr) {
    glMainWidget->getScene()->getLayer("Main")->getCamera().initGl();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Array<Coord, 4> axisBP(selectedAxis->getBoundingPolygonCoords());
    GlLine line;
    line.addPoint(axisBP[0], Color(255, 0, 0));
    line.addPoint(axisBP[1], Color(255, 0, 0));
    line.addPoint(axisBP[2], Color(255, 0, 0));
    line.addPoint(axisBP[3], Color(255, 0, 0));
    line.addPoint(axisBP[0], Color(255, 0, 0));
    line.draw(0, nullptr);
    return true;
  }

  return false;
}

void ParallelCoordsAxisSpacer::viewChanged(View *view) {
  parallelView = static_cast<ParallelCoordinatesView *>(view);
}
} // namespace tlp
