/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <iostream>
#include <QtDebug>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>
#include <QPinchGesture>

#include "tulip/Graph.h"
#include "tulip/MouseInteractors.h"
#include "tulip/GlMainWidget.h"
#include "tulip/AbstractView.h"
#include <tulip/Observable.h>

using namespace tlp;
using namespace std;

//===============================================================
bool MousePanNZoomNavigator::eventFilter(QObject *widget, QEvent *e) {
// according to Qt's doc, this constant has been defined by wheel mouse vendors
// we need it to interpret the value of QWheelEvent->delta()
#define WHEEL_DELTA 120
  if (e->type() == QEvent::Wheel &&
      (((QWheelEvent *) e)->orientation() == Qt::Vertical)) {
    GlMainWidget *g = (GlMainWidget *) widget;
    g->getScene()->zoomXY(((QWheelEvent *) e)->delta() / WHEEL_DELTA,
      ((QWheelEvent *) e)->x(), ((QWheelEvent *) e)->y());
    g->draw(false);
    return true;
  }

  if(e->type() == QEvent::Gesture) {
      GlMainWidget *g = (GlMainWidget *) widget;
      QGestureEvent* gesture = (QGestureEvent*)e;
      QPointF center;
      //swipe events and pan events are never fired, known Qt bug
      /*if(gesture->gesture(Qt::SwipeGesture)) {
        QSwipeGesture* swipe = (QSwipeGesture*)gesture->gesture(Qt::SwipeGesture);
        int x = cos(swipe->swipeAngle()) * swipe->property("velocity").toFloat();
        int y = sin(swipe->swipeAngle()) * swipe->property("velocity").toFloat();
        g->getScene()->translateCamera(x, y, 0);
      }*/
	  
    if(gesture->gesture(Qt::PinchGesture)) {
          QPinchGesture* pinch = (QPinchGesture*)gesture->gesture(Qt::PinchGesture);
          Camera& camera = g->getScene()->getCamera();

		  //store the camera scale factor when starting the gesture
          if(pinch->state() == Qt::GestureStarted) {
            cameraScaleFactor = camera.getZoomFactor();
			isGesturing = true;
          }

          if(pinch->changeFlags() & QPinchGesture::ScaleFactorChanged) {  
			//only set the zoom factor if two events in a row were in the same direction (zoom in or out) to smooth a bit the effect.
			if((pinch->lastScaleFactor() > 1 && pinch->scaleFactor() > 1) || (pinch->lastScaleFactor() <= 1 && pinch->scaleFactor() <= 1)) {
				camera.setZoomFactor(cameraScaleFactor * pinch->totalScaleFactor());
			}
          }

          if(pinch->changeFlags() & QPinchGesture::RotationAngleChanged)
          {
			/*//backup the current camera center
            Coord oldCenter = camera.getCenter();
            Coord oldEye = camera.getEyes();
			//sets the camera center to the center of the pich gesture
            Coord rotationCenter(g->mapFromGlobal(pinch->centerPoint().toPoint()).x(), g->mapFromGlobal(pinch->centerPoint().toPoint()).y(), oldCenter.getZ());
            Coord rotationEye=camera.getEyes()+(rotationCenter-oldCenter);
            camera.setCenter(rotationCenter);
            camera.setEyes(rotationEye);*/
			//rotates the camera
			camera.rotate((pinch->rotationAngle() - pinch->lastRotationAngle())/180*M_PI, 0, 0, 1);
			/*
			//restore old camera center and eyes
            camera.setCenter(oldCenter);
            camera.setEyes(oldEye); */
          }

		  if(pinch->state() == Qt::GestureFinished) {
			isGesturing = false;
          }
		  if(gesture->gesture(Qt::PanGesture)) {
			QPanGesture* pan = (QPanGesture*)gesture->gesture(Qt::PanGesture);

			if(pan->state() == Qt::GestureStarted) {
			  isGesturing = true;
			}
			if(pan->state() == Qt::GestureFinished) {
			  isGesturing = false;
			}
			center = pan->delta();
			g->getScene()->translateCamera(pan->delta().x(), -pan->delta().y(), 0);
		  }
      }
      g->draw(false);
	  return true;
  }

  return false;
}

//===============================================================
bool MouseElementDeleter::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent *qMouseEv = (QMouseEvent *) e;
  if(qMouseEv != NULL) {
    node tmpNode;
    edge tmpEdge;
    ElementType type;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if(e->type() == QEvent::MouseMove) {    
      if (glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge)) {
        glMainWidget->setCursor(QCursor(QPixmap(":/i_del.png")));
      }
      else {
        glMainWidget->setCursor(Qt::ArrowCursor);
      }
      return false;
    }
    else if (e->type() == QEvent::MouseButtonPress && qMouseEv->button()==Qt::LeftButton) {
      if (glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge)) {
        Observable::holdObservers();
        Graph* graph = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
        // allow to undo
        graph->push();
        switch(type) {
          case NODE: graph->delNode(tmpNode); break;
          case EDGE: graph->delEdge(tmpEdge); break;
        }
        glMainWidget->redraw();
        Observable::unholdObservers();
        return true;
      }
    }
  }
  return false;
}
//===============================================================
class MouseRotXRotY:public InteractorComponent
{
 public:
  MouseRotXRotY(){}
  ~MouseRotXRotY(){}
  int x,y;
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return this; }};

