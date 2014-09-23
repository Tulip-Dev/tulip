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

#include <tulip/GlLayer.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlRect.h>
#include <tulip/GlCircle.h>
#include <tulip/GlQuad.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

#include "ParallelTools.h"
#include "ParallelCoordinatesView.h"
#include "ParallelCoordsAxisSwapper.h"

using namespace std;

namespace tlp {

static Color axisHighlight(14,241,212,0);

static Color axisToSwapHighlight(0,255,0,0);

ParallelCoordsAxisSwapper::ParallelCoordsAxisSwapper() : parallelView(NULL), selectedAxis(NULL), otherAxisToSwap(NULL), initialSelectedAxisRotAngle(0), dragStarted(false), x(0), y(0), mouseMove(false), axisSwapStarted(false) {}

ParallelCoordsAxisSwapper::~ParallelCoordsAxisSwapper() {}

void ParallelCoordsAxisSwapper::viewChanged(View *view) {
  parallelView = dynamic_cast<ParallelCoordinatesView *>(view);
}

bool ParallelCoordsAxisSwapper::eventFilter(QObject *widget, QEvent *e) {

  GlMainWidget *glWidget = dynamic_cast<GlMainWidget *>(widget);

  mouseMove = false;

  if (e->type() == QEvent::MouseMove && !axisSwapStarted) {
    QMouseEvent *me = dynamic_cast<QMouseEvent *>(e);
    mouseMove = true;

    if (!dragStarted) {
      selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
    }
    else {
      x = glWidget->width() - me->x();
      y = me->y();
      Coord screenCoords(x, y, 0);
      Coord sceneCoords(glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords));

      if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
        float rotAngle = computeABACAngleWithAlKashi(Coord(0.0f, 0.0f, 0.0f), Coord(0.0f, 50.0f, 0.0f), sceneCoords);

        if (sceneCoords.getX() < 0.0f) {
          selectedAxis->setRotationAngle(rotAngle);
        }
        else {
          selectedAxis->setRotationAngle(-rotAngle);
        }

      }
      else {
        Coord translationVector(sceneCoords - selectedAxis->getBaseCoord());
        selectedAxis->translate(Coord(translationVector.getX(), 0.0f, 0.0f));
      }

      otherAxisToSwap = parallelView->getAxisUnderPointer(me->x(), me->y());
    }

    parallelView->refresh();
    return true;

  }
  else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
    if (selectedAxis != NULL && !dragStarted) {
      dragStarted = true;
      parallelView->removeAxis(selectedAxis);
      initialSelectedAxisRotAngle = selectedAxis->getRotationAngle();
      selectedAxis->setRotationAngle(0.0f);
      initialSelectedAxisCoord = selectedAxis->getBaseCoord();
      parallelView->getGlMainWidget()->draw();
    }

    return true;

  }
  else if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent *) e)->button() == Qt::LeftButton) {

    if (selectedAxis != NULL && dragStarted) {
      selectedAxis->setRotationAngle(0.0f);
      Coord translationVector(initialSelectedAxisCoord - selectedAxis->getBaseCoord());
      selectedAxis->translate(Coord(translationVector.getX(), translationVector.getY(), 0.0f));
      selectedAxis->setRotationAngle(initialSelectedAxisRotAngle);
      parallelView->addAxis(selectedAxis);

      if (otherAxisToSwap != NULL && otherAxisToSwap != selectedAxis) {
        axisSwapStarted = true;
        parallelView->swapAxis(selectedAxis, otherAxisToSwap);
        axisSwapStarted = false;
        otherAxisToSwap = NULL;
      }

      selectedAxis = NULL;
      dragStarted = false;
      parallelView->draw();
    }

    return true;
  }

  selectedAxis = NULL;
  return false;
}

bool ParallelCoordsAxisSwapper::draw(GlMainWidget *glMainWidget) {

  if (selectedAxis != NULL) {

    glMainWidget->getScene()->getLayer("Main")->getCamera().initGl();

    GlQuad *axisHighlightRect = NULL;
    BoundingBox axisBB;

    if (!dragStarted) {
      Array<Coord, 4> axisBP(selectedAxis->getBoundingPolygonCoords());
      axisHighlightRect = new GlQuad(axisBP[0], axisBP[1], axisBP[2], axisBP[3], axisHighlight);

    }
    else {
      if (otherAxisToSwap != NULL && otherAxisToSwap != selectedAxis) {
        Array<Coord, 4> axisBP(otherAxisToSwap->getBoundingPolygonCoords());
        axisHighlightRect = new GlQuad(axisBP[0], axisBP[1], axisBP[2], axisBP[3], axisToSwapHighlight);
      }
    }

    if (axisHighlightRect != NULL) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
      glEnable(GL_LIGHTING);
      axisHighlightRect->draw(0,0);
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      delete axisHighlightRect;
    }

    if (dragStarted && mouseMove) {
      selectedAxis->disableTrickForSelection();
      selectedAxis->draw(0,&glMainWidget->getScene()->getLayer("Main")->getCamera());
      selectedAxis->enableTrickForSelection();
    }

    return true;
  }

  return false;
}

}
