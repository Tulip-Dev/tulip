#include "tulip/Graph.h"
#include "tulip/TreeTest.h"
#include "tulip/AcyclicTest.h"
#include "tulip/ForEach.h"

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

bool TreeTest::isTree(Graph *graph) {
  if (instance==0)
    instance=new TreeTest();
  return instance->compute(graph);
}

//====================================================================
//Determines if a graph is a topological tree.  This means that
//if the graph were undirected, there would be no cycles.
bool TreeTest::isFreeTree(Graph *graph) {
  if (instance==0) instance = new TreeTest();
  MutableContainer<bool> visited;
  visited.setAll (false);
  node firstNode = graph->getOneNode();
  return instance->isFreeTree (graph, firstNode, firstNode,
				      visited);
}//isFreeTree

//====================================================================
//Turns a topological tree graph into a directed tree starting at
//the node root.
void TreeTest::makeRootedTree(Graph *graph, node root) {
  if (instance==0) instance=new TreeTest();
  graph->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned int)graph);
  if (!graph->isElement (root)) {
    cerr << "makeRootedTree:  Passed root is not element of graph" << endl;
    return;
  }//end if
  if (!TreeTest::isFreeTree (graph)) {
    cerr << "makeRootedTree:  Graph is not topological tree, so directed " 
	 << "tree cannot be made." << endl;
    return;
  }//end if
  instance->makeRootedTree (graph, root, root);
  assert (TreeTest::isTree (graph));
}//end makeDirectedTree

//====================================================================
//Determines if the passed graph is topologically a tree.  The 
//passed mutable container returns if we have visited a node
bool TreeTest::
isFreeTree (Graph *graph, node curRoot, node cameFrom,
		   MutableContainer<bool> &visited) {
  if (visited.get (curRoot.id)) return false;
  visited.set (curRoot.id, true);
  node curNode;
  forEach (curNode, graph->getInOutNodes(curRoot)) { 
    if (curNode != cameFrom)
      if (!isFreeTree (graph, curNode, curRoot, visited)) return false;
  }//end forEach
  return true;
}//end isFreeTree 

//====================================================================
//given that graph is topologically a tree, The function turns graph
//into a directed tree.
void TreeTest::
makeRootedTree (Graph *graph, node curRoot, node cameFrom) {
  edge curEdge;
  forEach (curEdge, graph->getInOutEdges(curRoot)) {
    node opposite = graph->opposite(curEdge, curRoot);
    if (opposite != cameFrom) {
      if (graph->target (curEdge) == curRoot) 
	graph->reverse(curEdge);
      makeRootedTree (graph, opposite, curRoot);
    }//end if
  }//end forEach
}//end makeDirectedTree

//====================================================================

bool TreeTest::compute(Graph *graph) { 
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

void TreeTest::addEdge(Graph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::delEdge(Graph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::reverseEdge(Graph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::addNode(Graph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
void TreeTest::delNode(Graph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}

void TreeTest::destroy(Graph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
