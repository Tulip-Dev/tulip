#include <assert.h>
#include <queue>

#include <tulip/TulipPlugin.h>
#include <tulip/TreeTest.h>
#include "PathSelection.h"
#include "LongestPath.h"
#include <tulip/SelectionProxy.h>
#include <tulip/MethodFactory.h>

SELECTIONPLUGIN(LongestPath,"Longestpath","Maylis Delest","02/06/2003","Alpha","0","1");

using namespace std;

LongestPath::LongestPath(const PropertyContext &context):Selection(context)  {
}

LongestPath::~LongestPath() {
}

void LongestPath::setNodeValue(node n) {
    selectionProxy->setNodeValue(n,true);
    Iterator<edge> *itE=superGraph->getOutEdges(n);
    for (;itE->hasNext();){
      edge e=itE->next();
      node n=superGraph->target(e);
      if (max==metricLevel->getNodeValue(n)) {
	selectionProxy->setEdgeValue(e,true);
	setNodeValue(n);
      }
    }delete itE;
}

bool LongestPath::run() {
  selectionProxy->setAllNodeValue(false);
  selectionProxy->setAllEdgeValue(false);
  bool cached=false;
  bool result=false;
  string erreur;
  metricLevel=superGraph->getLocalProperty<MetricProxy>("MaxLength", cached,result,erreur);
  SelectionProxy *root=superGraph->getProperty<SelectionProxy>("viewSelection");
  Iterator<node> *itN=superGraph->getNodes();
  node startNode;
  for  (; itN->hasNext();){
    startNode=itN->next();
    if (root->getNodeValue(startNode)) break;
  }delete itN;
  max=metricLevel->getNodeValue(startNode);
  setNodeValue(startNode);
  return true;
}

bool LongestPath::check(string &err) {
  if (TreeTest::isTree(superGraph)) {
    err = "";
    return true;
  }
  else {
    err="The graph must be a tree";
    return false;
  }
}
