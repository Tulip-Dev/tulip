#include "tulip/GraphTools.h"
#include "tulip/AcyclicTest.h"
#include "tulip/TreeTest.h"
#include "tulip/Graph.h"
#include "tulip/BooleanProperty.h"
#include "tulip/StableIterator.h"
#include "tulip/IntegerProperty.h"
#include "tulip/DoubleProperty.h"
#include <math.h>
using namespace std;
using namespace tlp;

void tlp::makeProperDag(Graph* graph,list<node> &addedNodes, 
			stdext::hash_map<edge,edge> &replacedEdges, IntegerProperty *edgeLength) {
  if (TreeTest::isTree(graph)) return;
  assert(AcyclicTest::isAcyclic(graph));
  //We compute the dag level metric on resulting sg.
  bool resultBool;
  string erreurMsg;
  DoubleProperty *dagLevel= new DoubleProperty(graph);
  resultBool = graph->computeProperty("DagLevel", dagLevel, erreurMsg);
  assert(resultBool);
  //we now transform the dag in a proper Dag, two linked nodes of a proper dag
  //must have a difference of one of dag level metric.
  node tmp1,tmp2;
  string tmpString;
  vector<edge> sgEdges(graph->numberOfEdges());
  Iterator<edge> *itE=graph->getEdges();
  for (int i=0;itE->hasNext();++i) {
    sgEdges[i]=itE->next();
  } delete itE;

  edgeLength->setAllEdgeValue(1);
  //  IntegerProperty *edgeLength=graph->getLocalProperty<IntegerProperty>("treeEdgeLength");
  for (vector<edge>::const_iterator itEdge=sgEdges.begin();itEdge!=sgEdges.end();++itEdge) {
    edge ite=*itEdge;
    int delta=(int)rint(dagLevel->getNodeValue(graph->target(ite))-dagLevel->getNodeValue(graph->source(ite)));
    if (delta>1) {
      tmp1=graph->addNode();
      replacedEdges[ite]=graph->addEdge(graph->source(ite),tmp1);
      addedNodes.push_back(tmp1);
      dagLevel->setNodeValue(tmp1,dagLevel->getNodeValue(graph->source(ite))+1);
      if (delta>2) {
	tmp2=graph->addNode();
	addedNodes.push_back(tmp2);
	edge e=graph->addEdge(tmp1,tmp2);
	edgeLength->setEdgeValue(e,delta-2);	  
	dagLevel->setNodeValue(tmp2,dagLevel->getNodeValue(graph->target(ite))-1);
	tmp1=tmp2;
      }
      graph->addEdge(tmp1,graph->target(ite));
    }
  }
  //  graph->getPropertyManager()->delLocalProperty("DagLevel");
  for (stdext::hash_map<edge,edge>::const_iterator it=replacedEdges.begin();it!=replacedEdges.end();++it) 
    graph->delEdge((*it).first);
   assert(AcyclicTest::isAcyclic(graph));
}

//le faire une fois le layer assignement et le barycenter efféctué
node tlp::makeSimpleSource(Graph* graph) {
  assert(AcyclicTest::isAcyclic(graph));
  node startNode=graph->addNode();
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if ((graph->indeg(itn)==0) && (itn!=startNode)) {
      graph->addEdge(startNode,itn);
    }
  }delete itN;
  assert(AcyclicTest::isAcyclic(graph));
  return startNode;
}



