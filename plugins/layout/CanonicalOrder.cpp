#include <tulip/SuperGraph.h>
#include <tulip/MapIterator.h>
#include <tulip/TlpTools.h>

using namespace std;

//==========================================================================
namespace {
  bool haveChoords(SuperGraph *graph, const node n, const set<node> &outerface){
    assert(graph->isElement(n));
    Iterator<node> *it = graph->getInOutNodes(n);
    unsigned int count =0;
    while (count < 3 && it->hasNext()) {
      if (outerface.find(it->next())!=outerface.end())
	++count;
    } delete it;
    return count > 2;
  }
}
//==========================================================================
namespace tlp {
  void canonicalOrdering(SuperGraph *graph, vector<node> &order) {
    //input embed graph planar maximum 
    unsigned int nbNodes = graph->numberOfNodes();
    order.resize(nbNodes);
    //choose V0,V1, Vn-1
    edge e = graph->getOneEdge();
    order[0] = graph->source(e);
    order[1] = graph->target(e);
    edge e2 = tlp::nextFaceEdge(graph, e, order[1]);
    order[nbNodes-1] = graph->opposite(e2, order[1]);
    SuperGraph *subgraph = tlp::newCloneSubGraph(graph);
    cerr << subgraph->numberOfNodes() << "," << subgraph->numberOfEdges() << endl;
    //  cerr << subgraph << endl;
    set<node> candidates;
    candidates.insert(order[nbNodes -1]);
    candidates.insert(order[0]);
    candidates.insert(order[1]);
    unsigned int k = nbNodes - 1;
    while (k>2) {
      set<node>::iterator it = candidates.begin();
      for (;it!=candidates.end();++it) {
	if (*it!=order[0] && *it!= order[1] && !haveChoords(subgraph, *it, candidates)) {
	  order[k] = *it;
	  --k;
	  Iterator<node> *itN = subgraph->getInOutNodes(*it);
	  while (itN->hasNext()) {
	    node n = itN->next();
	    if ( n!=order[0] && n!=order[1] )
	      candidates.insert(n);
	  } delete itN;
	  subgraph->delNode(*it);
	  candidates.erase(it);
	  break;
	}
      }
    }
    subgraph->delNode(order[0]);
    subgraph->delNode(order[1]);
    assert(subgraph->numberOfNodes()==1);
    order[2] = subgraph->getOneNode();
    graph->delSubGraph(subgraph);
  }
}
//==========================================================================
