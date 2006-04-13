#include "tulip/GraphTools.h"
#include "tulip/AcyclicTest.h"
#include "tulip/TreeTest.h"
#include "tulip/SuperGraph.h"
#include "tulip/Selection.h"
#include "tulip/StableIterator.h"
#include "tulip/Int.h"
#include "tulip/Metric.h"
#include <math.h>
using namespace std;

void tlp::makeProperDag(SuperGraph* superGraph,list<node> &addedNodes, 
			stdext::hash_map<edge,edge> &replacedEdges, Int *edgeLength) {
  if (TreeTest::isTree(superGraph)) return;
  assert(AcyclicTest::isAcyclic(superGraph));
  //We compute the dag level metric on resulting graph.
  bool resultBool;
  string erreurMsg;
  Metric *dagLevel= new Metric(superGraph);
  resultBool = superGraph->computeProperty("DagLevel", dagLevel, erreurMsg);
  assert(resultBool);
  //we now transform the dag in a proper Dag, two linked nodes of a proper dag
  //must have a difference of one of dag level metric.
  node tmp1,tmp2;
  string tmpString;
  vector<edge> graphEdges(superGraph->numberOfEdges());
  Iterator<edge> *itE=superGraph->getEdges();
  for (int i=0;itE->hasNext();++i) {
    graphEdges[i]=itE->next();
  } delete itE;

  edgeLength->setAllEdgeValue(1);
  //  Int *edgeLength=superGraph->getLocalProperty<Int>("treeEdgeLength");
  for (vector<edge>::const_iterator itEdge=graphEdges.begin();itEdge!=graphEdges.end();++itEdge) {
    edge ite=*itEdge;
    int delta=(int)rint(dagLevel->getNodeValue(superGraph->target(ite))-dagLevel->getNodeValue(superGraph->source(ite)));
    if (delta>1) {
      tmp1=superGraph->addNode();
      replacedEdges[ite]=superGraph->addEdge(superGraph->source(ite),tmp1);
      addedNodes.push_back(tmp1);
      dagLevel->setNodeValue(tmp1,dagLevel->getNodeValue(superGraph->source(ite))+1);
      if (delta>2) {
	tmp2=superGraph->addNode();
	addedNodes.push_back(tmp2);
	edge e=superGraph->addEdge(tmp1,tmp2);
	edgeLength->setEdgeValue(e,delta-2);	  
	dagLevel->setNodeValue(tmp2,dagLevel->getNodeValue(superGraph->target(ite))-1);
	tmp1=tmp2;
      }
      superGraph->addEdge(tmp1,superGraph->target(ite));
    }
  }
  //  superGraph->getPropertyManager()->delLocalProperty("DagLevel");
  for (stdext::hash_map<edge,edge>::const_iterator it=replacedEdges.begin();it!=replacedEdges.end();++it) 
    superGraph->delEdge((*it).first);
   assert(AcyclicTest::isAcyclic(superGraph));
}

//le faire une fois le layer assignement et le barycenter efféctué
node tlp::makeSimpleSource(SuperGraph* superGraph) {
  assert(AcyclicTest::isAcyclic(superGraph));
  node startNode=superGraph->addNode();
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if ((superGraph->indeg(itn)==0) && (itn!=startNode)) {
      superGraph->addEdge(startNode,itn);
    }
  }delete itN;
  assert(AcyclicTest::isAcyclic(superGraph));
  return startNode;
}



