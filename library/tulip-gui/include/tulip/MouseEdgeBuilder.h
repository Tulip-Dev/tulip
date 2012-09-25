/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef MOUSEADDEDGE_H
#define MOUSEADDEDGE_H
#include <vector>
#include <tulip/Coord.h>
#include <tulip/GLInteractor.h>
#include <tulip/ObservableGraph.h>
#include <tulip/Observable.h>
#include <tulip/ObservableProperty.h>

namespace tlp {
class Graph;
class LayoutProperty;

class TLP_QT_SCOPE MouseEdgeBuilder:public GLInteractorComponent, private Observable {
private:
  node source;
  bool started;
  Coord startPos,curPos;
  std::vector<Coord> bends;
  Graph *graph;
  LayoutProperty *layoutProperty;

public:
  MouseEdgeBuilder();
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  void initObserver(Graph *);
  void clearObserver();

  void treatEvent(const Event&);

  InteractorComponent *clone() {
    return new MouseEdgeBuilder();
  }
};


}
#endif
