#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "ConnectedComponent.h"


METRICPLUGIN(ConnectedComponent,"Connected Component","David Auber","01/07/2002","Alpha","0","1");

ConnectedComponent::ConnectedComponent(const PropertyContext &context):Metric(context) {}

ConnectedComponent::~ConnectedComponent(){}

void ConnectedComponent::dfs(node n, stdext::hash_map<node,bool> &flag,double value){
  if (flag.find(n)!=flag.end()) return;
  flag[n]=true;
  metricProxy->setNodeValue(n,value);
  Iterator<node> *itN=superGraph->getInOutNodes(n);
  for (;itN->hasNext();) {
    node itn=itN->next();
    dfs(itn,flag,value);
  }delete itN;
}


bool ConnectedComponent::run() {
  stdext::hash_map<node,bool> flag;

  double curComponent=0;
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (flag.find(itn)==flag.end()) {
      dfs(itn,flag,curComponent);
      curComponent++;
    }
  } delete itN;

  Iterator<edge> *itE=superGraph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    node source= superGraph->source(ite);
    node target= superGraph->target(ite);
    if (metricProxy->getNodeValue(source)==metricProxy->getNodeValue(target))
      metricProxy->setEdgeValue(ite,metricProxy->getNodeValue(source));
    else
      metricProxy->setEdgeValue(ite,curComponent);
  } delete itE;

  return true;
}








