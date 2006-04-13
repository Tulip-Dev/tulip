
#include <tulip/ForEach.h>
#include "ConnectedComponent.h"


METRICPLUGIN(ConnectedComponent,"Connected Component","David Auber","01/07/2002","Alpha","0","1");

ConnectedComponent::ConnectedComponent(const PropertyContext &context):MetricAlgorithm(context) {}
//======================================================
ConnectedComponent::~ConnectedComponent(){}
//======================================================
void ConnectedComponent::dfs(node n, MutableContainer<bool> &flag,double value){
  if (flag.get(n.id)) return;
  flag.set(n.id, true);
  metricResult->setNodeValue(n,value);
  node itn;
  forEach(itn, superGraph->getInOutNodes(n))
    dfs(itn,flag,value);
}
//======================================================
bool ConnectedComponent::run() {
  MutableContainer<bool> flag;
  flag.setAll(false);
  double curComponent=0;
  node itn;
  forEach(itn, superGraph->getNodes()) {
    if (!flag.get(itn.id)) {
      dfs(itn,flag,curComponent);
      curComponent++;
    }
  }
  Iterator<edge> *itE=superGraph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    node source= superGraph->source(ite);
    node target= superGraph->target(ite);
    if (metricResult->getNodeValue(source)==metricResult->getNodeValue(target))
      metricResult->setEdgeValue(ite,metricResult->getNodeValue(source));
    else
      metricResult->setEdgeValue(ite,curComponent);
  } delete itE;

  return true;
}
//======================================================







