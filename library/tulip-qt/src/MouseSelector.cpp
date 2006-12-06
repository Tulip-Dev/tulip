#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GlGraphWidget.h>

#include <tulip/MouseSelector.h>

using namespace std;
using namespace tlp;

//==================================================================
MouseSelector::MouseSelector():
  x(0),y(0),w(0),h(0),started(false),graph(0) {
}
//==================================================================
bool MouseSelector::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    if (qMouseEv->button()==Qt::LeftButton) {
      if (!started) {
	x = qMouseEv->x();
	y = qMouseEv->y();
	w = 0;
	h = 0;
	started = true;
	glGraphWidget->setMouseTracking(true);
	graph=glGraphWidget->getRenderingParameters().getGraph();
      }
      else {
	if (glGraphWidget->getRenderingParameters().getGraph()!=graph) {
	  graph = 0;
	  started = false;
	  glGraphWidget->setMouseTracking(false);
	  return false;
	}
      }
      return true;
    }
    if (qMouseEv->button()==Qt::MidButton){
      started = false;
      glGraphWidget->setMouseTracking(false);
      glGraphWidget->updateGL();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    if (glGraphWidget->getRenderingParameters().getGraph()!=graph) {
      graph=0;
      started=false;
      glGraphWidget->setMouseTracking(false);
    }
    if (started){
      if ((qMouseEv->x()>0) && (qMouseEv->x()<glGraphWidget->width()))
	w = qMouseEv->x() - x;
      if ((qMouseEv->y()>0) && (qMouseEv->y()<glGraphWidget->height()))
	h = qMouseEv->y() - y;
      glGraphWidget->updateGL();
      return true;
    }
    return false;
  }
  if  (e->type() == QEvent::MouseButtonRelease) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    if (glGraphWidget->getRenderingParameters().getGraph()!=graph) {
      graph=0;
      started=false;
      glGraphWidget->setMouseTracking(false);
      return false;
    }
    if (started) {
      glGraphWidget->setMouseTracking(false);
      Observable::holdObservers();
      BooleanProperty* selection=glGraphWidget->getRenderingParameters().getGraph()->getProperty<BooleanProperty>("viewSelection");
      bool boolVal = true; // add to selection
      if (qMouseEv->stateAfter() != Qt::ShiftButton) {
	if (qMouseEv->stateAfter() !=
#if defined(__APPLE__)
	    Qt::AltButton
#else
	    Qt::ControlButton
#endif
	    ) {
	  selection->setAllNodeValue(false);
	  selection->setAllEdgeValue(false);
	} else
	  boolVal = false; // remove from selection
      }
      if ((w==0) && (h==0)) {
	node tmpNode;
	edge tmpEdge;
	ElementType type;
	bool result = glGraphWidget->doSelect(x, y, type, tmpNode, tmpEdge);
	if (result){
	  switch(type) {
	  case NODE: selection->setNodeValue(tmpNode, boolVal); break;
	  case EDGE: selection->setEdgeValue(tmpEdge, boolVal); break;
	  }
	}
      } else {
	vector<node> tmpSetNode;
	vector<edge> tmpSetEdge;
	if (w < 0) {
	  w *= -1;
	  x -= w;
	}
	if (h<0) {
	  h *= -1;
	  y -= h;
	}
	glGraphWidget->doSelect(x, y, w, h, tmpSetNode, tmpSetEdge);
	vector<node>::const_iterator it;
	for (it=tmpSetNode.begin(); it!=tmpSetNode.end(); ++it) {
	  selection->setNodeValue(*it, boolVal);
	}
	vector<edge>::const_iterator ite;
	for (ite=tmpSetEdge.begin(); ite!=tmpSetEdge.end(); ++ite) {
	  selection->setEdgeValue(*ite, boolVal);
	}
      }
      started = false;
      glGraphWidget->updateGL();
      Observable::unholdObservers();
      return true;
    }
  }
  return false;
}
//==================================================================
bool MouseSelector::draw(GlGraphWidget *glGraphWidget){
  if (glGraphWidget->getRenderingParameters().getGraph()!=graph) {
    graph = 0;
    started = false;
    glGraphWidget->setMouseTracking(false);
  }
  if (!started) return false;
  float yy = glGraphWidget->height() - y;
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  gluOrtho2D (0.0, (GLdouble) glGraphWidget->width(), 0.0, (GLdouble) glGraphWidget->height());
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
  float col[4]={0.8,0.8,0.7,0.2};
  glColor4fv(col);
  glBegin(GL_QUADS);
  glVertex2f(x, yy);
  glVertex2f(x+w, yy);
  glVertex2f(x+w, yy-h);
  glVertex2f(x, yy-h);
  glEnd();
  glDisable(GL_BLEND);
  glLineWidth(2);
  glLineStipple(2, 0xAAAA);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, yy);
  glVertex2f(x+w, yy);
  glVertex2f(x+w, yy-h);
  glVertex2f(x, yy-h);
  glEnd();
  glLineWidth(1);
  glPopMatrix();
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW);
  glPopAttrib();
  return true;
}
