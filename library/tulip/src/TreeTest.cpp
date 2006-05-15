#include "tulip/Graph.h"
#include "tulip/TreeTest.h"
#include "tulip/AcyclicTest.h"


using namespace std;
using namespace tlp;
#ifdef _WIN32 
#ifdef DLL_EXPORT
TreeTest * TreeTest::instance=0;
#endif
#else
TreeTest * TreeTest::instance=0;
#endif
TreeTest::TreeTest(){
}

bool TreeTest::isTree(Graph *sg) {
  if (instance==0)
    instance=new TreeTest();
  return instance->compute(sg);
}


bool TreeTest::compute(Graph *sg) { 
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end()) {
    return resultsBuffer[(unsigned long)sg];
  }
  if (sg->numberOfEdges()!=sg->numberOfNodes()-1) {
    resultsBuffer[(unsigned long)sg]=false;
    sg->addObserver(this);
    return false;
  }
  bool rootNodeFound=false;
  Iterator<node> *it=sg->getNodes();
  while (it->hasNext()) {
    node tmp=it->next();
    if (sg->indeg(tmp)>1) {
      delete it;
      resultsBuffer[(unsigned long)sg]=false;
      sg->addObserver(this);
      return false;
    }
    if (sg->indeg(tmp)==0) {
      if (rootNodeFound) {
	delete it;
	resultsBuffer[(unsigned long)sg]=false;
	sg->addObserver(this);
	return false;
      }
      else
	rootNodeFound=true;
    }
  } delete it;
  if (AcyclicTest::isAcyclic(sg)) {
    resultsBuffer[(unsigned long)sg]=true;
    sg->addObserver(this);
    return true;
  }
  else {
    resultsBuffer[(unsigned long)sg]=false;
    sg->addObserver(this);
    return false;
  }
}

void TreeTest::addEdge(Graph *sg,const edge) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
void TreeTest::delEdge(Graph *sg,const edge) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
void TreeTest::reverseEdge(Graph *sg,const edge) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
void TreeTest::addNode(Graph *sg,const node) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
void TreeTest::delNode(Graph *sg,const node) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}

void TreeTest::destroy(Graph *sg) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
