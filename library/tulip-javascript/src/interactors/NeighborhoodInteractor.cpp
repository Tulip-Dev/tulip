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

#include "NeighborhoodInteractor.h"
#include "ZoomAndPanInteractor.h"

#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraph.h>
#include <tulip/ConcaveHullBuilder.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlCircle.h>
#include <tulip/ZoomAndPanAnimation.h>

#include "../TimeMeasurer.h"

using namespace std;
using namespace tlp;

class GraphLayoutMorphing : public AdditionalGlSceneAnimation {

public:
  virtual ~GraphLayoutMorphing() {
  }
  GraphLayoutMorphing(Graph *graph, LayoutProperty *srcLayout, LayoutProperty *destLayout, LayoutProperty *viewLayout)
      : graph(graph), srcLayout(srcLayout), destLayout(destLayout), viewLayout(viewLayout) {
  }

  void animationStep(double t) {
    Observable::holdObservers();
    for (node n : graph->getNodes()) {
      const Coord &startPos = srcLayout->getNodeValue(n);
      const Coord &endPos = destLayout->getNodeValue(n);
      viewLayout->setNodeValue(n, startPos + static_cast<float>(t) * (endPos - startPos));
    }

    for (edge e : graph->getEdges()) {
      const vector<Coord> &srcBends = srcLayout->getEdgeValue(e);
      const vector<Coord> &destBends = destLayout->getEdgeValue(e);
      vector<Coord> newBends;

      for (size_t i = 0; i < destBends.size(); ++i) {
        newBends.push_back(srcBends[i] + static_cast<float>(t) * (destBends[i] - srcBends[i]));
      }

      viewLayout->setEdgeValue(e, newBends);
    }
    Observable::unholdObservers();
  }

private:
  Graph *graph;
  LayoutProperty *srcLayout, *destLayout, *viewLayout;
};

static ZoomAndPanAnimation *zoomAndPanAnimation = nullptr;
static GraphLayoutMorphing *layoutMorphing = nullptr;
static const unsigned int baseAnimDuration = 1000;
static bool animating = false;
static double animDuration = 0;
static TimeMeasurer tm;
static const unsigned int delay = 40;

void animate(void *value) {
  NeighborhoodInteractor *interactor = reinterpret_cast<NeighborhoodInteractor *>(value);
  GlScene *scene = interactor->getScene();
  double t = tm.getElapsedTime() / animDuration;
  if (t < 1.0) {
    zoomAndPanAnimation->zoomAndPanAnimationStep(t);
    scene->setBackupBackBuffer(false);
    scene->forceRedraw();
    scene->requestDraw();
    timerFunc(delay, animate, value);
  } else {

    zoomAndPanAnimation->zoomAndPanAnimationStep(1.0);
    delete zoomAndPanAnimation;
    delete layoutMorphing;
    zoomAndPanAnimation = nullptr;
    layoutMorphing = nullptr;

    if (interactor->_bringAndGoAnimation) {
      interactor->buildNeighborhoodGraph(interactor->_selectedNeighborNode);

      layoutMorphing = new GraphLayoutMorphing(interactor->_neighborhoodGraph, interactor->_neighborhoodGraphOriginalLayout,
                                               interactor->_neighborhoodGraphCircleLayout, interactor->_neighborhoodGraphLayout);
      interactor->_bringAndGoAnimation = false;
      interactor->_circleLayoutSet = false;
      interactor->performZoomAndPan(interactor->_neighborhoodGraphCircleLayoutBB, layoutMorphing);
    } else {
      interactor->_circleLayoutSet = !interactor->_circleLayoutSet;
      animating = false;
      scene->setBackupBackBuffer(true);
      scene->forceRedraw();
      scene->requestDraw();
    }
  }
}

class NeighborNodesEdgeLengthOrdering : public binary_function<node, node, bool> {

public:
  NeighborNodesEdgeLengthOrdering(node centralNode, LayoutProperty *layout) : centralNode(centralNode), layout(layout) {
  }

  bool operator()(node n1, node n2) const {
    Coord centralNodeCoord = layout->getNodeValue(centralNode);
    Coord n1Coord = layout->getNodeValue(n1);
    Coord n2Coord = layout->getNodeValue(n2);
    float centralToN1Dist = centralNodeCoord.dist(n1Coord);
    float centralToN2Dist = centralNodeCoord.dist(n2Coord);
    return centralToN1Dist < centralToN2Dist;
  }

private:
  node centralNode;
  LayoutProperty *layout;
};

NeighborhoodInteractor::NeighborhoodInteractor(GlScene *scene)
    : _glGraph(nullptr), _neighborhoodGraph(nullptr), _glNeighborhoodGraph(nullptr), _neighborhoodGraphColors(nullptr),
      _neighborhoodGraphBackupColors(nullptr) {
  _glScene = scene;
  _znpInteractor = new ZoomAndPanInteractor(scene);
}

