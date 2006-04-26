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
  //  cerr << __PRETTY_FUNCTION__ << " : " << graph->getAttribute<string>("name") << endl;
  if (instance==0)
    instance=new ConnectedTest();
  graph->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned long)graph);  
  vector<node> toLink;  
  instance->connect(graph, toLink);
  for (unsigned int i = 1; i < toLink.size(); ++i)
    addedEdges.push_back(graph->addEdge(toLink[i-1], toLink[i]));
  assert(ConnectedTest::isConnected(graph));
}
//=================================================================
unsigned int ConnectedTest::numberOfConnectedComponnents(SuperGraph *graph) {
  if (graph->numberOfNodes()==0) return 0u;
  if (instance==0)
    instance=new ConnectedTest();
  graph->removeObserver(instance);
  vector<node> toLink;
  instance->connect(graph, toLink);
  unsigned int result;
  if (toLink.size()>0) 
    result =  toLink.size();
  else 
    result = 1u;
  instance->resultsBuffer[(unsigned long)graph] = (result == 1u);
  graph->addObserver(instance);
  return result;
}
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
ConnectedTest::ConnectedTest(){}
//=================================================================
bool ConnectedTest::compute(SuperGraph *graph) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)graph];
  if (graph->numberOfNodes()==0) return true;
  MutableContainer<bool> visited;
  visited.setAll(false);
  unsigned int count = 0;
  connectedTest(graph, graph->getOneNode(), visited, count);
  bool result = (count == graph->numberOfNodes());
  resultsBuffer[(unsigned long)graph]=result;
  graph->addObserver(this);
  return result;
}
//=================================================================
void ConnectedTest::connect(SuperGraph *graph, vector<node> &toLink) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) {
    if (resultsBuffer[(unsigned long)graph])
      return;
  }
  if (graph->numberOfNodes()==0) return;
  MutableContainer<bool> visited;
  visited.setAll(false);
  unsigned int count = 0;
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    if (!visited.get(n.id)) {
      toLink.push_back(n);
      connectedTest(graph, n, visited, count);
    }
  }
}
//=================================================================
void ConnectedTest::addEdge(SuperGraph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void ConnectedTest::delEdge(SuperGraph *graph,const edge) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void ConnectedTest::reverseEdge(SuperGraph *graph,const edge) {
}
//=================================================================
void ConnectedTest::addNode(SuperGraph *graph,const node) {
  resultsBuffer[(unsigned long)graph]=false;
}
//=================================================================
void ConnectedTest::delNode(SuperGraph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void ConnectedTest::destroy(SuperGraph *graph) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================

