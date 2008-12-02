#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMainWidget.h>

#include <tulip/MouseEdgeBuilder.h>

using namespace std;
using namespace tlp;

MouseEdgeBuilder::MouseEdgeBuilder():started(false){}

bool MouseEdgeBuilder::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;

    ElementType type;
    node tmpNode;
    edge tmpEdge;
    Graph * _graph = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
    LayoutProperty* mLayout = _graph->getProperty<LayoutProperty>("viewLayout");
    if (qMouseEv->buttons()==Qt::LeftButton) {
      if (!started) {
	bool result=glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge);
	if (result && (type == NODE)) {
	  started=true;
	  source=tmpNode;
	  glMainWidget->setMouseTracking(true);
	  curPos=startPos=mLayout->getNodeValue(source);
	  return true;
	}
	return false;
      }
      else {
	bool result = glMainWidget->doSelect(qMouseEv->x(),qMouseEv->y(),type,tmpNode,tmpEdge);
	if (result && (type == NODE)) {
	  Observable::holdObservers();
	  started=false;
	  glMainWidget->setMouseTracking(false);
	  // allow to undo
	  _graph->push();
	  edge newEdge = _graph->addEdge(source, tmpNode);
	  mLayout->setEdgeValue(newEdge, bends);
	  //	  mColors->setEdgeValue(newEdge, ((Application *)qApp)->edgeColor);
	  bends.clear();
	  glMainWidget->draw();
	  Observable::unholdObservers(); 
	}
	else {
	  Coord point((double) glMainWidget->width() - (double) qMouseEv->x(),
		 (double) qMouseEv->y(),
		 0);
	  bends.push_back(glMainWidget->getScene()->getCamera()->screenTo3DWorld(point));
	  glMainWidget->draw();
	  }
      }
      return true;
    }
    if (qMouseEv->buttons()==Qt::MidButton) {
      bends.clear();
      glMainWidget->setMouseTracking(false);
      started=false;
      glMainWidget->draw();
      return true;
    }
  }
  if  (e->type() == QEvent::MouseMove) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (!started) return false;
    
    Coord point((double) glMainWidget->width() - (double) qMouseEv->x(),
		(double) qMouseEv->y(),
		0);
    point = glMainWidget->getScene()->getCamera()->screenTo3DWorld(point);
    curPos.set(point[0], point[1], point[2]);
    glMainWidget->draw();
    return true;
  }

  return false;
}

bool MouseEdgeBuilder::draw(GlMainWidget *glMainWidget) {
  if (!started) return false;
  glStencilFunc(GL_LEQUAL,0,0xFFFF);
  glMainWidget->getScene()->getCamera()->initGl();
  glDisable(GL_LIGHTING);
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
