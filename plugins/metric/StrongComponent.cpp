#include <cmath>
#include "StrongComponent.h"
#include <tulip/DoubleProperty.h>

DOUBLEPLUGINOFGROUP(StrongComponent,"Strongly Connected Component","David Auber","12/06/2001","Alpha","1.0","Component");

using namespace std;
using namespace tlp;

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
  Iterator<node> *itN=graph->getOutNodes(n);
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
      doubleResult->setNodeValue(tmp,curComponent);
    }
    finished[n]=true;
    doubleResult->setNodeValue(n,curComponent);
    curComponent++;
    renum.pop();
  }
  return result;
}

StrongComponent::StrongComponent(const PropertyContext &context):DoubleAlgorithm(context) {}

StrongComponent::~StrongComponent() {}

bool StrongComponent::run() {
  stdext::hash_map<node,bool> visited(graph->numberOfNodes());
  stdext::hash_map<node,bool> finished(graph->numberOfNodes());
  stack<node> renum;
  stdext::hash_map<node,int> cachedValues(graph->numberOfNodes());
  int id=1;
  int curComponent=0;
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (!visited[itn]) {attachNumerotation(itn,visited,finished,cachedValues,id,renum,curComponent);}
  } delete itN;


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

bool StrongComponent::check(string &erreurMsg) {
  erreurMsg="";
  return true;
}

void StrongComponent::reset() {
}
