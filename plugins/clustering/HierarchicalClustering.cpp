#include <math.h>
#include <sstream>
#include <list>

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>

#include "HierarchicalClustering.h"

using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(HierarchicalClustering,"Hierarchical","David Auber","27/01/2000","Alpha","0","1");

//================================================================================
HierarchicalClustering::HierarchicalClustering(AlgorithmContext context):Algorithm(context)
{}
//================================================================================
HierarchicalClustering::~HierarchicalClustering()
{}
//================================================================================

class LessThan {
public:
  DoubleProperty *metric;
  bool operator() (node n1,node n2) {
    return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
  }
};

bool HierarchicalClustering::split(DoubleProperty *metric,list<node> &orderedNode) {
  Iterator<node> *itN=graph->getNodes();
  for (;itN->hasNext();)
    orderedNode.push_back(itN->next());
  delete itN;

  LessThan comp;
  comp.metric=metric;
  orderedNode.sort(comp);

  list<node>::iterator itListNode;
  double tmpDbl;

  //We split the sorted list in two part
  int nbElement=orderedNode.size();
  nbElement/=2;
  if (nbElement<10) return (true);
  itListNode=orderedNode.begin();
  double tmpDblStart;
  tmpDbl=metric->getNodeValue(*itListNode);
  tmpDblStart=tmpDbl;
  int n=0;
  ++itListNode;
  nbElement--;
  while ((itListNode!=orderedNode.end()) && ((nbElement>0) || (tmpDbl==metric->getNodeValue(*itListNode))) ) {
    tmpDbl=metric->getNodeValue(*itListNode);
    ++itListNode;
    n++;
    nbElement--;
  }
  orderedNode.erase(itListNode,orderedNode.end());
  return false;
}
//================================================================================
bool HierarchicalClustering::run() {

  string tmp1,tmp2;
  DoubleProperty *metric=graph->getProperty<DoubleProperty>("viewMetric");
  tmp1="Hierar Sup";
  tmp2="Hierar Inf";
  bool result=false;
  
  while (!result) {
    list<node> badNodeList;
    result = split(metric,badNodeList);
    if (!result) {
      BooleanProperty *sel1 =graph->getLocalProperty<BooleanProperty>("good select");
      BooleanProperty *sel2 =graph->getLocalProperty<BooleanProperty>("bad select");
      BooleanProperty *splitRes =graph->getLocalProperty<BooleanProperty>("split result");

      sel1->setAllNodeValue(true);
      sel1->setAllEdgeValue(true);
      sel2->setAllNodeValue(true);
      sel2->setAllEdgeValue(true);
      splitRes->setAllNodeValue(true);
      splitRes->setAllEdgeValue(true);

      list<node>::iterator itl;
      for (itl=badNodeList.begin();itl!=badNodeList.end();++itl)
	splitRes->setNodeValue(*itl,false);

      Iterator<node> *itN=graph->getNodes();
      for (;itN->hasNext();) {
	node nit=itN->next();
	if (splitRes->getNodeValue(nit)) {
	  sel2->setNodeValue(nit,false);
	  Iterator<edge> *itE=graph->getInOutEdges(nit);
	  for (;itE->hasNext();) {
	    edge ite=itE->next();
	    sel2->setEdgeValue(ite,false);
	  }delete itE;
	}
	else {
	  sel1->setNodeValue(nit,false);
	  Iterator<edge> *itE=graph->getInOutEdges(nit);
	  for (;itE->hasNext();) {
	    edge ite=itE->next();
	    sel1->setEdgeValue(ite,false);
	  } delete itE;
	}
      }delete itN;

      Graph * tmpSubGraph;
      tmpSubGraph = graph->addSubGraph(sel1);
      tmpSubGraph->setAttribute<string>("name",tmp1);
      (graph->addSubGraph(sel2))->setAttribute<string>("name",tmp2);
      graph->delLocalProperty("good select");
      graph->delLocalProperty("bad select");
      graph->delLocalProperty("split result");
      graph=tmpSubGraph;
    }
  }
  return true;
}
//================================================================================
bool HierarchicalClustering::check(string &erreurMsg)
{
  erreurMsg="";
  return true;
}
//================================================================================
void HierarchicalClustering::reset()
{
}
//================================================================================




