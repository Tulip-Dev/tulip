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
#include <tulip/GlTools.h>

#include <tulip/MouseEdgeSelector.h>

using namespace std;
using namespace tlp;

INTERACTORPLUGIN(MouseEdgeSelector, "MouseEdgeSelector", "Tulip Team", "16/04/2008", "Mouse Edge Selector", "1.0");

//==================================================================
MouseEdgeSelector::MouseEdgeSelector():
  x(0),y(0),w(0),h(0),started(false),graph(0) {
}
//==================================================================
bool MouseEdgeSelector::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (qMouseEv->button()==Qt::LeftButton) {
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
    if (qMouseEv->button()==Qt::MidButton){
      started = false;
      glMainWidget->setMouseTracking(false);
      glMainWidget->redraw();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove) {
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
      selection->setAllNodeValue(false);
      selection->setAllEdgeValue(false);
      if ((w==0) && (h==0)) {
	node tmpNode;
	edge tmpEdge;
	ElementType type;
	bool result = glMainWidget->doSelect(x, y, type, tmpNode, tmpEdge);
	if (result){
	  switch(type) {
	  case EDGE: selection->setEdgeValue(tmpEdge, true); break;
	  default:
	    break;
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
	vector<edge>::const_iterator ite;
	int compt=0;
	for (ite=tmpSetEdge.begin(); ite!=tmpSetEdge.end(); ++ite) {
	  selection->setEdgeValue(*ite, true);
	  compt++;
	}
	if(compt!=1){
	  selection->setAllNodeValue(false);
	  selection->setAllEdgeValue(false);
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
bool MouseEdgeSelector::draw(GlMainWidget *glMainWidget){
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
  setColor(col);
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
