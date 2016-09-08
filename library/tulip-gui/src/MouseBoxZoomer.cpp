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
#include <tulip/GlMainWidget.h>
#include <tulip/GlUtils.h>
#include <tulip/DrawingTools.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlGraph.h>
#include <tulip/Camera.h>
#include <tulip/MouseBoxZoomer.h>
#include <tulip/GlLayer.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlUtils.h>

#if defined(_MSC_VER)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace std;
using namespace tlp;

MouseBoxZoomer::MouseBoxZoomer(Qt::MouseButton button,
                               Qt::KeyboardModifier modifier)
  : _mButton(button), _kModifier(modifier), _firstY(-1), _curX(-1), _curY(-1), _dragStarted(false), _graph(0) {}
MouseBoxZoomer::~MouseBoxZoomer() {}
//=====================================================================
bool MouseBoxZoomer::eventFilter(QObject *widget, QEvent *e) {
  GlMainWidget *glw = static_cast<GlMainWidget *>(widget);
  Camera *camera = glw->getScene()->getMainLayer()->getCamera();

  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons() == _mButton &&
        (_kModifier == Qt::NoModifier ||
         qMouseEv->modifiers() & _kModifier)) {
      if (!_dragStarted) {
        _firstX = _curX = qMouseEv->x();
        _firstY = _curY = qMouseEv->y();
        _dragStarted = true;
        _graph = glw->getScene()->getMainGlGraph()->getGraph();
      }
      else {
        if (glw->getScene()->getMainGlGraph()->getGraph() != _graph) {
          _graph = nullptr;
          _dragStarted = false;
        }
      }

      return true;
    }

    if (qMouseEv->buttons()==Qt::MidButton) {
      _dragStarted = false;
      glw->redraw();
      return true;
    }

    return false;
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if ((qMouseEv->buttons() & _mButton) &&
        (_kModifier == Qt::NoModifier ||
         qMouseEv->modifiers() & _kModifier)) {
      if (glw->getScene()->getMainGlGraph()->getGraph() != _graph) {
        _graph = nullptr;
        _dragStarted = false;
      }

      if (_dragStarted) {
        _curX = clamp(qMouseEv->x(), 0, glw->width());
        _curY = clamp(qMouseEv->y(), 0, glw->height());
        glw->redraw();
        return true;
      }
    }
  }

  if (e->type() == QEvent::MouseButtonDblClick) {
    GlBoundingBoxSceneVisitor bbVisitor;
    glw->getScene()->getMainLayer()->acceptVisitor(&bbVisitor);
    QtGlSceneZoomAndPanAnimator zoomAnPan(glw, bbVisitor.getBoundingBox());
    zoomAnPan.animateZoomAndPan();
    return true;
  }

  if (e->type() == QEvent::MouseButtonRelease) {

    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if ((qMouseEv->button() == _mButton &&
         (_kModifier == Qt::NoModifier ||
          qMouseEv->modifiers() & _kModifier))) {
      if (glw->getScene()->getMainGlGraph()->getGraph() != _graph) {
        _graph = nullptr;
        _dragStarted = false;
      }

      if (_dragStarted) {
        _dragStarted = false;
        tlp::Coord bbMin = camera->viewportTo3DWorld(glw->screenToViewport(tlp::Coord(glw->width() - _firstX, _firstY)));
        tlp::Coord bbMax = camera->viewportTo3DWorld(glw->screenToViewport(tlp::Coord(glw->width() - _curX, _curY)));
        tlp::BoundingBox bb;
        bb.expand(bbMin);
        bb.expand(bbMax);

        if (bb.width() > 1 && bb.height() > 1) {
          QtGlSceneZoomAndPanAnimator zoomAnPan(glw, bb);
          zoomAnPan.animateZoomAndPan();
        }
      }

      _firstX = _curX = -1;
      _firstY = _curY = -1;
      return true;
    }
  }

  return false;
}
//=====================================================================
bool MouseBoxZoomer::draw(GlMainWidget *glw) {
  if (!_dragStarted) return false;

  if (glw->getScene()->getMainGlGraph()->getGraph() != _graph) {
    _graph = nullptr;
    _dragStarted = false;
  }

  Camera *camera = glw->getScene()->getMainLayer()->getCamera();
  camera->initGl2D();
  tlp::Vec2f bl(glw->screenToViewport(std::min(_firstX, _curX)),
                glw->screenToViewport(std::min(glw->height() - _firstY, glw->height() - _curY)));
  tlp::Vec2f tr(glw->screenToViewport(std::max(_firstX, _curX)),
                glw->screenToViewport(std::max(glw->height() - _firstY, glw->height() - _curY)));
  GlRect2D rect(bl, tr, tlp::Color(0,0,255,100), tlp::Color::Black);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  rect.draw(*camera);
  glDisable(GL_BLEND);
  camera->initGl();

  return true;
}

//=====================================================================
