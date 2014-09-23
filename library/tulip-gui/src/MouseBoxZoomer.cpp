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
#include <tulip/GlTools.h>
#include <tulip/DrawingTools.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/Camera.h>
#include <tulip/MouseBoxZoomer.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
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
  GlGraphInputData *inputData = glw->getScene()->getGlGraphComposite()->getInputData();

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
        graph = inputData->getGraph();
      }
      else {
        if (inputData->getGraph() != graph) {
          graph = NULL;
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
      if (inputData->getGraph() != graph) {
        graph = NULL;
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
    GlBoundingBoxSceneVisitor bbVisitor(inputData);
    glw->getScene()->getLayer("Main")->acceptVisitor(&bbVisitor);
    QtGlSceneZoomAndPanAnimator zoomAnPan(glw, bbVisitor.getBoundingBox());
    zoomAnPan.animateZoomAndPan();
    return true;
  }

  if (e->type() == QEvent::MouseButtonRelease) {

    QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);

    if ((qMouseEv->button() == mButton &&
         (kModifier == Qt::NoModifier ||
          qMouseEv->modifiers() & kModifier))) {
      if (inputData->getGraph() != graph) {
        graph = NULL;
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
            sceneBB.expand(glw->getScene()->getGraphCamera().screenTo3DWorld(bbMin));
            sceneBB.expand(glw->getScene()->getGraphCamera().screenTo3DWorld(bbMax));

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

  if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
    graph = NULL;
    started = false;
  }

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)glw->width(), 0.0, (GLdouble)glw->height(), -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

  float col[4] = {0.8f, 0.4f, 0.4f, 0.2f};
  setColor(col);
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x+w, y);
  glVertex2f(x+w, y-h);
  glVertex2f(x, y-h);
  glEnd();
  glDisable(GL_BLEND);

  glLineWidth(2);
  glLineStipple(2, 0xAAAA);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, y);
  glVertex2f(x+w, y);
  glVertex2f(x+w, y-h);
  glVertex2f(x, y-h);
  glEnd();

  glPopMatrix();
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW);
  glPopAttrib();
  return true;
}

//=====================================================================
