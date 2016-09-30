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
#include <tulip/BooleanProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlUtils.h>
#include <tulip/MouseSelector.h>
#include <tulip/GlGraph.h>
#include <tulip/Camera.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlLayer.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace std;
using namespace tlp;

//==================================================================
MouseSelector::MouseSelector(Qt::MouseButton button, Qt::KeyboardModifier modifier, SelectionMode mode)
    : mButton(button), kModifier(modifier), firstX(0), firstY(0), curX(0), curY(0), started(false), graph(nullptr), _mode(mode) {
}
//==================================================================
bool MouseSelector::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);
  GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);
  Graph *g = glMainWidget->getScene()->getMainGlGraph()->getGraph();

  if (e->type() == QEvent::MouseButtonPress) {

    if (qMouseEv->buttons() == mButton && (kModifier == Qt::NoModifier || qMouseEv->modifiers() & kModifier)) {
      if (!started) {
        firstX = curX = qMouseEv->x();
        firstY = curY = qMouseEv->y();
        started = true;
        graph = g;
        mousePressModifier = qMouseEv->modifiers();
      } else {
        if (g != graph) {
          graph = nullptr;
          started = false;
          return false;
        }
      }

      return true;
    }

    if (qMouseEv->buttons() == Qt::MidButton) {
      started = false;
      glMainWidget->redraw();
      return true;
    }
  }

  if (e->type() == QEvent::MouseMove && ((qMouseEv->buttons() & mButton) && (kModifier == Qt::NoModifier || qMouseEv->modifiers() & kModifier))) {

    if (g != graph) {
      graph = nullptr;
      started = false;
    }

    if (started) {
      curX = clamp(qMouseEv->x(), 0, glMainWidget->width());
      curY = clamp(qMouseEv->y(), 0, glMainWidget->height());

      glMainWidget->redraw();
      return true;
    }

    return false;
  }

  if (e->type() == QEvent::MouseButtonRelease) {

    if (g != graph) {
      graph = nullptr;
      started = false;
      return false;
    }

    if (started) {
      Observable::holdObservers();
      BooleanProperty *selection = glMainWidget->getScene()->getMainGlGraph()->getInputData().getElementSelection();
      bool revertSelection = false; // add to selection
      bool boolVal = true;
      bool needPush = true; // undo management

      if (mousePressModifier !=
#if defined(__APPLE__)
          Qt::AltModifier
#else
          Qt::ControlModifier
#endif
          ) {
        if (mousePressModifier == Qt::ShiftModifier && kModifier != Qt::ShiftModifier)
          boolVal = false;
        else {
          if (selection->getNodeDefaultValue() == true || selection->getEdgeDefaultValue() == true) {
            graph->push();
            selection->setAllNodeValue(false);
            selection->setAllEdgeValue(false);
            needPush = false;
          }

          Iterator<node> *itn = selection->getNonDefaultValuatedNodes();

          if (itn->hasNext()) {
            if (needPush) {
              graph->push();
              needPush = false;
            }

            delete itn;
            selection->setAllNodeValue(false);
          } else
            delete itn;

          Iterator<edge> *ite = selection->getNonDefaultValuatedEdges();

          if (ite->hasNext()) {
            if (needPush) {
              graph->push();
              needPush = false;
            }

            delete ite;
            selection->setAllEdgeValue(false);
          } else
            delete ite;
        }
      } else {
        boolVal = true;
      }

      if (abs(firstX - curX) == 0 || abs(firstY - curY) == 0) {
        SelectedEntity selectedEntity;
        bool result = glMainWidget->pickNodesEdges(curX, glMainWidget->height() - curY, selectedEntity);

        if (result) {
          switch (selectedEntity.getEntityType()) {
          case SelectedEntity::NODE_SELECTED:

            if (_mode == EdgesAndNodes || _mode == NodesOnly) {
              result = selection->getNodeValue(selectedEntity.getNode());

              if (revertSelection || boolVal != result) {
                if (needPush) {
                  graph->push();
                  needPush = false;
                }

                selection->setNodeValue(selectedEntity.getNode(), !result);
              }
            }

            break;

          case SelectedEntity::EDGE_SELECTED:

            if (_mode == EdgesAndNodes || _mode == EdgesOnly) {
              result = selection->getEdgeValue(selectedEntity.getEdge());

              if (revertSelection || boolVal != result) {
                if (needPush) {
                  graph->push();
                  needPush = false;
                }

                selection->setEdgeValue(selectedEntity.getEdge(), !result);
              }
            }

            break;

          default:
            break;
          }
        }
      } else {
        vector<SelectedEntity> tmpSetNode;
        vector<SelectedEntity> tmpSetEdge;

        int x = std::min(firstX, curX);
        int y = std::min(glMainWidget->height() - firstY, glMainWidget->height() - curY);
        int w = std::max(firstX, curX) - x;
        int h = std::max(glMainWidget->height() - firstY, glMainWidget->height() - curY) - y;
        glMainWidget->pickNodesEdges(x, y, w, h, tmpSetNode, tmpSetEdge);

        if (needPush)
          graph->push();

        vector<SelectedEntity>::const_iterator it;

        if (_mode == EdgesAndNodes || _mode == NodesOnly) {
          for (it = tmpSetNode.begin(); it != tmpSetNode.end(); ++it) {
            selection->setNodeValue((*it).getNode(), revertSelection ? !selection->getNodeValue((*it).getNode()) : boolVal);
          }
        }

        if (_mode == EdgesAndNodes || _mode == EdgesOnly) {
          for (it = tmpSetEdge.begin(); it != tmpSetEdge.end(); ++it) {
            selection->setEdgeValue((*it).getEdge(), revertSelection ? !selection->getEdgeValue((*it).getEdge()) : boolVal);
          }
        }
      }

      started = false;
      Observable::unholdObservers();
      firstX = curX = -1;
      firstY = curY = -1;
      glMainWidget->draw();
      return true;
    }
  }

  return false;
}
//==================================================================
bool MouseSelector::draw(GlMainWidget *glMainWidget) {
  if (!started)
    return false;

  if (glMainWidget->getScene()->getMainGlGraph()->getGraph() != graph) {
    graph = nullptr;
    started = false;
  }

  if (firstX != -1) {
    Color color(204, 204, 179, 100);

    if (mousePressModifier ==
#if defined(__APPLE__)
        Qt::AltModifier
#else
        Qt::ControlModifier
#endif
        ) {
      color[0] = 255;
      color[1] = 204;
      color[2] = 255;
    } else if (mousePressModifier == Qt::ShiftModifier) {
      color[0] = 255;
      color[1] = 179;
      color[2] = 179;
    }

    Color outlineColor(color);
    outlineColor[3] = 255;

    Camera *camera = glMainWidget->getScene()->getMainLayer()->getCamera();
    camera->initGl2D();
    Vec4i viewport = camera->getViewport();

    tlp::Vec2f bl(std::min(firstX, curX), std::min(viewport[3] - firstY, viewport[3] - curY));
    tlp::Vec2f tr(std::max(firstX, curX), std::max(viewport[3] - firstY, viewport[3] - curY));
    GlRect2D rect(bl, tr, color, outlineColor);
    rect.setOutlineWidth(2);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rect.draw(*camera);
    glDisable(GL_BLEND);

    camera->initGl();
  }

  return true;
}
