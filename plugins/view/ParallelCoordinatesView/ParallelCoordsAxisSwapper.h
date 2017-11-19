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

#ifndef PARALLELCOORDSAXISSWAPPER_H_
#define PARALLELCOORDSAXISSWAPPER_H_

#include <tulip/GLInteractor.h>
#include <tulip/Coord.h>

namespace tlp {

class ParallelAxis;
class ParallelCoordinatesView;

class ParallelCoordsAxisSwapper : public GLInteractorComponent {

public:
  ParallelCoordsAxisSwapper();
  ~ParallelCoordsAxisSwapper() override;
  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;

private:
  ParallelCoordinatesView *parallelView;
  ParallelAxis *selectedAxis;
  ParallelAxis *otherAxisToSwap;
  Coord initialSelectedAxisCoord;
  float initialSelectedAxisRotAngle;
  bool dragStarted;
  int x, y;
  bool mouseMove;
  bool axisSwapStarted;
};
}

#endif /* PARALLELCOORDSAXISSWAPPER_H_ */