bool MouseRotXRotY::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    x = ((QMouseEvent *) e)->x();
    y = ((QMouseEvent *) e)->y();
    return true;
  }
  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    int deltaX,deltaY;
    deltaX=qMouseEv->x()-x;
    deltaY=qMouseEv->y()-y;
    if (abs(deltaX)>abs(deltaY))
      deltaY=0;
    else
      deltaX=0;
    if (deltaY!=0) glMainWidget->getScene()->rotateScene(deltaY,0,0);
    if (deltaX!=0) glMainWidget->getScene()->rotateScene(0,deltaX,0);
    x=qMouseEv->x();
    y=qMouseEv->y();
    glMainWidget->draw();
    return true;
  }
  return false;
}
//===============================================================
class MouseZoomRotZ:public InteractorComponent
{
 public:
  MouseZoomRotZ():inRotation(false),inZoom(false){}
  ~MouseZoomRotZ(){}
  int x,y;
  bool inRotation, inZoom;
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return this; }};

bool MouseZoomRotZ::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    x = ((QMouseEvent *) e)->x();
    y = ((QMouseEvent *) e)->y();
    inRotation=false;
    inZoom=false;
    return true;
  }
  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    int deltaX,deltaY;

    if(!inRotation && !inZoom){
      deltaX = qMouseEv->x() - x;
      deltaY= qMouseEv->y() - y;
      if (deltaY && abs(deltaX) >= 3 * abs(deltaY)) {
        inRotation=true;
        inZoom=false;
      }else if (deltaX && abs(deltaY) >=  3 * abs(deltaX)) {
        inZoom=true;
        inRotation=false;
      }else{

      }
      x = qMouseEv->x();
      y = qMouseEv->y();
    }

    if (inZoom) {
      // Zoom
      deltaY = qMouseEv->y() - y;
      glMainWidget->getScene()->zoom(-deltaY/2);
      y = qMouseEv->y();
    }
    if(inRotation) {
      // Rotation
      deltaX = qMouseEv->x() - x;
      glMainWidget->getScene()->rotateScene(0,0,deltaX);
      x = qMouseEv->x();
    }
    glMainWidget->draw();
    return true;
  }
  return false;
}
//===============================================================
class MouseMove:public InteractorComponent
{
 public:
  int x,y;
  MouseMove(){}
  ~MouseMove(){}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return this; }};

bool MouseMove::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    x = ((QMouseEvent *) e)->x();
    y = ((QMouseEvent *) e)->y();
    return true;
  }
  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (qMouseEv->x() != x)
      glMainWidget->getScene()->translateCamera(qMouseEv->x()-x,0,0);
    if (qMouseEv->y() != y)
    glMainWidget->getScene()->translateCamera(0,y-qMouseEv->y(),0);
    x = qMouseEv->x();
    y = qMouseEv->y();
    glMainWidget->draw(false);
    return true;
  }
  return false;
}
//===============================================================
bool MouseNKeysNavigator::eventFilter(QObject *widget, QEvent *e) {
  if(isGesturing) {
	return MousePanNZoomNavigator::eventFilter(widget, e);
  }

  if(currentSpecInteractorComponent){
    if(currentSpecInteractorComponent->eventFilter(widget,e))
      return true;
  }

  if (e->type() == QEvent::MouseButtonPress) {
    if (((QMouseEvent *) e)->buttons() == Qt::LeftButton) {
      oldCursor=((QWidget*)widget)->cursor();
      InteractorComponent *currentMouse;
      // give focus so we can catch key events
      ((GlMainWidget *)widget)->setFocus();
      if (((QMouseEvent *) e)->modifiers() &
#if defined(__APPLE__)
          Qt::AltModifier
#else
          Qt::ControlModifier
#endif
      )
        currentMouse = new MouseZoomRotZ();
      else if (((QMouseEvent *) e)->modifiers() & Qt::ShiftModifier)
        currentMouse = new MouseRotXRotY();
      else{
        currentMouse = new MouseMove();
        ((QWidget*)widget)->setCursor(QCursor(Qt::ClosedHandCursor));
      }
      bool result = currentMouse->eventFilter(widget, e);

      currentSpecInteractorComponent=currentMouse;

      //currentMouseID = abstractView->pushInteractor(currentMouse);
      return result;
    }
    return false;
  }
  if (e->type() == QEvent::MouseButtonRelease) {
    ((QWidget*)widget)->setCursor(oldCursor);
    delete currentSpecInteractorComponent;
    currentSpecInteractorComponent=NULL;
    return true;
  }
  if (e->type() == QEvent::KeyPress) {
    int delta = (((QKeyEvent *) e)->isAutoRepeat() ? 3 : 1);
    GlMainWidget *g = (GlMainWidget *) widget;
    switch(((QKeyEvent *) e)->key()) {
    case Qt::Key_Left: g->getScene()->translateCamera(delta * 2,0,0); break;
    case Qt::Key_Right: g->getScene()->translateCamera(-1 * delta * 2,0,0); break;
    case Qt::Key_Up: g->getScene()->translateCamera(0,-1 * delta * 2,0); break;
    case Qt::Key_Down: g->getScene()->translateCamera(0,delta * 2,0); break;
    case Qt::Key_PageUp: g->getScene()->zoom(delta); break;
    case Qt::Key_PageDown: g->getScene()->zoom(-1 * delta); break;
    case Qt::Key_Home: g->getScene()->translateCamera(0,0,-1 * delta * 2); break;
    case Qt::Key_End: g->getScene()->translateCamera(0,0,delta * 2); break;
    case Qt::Key_Insert: g->getScene()->rotateScene(0,0,-1 * delta * 2); break;
    case Qt::Key_Delete : g->getScene()->rotateScene(0,0,delta * 2); break;
    default:
      return false;
    }
    g->draw();
    return true;
  }
  if (e->type() == QEvent::KeyRelease) {
    switch(((QKeyEvent *) e)->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_Insert:
    case Qt::Key_Delete: break;
    default:
      return false;
    }
    return true;
  }

  return MousePanNZoomNavigator::eventFilter(widget, e);
}
