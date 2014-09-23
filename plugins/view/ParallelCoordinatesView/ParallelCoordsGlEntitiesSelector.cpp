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

#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>

#include "ParallelCoordsGlEntitiesSelector.h"


using namespace std;

namespace tlp {

bool ParallelCoordsGlEntitiesSelector::eventFilter(QObject *widget, QEvent *e) {

  ParallelCoordinatesView *parallelView = dynamic_cast<ParallelCoordinatesView *>(view());
  GlMainWidget *glMainWidget = dynamic_cast<GlMainWidget *>(widget);


  if (e->type() == QEvent::MouseButtonPress) {

    QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons()== Qt::LeftButton) {

      if (!started) {
        x = qMouseEv->x();
        y = qMouseEv->y();
        w = 0;
        h = 0;
        started = true;
        graph = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      }

      return true;
    }

  }

  if (e->type() == QEvent::MouseMove) {

    QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons() & Qt::LeftButton && started) {
      if ((qMouseEv->x() > 0) && (qMouseEv->x() < glMainWidget->width()))
        w = qMouseEv->x() - x;

      if ((qMouseEv->y() > 0) && (qMouseEv->y() < glMainWidget->height()))
        h = qMouseEv->y() - y;

      parallelView->refresh();
      return true;
    }
  }

  if (e->type() == QEvent::MouseButtonRelease) {

    QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

    if (started) {
      Observable::holdObservers();
      bool boolVal = true; // add to selection

      if (qMouseEv->modifiers() != Qt::ControlModifier) {
        if (qMouseEv->modifiers() !=
#if defined(__APPLE__)
            Qt::AltModifier
#else
            Qt::ShiftModifier
#endif
           ) {

          unselectAllEntitiesHandler(parallelView);

        }
        else
          boolVal = false; // remove from selection
      }

      if ((w == 0) && (h == 0)) {

        selectedEntitiesHandler(parallelView, x, y, boolVal);

      }
      else {

        if (w < 0) {
          w *= -1;
          x -= w;
        }

        if (h < 0) {
          h *= -1;
          y -= h;
        }

        selectedEntitiesHandler(parallelView, x, y, w, h, boolVal);
      }

      started = false;
      Observable::unholdObservers();
      return true;
    }
  }

  return false;
}

}
