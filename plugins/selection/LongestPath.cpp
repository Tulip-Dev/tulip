#include <assert.h>
#include <queue>

#include <tulip/TulipPlugin.h>
#include <tulip/TreeTest.h>
#include "PathBooleanProperty.h"
#include "LongestPath.h"

BOOLEANPLUGIN(LongestPath,"Longestpath","Maylis Delest","02/06/2003","Alpha","0","1");

using namespace std;
using namespace tlp;

LongestPath::LongestPath(const PropertyContext &context):BooleanAlgorithm(context)  {
}

LongestPath::~LongestPath() {
}

void LongestPath::setNodeValue(node n) {
    BooleanResult->setNodeValue(n,true);
    Iterator<edge> *itE=graph->getOutEdges(n);
    for (;itE->hasNext();){
      edge e=itE->next();
      node n=graph->target(e);
      if (max==metricLevel->getNodeValue(n)) {
	BooleanResult->setEdgeValue(e,true);
	setNodeValue(n);
      }
    }delete itE;
}

bool LongestPath::run() {
  BooleanResult->setAllNodeValue(false);
  BooleanResult->setAllEdgeValue(false);
  bool cached=false;
  bool result=false;
  string erreur;
  metricLevel=graph->getLocalProperty<DoubleProperty>("MaxLength", cached,result,erreur);
  Selection *root=graph->getProperty<BooleanProperty>("viewSelection");
  Iterator<node> *itN=graph->getNodes();
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
  if (TreeTest::isTree(graph)) {
    err = "";
    return true;
  }
  else {
    err="The graph must be a tree";
    return false;
  }
}
