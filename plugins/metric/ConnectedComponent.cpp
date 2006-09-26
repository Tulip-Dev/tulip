
#include <tulip/ForEach.h>
#include "ConnectedComponent.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(ConnectedComponent,"Connected Component","David Auber","01/07/2002","Alpha","0","1","Component");

ConnectedComponent::ConnectedComponent(const PropertyContext &context):DoubleAlgorithm(context) {}
//======================================================
ConnectedComponent::~ConnectedComponent(){}
//======================================================
void ConnectedComponent::dfs(node n, MutableContainer<bool> &flag,double value){
  if (flag.get(n.id)) return;
  flag.set(n.id, true);
  doubleResult->setNodeValue(n,value);
  node itn;
  forEach(itn, graph->getInOutNodes(n))
    dfs(itn,flag,value);
}
//======================================================
bool ConnectedComponent::run() {
  MutableContainer<bool> flag;
  flag.setAll(false);
  double curComponent=0;
  node itn;
  forEach(itn, graph->getNodes()) {
    if (!flag.get(itn.id)) {
      dfs(itn,flag,curComponent);
      curComponent++;
    }
  }
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    node source= graph->source(ite);
    node target= graph->target(ite);
    if (doubleResult->getNodeValue(source)==doubleResult->getNodeValue(target))
      doubleResult->setEdgeValue(ite,doubleResult->getNodeValue(source));
    else
      doubleResult->setEdgeValue(ite,curComponent);
  } delete itE;

  return true;
}
//======================================================







