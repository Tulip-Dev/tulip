#include "tulip/Graph.h"
#include "tulip/ConnectedTest.h"
#include "tulip/TreeTest.h"
#include "tulip/AcyclicTest.h"
#include "tulip/ForEach.h"
#include "tulip/GraphTools.h"
#include "tulip/ExtendedClusterOperation.h"
#include "tulip/BooleanProperty.h"

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
  return (firstNode.isValid() &&
	  instance->isFreeTree (graph, firstNode, firstNode,
				visited))
    ? ConnectedTest::isConnected(graph)
    : false;
}//isFreeTree

//====================================================================
//Turns a topological tree graph into a directed tree starting at
//the node root.
void TreeTest::makeRootedTree(Graph *graph, node root) {
  if (instance==0) instance=new TreeTest();
  graph->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned long)graph);
  if (!graph->isElement (root)) {
    cerr << "makeRootedTree:  Passed root is not element of graph" << endl;
    return;
  }//end if
  if (!TreeTest::isFreeTree (graph)) {
    cerr << "makeRootedTree:  Graph is not topological tree, so rooted " 
	 << "tree cannot be made." << endl;
    return;
  }//end if
  instance->makeRootedTree (graph, root, root);
  assert (TreeTest::isTree (graph));
}//end makeRootedTree

//====================================================================
//Determines if the passed graph is topologically a tree.  The 
//passed mutable container returns if we have visited a node
bool TreeTest::isFreeTree (Graph *graph, node curRoot, node cameFrom,
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
void TreeTest::makeRootedTree (Graph *graph, node curRoot, node cameFrom) {
  edge curEdge;
  forEach (curEdge, graph->getInOutEdges(curRoot)) {
    node opposite = graph->opposite(curEdge, curRoot);
    if (opposite != cameFrom) {
      if (graph->target (curEdge) == curRoot)
	graph->reverse(curEdge);
      makeRootedTree (graph, opposite, curRoot);
    }//end if
  }//end forEach
}//end makeRootedTree

//====================================================================
Graph *TreeTest::computeTree(Graph *graph, Graph *rGraph, bool isConnected,
			     PluginProgress *pluginProgress) {
  // nothing todo if the graph is already a tree
  if (TreeTest::isTree(graph))
    return graph;
  
  // if needed, create a clone of the graph
  // as a working copy
  Graph *gClone = graph;
  if (!rGraph) {
    // the name used for subgraph clone when computing a tree
#define CLONE_NAME "CloneForTree"
#define CLONE_ROOT "CloneRoot"
    rGraph = gClone = tlp::newCloneSubGraph(graph, CLONE_NAME);
    rGraph->setAttribute(CLONE_ROOT, node());
  }
  // if the graph is topologically a tree, make it rooted
  // using a 'center' of the graph as root
  if (TreeTest::isFreeTree(gClone)) {
    TreeTest::makeRootedTree(gClone, graphCenterHeuristic(gClone));
    return gClone;
  }

  // if the graph is connected,
  // extract a spanning tree,
  // and make it rooted
  if (isConnected || ConnectedTest::isConnected(gClone)) {
    BooleanProperty treeSelection(gClone);
    selectMinimumSpanningTree(gClone, &treeSelection, 0, pluginProgress);
    if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
      return 0;
    return TreeTest::computeTree(gClone->addSubGraph(&treeSelection),
				 rGraph, true, pluginProgress);
  }

  // graph is not connected
  // compute the connected components's subgraphs
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(rGraph, components);
  for (unsigned int i = 0; i < components.size(); ++i) {
    tlp::inducedSubGraph(rGraph, components[i]);
  }

  // create a new subgraph for the tree
  Graph *tree = rGraph->addSubGraph();
  node root = tree->addNode();
  rGraph->setAttribute(CLONE_ROOT, root);
  Graph *gConn;

  // connected components subgraphs loop
  forEach(gConn, rGraph->getSubGraphs()) {
    if (gConn == tree)
      continue;
    // compute a tree for each subgraph
    // add each element of that tree
    // to our main tree
    // and connect the main root to each
    // subtree root
    Graph *sTree = TreeTest::computeTree(gConn, rGraph, true, pluginProgress);
    if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
      return 0;
    node n;
    forEach(n, sTree->getNodes()) {
      tree->addNode(n);
      if (sTree->indeg(n) == 0)
	tree->addEdge(root, n);
    }
    edge e;
    forEach(e, sTree->getEdges())
      tree->addEdge(e);
  }
  assert (TreeTest::isTree(tree));
  return tree;
}

void TreeTest::cleanComputedTree(tlp::Graph *graph, tlp::Graph *tree) {
  if (graph == tree)
    return;
  // get the subgraph clone
  Graph *sg = tree;
  string nameAtt("name");
  string name = sg->getAttribute<string>(nameAtt);
  while(name != CLONE_NAME) {
    sg = sg->getSuperGraph();
    name = sg->getAttribute<string>(nameAtt);
  }
  // get its added root
  node root = sg->getAttribute<node>(CLONE_ROOT);
  // delete it if needed
  if (root.isValid())
    graph->delNode(root);
  // delete the clone
  graph->delAllSubGraphs(sg);
}

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
