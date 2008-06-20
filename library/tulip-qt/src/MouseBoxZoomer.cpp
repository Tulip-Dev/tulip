#include <iostream>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/GlMainWidget.h>

#include "tulip/MouseBoxZoomer.h"

using namespace std;
using namespace tlp;

INTERACTORPLUGIN(MouseBoxZoomer, "MouseBoxZoomer", "Tulip Team", "16/04/2008", "Mouse Box Zoomer", "1.0", 3);

MouseBoxZoomer::MouseBoxZoomer(Qt::MouseButton button,
			       Qt::KeyboardModifier modifier)
  : mButton(button), kModifier(modifier), x(0), y(0), w(0), h(0), started(false), graph(0) {}
MouseBoxZoomer::~MouseBoxZoomer() {}
//=====================================================================
bool MouseBoxZoomer::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glw = (GlMainWidget *) widget;
    if (qMouseEv->buttons() == mButton &&
	(kModifier == Qt::NoModifier ||
	 ((QMouseEvent *) e)->modifiers() & kModifier)) {
      if (!started) {
	x = qMouseEv->x();
	y =  glw->height() - qMouseEv->y();
	w = 0; h = 0;
	started = true;
	glw->setMouseTracking(true);
	graph = glw->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      }
      else {
	if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
	  graph = NULL;
	  started = false;
	  glw->setMouseTracking(false);
	}
      }
      return true;
    }
    if (qMouseEv->buttons()==Qt::MidButton){
      started = false;
      glw->setMouseTracking(false);
      glw->redraw();
      return true;
    }
    return false;
  }
  if (e->type() == QEvent::MouseMove &&
      ((((QMouseEvent *) e)->buttons() & mButton) &&
       	(kModifier == Qt::NoModifier ||
	 ((QMouseEvent *) e)->modifiers() & kModifier))) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glw = (GlMainWidget *) widget;
    if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
      graph = NULL;
      started = false;
      glw->setMouseTracking(false);
    }
    if (started){
      if ((qMouseEv->x() > 0) && (qMouseEv->x() < glw->width()))
	w = qMouseEv->x() - x;
      if ((qMouseEv->y() > 0) && (qMouseEv->y() < glw->height()))
	h = y - (glw->height() - qMouseEv->y());
      glw->redraw();
    return true;
    }
  }
  if (e->type() == QEvent::MouseButtonRelease &&
      (((QMouseEvent *) e)->button() == mButton &&
       	(kModifier == Qt::NoModifier ||
	 ((QMouseEvent *) e)->modifiers() & kModifier))) {
    GlMainWidget *glw = (GlMainWidget *) widget;
    if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
      graph = NULL;
      started = false;
      glw->setMouseTracking(false);
    }
    if (started) {
      started = false;
      if ((abs(w) > 10) && (abs(h) > 10)) {
	//      cerr << __FUNCTION__ << ": " << this << "(x,y)=(" << e->x() << "," << e->y() << ")" << endl;
	int width = glw->width();
	int height = glw->height();
	glw->getScene()->translateCamera(width/2 - (x+w/2), height/2 - (y-h/2), 0);
	w = abs(w); h = abs(h);
	Camera cam = *glw->getScene()->getCamera();
	//we prevent zooming in a minimal square area less than 4x4: a least
	//one of the 2 lengths must be higher than 3
	if (w > h) {
	  cam.setZoomFactor(cam.getZoomFactor() * ((double) width / (double) w));
	}
	else {
	  cam.setZoomFactor(cam.getZoomFactor() * ((double) height / (double) h));
	}
	glw->getScene()->setCamera(&cam);
	glw->draw();
      }
    }
    return true;
  }
  return false;
}
//=====================================================================
bool MouseBoxZoomer::draw(GlMainWidget *glw) {
  if (!started) return false;
  if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
    graph = NULL;
    started = false;
    glw->setMouseTracking(false);
  }
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)glw->width(), 0.0, (GLdouble)glw->height());
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
  
  float col[4] = {0.8, 0.4, 0.4, 0.2};
  glColor4fv(col);
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
  /*
  glLineWidth(1);
  glDisable(GL_LINE_STIPPLE);  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  */
  glPopMatrix();
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW);
  glPopAttrib();
  return true;
}

//=====================================================================
