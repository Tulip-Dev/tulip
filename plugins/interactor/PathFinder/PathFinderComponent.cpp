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

#include "PathFinderComponent.h"

#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainView.h>

#include "highlighters/PathHighlighter.h"
#include "PathFinder.h"

#include <QMouseEvent>
#include <QMessageBox>

using namespace std;
using namespace tlp;


PathFinderComponent::PathFinderComponent(PathFinder *parent) : parent(parent), graphPopable(false), timerId(0), glMW(NULL) {
}

PathFinderComponent::~PathFinderComponent() {
  qDeleteAll(highlighters);
}

bool PathFinderComponent::eventFilter(QObject *obj, QEvent *event) {
  QMouseEvent *qMouseEv = static_cast<QMouseEvent *> (event);
  GlMainWidget *glw= dynamic_cast<GlMainWidget *> (obj);

  if (glw==NULL)
    return false;

  if(event->type() == QEvent::MouseMove) {
    // we need to avoid too much calls to
    // time consuming pickNodesEdges method
    // so it is called only if the mouse is not moving
    // during 500ms
    // so we kill the last timer if needed,
    // then register the last mouse pos and start
    // a one shot 500 ms duration timer
    if (timerId)
      killTimer(timerId);

    lastX = qMouseEv->x();
    lastY = qMouseEv->y();
    glMW = glw;
    timerId = startTimer(500);
    glw->setCursor(Qt::ArrowCursor);
  }

  if (event->type() == QEvent::MouseButtonPress &&
      qMouseEv->button() == Qt::LeftButton) {
    // kill current timer if needed
    if (timerId) {
      killTimer(timerId);
      timerId = 0;
    }

    Observable::holdObservers();

    vector<SelectedEntity> selNodes;
    vector<SelectedEntity> selEdges;
    glw->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), 0, 0, selNodes, selEdges);

    clearHighlighters(glw);
    BooleanProperty *selectionProperty = glw->getScene()->getGlGraphComposite()->getInputData()->getElementSelected();
    selectionProperty->setAllNodeValue(false);
    selectionProperty->setAllEdgeValue(false);

    if (selNodes.empty()) {
      src = node();
      tgt = node();
      glw->setCursor(Qt::ArrowCursor);
    }
    else {
      node tmp(selNodes[0].getComplexEntityId());

      if ((src.isValid() && tmp == src) || (tgt.isValid() && tmp == tgt)) { // User clicked back on the source or target node: reset the selection.
        src = node();
        tgt = node();
      }
      else if (src.isValid())
        tgt = tmp;
      else
        src = tmp;

      glw->setCursor(Qt::CrossCursor);
    }

    selectPath(glw,glw->getScene()->getGlGraphComposite()->getGraph());

    Observable::unholdObservers();

    glw->redraw();

    return (src.isValid() || tgt.isValid());
  }

  return false;
}

void PathFinderComponent::timerEvent(QTimerEvent *ev) {
  if (ev->timerId() == timerId) {
    // kill the timer to avoid a new event in 500 ms
    killTimer(timerId);
    timerId = 0;
    SelectedEntity entity;

    // check if there is a node at the current mouse pos
    if((glMW->pickNodesEdges(lastX, lastY,entity))&&(entity.getEntityType()==SelectedEntity::NODE_SELECTED))
      // find one, show a cross
      glMW->setCursor(Qt::CrossCursor);
    else
      // none show an arrow
      glMW->setCursor(Qt::ArrowCursor);
  }
}

void PathFinderComponent::selectPath(GlMainWidget *glMainWidget, Graph *graph) {
  GlGraphInputData *inputData = glMainWidget->getScene()->getGlGraphComposite()->getInputData();

  BooleanProperty *selection = inputData->getElementSelected();

  if (src.isValid() && tgt.isValid()) { // We only select a path if source and target are valid
    Observable::holdObservers();
    DoubleProperty *weights = 0;
    string weightsMetricName = parent->getWeightMetricName();

    if (weightsMetricName.compare(NO_METRIC) != 0 && graph->existProperty(weightsMetricName)) {
      PropertyInterface *prop = graph->getProperty(weightsMetricName);

      if (prop && prop->getTypename().compare("double") == 0)
        weights = graph->getProperty<DoubleProperty> (weightsMetricName);
    }

    bool pathFound = PathAlgorithm::computePath(graph, parent->getPathsType(), parent->getEdgeOrientation(), src, tgt, selection, weights, parent->getTolerance());
    Observable::unholdObservers();

    if (!pathFound) {
      selection->setAllNodeValue(false);
      selection->setAllEdgeValue(false);
      selection->setNodeValue(src, true);
      QMessageBox::warning(0, "Path finder", "Path do not exist.");

    }
    else
      // A path has been found: highlight it
      runHighlighters(glMainWidget,selection, src, tgt);
  }
  else if (src.isValid())
    selection->setNodeValue(src, true);

}

void PathFinderComponent::runHighlighters(GlMainWidget *glMainWidget,BooleanProperty *selection, node src, node tgt) {
  glMainWidget->getScene()->getGlGraphComposite()->getGraph()->push(true);
  graphPopable = true;
  vector<string> activeHighlighters(parent->getActiveHighlighters());

  for (vector<string>::iterator it = activeHighlighters.begin(); it != activeHighlighters.end(); ++it) {
    PathHighlighter *hler = findHighlighter(*it);

    if (hler)
      hler->highlight(parent, glMainWidget, selection, src, tgt);
  }
}

void PathFinderComponent::clearHighlighters(GlMainWidget *glMainWidget) {
  if (graphPopable && glMainWidget->getScene()->getGlGraphComposite()->getGraph()->canPop()) {
    glMainWidget->getScene()->getGlGraphComposite()->getGraph()->pop(false);
    graphPopable = false;
  }

  vector<string> activeHighlighters(parent->getHighlighters());

  for (vector<string>::iterator it = activeHighlighters.begin(); it != activeHighlighters.end(); ++it) {
    PathHighlighter *hler = findHighlighter(*it);

    if (hler)
      hler->clear();
  }
}

PathHighlighter *PathFinderComponent::findHighlighter(const string &name) {
  foreach(PathHighlighter *p, highlighters) {
    if (p->getName() == name)
      return p;
  }
  return NULL;
}

void PathFinderComponent::addHighlighter(PathHighlighter *highlighter) {
  highlighters.insert(highlighter);
}

QSet<PathHighlighter *> PathFinderComponent::getHighlighters() {
  return highlighters;
}

void PathFinderComponent::clear() {
  GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}
