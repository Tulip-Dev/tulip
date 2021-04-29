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

#include "PathFinderComponent.h"

#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainView.h>

#include "highlighters/PathHighlighter.h"
#include "PathFinder.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>

using namespace std;
using namespace tlp;

PathFinderComponent::PathFinderComponent(PathFinder *parent)
    : parent(parent), graphPopable(false) {}

PathFinderComponent::~PathFinderComponent() {
  qDeleteAll(highlighters);
}

bool PathFinderComponent::eventFilter(QObject *obj, QEvent *event) {
  QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(event);
  GlMainWidget *glw = dynamic_cast<GlMainWidget *>(obj);

  if (glw == nullptr)
    return false;

  if (event->type() == QEvent::MouseMove) {
    SelectedEntity entity;
    if (glw->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), entity) &&
        entity.getEntityType() == SelectedEntity::NODE_SELECTED) {
      tmp.id = entity.getComplexEntityId();
      glw->setCursor(Qt::CrossCursor);
      return true;
    } else {
      tmp = node();
      glw->setCursor(Qt::ArrowCursor);
    }
  } else if (event->type() == QEvent::MouseButtonDblClick && qMouseEv->button() == Qt::LeftButton) {
    // double click will deselect all
    Observable::holdObservers();

    BooleanProperty *selectionProperty =
        glw->getScene()->getGlGraphComposite()->getInputData()->getElementSelected();
    selectionProperty->setAllNodeValue(false);
    selectionProperty->setAllEdgeValue(false);

    if (tmp.isValid()) {
      // double click on a node
      // user is selecting a new path source
      src = tmp;
      // select it
      selectionProperty->setNodeValue(src, true);
    } else
      src = node();

    // invalidate the path target
    tgt = node();

    Observable::unholdObservers();

    return true;
  } else if (event->type() == QEvent::MouseButtonPress && qMouseEv->button() == Qt::LeftButton &&
             tmp.isValid()) {
    if (!src.isValid()) {
      // user can select the path source with a simple click
      // if no source already exists
      Observable::holdObservers();

      BooleanProperty *selectionProperty =
          glw->getScene()->getGlGraphComposite()->getInputData()->getElementSelected();
      selectionProperty->setAllNodeValue(false);
      selectionProperty->setAllEdgeValue(false);
      // select it
      selectionProperty->setNodeValue(src = tmp, true);

      Observable::unholdObservers();
    } else {
      // as the path source already exists
      // we assume that the user is selecting a new path target
      tgt = tmp;
      // but we wait a bit to ensure the current event does not belong to a
      // QEvent::MouseButtonDblClick event
      obj->startTimer(QApplication::doubleClickInterval() + 5);
    }
    return true;
  } else if (event->type() == QEvent::Timer) {
    obj->killTimer(static_cast<QTimerEvent *>(event)->timerId());
    // tgt could have been invalidate by a QEvent::MouseButtonDblClick event
    if (tgt.isValid()) {
      Observable::holdObservers();

      BooleanProperty *selectionProperty =
          glw->getScene()->getGlGraphComposite()->getInputData()->getElementSelected();
      selectionProperty->setAllNodeValue(false);
      selectionProperty->setAllEdgeValue(false);
      selectPath(glw, glw->getScene()->getGlGraphComposite()->getGraph());

      Observable::unholdObservers();

      glw->redraw();
    }
    return true;
  }

  return false;
}

void PathFinderComponent::selectPath(GlMainWidget *glMainWidget, Graph *graph) {
  GlGraphInputData *inputData = glMainWidget->getScene()->getGlGraphComposite()->getInputData();

  BooleanProperty *selection = inputData->getElementSelected();

  if (src.isValid() && tgt.isValid()) { // We only select a path if source and target are valid
    Observable::holdObservers();
    DoubleProperty *weights = nullptr;
    string weightsMetricName = parent->getWeightMetricName();

    if (weightsMetricName.compare(NO_METRIC) != 0 && graph->existProperty(weightsMetricName)) {
      PropertyInterface *prop = graph->getProperty(weightsMetricName);

      if (prop && prop->getTypename().compare("double") == 0)
        weights = graph->getProperty<DoubleProperty>(weightsMetricName);
    }

    bool pathFound =
        PathAlgorithm::computePath(graph, parent->getPathsType(), parent->getEdgeOrientation(), src,
                                   tgt, selection, weights, parent->getTolerance());
    Observable::unholdObservers();

    if (!pathFound) {
      selection->setAllNodeValue(false);
      selection->setAllEdgeValue(false);
      selection->setNodeValue(src, true);
      QMessageBox::warning(QApplication::activeWindow(), "Path finder",
                           "A path between the selected nodes cannot be found.");

    } else
      // A path has been found: highlight it
      runHighlighters(glMainWidget, selection, src, tgt);
  } else if (src.isValid()) {
    selection->setNodeValue(src, true);
  }
}

void PathFinderComponent::runHighlighters(GlMainWidget *glMainWidget, BooleanProperty *selection,
                                          node src, node tgt) {
  glMainWidget->getScene()->getGlGraphComposite()->getGraph()->push(true);
  graphPopable = true;
  vector<string> activeHighlighters(parent->getActiveHighlighters());

  for (vector<string>::iterator it = activeHighlighters.begin(); it != activeHighlighters.end();
       ++it) {
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

  for (vector<string>::iterator it = activeHighlighters.begin(); it != activeHighlighters.end();
       ++it) {
    PathHighlighter *hler = findHighlighter(*it);

    if (hler)
      hler->clear();
  }
}

PathHighlighter *PathFinderComponent::findHighlighter(const string &name) {
  for (auto p : highlighters) {
    if (p->getName() == name)
      return p;
  }

  return nullptr;
}

void PathFinderComponent::addHighlighter(PathHighlighter *highlighter) {
  highlighters.insert(highlighter);
}

QSet<PathHighlighter *> PathFinderComponent::getHighlighters() {
  return highlighters;
}

void PathFinderComponent::clear() {
  GlMainView *glMainView = static_cast<GlMainView *>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}
