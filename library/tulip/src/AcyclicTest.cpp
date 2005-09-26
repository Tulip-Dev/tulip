#include "tulip/SuperGraph.h"
#include "tulip/AcyclicTest.h"
#include "tulip/SelectionProxy.h"

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