void NeighborhoodInteractor::desactivate() {
  destroyNeighborhoodGraph();
}

void NeighborhoodInteractor::setScene(GlScene *glScene) {
  GlSceneInteractor::setScene(glScene);
  _znpInteractor->setScene(glScene);
}

bool NeighborhoodInteractor::mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers) {
  if (!_glScene || animating)
    return false;
  Vec4i viewport = _glScene->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3])
    return false;
  if (button == LEFT_BUTTON && state == UP) {
    if (!_selectedNeighborNode.isValid()) {
      _circleLayoutSet = false;
      SelectedEntity selectedEntity;
      _glScene->selectEntity(RenderingNodes, x, viewport[3] - y, selectedEntity);
      if (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        _glGraph = selectedEntity.getGlGraph();
        buildNeighborhoodGraph(selectedEntity.getNode());

        _glScene->requestDraw();
      } else {
        destroyNeighborhoodGraph();
        _glScene->requestDraw();
      }
    } else {
      if (_selectedNeighborNode == _centralNode) {
        if (!_circleLayoutSet) {
          layoutMorphing =
              new GraphLayoutMorphing(_neighborhoodGraph, _neighborhoodGraphOriginalLayout, _neighborhoodGraphCircleLayout, _neighborhoodGraphLayout);
          performZoomAndPan(_neighborhoodGraphCircleLayoutBB, layoutMorphing);
        } else {
          layoutMorphing =
              new GraphLayoutMorphing(_neighborhoodGraph, _neighborhoodGraphCircleLayout, _neighborhoodGraphOriginalLayout, _neighborhoodGraphLayout);
          performZoomAndPan(_neighborhoodGraphOriginalLayoutBB, layoutMorphing);
        }
      } else {
        if (!_circleLayoutSet) {
          buildNeighborhoodGraph(_selectedNeighborNode);
          _selectedNeighborNode = node();
          _glScene->requestDraw();
        } else {
          node currentCentralNode = _centralNode;
          buildNeighborhoodGraph(_selectedNeighborNode);
          BoundingBox destBB = _neighborhoodGraphCircleLayoutBB;

          buildNeighborhoodGraph(currentCentralNode);

          Observable::holdObservers();
          *_neighborhoodGraphLayout = *_neighborhoodGraphCircleLayout;
          Observable::unholdObservers();
          layoutMorphing =
              new GraphLayoutMorphing(_neighborhoodGraph, _neighborhoodGraphCircleLayout, _neighborhoodGraphOriginalLayout, _neighborhoodGraphLayout);
          _bringAndGoAnimation = true;
          performZoomAndPan(destBB, layoutMorphing);
        }
      }
    }
    return _znpInteractor->mouseCallback(button, state, x, y, modifiers);
  } else {
    return _znpInteractor->mouseCallback(button, state, x, y, modifiers);
  }
}

bool NeighborhoodInteractor::mouseMoveCallback(int x, int y, const int &modifiers) {
  if (!_glScene || animating)
    return false;
  if (!_centralNode.isValid()) {
    return _znpInteractor->mouseMoveCallback(x, y, modifiers);
  } else {
    Observable::holdObservers();
    *_neighborhoodGraphColors = *_neighborhoodGraphBackupColors;
    _selectedNeighborNode = selectNodeInAugmentedDisplayGraph(x, y);
    if (_selectedNeighborNode.isValid()) {
      if (_selectedNeighborNode != _centralNode) {
        _neighborhoodGraphColors->setNodeValue(_selectedNeighborNode, Color(0, 255, 0));
      } else {
        _neighborhoodGraphColors->setNodeValue(_selectedNeighborNode, Color(0, 0, 255));
      }
    }
    Observable::unholdObservers();
    _glScene->requestDraw();
    return true;
  }
}

void NeighborhoodInteractor::destroyNeighborhoodGraph() {
  if (_neighborhoodGraph) {
    delete _glNeighborhoodGraph;
    delete _neighborhoodGraphBackupColors;
    delete _neighborhoodGraphColors;
    delete _neighborhoodGraphLayout;
    delete _neighborhoodGraphCircleLayout;
    delete _neighborhoodGraphOriginalLayout;
    _glGraph->getGraph()->delSubGraph(_neighborhoodGraph);
    _neighborhoodGraph = nullptr;
    _neighborhoodGraphBackupColors = nullptr;
    _neighborhoodGraphColors = nullptr;
    _neighborhoodGraphLayout = nullptr;
    _neighborhoodGraphCircleLayout = nullptr;
    _neighborhoodGraphOriginalLayout = nullptr;
    _glNeighborhoodGraph = nullptr;
    _centralNode = node();
  }
}

