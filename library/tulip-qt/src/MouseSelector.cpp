#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GlMainWidget.h>

#include <tulip/MouseSelector.h>

using namespace std;
using namespace tlp;

//==================================================================
MouseSelector::MouseSelector(Qt::MouseButton button,
			     Qt::KeyboardModifier modifier):
  mButton(button), kModifier(modifier), x(0),y(0),w(0),h(0),
  started(false),graph(0) {
}
//==================================================================
bool MouseSelector::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (qMouseEv->buttons()== mButton &&
	(kModifier == Qt::NoModifier ||
	 ((QMouseEvent *) e)->modifiers() & kModifier)) {
      if (!started) {
	x = qMouseEv->x();
	y = qMouseEv->y();
	w = 0;
	h = 0;
	started = true;
	glMainWidget->setMouseTracking(true);
	graph=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      }
      else {
	if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
	  graph = 0;
	  started = false;
	  glMainWidget->setMouseTracking(false);
	  return false;
	}
      }
      return true;
    }
    if (qMouseEv->buttons()==Qt::MidButton){
      started = false;
      glMainWidget->setMouseTracking(false);
      glMainWidget->redraw();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove &&
      ((((QMouseEvent *) e)->buttons() & mButton) &&
       (kModifier == Qt::NoModifier ||
	((QMouseEvent *) e)->modifiers() & kModifier))) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
      graph=0;
      started=false;
      glMainWidget->setMouseTracking(false);
    }
    if (started){
      if ((qMouseEv->x()>0) && (qMouseEv->x()<glMainWidget->width()))
	w = qMouseEv->x() - x;
      if ((qMouseEv->y()>0) && (qMouseEv->y()<glMainWidget->height()))
	h = qMouseEv->y() - y;
      glMainWidget->redraw();
      return true;
    }
    return false;
  }
  if  (e->type() == QEvent::MouseButtonRelease) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
      graph=0;
      started=false;
      glMainWidget->setMouseTracking(false);
      return false;
    }
    if (started) {
      glMainWidget->setMouseTracking(false);
      Observable::holdObservers();
      BooleanProperty* selection=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getProperty<BooleanProperty>("viewSelection");
      bool revertSelection = false; // add to selection
      bool boolVal = true;
      if (qMouseEv->modifiers() !=
#if defined(__APPLE__)
	  Qt::AltModifier
#else
	  Qt::ControlModifier
#endif
	  ) {
	if (qMouseEv->modifiers() == Qt::ShiftModifier)
	  boolVal = false;
	else {
	  selection->setAllNodeValue(false);
	  selection->setAllEdgeValue(false);
	}
	} else
	  revertSelection = true; // revert selection
      if ((w==0) && (h==0)) {
	node tmpNode;
	edge tmpEdge;
	ElementType type;
	bool result = glMainWidget->doSelect(x, y, type, tmpNode, tmpEdge);
	if (result){
	  switch(type) {
	  case NODE: selection->setNodeValue(tmpNode,
					     revertSelection ?
					     !selection->getNodeValue(tmpNode)
					     : boolVal); break;
	  case EDGE: selection->setEdgeValue(tmpEdge,
					     revertSelection ?
					     !selection->getEdgeValue(tmpEdge)
					     : boolVal); break;
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
	glMainWidget->doSelect(x, y, w, h, tmpSetNode, tmpSetEdge);
	vector<node>::const_iterator it;
	for (it=tmpSetNode.begin(); it!=tmpSetNode.end(); ++it) {
	  selection->setNodeValue(*it, 
				  revertSelection ?
				  !selection->getNodeValue(*it)
				  : boolVal);
	}
	vector<edge>::const_iterator ite;
	for (ite=tmpSetEdge.begin(); ite!=tmpSetEdge.end(); ++ite) {
	  selection->setEdgeValue(*ite, 
				  revertSelection ?
				  !selection->getEdgeValue(*ite)
				  : boolVal);
	}
      }
      started = false;
      glMainWidget->redraw();
      Observable::unholdObservers();
      return true;
    }
  }
  return false;
}
//==================================================================
bool MouseSelector::draw(GlMainWidget *glMainWidget){
  if (!started) return false;
  if (glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()!=graph) {
    graph = 0;
    started = false;
    glMainWidget->setMouseTracking(false);
  }
  float yy = glMainWidget->height() - y;
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  gluOrtho2D (0.0, (GLdouble) glMainWidget->width(), 0.0, (GLdouble) glMainWidget->height());
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
