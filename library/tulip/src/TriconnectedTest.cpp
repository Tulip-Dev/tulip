#include "tulip/Graph.h"
#include "tulip/TriconnectedTest.h"
#include "tulip/BiconnectedTest.h"
#include "tulip/MutableContainer.h"

using namespace std;
using namespace tlp;
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
bool TriconnectedTest::isTriconnected(Graph *graph) {
  if (instance==0)
    instance=new TriconnectedTest();
  return instance->compute(graph);
}
//=================================================================
bool TriconnectedTest::compute(Graph *graph) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)graph];
  if (graph->numberOfNodes()==0) return false;
  graph->addObserver(this);
  bool result = true;
  Graph *tmp = tlp::newCloneSubGraph(graph);
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    tmp->delNode(n);
    if (!BiconnectedTest::isBiconnected(tmp)) {
      result = false;
      break;
    }
    tmp->addNode(n);
    Iterator<edge> *itE = graph->getInOutEdges(n);
    while(itE->hasNext()) {
      tmp->addEdge(itE->next());
    } delete itE;
  } delete itN;
  graph->delSubGraph(tmp);
  resultsBuffer[(unsigned long)graph] = result;
  return result;
}
//=================================================================
void TriconnectedTest::addEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void TriconnectedTest::delEdge(Graph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void TriconnectedTest::reverseEdge(Graph *graph,const edge) {
}
//=================================================================
void TriconnectedTest::addNode(Graph *graph,const node) {
  resultsBuffer[(unsigned long)graph]=false;
}
//=================================================================
void TriconnectedTest::delNode(Graph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void TriconnectedTest::destroy(Graph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================