void NeighborhoodInteractor::buildNeighborhoodGraph(node centralNode) {
  destroyNeighborhoodGraph();
  if (_glGraph) {
    _neighborhoodGraph = _glGraph->getGraph()->addSubGraph();
    _neighborhoodGraph->addNode(centralNode);
    node neigh;
    forEach(neigh, _glGraph->getGraph()->getInOutNodes(centralNode)) {
      _neighborhoodGraph->addNode(neigh);
      _neighborhoodGraph->addEdges(_glGraph->getGraph()->getEdges(centralNode, neigh, false));
    }
    _glNeighborhoodGraph = new GlGraph(_neighborhoodGraph, new GlCPULODCalculator);

    _neighborhoodGraphColors = new ColorProperty(_neighborhoodGraph);
    _neighborhoodGraphBackupColors = new ColorProperty(_neighborhoodGraph);
    *_neighborhoodGraphBackupColors = *(_glNeighborhoodGraph->getInputData().getElementColor());
    *_neighborhoodGraphColors = *_neighborhoodGraphBackupColors;
    _glNeighborhoodGraph->getInputData().setElementColor(_neighborhoodGraphColors);

    _neighborhoodGraphLayout = new LayoutProperty(_neighborhoodGraph);
    _neighborhoodGraphCircleLayout = new LayoutProperty(_neighborhoodGraph);
    _neighborhoodGraphOriginalLayout = new LayoutProperty(_neighborhoodGraph);

    *_neighborhoodGraphOriginalLayout = *(_glNeighborhoodGraph->getInputData().getElementLayout());
    *_neighborhoodGraphLayout = *_neighborhoodGraphOriginalLayout;
    _glNeighborhoodGraph->getInputData().setElementLayout(_neighborhoodGraphLayout);

    _circleCenter = _neighborhoodGraphLayout->getNodeValue(centralNode);
    _centralNode = centralNode;

    computeNeighborhoodGraphCircleLayout();
    computeNeighborhoodGraphBoundingBoxes();
  }
}

node NeighborhoodInteractor::selectNodeInAugmentedDisplayGraph(int x, int y) {
  node n;
  edge e;
  Vec4i viewport = _glScene->getViewport();
  _glNeighborhoodGraph->pickNodeOrEdge(*(_glScene->getMainLayer()->getCamera()), x, viewport[3] - y, n, e);
  return n;
}

void NeighborhoodInteractor::computeNeighborhoodGraphCircleLayout() {

  Size centralNodeSize = _glGraph->getInputData().getElementSize()->getNodeValue(_centralNode);
  Coord centralNodeCoord = _neighborhoodGraphLayout->getNodeValue(_centralNode);
  _neighborhoodGraphCircleLayout->setNodeValue(_centralNode, centralNodeCoord);

  std::vector<node> neighborsNodes;
  for (node n : _neighborhoodGraph->getNodes()) {
    if (n != _centralNode) {
      neighborsNodes.push_back(n);
    }
  }

  sort(neighborsNodes.begin(), neighborsNodes.end(), NeighborNodesEdgeLengthOrdering(_centralNode, _neighborhoodGraphLayout));

  BoundingBox centralNodeBB(Coord(centralNodeCoord.getX() - centralNodeSize.getW() / 2, centralNodeCoord.getY() - centralNodeSize.getH() / 2, 0),
                            Coord(centralNodeCoord.getX() + centralNodeSize.getW() / 2, centralNodeCoord.getY() + centralNodeSize.getH() / 2, 0));
  vector<BoundingBox> neighborhoodNodesNewLayoutBB;
  neighborhoodNodesNewLayoutBB.resize(neighborsNodes.size());

  for (size_t i = 0; i < neighborsNodes.size(); ++i) {

    Size neighborNodeSize = _glGraph->getInputData().getElementSize()->getNodeValue(neighborsNodes[i]);
    Coord neighborNodeCoord = _neighborhoodGraphLayout->getNodeValue(neighborsNodes[i]);
    float edgeLength = centralNodeCoord.dist(neighborNodeCoord);

    bool nodePosOk = false;
    unsigned int j = 1;

    while (!nodePosOk) {
      float ratio = ((centralNodeSize.getW() / 2) + j++ * neighborNodeSize.getW()) / edgeLength;
      Coord newNodeCoord = centralNodeCoord + ratio * (neighborNodeCoord - centralNodeCoord);
      neighborhoodNodesNewLayoutBB[i] =
          BoundingBox(Coord(newNodeCoord.getX() - neighborNodeSize.getW() / 2, newNodeCoord.getY() - neighborNodeSize.getH() / 2, 0),
                      Coord(newNodeCoord.getX() + neighborNodeSize.getW() / 2, newNodeCoord.getY() + neighborNodeSize.getH() / 2, 0));
      nodePosOk = true;

      if (i > 0) {
        for (unsigned int k = 0; k < i; ++k) {
          nodePosOk = nodePosOk && !neighborhoodNodesNewLayoutBB[i].intersects(neighborhoodNodesNewLayoutBB[k]) &&
                      !neighborhoodNodesNewLayoutBB[i].intersects(centralNodeBB);
        }
      }

      if (nodePosOk) {
        _neighborhoodGraphCircleLayout->setNodeValue(neighborsNodes[i], newNodeCoord);
      }
    }
  }

  for (edge e : _neighborhoodGraph->getEdges()) {
    const std::pair<node, node> &eEnds = _neighborhoodGraph->ends(e);
    node srcNode = eEnds.first;
    node tgtNode = eEnds.second;
    Coord finalBendsCoord;

    if (srcNode != _centralNode) {
      Coord srcNodeCoord = _neighborhoodGraphCircleLayout->getNodeValue(srcNode);
      finalBendsCoord = _circleCenter + (srcNodeCoord - _circleCenter) / 2.f;
    } else {
      Coord tgtNodeCoord = _neighborhoodGraphCircleLayout->getNodeValue(tgtNode);
      finalBendsCoord = _circleCenter + (tgtNodeCoord - _circleCenter) / 2.f;
    }

    vector<Coord> edgesBends = _neighborhoodGraphLayout->getEdgeValue(e);

    for (size_t i = 0; i < edgesBends.size(); ++i) {
      edgesBends[i] = finalBendsCoord;
    }

    _neighborhoodGraphCircleLayout->setEdgeValue(e, edgesBends);
  }
}

