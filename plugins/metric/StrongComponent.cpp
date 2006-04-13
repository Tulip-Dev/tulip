#include <cmath>
#include "StrongComponent.h"
#include <tulip/Metric.h>

METRICPLUGIN(StrongComponent,"Strongly Connected Component","David Auber","12/06/2001","Alpha","0","1");

using namespace std;

int StrongComponent::attachNumerotation(node n,
					stdext::hash_map<node,bool> &visited,
					stdext::hash_map<node,bool> &finished,
					stdext::hash_map<node,int> &minAttach,
					int &id,
					stack<node> &renum,
					int &curComponent
					)
{
  if (visited[n]) return minAttach[n];
  visited[n]=true;
  int myId=id;  
  id++;
  minAttach[n]=myId;
  renum.push(n);
  int result=myId;
  Iterator<node> *itN=superGraph->getOutNodes(n);
  for (;itN->hasNext();) {
    node tmpN=itN->next();
    if (!finished[tmpN]) {
      int tmp=attachNumerotation(tmpN,visited,finished,minAttach,id,renum,curComponent);
      if (result>tmp) result=tmp;
    }
  } delete itN;
  minAttach[n]=result;
  if (result==myId) {
    while (renum.top()!=n) {
      node tmp=renum.top();
      renum.pop();
      finished[tmp]=true;
      minAttach[tmp]=result;
      metricResult->setNodeValue(tmp,curComponent);
    }
    finished[n]=true;
    metricResult->setNodeValue(n,curComponent);
    curComponent++;
    renum.pop();
  }
  return result;
}

StrongComponent::StrongComponent(const PropertyContext &context):MetricAlgorithm(context) {}

StrongComponent::~StrongComponent() {}

bool StrongComponent::run() {
  stdext::hash_map<node,bool> visited(superGraph->numberOfNodes());
  stdext::hash_map<node,bool> finished(superGraph->numberOfNodes());
  stack<node> renum;
  stdext::hash_map<node,int> cachedValues(superGraph->numberOfNodes());
  int id=1;
  int curComponent=0;
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (!visited[itn]) {attachNumerotation(itn,visited,finished,cachedValues,id,renum,curComponent);}
  } delete itN;


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

bool StrongComponent::check(string &erreurMsg) {
  erreurMsg="";
  return true;
}

void StrongComponent::reset() {
}
