#include <iostream>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>
#include "tulip/Qt3ForTulip.h"

#include "tulip/Graph.h"
#include "tulip/MouseInteractors.h"
#include "tulip/GlMainWidget.h"
#include "tulip/ElementPropertiesWidget.h"
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
    g->draw();
    return true;
  }
    
  return false;
}
//===============================================================
bool MouseElementDeleter::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress &&
      ((QMouseEvent *) e)->button()==Qt::LeftButton) {
    QMouseEvent *qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    ElementType type;
    node tmpNode;
    edge tmpEdge;
    bool result = glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge);
    if(result == true) {
      Observable::holdObservers();
      switch(type) {
	case NODE: glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()->delNode(tmpNode); break;
	case EDGE: glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()->delEdge(tmpEdge); break;
      }
      glMainWidget->redraw();
      Observable::unholdObservers();
    }
    return true;
  }
  return false;
}
//===============================================================
class MouseRotXRotY:public GWInteractor
{
 public:
  MouseRotXRotY(){}
  ~MouseRotXRotY(){}
  int x,y;
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return this; }};

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
class MouseZoomRotZ:public GWInteractor
{
 public:
  MouseZoomRotZ(){}
  ~MouseZoomRotZ(){}
  int x,y;
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return this; }};

bool MouseZoomRotZ::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    x = ((QMouseEvent *) e)->x();
    y = ((QMouseEvent *) e)->y();
    return true;
  }
  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
#define NO_ZOOM -1
#define NO_ROTATION -1
    int deltaX,deltaY;
    if (x == NO_ROTATION) {
      // Zoom
      deltaY = qMouseEv->y() - y;
      glMainWidget->getScene()->zoom(-deltaY/2);
      y = qMouseEv->y();
    }
    else {
      deltaX = qMouseEv->x() - x;
      if (y == NO_ZOOM) {
	// Rotation
	glMainWidget->getScene()->rotateScene(0,0,deltaX);
	x = qMouseEv->x();
      }
      else {
	deltaY= qMouseEv->y() - y;
	// at this step deltaX and deltaY cumulate the move
	// on both axis since the previous mPressEvent call,
	// in order we can find the main axis of the move.
	if (deltaY && abs(deltaX) >= 3 * abs(deltaY)) {
	  // X axis => rotation
	  y = NO_ZOOM;
	  x = qMouseEv->x();
	}	
	else if (deltaX && abs(deltaY) >=  3 * abs(deltaX)) {
	  // Y axis => zoom
	  x = NO_ROTATION;
	  y = qMouseEv->y();
	}
      }
    }
    glMainWidget->draw();
    return true;
  }
  return false;
}
//===============================================================
class MouseMove:public GWInteractor
{
 public:
  int x,y;
  MouseMove(){}
  ~MouseMove(){}
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return this; }};

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
    glMainWidget->draw();
    return true;
  }
  return false;
}
//===============================================================
bool MouseNKeysNavigator::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    if (((QMouseEvent *) e)->button() == Qt::LeftButton) {
      GWInteractor *currentMouse;
      // give focus so we can catch key events
      ((GlMainWidget *)widget)->setFocus();
      if (((QMouseEvent *) e)->state() &
#if defined(__APPLE__)
	  Qt::AltButton
#else
	  Qt::ControlButton
#endif
	  )
	currentMouse = new MouseZoomRotZ();
      else if (((QMouseEvent *) e)->state() & Qt::ShiftButton)
	currentMouse = new MouseRotXRotY();
      else
	currentMouse = new MouseMove();
      bool result = currentMouse->eventFilter(widget, e);
      currentMouseID = ((GlMainWidget *)widget)->pushInteractor(currentMouse);
      return result;
    }
    currentMouseID = GWInteractor::invalidID;
    return false;
  }
  if (e->type() == QEvent::MouseButtonRelease &&
      currentMouseID != GWInteractor::invalidID) {
    ((GlMainWidget *)widget)->removeInteractor(currentMouseID);
    currentMouseID = GWInteractor::invalidID;
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
    case Qt::Key_Prior: g->getScene()->zoom(delta); break;
    case Qt::Key_Next: g->getScene()->zoom(-1 * delta); break;
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
    case Qt::Key_Prior:
    case Qt::Key_Next:
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
