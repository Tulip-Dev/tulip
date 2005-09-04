#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/PluginProgress.h>
#include <tulip/SuperGraph.h>
#include <tulip/PropertyManager.h>
#include <tulip/SelectionProxy.h>
#include <tulip/LayoutProxy.h>
#include <tulip/SizesProxy.h>
#include <tulip/MetricProxy.h>
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
  SuperGraph *superGraph=glGraphWidget->getGlGraph()->getSuperGraph();
  set<node> tmpSetNode;
  set<edge> tmpSetEdge;
  double centX= (x+x+w)/2;
  double centY= (y+y-h)/2;
  if (w<0) w*=-1;
  if (h<0) h*=-1;
  glGraphWidget->getGlGraph()->doSelect((int)centX,(int)centY,(int)w,(int)h,tmpSetNode,tmpSetEdge);

  if (tmpSetNode.empty()) return;

  //  SizesProxy *sizes=superGraph->getProperty<SizesProxy>("viewSize");
  MetricProxy *sizes=superGraph->getProperty<MetricProxy>("viewMetric");
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
  LayoutProxy *tree=superGraph->getLocalProperty<LayoutProxy>("Bubble Tree",cached,result,err,0,&tmpData);
  if (cached) tree->recompute(err);
  *superGraph->getProperty<LayoutProxy>("viewLayout")=*tree;
  superGraph->delLocalProperty("Bubble Tree");
  glGraphWidget->redraw();
  Observable::unholdObservers();
  }

}

void MouseTreeFishEyes:: mReleaseEvent(GlGraphWidget *glGraphWidget,QMouseEvent *qMouseEv){
}
