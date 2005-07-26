#include "tulip/SuperGraph.h"
#include "tulip/ConnectedTest.h"
#include "tulip/MutableContainer.h"

using namespace std;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
ConnectedTest * ConnectedTest::instance=0;
#endif
#else
ConnectedTest * ConnectedTest::instance=0;
#endif
//=================================================================
bool ConnectedTest::isConnected(SuperGraph *graph) {
  if (instance==0)
    instance=new ConnectedTest();
  return instance->compute(graph);
}
//=================================================================
void ConnectedTest::makeConnected(SuperGraph *graph, vector<edge> &addedEdges) {
  if (instance==0)
    instance=new ConnectedTest();
  return instance->connect(graph, addedEdges);
}
//=================================================================
ConnectedTest::ConnectedTest(){}
//=================================================================
void connectedTest(SuperGraph *graph, node n, 
		   MutableContainer<bool> &visited,
		   unsigned int &count) {
  if (visited.get(n.id)) return;
  visited.set(n.id,true);
  ++count;
  Iterator<node> *it=graph->getInOutNodes(n);
  while (it->hasNext()) {
    connectedTest(graph, it->next(), visited, count);
  } delete it;
}
//=================================================================
bool ConnectedTest::compute(SuperGraph *graph) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned int)graph];
  if (graph->numberOfNodes()==0) return true;
  graph->addObserver(this);
  MutableContainer<bool> visited;
  visited.setAll(false);
  unsigned int count = 0;
  connectedTest(graph, graph->getOneNode(), visited, count);
  bool result = (count == graph->numberOfNodes());
  resultsBuffer[(unsigned int)graph]=result;
  return result;
}
//=================================================================
void ConnectedTest::connect(SuperGraph *graph, vector<edge> &addedEdges) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end()) {
    if (resultsBuffer[(unsigned int)graph])
      return;
    else
      graph->removeObserver(this);  
  }
  if (graph->numberOfNodes()==0) return;
  MutableContainer<bool> visited;
  visited.setAll(false);
  unsigned int count = 0;
  vector<node> toLink;
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    if (!visited.get(n.id)) {
      toLink.push_back(n);
      connectedTest(graph, n, visited, count);
    }
  }
  resultsBuffer[(unsigned int)graph] = true;
  for (unsigned int i = 1; i < toLink.size(); ++i)
    addedEdges.push_back(graph->addEdge(toLink[0], toLink[i]));
  graph->addObserver(this);  
}
//=================================================================
void ConnectedTest::addEdge(SuperGraph *graph,const edge) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned int)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void ConnectedTest::delEdge(SuperGraph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void ConnectedTest::reverseEdge(SuperGraph *graph,const edge) {
}
//=================================================================
void ConnectedTest::addNode(SuperGraph *graph,const node) {
  resultsBuffer[(unsigned int)graph]=false;
}
//=================================================================
void ConnectedTest::delNode(SuperGraph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void ConnectedTest::destroy(SuperGraph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================

