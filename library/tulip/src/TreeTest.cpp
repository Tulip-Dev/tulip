#include "tulip/SuperGraph.h"
#include "tulip/TreeTest.h"
#include "tulip/AcyclicTest.h"


using namespace std;
#ifdef _WIN32 
#ifdef DLL_EXPORT
TreeTest * TreeTest::instance=0;
#endif
#else
TreeTest * TreeTest::instance=0;
#endif
TreeTest::TreeTest(){
}

bool TreeTest::isTree(SuperGraph *graph) {
  if (instance==0)
    instance=new TreeTest();
  return instance->compute(graph);
}


bool TreeTest::compute(SuperGraph *graph) { 
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) {
    return resultsBuffer[(unsigned long)graph];
  }
  if (graph->numberOfEdges()!=graph->numberOfNodes()-1) {
    resultsBuffer[(unsigned long)graph]=false;
    graph->addObserver(this);
    return false;
  }
  bool rootNodeFound=false;
  Iterator<node> *it=graph->getNodes();
  while (it->hasNext()) {
    node tmp=it->next();
    if (graph->indeg(tmp)>1) {
      delete it;
      resultsBuffer[(unsigned long)graph]=false;
      graph->addObserver(this);
      return false;
    }
    if (graph->indeg(tmp)==0) {
      if (rootNodeFound) {
	delete it;
	resultsBuffer[(unsigned long)graph]=false;
	graph->addObserver(this);
	return false;
      }
      else
	rootNodeFound=true;
    }
  } delete it;
  if (AcyclicTest::isAcyclic(graph)) {
    resultsBuffer[(unsigned long)graph]=true;
    graph->addObserver(this);
    return true;
  }
  else {
    resultsBuffer[(unsigned long)graph]=false;
    graph->addObserver(this);
    return false;
  }
}

void TreeTest::addEdge(SuperGraph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::delEdge(SuperGraph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::reverseEdge(SuperGraph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::addNode(SuperGraph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::delNode(SuperGraph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}

void TreeTest::destroy(SuperGraph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
