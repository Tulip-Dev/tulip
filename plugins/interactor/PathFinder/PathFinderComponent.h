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

#ifndef PATHFINDERCOMPONENT_H_
#define PATHFINDERCOMPONENT_H_

#include <QSet>

#include <tulip/GLInteractor.h>
#include <tulip/Node.h>

namespace tlp {
class Graph;
class GlMainView;
class BooleanProperty;
class DoubleProperty;

class PathHighlighter;
class PathFinder;

/**
 * @brief The main component of the PathFinder interactor. Runs the path finding algorithm when two nodes have been selected and
 * triggers the highlighters if a path has been found.
 */
class PathFinderComponent: public GLInteractorComponent {
public:
  PathFinderComponent(PathFinder *parent);
  ~PathFinderComponent();
  bool eventFilter(QObject *, QEvent *);
  void timerEvent(QTimerEvent *);

  /**
   * Install a path highlighter. This highlighter is not always activated. The component will ask its interactor parent before triggering an highlighter.
   * @param highlighter The highlighter to install.
   */
  void addHighlighter(PathHighlighter *highlighter);

  /**
   * @return A list of all the installed highlighters.
   */
  QSet<PathHighlighter *> getHighlighters();

  void clear();

private:
  tlp::node src;
  tlp::node tgt;
  PathFinder *parent;
  bool graphPopable;
  int timerId;
  int lastX;
  int lastY;
  tlp::GlMainWidget *glMW;

  QSet<PathHighlighter *> highlighters;
  void runHighlighters(tlp::GlMainWidget *glMainWidget,tlp::BooleanProperty *selection, tlp::node src, tlp::node tgt);
  void clearHighlighters(tlp::GlMainWidget *glMainWidget);

  void selectPath(GlMainWidget *glMainWidget,tlp::Graph *graph);
  PathHighlighter *findHighlighter(const std::string &name);
};

}

#endif /* PATHFINDERCOMPONENT_H_ */
