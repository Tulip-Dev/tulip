#include "tulip/Graph.h"
#include "tulip/TriconnectedTest.h"
#include "tulip/BiconnectedTest.h"
#include "tulip/MutableContainer.h"
#include "tulip/TlpTools.h"

using namespace std;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
TriconnectedTest * TriconnectedTest::instance=0;
#endif
#else
TriconnectedTest * TriconnectedTest::instance=0;
#endif
//=================================================================
TriconnectedTest::TriconnectedTest(){
}
//=================================================================
bool TriconnectedTest::isTriconnected(Graph *sg) {
  if (instance==0)
    instance=new TriconnectedTest();
  return instance->compute(sg);
}
//=================================================================
bool TriconnectedTest::compute(Graph *sg) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)sg];
  if (sg->numberOfNodes()==0) return false;
  sg->addObserver(this);
  bool result = true;
  Graph *tmp = tlp::newCloneSubGraph(sg);
  Iterator<node> *itN = sg->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    tmp->delNode(n);
    if (!BiconnectedTest::isBiconnected(tmp)) {
      result = false;
      break;
    }
    tmp->addNode(n);
    Iterator<edge> *itE = sg->getInOutEdges(n);
    while(itE->hasNext()) {
      tmp->addEdge(itE->next());
    } delete itE;
  } delete itN;
  sg->delSubGraph(tmp);
  resultsBuffer[(unsigned long)sg] = result;
  return result;
}
//=================================================================
void TriconnectedTest::addEdge(Graph *sg,const edge) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void TriconnectedTest::delEdge(Graph *sg,const edge) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void TriconnectedTest::reverseEdge(Graph *sg,const edge) {
}
//=================================================================
void TriconnectedTest::addNode(Graph *sg,const node) {
  resultsBuffer[(unsigned long)sg]=false;
}
//=================================================================
void TriconnectedTest::delNode(Graph *sg,const node) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void TriconnectedTest::destroy(Graph *sg) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================

