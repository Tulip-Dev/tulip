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

#ifndef PARALLELCOORDSAXISSWAPPER_H_
#define PARALLELCOORDSAXISSWAPPER_H_

#include <QCursor>
#include <QMouseEvent>

#include <string>

#include <tulip/GlRect.h>
#include <tulip/GLInteractor.h>

namespace tlp {

class ParallelCoordsAxisSwapper : public GLInteractorComponent {

public :
  ParallelCoordsAxisSwapper();
  ~ParallelCoordsAxisSwapper();
  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);
  void viewChanged(View *view);

private :

  ParallelCoordinatesView *parallelView;
  ParallelAxis *selectedAxis;
  ParallelAxis *otherAxisToSwap;
  Coord initialSelectedAxisCoord;
  float initialSelectedAxisRotAngle;
  bool dragStarted;
  int x,y;
  bool mouseMove;
  bool axisSwapStarted;
};

}

#endif /* PARALLELCOORDSAXISSWAPPER_H_ */
