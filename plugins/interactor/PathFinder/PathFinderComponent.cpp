/*
 * PathFinderComponent.cpp
 *
 *  Created on: Apr 14, 2010
 *      Author: Ludwig Fiolka
 */
#include "PathFinderComponent.h"

#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainView.h>

#include "highlighters/PathHighlighter.h"
#include "PathFinder.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QMessageBox>

using namespace std;
using namespace tlp;


PathFinderComponent::PathFinderComponent(PathFinder *parent) : parent(parent), graphPopable(false) {
}

PathFinderComponent::~PathFinderComponent() {
    for (set<PathHighlighter *>::iterator it = highlighters.begin(); it != highlighters.end(); ++it)
        delete (*it);
}

bool PathFinderComponent::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *> (event);
    GlMainWidget *glw= dynamic_cast<GlMainWidget *> (obj);

    if (glw==NULL)
        return false;

    if(event->type() == QEvent::MouseMove) {
        SelectedEntity entity;
        if((glw->pickNodesEdges(qMouseEv->x(),qMouseEv->y(),entity))&&(entity.getEntityType()==SelectedEntity::NODE_SELECTED)) {
            glw->setCursor(Qt::CrossCursor);
            return true;
        }
        else {
            glw->setCursor(Qt::ArrowCursor);
        }
    }

    if (event->type() == QEvent::MouseButtonPress) {

      Observable::holdObservers();

        vector<SelectedEntity> selNodes;
        vector<SelectedEntity> selEdges;
        glw->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), 0, 0, selNodes, selEdges);

        clearHighlighters(glw);
        BooleanProperty *selectionProperty = glw->getScene()->getGlGraphComposite()->getInputData()->getElementSelected();
        selectionProperty->setAllNodeValue(false);
        selectionProperty->setAllEdgeValue(false);

        if (selNodes.size() == 0) {
            src = node();
            tgt = node();
        } else {
            node tmp(selNodes[0].getComplexEntityId());
            if ((src.isValid() && tmp == src) || (tgt.isValid() && tmp == tgt)) { // User clicked back on the source or target node: reset the selection.
                src = node();
                tgt = node();
            } else if (src.isValid())
                tgt = tmp;
            else
                src = tmp;
        }

        selectPath(glw,glw->getScene()->getGlGraphComposite()->getGraph());

      Observable::unholdObservers();

        glw->redraw();
    }
    return false;
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

        } else
            // A path has been found: highlight it
            runHighlighters(glMainWidget,selection, src, tgt);
    } else if (src.isValid())
        selection->setNodeValue(src, true);

}

InteractorComponent *PathFinderComponent::clone() {
    PathFinderComponent *result = new PathFinderComponent(parent);
    for (set<PathHighlighter *>::iterator it = highlighters.begin(); it != highlighters.end(); ++it)
        result->addHighlighter((*it)->clone());
    return result;
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
    for (set<PathHighlighter *>::iterator it = highlighters.begin(); it != highlighters.end(); ++it) {
        if ((*it)->getName().compare(name) == 0)
            return *it;
    }
    return 0;
}

void PathFinderComponent::addHighlighter(PathHighlighter *highlighter) {
    highlighters.insert(highlighter);
}

set<PathHighlighter *> PathFinderComponent::getHighlighters() {
    return highlighters;
}

void PathFinderComponent::clear() {
  GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}
