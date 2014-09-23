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

#include "MouseLassoNodesSelector.h"
#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/ViewNames.h"

#include <QMouseEvent>

#include <tulip/MouseInteractors.h>
#include <tulip/BooleanProperty.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlNode.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include <algorithm>

using namespace std;
using namespace tlp;

MouseLassoNodesSelectorInteractor::MouseLassoNodesSelectorInteractor(const tlp::PluginContext *):NodeLinkDiagramComponentInteractor(":/i_lasso.png", "Select nodes in a freehand drawn region") {
  setConfigurationWidgetText(QString("<h3>Select nodes in a freehand drawn region</h3>")+ "<b>Mouse left</b> down begins the freehand drawing of the selection region,<br/><b>Mouse left</b> up ends the drawing of the region.<br/>All the nodes enclosed in the region are selected and the edges linking them too.");
  setPriority(StandardInteractorPriority::FreeHandSelection);
}

bool MouseLassoNodesSelectorInteractor::isCompatible(const std::string &viewName) const {
  return ((viewName==NodeLinkDiagramComponent::viewName)
          ||(viewName==ViewName::ScatterPlot2DViewName)
         );
}


void MouseLassoNodesSelectorInteractor::construct() {
  push_back(new MouseLassoNodesSelectorInteractorComponent());
  push_back(new MousePanNZoomNavigator());
}

PLUGIN(MouseLassoNodesSelectorInteractor)

MouseLassoNodesSelectorInteractorComponent::MouseLassoNodesSelectorInteractorComponent() : drawInteractor(false), camera(NULL),dragStarted(false) {}

MouseLassoNodesSelectorInteractorComponent::~MouseLassoNodesSelectorInteractorComponent() {}

const float POINT_RADIUS = 3.;

// found on http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/
bool pointInsidePolygon(const vector<Coord> &polygon, const Coord &point) {
  unsigned int i, j;
  bool ret = false;

  for (i = 0, j = polygon.size() - 1 ; i < polygon.size() ; j = i++) {
    if ((((polygon[i].getY() <= point.getY()) && (point.getY() < polygon[j].getY())) ||
         ((polygon[j].getY() <= point.getY()) && (point.getY() < polygon[i].getY()))) &&
        (point.getX() < (polygon[j].getX() - polygon[i].getX()) * (point.getY() - polygon[i].getY()) / (polygon[j].getY() - polygon[i].getY()) + polygon[i].getX()))
      ret = !ret;
  }

  return ret;
}


bool isPolygonAincludesInB(const vector<Coord> &A, const vector<Coord> &B) {
  bool ret = true;

  for (unsigned int i = 0 ; i < A.size() ; ++i) {
    ret = ret && pointInsidePolygon(B, A[i]);

    if (!ret) break;
  }

  return ret;
}

void MouseLassoNodesSelectorInteractorComponent::selectGraphElementsUnderPolygon(GlMainWidget *glWidget) {
  vector<Coord> polygonScr;
  BoundingBox polygonScrBB;

  for (unsigned int i = 0 ; i < polygon.size() ; ++i) {
    polygonScr.push_back(polygon[i]);
    polygonScrBB.expand(polygonScr[i]);
  }

  polygonScr.push_back(polygon[0]);

  int xStart = (int) polygonScrBB[0][0];
  int yStart = (int) polygonScrBB[0][1];
  int xEnd = (int) polygonScrBB[1][0];
  int yEnd = (int) polygonScrBB[1][1];

  vector<SelectedEntity> tmpNodes;
  vector<SelectedEntity> tmpEdges;
  glWidget->pickNodesEdges(xStart, glWidget->height() - yEnd, (xEnd - xStart), (yEnd - yStart), tmpNodes, tmpEdges);

  if (!tmpNodes.empty()) {
    vector<node> selectedNodes;
    GlNode glNode(0);

    bool needPush = true;

    for (unsigned int i = 0 ; i < tmpNodes.size() ; ++i) {
      glNode.id = tmpNodes[i].getComplexEntityId();
      BoundingBox nodeBB(glNode.getBoundingBox(glWidget->getScene()->getGlGraphComposite()->getInputData()));
      float dx = nodeBB[1][0] - nodeBB[0][0];
      float dy = nodeBB[1][1] - nodeBB[0][1];
      float dz = nodeBB[1][2] - nodeBB[0][2];
      const float f = 0.2f;
      nodeBB[0][0] = nodeBB[0][0] + f * dx;
      nodeBB[0][1] = nodeBB[0][1] + f * dy;
      nodeBB[0][2] = nodeBB[0][2] + f * dz;
      nodeBB[1][0] = nodeBB[1][0] - f * dx;
      nodeBB[1][1] = nodeBB[1][1] - f * dy;
      nodeBB[1][2] = nodeBB[1][2] - f * dz;
      vector<float> xVec;
      vector<float> yVec;
      Coord nodeBBBLBScr(camera->worldTo2DScreen(Coord(nodeBB[0])));
      xVec.push_back(nodeBBBLBScr.getX());
      yVec.push_back(nodeBBBLBScr.getY());
      Coord nodeBBTLBScr(camera->worldTo2DScreen(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[0][2])));
      xVec.push_back(nodeBBTLBScr[0]);
      yVec.push_back(nodeBBTLBScr[1]);
      Coord nodeBBTRBScr(camera->worldTo2DScreen(Coord(nodeBB[1][0], nodeBB[1][1], nodeBB[0][2])));
      xVec.push_back(nodeBBTRBScr[0]);
      yVec.push_back(nodeBBTRBScr[1]);
      Coord nodeBBBRBScr(camera->worldTo2DScreen(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[0][2])));
      xVec.push_back(nodeBBBRBScr[0]);
      yVec.push_back(nodeBBBRBScr[1]);
      Coord nodeBBBLFScr(camera->worldTo2DScreen(Coord(nodeBB[0][0], nodeBB[0][1], nodeBB[1][2])));
      xVec.push_back(nodeBBBLFScr[0]);
      yVec.push_back(nodeBBBLFScr[1]);
      Coord nodeBBTLFScr(camera->worldTo2DScreen(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[1][2])));
      xVec.push_back(nodeBBTLFScr[0]);
      yVec.push_back(nodeBBTLFScr[1]);
      Coord nodeBBTRFScr(camera->worldTo2DScreen(Coord(nodeBB[1])));
      xVec.push_back(nodeBBTRFScr[0]);
      yVec.push_back(nodeBBTRFScr[1]);
      Coord nodeBBBRFScr(camera->worldTo2DScreen(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[1][2])));
      xVec.push_back(nodeBBBRFScr[0]);
      yVec.push_back(nodeBBBRFScr[1]);
      vector<Coord> quad;
      quad.push_back(Coord(*min_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*min_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*max_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*max_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(quad[0]);

      if (isPolygonAincludesInB(quad, polygonScr)) {
        if (needPush) {
          viewSelection->getGraph()->push();
          needPush = false;
        }

        viewSelection->setNodeValue(node(tmpNodes[i].getComplexEntityId()), true);
        selectedNodes.push_back(node(tmpNodes[i].getComplexEntityId()));
      }
    }

    for (unsigned int i = 0 ; i < selectedNodes.size() ; ++i) {
      for (unsigned int j = i+1 ; j < selectedNodes.size() ; ++j) {
        vector<edge> edges = graph->getEdges(selectedNodes[i], selectedNodes[j], false);

        for (size_t i = 0 ; i < edges.size() ; ++i) {
          viewSelection->setEdgeValue(edges[i], true);
        }
      }
    }

  }
}


