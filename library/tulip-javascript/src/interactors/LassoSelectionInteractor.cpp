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

/**
 *
 * tulip.js : port of the Tulip framework to JavaScript through emscripten
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include "LassoSelectionInteractor.h"
#include "ZoomAndPanInteractor.h"

#include <tulip/GlConcavePolygon.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraph.h>

#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;

LassoSelectionInteractor::LassoSelectionInteractor(GlScene *scene) : _dragStarted(false) {
  _glScene = scene;
  _znpInteractor = new ZoomAndPanInteractor(scene);
}

void LassoSelectionInteractor::setScene(GlScene *glScene) {
  GlSceneInteractor::setScene(glScene);
  _znpInteractor->setScene(glScene);
}

bool LassoSelectionInteractor::mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers) {
  Vec4i viewport = _glScene->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3])
    return false;
  Coord screenCoord(x, viewport[3] - y);
  if (button == LEFT_BUTTON && state == DOWN) {
    if (!_dragStarted) {
      _dragStarted = true;
      _polygon.push_back(screenCoord);
      _glScene->requestDraw();
      return true;
    }
  } else if (button == LEFT_BUTTON && state == UP) {
    Observable::holdObservers();
    selectGraphElementsUnderPolygon();
    Observable::unholdObservers();
    _dragStarted = false;
    _polygon.clear();
    _glScene->requestDraw();
    return true;
  } else if (button == WHEEL) {
    return _znpInteractor->mouseCallback(button, state, x, y, modifiers);
  }
  return false;
}

bool LassoSelectionInteractor::mouseMoveCallback(int x, int y, const int & /* modifiers */) {
  Vec4i viewport = _glScene->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3])
    return false;
  Coord screenCoord(x, viewport[3] - y);
  if (_dragStarted) {
    _polygon.push_back(screenCoord);
    _glScene->requestDraw();
    return true;
  }
  return false;
}

static BoundingBox getNodeBoundingBox(Graph *graph, node n) {
  LayoutProperty *viewLayout = graph->getProperty<LayoutProperty>("viewLayout");
  SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");
  const Coord &nodePos = viewLayout->getNodeValue(n);
  const Size &nodeSize = viewSize->getNodeValue(n);
  return BoundingBox(nodePos - nodeSize / 2.f, nodePos + nodeSize / 2.f);
}

static bool pointInsidePolygon(const vector<Coord> &polygon, const Coord &point) {
  unsigned int i, j;
  bool ret = false;

  for (i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
    if ((((polygon[i].getY() <= point.getY()) && (point.getY() < polygon[j].getY())) ||
         ((polygon[j].getY() <= point.getY()) && (point.getY() < polygon[i].getY()))) &&
        (point.getX() <
         (polygon[j].getX() - polygon[i].getX()) * (point.getY() - polygon[i].getY()) / (polygon[j].getY() - polygon[i].getY()) + polygon[i].getX()))
      ret = !ret;
  }

  return ret;
}

static bool isPolygonAincludesInB(const vector<Coord> &A, const vector<Coord> &B) {
  bool ret = true;

  for (unsigned int i = 0; i < A.size(); ++i) {
    ret = ret && pointInsidePolygon(B, A[i]);

    if (!ret)
      break;
  }

  return ret;
}

