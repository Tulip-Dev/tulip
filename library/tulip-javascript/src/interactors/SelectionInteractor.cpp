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

#include <tulip/BoundingBox.h>
#include <tulip/Coord.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipViewSettings.h>

#include <tulip/GlScene.h>
#include <tulip/GlGraph.h>
#include <tulip/Camera.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlLayer.h>
#include <tulip/GlUtils.h>
#include <tulip/GlyphsRenderer.h>

#include "SelectionInteractor.h"
#include "ZoomAndPanInteractor.h"

using namespace std;
using namespace tlp;

SelectionInteractor::SelectionInteractor(GlScene *scene)
    : _firstX(-1), _firstY(-1), _curX(-1), _curY(-1), _dragStarted(false), _znpInteractor(NULL), _selectOnlyEdgesConnectedToSelectedNodes(false) {
  _glScene = scene;
  _znpInteractor = new ZoomAndPanInteractor(scene);
}

void SelectionInteractor::setScene(GlScene *glScene) {
  GlSceneInteractor::setScene(glScene);
  _znpInteractor->setScene(glScene);
}

bool SelectionInteractor::mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers) {
  if (!_glScene)
    return false;
  tlp::Vec4i viewport = _glScene->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3])
    return false;
  _mouseButton = button;
  if (button == LEFT_BUTTON) {
    if (state == DOWN) {
      _firstX = _curX = x;
      _firstY = _curY = y;
      _dragStarted = true;
      return true;
    } else if (state == UP && _dragStarted) {
      _dragStarted = false;
      tlp::Observable::holdObservers();

      set<GlEntity *> glEntities = _glScene->getEntities();
      for (set<GlEntity *>::iterator it = glEntities.begin(); it != glEntities.end(); ++it) {
        GlGraph *glGraph = dynamic_cast<GlGraph *>(*it);
        if (glGraph) {
          BooleanProperty *viewSelection = glGraph->getGraph()->getProperty<BooleanProperty>("viewSelection");
          if (!modifiers) {
            viewSelection->setAllNodeValue(false);
            viewSelection->setAllEdgeValue(false);
          }
        }
      }

      if (abs(_firstX - _curX) == 0 || abs(_firstY - _curY) == 0) {
        _glScene->selectEntity(RenderingNodesEdges, _firstX, viewport[3] - _firstY, _selectedEntity);
        if (_selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
          BooleanProperty *viewSelection = _selectedEntity.getGlGraph()->getGraph()->getProperty<BooleanProperty>("viewSelection");
          if (modifiers & ACTIVE_SHIFT) {
            viewSelection->setNodeValue(_selectedEntity.getNode(), false);
          } else {
            Graph *graph = _selectedEntity.getGlGraph()->getGraph();
            viewSelection->setNodeValue(_selectedEntity.getNode(), true);
            if (_selectOnlyEdgesConnectedToSelectedNodes) {
              tlp::edge e;
              forEach(e, graph->getInOutEdges(_selectedEntity.getNode())) {
                viewSelection->setEdgeValue(e, true);
              }
            }
          }
        }
        if (_selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {
          BooleanProperty *viewSelection = _selectedEntity.getGlGraph()->getGraph()->getProperty<BooleanProperty>("viewSelection");
          if (modifiers & ACTIVE_SHIFT) {
            viewSelection->setEdgeValue(_selectedEntity.getEdge(), false);
          } else {
            viewSelection->setEdgeValue(_selectedEntity.getEdge(), true);
          }
        }
      } else {
        int x = std::min(_firstX, _curX);
        int y = std::min(viewport[3] - _firstY, viewport[3] - _curY);
        int width = std::max(_firstX, _curX) - x;
        int height = std::max(viewport[3] - _firstY, viewport[3] - _curY) - y;
        _glScene->selectEntities(RenderingNodesEdges, x, y, width, height, _selectedEntities);
        for (size_t i = 0; i < _selectedEntities.size(); ++i) {
          if (_selectedEntities[i].getEntityType() == SelectedEntity::NODE_SELECTED) {
            BooleanProperty *viewSelection = _selectedEntities[i].getGlGraph()->getGraph()->getProperty<BooleanProperty>("viewSelection");
            if (modifiers & ACTIVE_SHIFT) {
              viewSelection->setNodeValue(_selectedEntities[i].getNode(), false);
            } else {
              viewSelection->setNodeValue(_selectedEntities[i].getNode(), true);
            }
          }
        }
        for (size_t i = 0; i < _selectedEntities.size(); ++i) {
          if (_selectedEntities[i].getEntityType() == SelectedEntity::EDGE_SELECTED) {
            BooleanProperty *viewSelection = _selectedEntities[i].getGlGraph()->getGraph()->getProperty<BooleanProperty>("viewSelection");
            if (modifiers & ACTIVE_SHIFT) {
              viewSelection->setEdgeValue(_selectedEntities[i].getEdge(), false);
            } else {
              Graph *graph = _selectedEntities[i].getGlGraph()->getGraph();
              if (_selectOnlyEdgesConnectedToSelectedNodes) {
                if (viewSelection->getNodeValue(graph->source(_selectedEntities[i].getEdge())) ||
                    viewSelection->getNodeValue(graph->target(_selectedEntities[i].getEdge()))) {
                  viewSelection->setEdgeValue(_selectedEntities[i].getEdge(), true);
                }
              } else {
                viewSelection->setEdgeValue(_selectedEntities[i].getEdge(), true);
              }
            }
          }
        }
      }
      tlp::Observable::unholdObservers();
      _firstX = _curX = -1;
      _firstY = _curY = -1;
      _glScene->requestDraw();
      return true;
    }
  } else if (button == WHEEL) {
    return _znpInteractor->mouseCallback(button, state, x, y, modifiers);
  }
  return false;
}

bool SelectionInteractor::mouseMoveCallback(int x, int y, const int & /* modifiers */) {
  if (!_glScene)
    return false;
  tlp::Vec4i viewport = _glScene->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3])
    return false;
  if (_mouseButton == LEFT_BUTTON && _dragStarted) {
    _curX = x;
    _curY = y;
    _glScene->requestDraw();
    return true;
  }
  return false;
}

void SelectionInteractor::draw() {
  if (_firstX != -1) {
    Camera *camera = _glScene->getMainLayer()->getCamera();
    camera->initGl2D();
    Vec4i viewport = camera->getViewport();
    tlp::Vec2f bl(std::min(_firstX, _curX), std::min(viewport[3] - _firstY, viewport[3] - _curY));
    tlp::Vec2f tr(std::max(_firstX, _curX), std::max(viewport[3] - _firstY, viewport[3] - _curY));
    GlRect2D rect(bl, tr, tlp::Color(255, 0, 0, 100), tlp::Color::Black);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rect.draw(*camera);
    glDisable(GL_BLEND);
    camera->initGl();
  }
}
