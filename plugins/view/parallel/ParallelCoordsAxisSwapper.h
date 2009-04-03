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

#ifndef PARALLELCOORDSAXISSWAPPER_H_
#define PARALLELCOORDSAXISSWAPPER_H_

#include <tulip/GlRect.h>

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

class ParallelCoordsAxisSwapper : public InteractorComponent {
public :
  ParallelCoordsAxisSwapper() : selectedAxis(NULL), otherAxisToSwap(NULL), dragStarted(false), axisHighlightRect(NULL), axisSwapStarted(false) {}
  ~ParallelCoordsAxisSwapper();
  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);
  InteractorComponent *clone() { return new ParallelCoordsAxisSwapper(); }
  void setView(View *view);

private :

  ParallelCoordinatesView *parallelView;
  ParallelAxis *selectedAxis;
  ParallelAxis *otherAxisToSwap;
  Coord initialSelectedAxisCoord;
  bool dragStarted;
  int x,y;
  GlRect *axisHighlightRect;
  bool mouseMove;
  bool axisSwapStarted;
};

}

#endif /* PARALLELCOORDSAXISSWAPPER_H_ */
