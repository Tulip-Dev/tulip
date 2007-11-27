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
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlGraphWidget.h>

#include <tulip/MouseEdgeBuilder.h>

using namespace std;
using namespace tlp;

MouseEdgeBuilder::MouseEdgeBuilder():started(false){}

bool MouseEdgeBuilder::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;

    ElementType type;
    node tmpNode;
    edge tmpEdge;
    Graph * _graph = glGraphWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
    LayoutProperty* mLayout = _graph->getProperty<LayoutProperty>("viewLayout");
    if (qMouseEv->button()==Qt::LeftButton) {
      if (!started) {
	bool result=glGraphWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge);
	if (result && (type == NODE)) {
	  started=true;
	  source=tmpNode;
	  glGraphWidget->setMouseTracking(true);
	  curPos=startPos=mLayout->getNodeValue(source);
	  return true;
	}
	return false;
      }
      else {
	bool result = glGraphWidget->doSelect(qMouseEv->x(),qMouseEv->y(),type,tmpNode,tmpEdge);
	if (result && (type == NODE)) {
	  Observable::holdObservers();
	  started=false;
	  glGraphWidget->setMouseTracking(false);
	  cout << "new edge" << endl;
	  edge newEdge = glGraphWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()->addEdge(source, tmpNode);
	  mLayout->setEdgeValue(newEdge, bends);
	  //	  mColors->setEdgeValue(newEdge, ((Application *)qApp)->edgeColor);
	  bends.clear();
	  glGraphWidget->draw();
	  Observable::unholdObservers(); 
	}
	else {
	  Coord point((double) glGraphWidget->width() - (double) qMouseEv->x(),
		 (double) qMouseEv->y(),
		 0);
	  bends.push_back(glGraphWidget->getScene()->getCamera()->screenTo3DWorld(point));
	  glGraphWidget->draw();
	  }
      }
      return true;
    }
    if (qMouseEv->button()==Qt::MidButton) {
      bends.clear();
      glGraphWidget->setMouseTracking(false);
      started=false;
      glGraphWidget->draw();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
    if (!started) return false;
    
    Coord point((double) glGraphWidget->width() - (double) qMouseEv->x(),
		(double) qMouseEv->y(),
		0);
    point = glGraphWidget->getScene()->getCamera()->screenTo3DWorld(point);
    curPos.set(point[0], point[1], point[2]);
    glGraphWidget->draw();
    return true;
  }

  return false;
}

bool MouseEdgeBuilder::draw(GlGraphWidget *glGraphWidget) {
  if (!started) return false;
  float color[4];
  color[0]=1; color[1]=0;  color[2]=0;  color[3]=1;
  vector<Coord>::iterator lCoordIt=bends.begin();
  glColor4fv(color);
  glBegin(GL_LINE_STRIP);{
    glVertex3f(startPos.getX(),startPos.getY(),startPos.getZ());
    while(lCoordIt!=bends.end()) {
      glVertex3f(lCoordIt->getX(),lCoordIt->getY(),lCoordIt->getZ());
      ++lCoordIt;
    }
    glVertex3f(curPos.getX(),curPos.getY(),curPos.getZ());
  }glEnd();
  return true;
}