float NeighborhoodInteractor::computeNeighborhoodGraphRadius(LayoutProperty *neighborhoodGraphLayoutProp) {
  float radius = 0;
  Coord centralNodeCoord = neighborhoodGraphLayoutProp->getNodeValue(_centralNode);
  for (node n : _neighborhoodGraph->getNodes()) {
    Coord nodeCoord = neighborhoodGraphLayoutProp->getNodeValue(n);
    Size nodeSize = _glGraph->getInputData().getElementSize()->getNodeValue(n);
    float dist = centralNodeCoord.dist(nodeCoord) + nodeSize.getW();

    if (dist > radius) {
      radius = dist;
    }
  }
  return radius;
}

void NeighborhoodInteractor::computeNeighborhoodGraphBoundingBoxes() {
  Coord centralNodeCoord = _neighborhoodGraphOriginalLayout->getNodeValue(_centralNode);
  float originalLayoutRadius = computeNeighborhoodGraphRadius(_neighborhoodGraphOriginalLayout);
  float circleLayoutRadius = computeNeighborhoodGraphRadius(_neighborhoodGraphCircleLayout);
  _neighborhoodGraphOriginalLayoutBB[0] = centralNodeCoord - Coord(originalLayoutRadius, originalLayoutRadius);
  _neighborhoodGraphOriginalLayoutBB[1] = centralNodeCoord + Coord(originalLayoutRadius, originalLayoutRadius);
  _neighborhoodGraphCircleLayoutBB[0] = centralNodeCoord - Coord(circleLayoutRadius, circleLayoutRadius);
  _neighborhoodGraphCircleLayoutBB[1] = centralNodeCoord + Coord(circleLayoutRadius, circleLayoutRadius);
}

void NeighborhoodInteractor::performZoomAndPan(const BoundingBox &destBB, AdditionalGlSceneAnimation *additionalAnimation) {
  zoomAndPanAnimation = new ZoomAndPanAnimation(_glScene->getMainLayer()->getCamera(), destBB, baseAnimDuration);
  animDuration = zoomAndPanAnimation->getAnimationDuration();
  zoomAndPanAnimation->setAdditionalGlSceneAnimation(additionalAnimation);
  tm.reset();
  animating = true;
  timerFunc(delay, animate, this);
}

void NeighborhoodInteractor::draw() {
  if (!_glScene)
    return;
  if (_neighborhoodGraph) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Camera *camera = _glScene->getMainLayer()->getCamera();
    camera->initGl();
    GlCircle circle(_circleCenter, computeNeighborhoodGraphRadius(_neighborhoodGraphLayout), Color(255, 255, 255, 200), Color(0, 0, 0));
    glDisable(GL_DEPTH_TEST);
    circle.draw(*camera);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    _glNeighborhoodGraph->setRenderingParameters(_glGraph->getRenderingParameters());
    _glNeighborhoodGraph->draw(*camera, *(_glScene->getMainLayer()->getLight()));
  }
}
