#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/PluginProgress.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GlGraphWidget.h>


#include "MouseTreeFishEyes.h"

using namespace std;
using namespace tlp;

void MouseTreeFishEyes::mPressEvent(GlGraphWidget *glGraphWidget,QMouseEvent *qMouseEv) {
  if (qMouseEv->button()==QEvent::LeftButton) {
    Observable::holdObservers();
  double x=qMouseEv->x();
  double y=glGraphWidget->height() - qMouseEv->y();
  double w=30,h=30;
  glGraphWidget->setMouseTracking(true);
  Graph *graph=glGraphWidget->getGlGraph()->getGraph();
  set<node> tmpSetNode;
  set<edge> tmpSetEdge;
  double centX= (x+x+w)/2;
  double centY= (y+y-h)/2;
  if (w<0) w*=-1;
  if (h<0) h*=-1;
  glGraphWidget->getGlGraph()->doSelect((int)centX,(int)centY,(int)w,(int)h,tmpSetNode,tmpSetEdge);

  if (tmpSetNode.empty()) return;

  //  Sizes *sizes=graph->getProperty<SizeProperty>("viewSize");
  DoubleProperty *sizes=graph->getProperty<DoubleProperty>("viewMetric");
  set<node>::const_iterator it;
  if (qMouseEv->state()!=QEvent::ControlButton) {
    for (it=tmpSetNode.begin();it!=tmpSetNode.end();++it) {
      sizes->setNodeValue(*it,sizes->getNodeValue(*it)*2.0);
    }
  }
  else {
    for (it=tmpSetNode.begin();it!=tmpSetNode.end();++it) {
      sizes->setNodeValue(*it,sizes->getNodeValue(*it)/2.0);
    }
  }
  bool result,cached;
  string err;
  DataSet tmpData;
  tmpData.set("complexity",false);
  Layout *tree=graph->getLocalProperty<LayoutProperty>("Bubble Tree",cached,result,err,0,&tmpData);
  if (cached) tree->recompute(err);
  *graph->getProperty<LayoutProperty>("viewLayout")=*tree;
  graph->delLocalProperty("Bubble Tree");
  glGraphWidget->redraw();
  Observable::unholdObservers();
  }

}

void MouseTreeFishEyes:: mReleaseEvent(GlGraphWidget *glGraphWidget,QMouseEvent *qMouseEv){
}