void LassoSelectionInteractor::selectGraphElementsUnderPolygon() {

  vector<Coord> polygonScr;
  BoundingBox polygonScrBB;

  for (size_t i = 0; i < _polygon.size(); ++i) {
    polygonScr.push_back(_polygon[i]);
    polygonScrBB.expand(polygonScr[i]);
  }

  polygonScr.push_back(_polygon[0]);

  int xStart = static_cast<int>(polygonScrBB[0][0]);
  int yStart = static_cast<int>(polygonScrBB[0][1]);
  int xEnd = static_cast<int>(polygonScrBB[1][0]);
  int yEnd = static_cast<int>(polygonScrBB[1][1]);

  vector<SelectedEntity> selectedEntities;

  if (!polygonScrBB.isValid()) {
    return;
  }

  set<GlEntity *> glEntities = _glScene->getEntities();
  for (set<GlEntity *>::iterator it = glEntities.begin(); it != glEntities.end(); ++it) {
    GlGraph *glGraph = dynamic_cast<GlGraph *>(*it);
    if (glGraph) {
      BooleanProperty *viewSelection = glGraph->getGraph()->getProperty<BooleanProperty>("viewSelection");
      viewSelection->setAllNodeValue(false);
      viewSelection->setAllEdgeValue(false);
    }
  }

  _glScene->selectEntities(RenderingNodes, xStart, yStart, (xEnd - xStart), (yEnd - yStart), selectedEntities);

  Camera *camera = _glScene->getMainLayer()->getCamera();

  if (!selectedEntities.empty()) {

    map<Graph *, vector<node>> selectedNodes;

    for (size_t i = 0; i < selectedEntities.size(); ++i) {

      node n = selectedEntities[i].getNode();
      tlp::Graph *graph = selectedEntities[i].getGlGraph()->getGraph();
      tlp::BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");

      BoundingBox nodeBB = getNodeBoundingBox(graph, n);
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
      Coord nodeBBBLBScr(camera->worldTo2DViewport(Coord(nodeBB[0])));
      xVec.push_back(nodeBBBLBScr.getX());
      yVec.push_back(nodeBBBLBScr.getY());
      Coord nodeBBTLBScr(camera->worldTo2DViewport(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[0][2])));
      xVec.push_back(nodeBBTLBScr[0]);
      yVec.push_back(nodeBBTLBScr[1]);
      Coord nodeBBTRBScr(camera->worldTo2DViewport(Coord(nodeBB[1][0], nodeBB[1][1], nodeBB[0][2])));
      xVec.push_back(nodeBBTRBScr[0]);
      yVec.push_back(nodeBBTRBScr[1]);
      Coord nodeBBBRBScr(camera->worldTo2DViewport(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[0][2])));
      xVec.push_back(nodeBBBRBScr[0]);
      yVec.push_back(nodeBBBRBScr[1]);
      Coord nodeBBBLFScr(camera->worldTo2DViewport(Coord(nodeBB[0][0], nodeBB[0][1], nodeBB[1][2])));
      xVec.push_back(nodeBBBLFScr[0]);
      yVec.push_back(nodeBBBLFScr[1]);
      Coord nodeBBTLFScr(camera->worldTo2DViewport(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[1][2])));
      xVec.push_back(nodeBBTLFScr[0]);
      yVec.push_back(nodeBBTLFScr[1]);
      Coord nodeBBTRFScr(camera->worldTo2DViewport(Coord(nodeBB[1])));
      xVec.push_back(nodeBBTRFScr[0]);
      yVec.push_back(nodeBBTRFScr[1]);
      Coord nodeBBBRFScr(camera->worldTo2DViewport(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[1][2])));
      xVec.push_back(nodeBBBRFScr[0]);
      yVec.push_back(nodeBBBRFScr[1]);
      vector<Coord> quad;
      quad.push_back(Coord(*min_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*min_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*max_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*max_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(quad[0]);

      if (isPolygonAincludesInB(quad, polygonScr)) {
        viewSelection->setNodeValue(n, true);
        selectedNodes[graph].push_back(n);
      }
    }

    for (map<Graph *, vector<node>>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); ++it) {
      tlp::BooleanProperty *viewSelection = it->first->getProperty<BooleanProperty>("viewSelection");
      for (size_t i = 0; i < it->second.size(); ++i) {
        for (size_t j = i + 1; j < it->second.size(); ++j) {
          vector<edge> edges = it->first->getEdges(it->second[i], it->second[j], false);
          for (size_t i = 0; i < edges.size(); ++i) {
            viewSelection->setEdgeValue(edges[i], true);
          }
        }
      }
    }
  }
}

void LassoSelectionInteractor::draw() {
  if (_polygon.empty())
    return;
  Camera *camera = _glScene->getMainLayer()->getCamera();
  camera->initGl2D();
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Color polygonColor = Color::Green;
  Color polygonOutlineColor = polygonColor;
  polygonColor[3] = 100;
  GlConcavePolygon glPolygon(_polygon, polygonColor, polygonOutlineColor);
  glPolygon.draw(*camera);
  glDisable(GL_BLEND);
  camera->initGl();
}
