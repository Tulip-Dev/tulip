/*
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
///@cond DOXYGEN_HIDDEN

#ifndef MOUSEADDEDGE_H
#define MOUSEADDEDGE_H

#include <vector>
#include <tulip/Coord.h>
#include <tulip/GLInteractor.h>
#include <tulip/Observable.h>

namespace tlp {
class Graph;
class LayoutProperty;

class TLP_QT_SCOPE MouseEdgeBuilder:public GLInteractorComponent, public Observable {
private:
  node _source;
  bool _started;
  Coord _startPos,_curPos;
  std::vector<Coord> _bends;
  Graph *_graph;
  LayoutProperty *_layoutProperty;

protected:
  /**
   * @brief addLink Adds a link between a source and destination nodes. Adds an edge by default
   * @param source
   * @param dest
   */
  virtual void addLink(QObject *, const node source, const node dest);
  /**
   * @brief source gets the source node.
   * @return
   */
  node source()const {
    return _source;
  }
  /**
   * @brief bends returns the bends list.
   * @return
   */
  const std::vector<Coord>& bends()const {
    return _bends;
  }
  /**
   * @brief setBends updates the bends list.
   * @param bends
   */
  void setBends(const std::vector<Coord>& bends) {
    _bends = bends;
  }

public:
  MouseEdgeBuilder();
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  void initObserver(Graph *);
  void clearObserver();

  void treatEvent(const Event&);

  void clear();
};


}
#endif
///@endcond
