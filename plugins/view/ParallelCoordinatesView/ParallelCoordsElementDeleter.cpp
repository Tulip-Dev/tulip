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

#include "ParallelCoordinatesView.h"
#include "ParallelCoordsElementDeleter.h"

using namespace std;

namespace tlp {

bool ParallelCoordsElementDeleter::eventFilter(QObject *, QEvent *e) {

  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *me = dynamic_cast<QMouseEvent *>(e);

    if (me->buttons()==Qt::LeftButton) {
      ParallelCoordinatesView *parallelView = dynamic_cast<ParallelCoordinatesView *>(view());
      Observable::holdObservers();
      parallelView->deleteDataUnderPointer(me->x(), me->y());
      Observable::unholdObservers();
      return true;
    }
  }

  return false;
}

}