bool MouseLassoNodesSelectorInteractorComponent::eventFilter(QObject *obj, QEvent *e) {
  GlMainWidget *glWidget=(GlMainWidget *)(obj);

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  QMouseEvent *me = dynamic_cast<QMouseEvent *>(e);

  if (!me) return false;

  camera = &glWidget->getScene()->getLayer("Main")->getCamera();
  graph = glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  viewSelection = graph->getProperty<BooleanProperty>("viewSelection");

  currentPointerScreenCoord = Coord(me->x(), glWidget->height() - me->y());

  if (me->type() == QEvent::MouseMove) {
    if (dragStarted) {
      polygon.push_back(currentPointerScreenCoord);
      drawInteractor = true;
      glWidget->redraw();

    }

    return true;
  }
  else if (me->type() == QEvent::MouseButtonPress) {
    if (me->button() == Qt::LeftButton) {
      dragStarted = true;
      polygon.push_back(currentPointerScreenCoord);
    }
    else if (me->button() == Qt::RightButton) {
      dragStarted = false;

      if (!polygon.empty()) {
        polygon.clear();
        drawInteractor = true;
        glWidget->redraw();
      }
      else {
        Observable::holdObservers();
        SelectedEntity selectedEntity;
        bool result = glWidget->pickNodesEdges(me->x(), me->y(),  selectedEntity);

        if (result && selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
          bool sel = viewSelection->getNodeValue(node(selectedEntity.getComplexEntityId()));
          viewSelection->setNodeValue(node(selectedEntity.getComplexEntityId()), !sel);
        }

        Observable::unholdObservers();
      }
    }

    return true;
  }
  else if (me->type() == QEvent::MouseButtonRelease) {
    dragStarted = false;

    if (me->button() == Qt::LeftButton && polygon.size() > 10) {
      Observable::holdObservers();

      if (me->modifiers() != Qt::ControlModifier) {
        viewSelection->setAllNodeValue(false);
        viewSelection->setAllEdgeValue(false);
      }

      selectGraphElementsUnderPolygon(glWidget);
      Observable::unholdObservers();

    }

    polygon.clear();

  }

  return false;
}

bool MouseLassoNodesSelectorInteractorComponent::draw(GlMainWidget *glWidget) {

  if (!drawInteractor) {
    return false;
  }

  if (!polygon.empty()) {

    Camera camera2D(camera->getScene(), false);

    Color backgroundColor = glWidget->getScene()->getBackgroundColor();
    Color foregroundColor;
    int bgV = backgroundColor.getV();

    if (bgV < 128) {
      foregroundColor = Color(255,255,255);
    }
    else {
      foregroundColor = Color(0,0,0);
    }

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera2D.initGl();
    GlComplexPolygon complexPolygon(polygon, Color(0,255,0,100), Color(0,255,0));
    complexPolygon.draw(0,0);
  }


  drawInteractor = false;

  return true;
}
