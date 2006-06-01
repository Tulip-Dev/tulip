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

bool TreeTest::isTree(Graph *sg) {
  if (instance==0)
    instance=new TreeTest();
  return instance->compute(sg);
}

//====================================================================
//Determines if a graph is a topological tree.  This means that
//if the graph were undirected, there would be no cycles.
bool TreeTest::isTopologicalTree(Graph *sg) {
  if (instance==0) instance = new TreeTest();
  MutableContainer<bool> visited;
  visited.setAll (false);
  node firstNode = sg->getOneNode();
  return instance->isTopologicalTree (sg, firstNode, firstNode,
				      visited);
}//isTopologicalTree

//====================================================================
//Turns a topological tree graph into a directed tree starting at
//the node root.
void TreeTest::makeRootedTree(Graph *sg, node root) {
  if (instance==0) instance=new TreeTest();
  sg->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned int)sg);
  if (!sg->isElement (root)) {
    cerr << "makeRootedTree:  Passed root is not element of graph" << endl;
    return;
  }//end if
  if (!TreeTest::isTopologicalTree (sg)) {
    cerr << "makeRootedTree:  Graph is not topological tree, so directed " 
	 << "tree cannot be made." << endl;
    return;
  }//end if
  instance->makeRootedTree (sg, root, root);
  assert (TreeTest::isTree (sg));
}//end makeDirectedTree

//====================================================================
//Determines if the passed graph is topologically a tree.  The 
//passed mutable container returns if we have visited a node
bool TreeTest::
isTopologicalTree (Graph *sg, node curRoot, node cameFrom,
		   MutableContainer<bool> &visited) {
  if (visited.get (curRoot.id)) return false;
  visited.set (curRoot.id, true);
  node curNode;
  forEach (curNode, sg->getInOutNodes(curRoot)) { 
    if (curNode != cameFrom)
      if (!isTopologicalTree (sg, curNode, curRoot, visited)) return false;
  }//end forEach
  return true;
}//end isTopologicalTree 

//====================================================================
//given that graph is topologically a tree, The function turns graph
//into a directed tree.
void TreeTest::
makeRootedTree (Graph *sg, node curRoot, node cameFrom) {
  edge curEdge;
  forEach (curEdge, sg->getInOutEdges(curRoot)) {
    node opposite = sg->opposite(curEdge, curRoot);
    if (opposite != cameFrom) {
      if (sg->target (curEdge) == curRoot) 
	sg->reverse(curEdge);
      makeRootedTree (sg, opposite, curRoot);
    }//end if
  }//end forEach
}//end makeDirectedTree

//====================================================================
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
