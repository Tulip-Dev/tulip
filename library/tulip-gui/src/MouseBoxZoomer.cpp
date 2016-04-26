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

MouseBoxZoomer::MouseBoxZoomer(Qt::MouseButton button,
                               Qt::KeyboardModifier modifier)
  : mButton(button), kModifier(modifier), x(0), y(0), w(0), h(0), started(false), graph(0) {}
MouseBoxZoomer::~MouseBoxZoomer() {}
//=====================================================================
bool MouseBoxZoomer::eventFilter(QObject *widget, QEvent *e) {
  GlMainWidget *glw = static_cast<GlMainWidget *>(widget);

  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons() == mButton &&
        (kModifier == Qt::NoModifier ||
         qMouseEv->modifiers() & kModifier)) {
      if (!started) {
        x = qMouseEv->x();
        y =  glw->height() - qMouseEv->y();
        w = 0;
        h = 0;
        started = true;
        graph = glw->getScene()->getMainGlGraph()->getGraph();
      }
      else {
        if (glw->getScene()->getMainGlGraph()->getGraph() != graph) {
          graph = nullptr;
          started = false;
        }
      }

      return true;
    }

    if (qMouseEv->buttons()==Qt::MidButton) {
      started = false;
      glw->redraw();
      return true;
    }

    return false;
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if ((qMouseEv->buttons() & mButton) &&
        (kModifier == Qt::NoModifier ||
         qMouseEv->modifiers() & kModifier)) {
      if (glw->getScene()->getMainGlGraph()->getGraph() != graph) {
        graph = nullptr;
        started = false;
      }

      if (started) {
        if ((qMouseEv->x() > 0) && (qMouseEv->x() < glw->width()))
          w = qMouseEv->x() - x;

        if ((qMouseEv->y() > 0) && (qMouseEv->y() < glw->height()))
          h = y - (glw->height() - qMouseEv->y());

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

    if ((qMouseEv->button() == mButton &&
         (kModifier == Qt::NoModifier ||
          qMouseEv->modifiers() & kModifier))) {
      if (glw->getScene()->getMainGlGraph()->getGraph() != graph) {
        graph = nullptr;
        started = false;
      }

      if (started) {
        started = false;

        if(!(w==0 && h==0)) {
          int width = glw->width();
          int height = glw->height();

          Coord bbMin(width-x, height - y+h);
          Coord bbMax(width-(x+w), height - y);

          if (abs(bbMax[0] - bbMin[0]) > 1 && abs(bbMax[1] - bbMin[1]) > 1) {

            BoundingBox sceneBB;
            sceneBB.expand(glw->getScene()->getMainLayer()->getCamera()->viewportTo3DWorld(glw->screenToViewport(bbMin)));
            sceneBB.expand(glw->getScene()->getMainLayer()->getCamera()->viewportTo3DWorld(glw->screenToViewport(bbMax)));

            QtGlSceneZoomAndPanAnimator zoomAnPan(glw, sceneBB);
            zoomAnPan.animateZoomAndPan();
          }
        }
      }

      return true;
    }
  }

  return false;
}
//=====================================================================
bool MouseBoxZoomer::draw(GlMainWidget *glw) {
  if (!started) return false;

  if (glw->getScene()->getMainGlGraph()->getGraph() != graph) {
    graph = nullptr;
    started = false;
  }

  Camera *camera = glw->getScene()->getMainLayer()->getCamera();
  Camera camera2d(false);
  tlp::Vec4i viewport = camera->getViewport();
  camera2d.setViewport(viewport);
  camera2d.initGl();
  GlRect2D rect(x, y, w, h, tlp::Color(0,0,255,100), tlp::Color::Black);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  rect.draw(camera2d);
  glDisable(GL_BLEND);

  return true;
}

//=====================================================================
