#include <assert.h>
#include <queue>

#include <tulip/TulipPlugin.h>
#include <tulip/TreeTest.h>
#include "PathSelection.h"
#include "LongestPath.h"

SELECTIONPLUGIN(LongestPath,"Longestpath","Maylis Delest","02/06/2003","Alpha","0","1");

using namespace std;

LongestPath::LongestPath(const PropertyContext &context):SelectionAlgorithm(context)  {
}

LongestPath::~LongestPath() {
}

void LongestPath::setNodeValue(node n) {
    selectionObj->setNodeValue(n,true);
    Iterator<edge> *itE=superGraph->getOutEdges(n);
    for (;itE->hasNext();){
      edge e=itE->next();
      node n=superGraph->target(e);
      if (max==metricLevel->getNodeValue(n)) {
	selectionObj->setEdgeValue(e,true);
	setNodeValue(n);
      }
    }delete itE;
}

bool LongestPath::run() {
  selectionObj->setAllNodeValue(false);
  selectionObj->setAllEdgeValue(false);
  bool cached=false;
  bool result=false;
  string erreur;
  metricLevel=superGraph->getLocalProperty<Metric>("MaxLength", cached,result,erreur);
  Selection *root=superGraph->getProperty<Selection>("viewSelection");
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
