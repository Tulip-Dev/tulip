#include <iostream>

#if (QT_REL == 3)
#include <qevent.h>
#else
#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/Graph.h>
#include <tulip/GlGraphWidget.h>

#include "tulip/MouseBoxZoomer.h"

using namespace std;
using namespace tlp;

MouseBoxZoomer::MouseBoxZoomer() : x(0), y(0), w(0), h(0), started(false), graph(0) {}
MouseBoxZoomer::~MouseBoxZoomer() {}
//=====================================================================
bool MouseBoxZoomer::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glw = (GlGraphWidget *) widget;
    if (qMouseEv->button() == Qt::LeftButton) {
      if (!started) {
	x = qMouseEv->x();
	y =  glw->height() - qMouseEv->y();
	w = 0; h = 0;
	started = true;
	glw->setMouseTracking(true);
	graph = glw->getRenderingParameters().getGraph();
      }
      else {
	if (glw->getRenderingParameters().getGraph() != graph) {
	  graph = NULL;
	  started = false;
	  glw->setMouseTracking(false);
	}
      }
      return true;
    }
    if (qMouseEv->button()==Qt::MidButton){
      started = false;
      glw->setMouseTracking(false);
      glw->updateGL();
      return true;
    }
    return false;
  }
  if (e->type() == QEvent::MouseMove &&
      ((QMouseEvent *) e)->state() & Qt::LeftButton) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glw = (GlGraphWidget *) widget;
    if (glw->getRenderingParameters().getGraph() != graph) {
      graph = NULL;
      started = false;
      glw->setMouseTracking(false);
    }
    if (started){
      if ((qMouseEv->x() > 0) && (qMouseEv->x() < glw->width()))
	w = qMouseEv->x() - x;
      if ((qMouseEv->y() > 0) && (qMouseEv->y() < glw->height()))
	h = y - (glw->height() - qMouseEv->y());
      glw->updateGL();
    return true;
    }
  }
  if (e->type() == QEvent::MouseButtonRelease &&
      ((QMouseEvent *) e)->button() == Qt::LeftButton) {
    GlGraphWidget *glw = (GlGraphWidget *) widget;
    if (glw->getRenderingParameters().getGraph() != graph) {
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
	glw->translateCamera(width/2 - (x+w/2), height/2 - (y-h/2), 0);
	w = abs(w); h = abs(h);
	GlGraphRenderingParameters param = glw->getRenderingParameters();
	Camera cam = param.getCamera();
	//we prevent zooming in a minimal square area less than 4x4: a least
	//one of the 2 lengths must be higher than 3
	if (w > h) {
	  cam.zoomFactor *= (double) width / (double) w;
	}
	else {
	  cam.zoomFactor *= (double) height / (double) h;
	}
	param.setCamera(cam);
	glw->setRenderingParameters(param);
	glw->draw();
      }
    }
    return true;
  }
  return false;
}
//=====================================================================
bool MouseBoxZoomer::draw(GlGraphWidget *glw) {
  if (glw->getRenderingParameters().getGraph() != graph) {
    graph = NULL;
    started = false;
    glw->setMouseTracking(false);
  }
  if (!started) return false;
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
