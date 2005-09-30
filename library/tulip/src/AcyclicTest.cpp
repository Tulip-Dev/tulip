#include "tulip/SuperGraph.h"
#include "tulip/AcyclicTest.h"
#include "tulip/SelectionProxy.h"
#include "tulip/StableIterator.h"

using namespace std;

#ifdef _WIN32 
#ifdef DLL_EXPORT
AcyclicTest * AcyclicTest::instance=0;
#endif
#else
AcyclicTest * AcyclicTest::instance=0;
#endif
//**********************************************************************
AcyclicTest::AcyclicTest(){
}
//**********************************************************************
bool AcyclicTest::isAcyclic(SuperGraph *graph) {
  if (instance==0)
    instance=new AcyclicTest();
  return instance->compute(graph);
}
//**********************************************************************
void AcyclicTest::makeAcyclic(SuperGraph* graph,set<edge> &reversed, list<tlp::SelfLoops> &selfLoops) {
  if (AcyclicTest::isAcyclic(graph)) return;
  string erreurMsg;
  SelectionProxy spanningDag(graph);
  graph->computeProperty("SpanningDag", &spanningDag, erreurMsg);
  StableIterator<edge> itE(graph->getEdges());
  //We replace self loops by three edges an two nodes.
  while (itE.hasNext()) {
    edge ite=itE.next();
    if ((spanningDag.getEdgeValue(ite))==false) {
      if (graph->source(ite)==graph->target(ite)) {
	node n1=graph->addNode();
	node n2=graph->addNode();
	selfLoops.push_back(tlp::SelfLoops(n1 ,
				      n2 , 
				      graph->addEdge(graph->source(ite),n1) , 
				      graph->addEdge(n1,n2) , 
				      graph->addEdge(graph->source(ite),n2) , 
				      ite ));
	graph->delEdge(ite);
      }
      else {
	reversed.insert(ite);
	graph->reverse(ite);
      }
    }
  }
  assert(AcyclicTest::isAcyclic(graph));
 }
//**********************************************************************
bool AcyclicTest::acyclicTest(SuperGraph *graph,node n,SelectionProxy *visited,SelectionProxy *finished) {
  bool result=true;
  visited->setNodeValue(n,true);
  Iterator<node> *it=graph->getOutNodes(n);
  while (it->hasNext()) {
    node tmp=it->next();
    if ((visited->getNodeValue(tmp))==true) {
      if ((finished->getNodeValue(tmp))==false) {
	delete it;
	return false;
      }
    }
    else {
      result=result && acyclicTest(graph,tmp,visited,finished);
      if (result==false) {
	delete it;
	return false;
      }
    }
  } delete it;
  finished->setNodeValue(n,true);
  return true;
}
//**********************************************************************
bool AcyclicTest::compute(SuperGraph *graph) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end()) return resultsBuffer[(unsigned int)graph];
  SelectionProxy visited(graph);
  SelectionProxy finished(graph);
  visited.setAllNodeValue(false);
  finished.setAllNodeValue(false);
  bool result=true;
  Iterator<node> *it=graph->getNodes();
  while (it->hasNext()) {
    node curNode=it->next();
    if (!visited.getNodeValue(curNode)) {
      result = result && acyclicTest(graph,curNode,&visited,&finished);
      if (!result) {
	delete it;
	resultsBuffer[(unsigned int)graph]=false;
	graph->addObserver(this);
	return false;
      }
    }
  } delete it;
  if (result==true) {
    resultsBuffer[(unsigned int)graph]=true;
    graph->addObserver(this);
  }
  return true;
}
//**********************************************************************
void AcyclicTest::destroy(SuperGraph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//**********************************************************************
void AcyclicTest::reverseEdge(SuperGraph *graph,const edge e) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//**********************************************************************
void AcyclicTest::addEdge(SuperGraph *graph,const edge e) {
  if (resultsBuffer[(unsigned int)graph]==false) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//**********************************************************************
void AcyclicTest::delEdge(SuperGraph *graph,const edge e) {
  if (resultsBuffer[(unsigned int)graph]==true) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//**********************************************************************
