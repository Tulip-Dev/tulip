#include "tulip/Graph.h"
#include "tulip/StableIterator.h"
#include "tulip/BiconnectedTest.h"
#include "tulip/ConnectedTest.h"
#include "tulip/MutableContainer.h"

using namespace std;
using namespace tlp;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
BiconnectedTest * BiconnectedTest::instance=0;
#endif
#else
BiconnectedTest * BiconnectedTest::instance=0;
#endif

//=================================================================
bool BiconnectedTest::isBiconnected(Graph *graph) {
  if (instance==0)
    instance=new BiconnectedTest();
  return instance->compute(graph);
}
//=================================================================
void BiconnectedTest::makeBiconnected(Graph *graph, vector<edge> &addedEdges) {
  //  cerr << __PRETTY_FUNCTION__ << " : " << graph->getAttribute<string>("name") << endl;
  //  if (BiconnectedTest::isBiconnected(graph)) return;
  if (instance==0) instance=new BiconnectedTest();
  graph->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned long)graph);  
  instance->connect(graph, addedEdges);
  assert(BiconnectedTest::isBiconnected(graph));
}
//=================================================================
BiconnectedTest::BiconnectedTest(){
}
//=================================================================
void makeBiconnectedDFS(Graph *graph, node from, 
			MutableContainer<int> &low,
			MutableContainer<int> &dfsNumber,
			MutableContainer<node> &supergraph,
			unsigned int &count,
			vector<edge> &addedEdges) {
  node u;
  dfsNumber.set(from.id, count++);
  low.set(from.id, dfsNumber.get(from.id));
  StableIterator<node> itN(graph->getInOutNodes(from));
  while (itN.hasNext()) {
    node w = itN.next();
    if (from == w) continue;
    if (!u.isValid()) u = w;
    if (dfsNumber.get(w.id) == -1) {
      supergraph.set(w.id, from);
      makeBiconnectedDFS(graph, w, low, dfsNumber, supergraph, count, addedEdges);
      if (low.get(w.id) == dfsNumber.get(from.id)) {
	if (w == u && supergraph.get(from.id).isValid())
	  addedEdges.push_back(graph->addEdge(w, supergraph.get(from.id)));
	if (w != u) 
	  addedEdges.push_back(graph->addEdge(u, w));
      }
      low.set(from.id, std::min(low.get(from.id), low.get(w.id)));
    }
    else
      low.set(from.id, std::min(low.get(from.id), dfsNumber.get(w.id)));
  } 
}
//=================================================================
bool biconnectedTest(Graph *graph, node v, 
		     MutableContainer<bool> &mark,
		     MutableContainer<unsigned int> &low,
		     MutableContainer<unsigned int> &dfsNumber,
		     MutableContainer<node> &supergraph,
		     unsigned int &count) {
  mark.set(v.id,true);
  dfsNumber.set(v.id,count);
  low.set(v.id,count);
  ++count;
  Iterator<node> *it=graph->getInOutNodes(v);
  while (it->hasNext()) {
    node w=it->next();
    if (!mark.get(w.id)) {
      if (dfsNumber.get(v.id)==1) {
	if (count != 2) {
	  //  cerr <<"Root Node :" << v.id << " is a separator" << endl;
	  delete it;
	  return false;
	}
      }
      supergraph.set(w.id,v);
      if (!biconnectedTest(graph,w,mark,low,dfsNumber,supergraph,count)) {
	delete it;
	return false;
      }
      if (dfsNumber.get(v.id)!=1) {
	if (low.get(w.id)>=dfsNumber.get(v.id)) {
	  delete it;
	  //cerr <<"Node :" << v.id << " is a separator" << endl;
	  return false;
	}
	else
	  low.set(v.id, std::min(low.get(v.id), low.get(w.id)));
      }
    }
    else
      if (supergraph.get(v.id)!=w) {
	low.set(v.id, std::min(low.get(v.id), dfsNumber.get(w.id)));
      }
  } delete it;
  return true;
}
//=================================================================
void BiconnectedTest::connect(Graph *graph, vector<edge> &addedEdges) {
  //  cerr << __PRETTY_FUNCTION__ << " : " << graph->getAttribute<string>("name") << endl;
  ConnectedTest::makeConnected(graph, addedEdges);
  MutableContainer<int> low;
  MutableContainer<int> dfsNumber;
  dfsNumber.setAll(-1);
  MutableContainer<node> supergraph;
  supergraph.setAll(node());
  unsigned int count = 0;
  node root = graph->getOneNode();
  if (root.isValid())
    makeBiconnectedDFS(graph, root, low, dfsNumber,
		       supergraph, count, addedEdges);
}
//=================================================================
bool BiconnectedTest::compute(Graph *graph) { 
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)graph];

  MutableContainer<bool> mark;
  mark.setAll(false);
  MutableContainer<unsigned int> low;
  MutableContainer<unsigned int> dfsNumber;
  MutableContainer<node> supergraph;
  unsigned int count = 1;
  bool result = false;
  Iterator<node> *it=graph->getNodes();
  if (it->hasNext())
    result=(biconnectedTest(graph,it->next(),mark,low,dfsNumber,supergraph,count));
  delete it;
  if (count!=graph->numberOfNodes()+1) {
    //  cerr << "connected test" << endl; 
    result=false;
  } //connected test
  resultsBuffer[(unsigned long)graph]=result;
  graph->addObserver(this);
  return result;
}
//=================================================================
void BiconnectedTest::addEdge(Graph *graph,const edge) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void BiconnectedTest::delEdge(Graph *graph,const edge) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void BiconnectedTest::reverseEdge(Graph *graph,const edge) {
}
//=================================================================
void BiconnectedTest::addNode(Graph *graph,const node) {
  //  cerr << __PRETTY_FUNCTION__  << (unsigned)graph << endl;
  resultsBuffer[(unsigned long)graph]=false;
}
//=================================================================
void BiconnectedTest::delNode(Graph *graph,const node) {
  //  cerr << __PRETTY_FUNCTION__  << (unsigned)graph << endl;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void BiconnectedTest::destroy(Graph *graph) {
  //  cerr << __PRETTY_FUNCTION__ << (unsigned)graph << endl;
  //  cerr << "Graph name : " << graph->getAttribute<string>("name") << endl;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================

