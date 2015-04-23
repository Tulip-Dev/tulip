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

#include <QMouseEvent>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/GlTools.h>
#include <tulip/GlLine.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/MouseEdgeBuilder.h>
#include <tulip/Camera.h>

using namespace std;
using namespace tlp;

MouseEdgeBuilder::MouseEdgeBuilder():_source(node()),_started(false),_graph(NULL),_layoutProperty(NULL) {}

bool MouseEdgeBuilder::eventFilter(QObject *widget, QEvent *e) {

  GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);

  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    SelectedEntity selectedEntity;
    GlGraphInputData *inputData=glMainWidget->getScene()->getGlGraphComposite()->getInputData();
    Graph * _graph = inputData->getGraph();

    LayoutProperty* mLayout = inputData->getElementLayout();

    if (qMouseEv->buttons()==Qt::LeftButton) {
      if (!_started) {
        bool result=glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity);

        if (result && (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED)) {
          _started=true;
          initObserver(_graph);
          _source=node(selectedEntity.getComplexEntityId());
          _curPos=_startPos=mLayout->getNodeValue(_source);
          return true;
        }

        return false;
      }
      else {
        bool result = glMainWidget->pickNodesEdges(qMouseEv->x(),qMouseEv->y(),selectedEntity);

        if (result && (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED)) {
          Observable::holdObservers();
          _started=false;
          clearObserver();
          // allow to undo
          _graph->push();
          addLink(widget,_source,node(selectedEntity.getComplexEntityId()));
          Observable::unholdObservers();

        }
        else {
          Coord point(glMainWidget->width() - qMouseEv->x(), qMouseEv->y(), 0);
          _bends.push_back(glMainWidget->getScene()->getGraphCamera().screenTo3DWorld(point));
          glMainWidget->redraw();
        }
      }

      return true;
    }

    if (qMouseEv->buttons()==Qt::MidButton) {
      _bends.clear();
      _started=false;
      _source=node();
      clearObserver();
      glMainWidget->draw();
      return true;
    }
  }

  if  (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if (!_started) {
      SelectedEntity selectedEntity;
      bool hoveringOverNode = glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED;

      if (!hoveringOverNode) {
        glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
        return false;
      }
      else {
        glMainWidget->setCursor(QCursor(Qt::CrossCursor));
      }
    }
    else {
      SelectedEntity selectedEntity;

      if(glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        glMainWidget->setCursor(QCursor(Qt::CrossCursor));
      }
      else {
        glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
      }

      Coord point(glMainWidget->width() - qMouseEv->x(), qMouseEv->y(), 0);
      point = glMainWidget->getScene()->getGraphCamera().screenTo3DWorld(point);
      _curPos.set(point[0], point[1], point[2]);
      glMainWidget->redraw();
    }

    return true;
  }

  return false;
}

bool MouseEdgeBuilder::draw(GlMainWidget *glMainWidget) {
  if (!_started) return false;

  glDisable(GL_STENCIL_TEST);
  glMainWidget->getScene()->getGraphCamera().initGl();
  vector<Coord> lineVertices;
  lineVertices.push_back(_startPos);
  lineVertices.insert(lineVertices.end(), _bends.begin(), _bends.end());
  lineVertices.push_back(_curPos);
  vector<Color> lineColors;
  lineColors.resize(lineVertices.size(), Color(255,0,0,255));
  GlLine editedEdge(lineVertices, lineColors);
  editedEdge.draw(0,0);
  return true;
}

void MouseEdgeBuilder::initObserver(Graph *newGraph) {
  newGraph->addListener(this);
  _graph=newGraph;
  _layoutProperty=newGraph->getProperty<LayoutProperty>("viewLayout");
  _layoutProperty->addListener(this);
}

void MouseEdgeBuilder::clearObserver() {
  if(_graph)
    _graph->removeListener(this);

  _graph=NULL;

  if(_layoutProperty)
    _layoutProperty->removeListener(this);

  _layoutProperty=NULL;
}

void MouseEdgeBuilder::treatEvent(const Event& evt) {
  if (typeid(evt) == typeid(GraphEvent)) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

    if(graphEvent && graphEvent->getType() == GraphEvent::TLP_DEL_NODE && graphEvent->getNode() == _source) {
      _bends.clear();
      _started=false;
      clearObserver();
    }
  }
  else {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&evt);

    if(propertyEvent && propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE && propertyEvent->getNode() == _source && propertyEvent->getProperty() == _layoutProperty) {
      _startPos=_layoutProperty->getNodeValue(_source);
    }
  }
}

void MouseEdgeBuilder::clear() {
  GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}

void MouseEdgeBuilder::addLink(QObject *widget, const node source, const node target) {
  GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);
  Graph * g = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();

  LayoutProperty* mLayout = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getElementLayout();
  edge newEdge = g->addEdge(source, target);
  mLayout->setEdgeValue(newEdge, bends());
  _bends.clear();
}
