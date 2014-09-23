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

#ifndef PARALLELCOORDSGLENTITIESSELECTOR_H_
#define PARALLELCOORDSGLENTITIESSELECTOR_H_

#include <QCursor>
#include <QMouseEvent>

#include <tulip/MouseSelector.h>

#include "ParallelCoordinatesView.h"

namespace tlp {

class ParallelCoordsGlEntitiesSelector : public MouseSelector {
public :
  ParallelCoordsGlEntitiesSelector(Qt::MouseButton button = Qt::LeftButton,
                                   Qt::KeyboardModifier modifier = Qt::NoModifier) : MouseSelector(button, modifier) {}
  ~ParallelCoordsGlEntitiesSelector() {}
  bool eventFilter(QObject *, QEvent *);
  virtual void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) = 0;
  virtual void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection) = 0;
  virtual void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection) = 0;
};

}

#endif /* PARALLELCOORDSGLENTITIESSELECTOR_H_ */
