/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <stack>
#include <tulip/Graph.h>
#include <tulip/ConnectedTest.h>
#include <tulip/TreeTest.h>
#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include <tulip/GraphTools.h>
#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;
TreeTest * TreeTest::instance=NULL;

TreeTest::TreeTest() {
}

bool TreeTest::isTree(const tlp::Graph* graph) {
  if (instance==NULL)
    instance=new TreeTest();

  return instance->compute(graph);
}

//====================================================================
//Determines if a graph is a topological tree.  This means that
//if the graph was undirected, there would be no cycle
bool TreeTest::isFreeTree(const tlp::Graph* graph) {
  if (instance==NULL) instance = new TreeTest();

  node firstNode = graph->getOneNode();
  return (firstNode.isValid() &&
          instance->isFreeTree (graph, firstNode))
         ? ConnectedTest::isConnected(graph)
         : false;
}//isFreeTree
//====================================================================
// simple structure to implement
// the further isFreeTree dfs loop
struct dfsFreeTreeStruct {
  node curRoot;
  node cameFrom;
  Iterator<node>* neighbours;

  dfsFreeTreeStruct(node root = node(), node from = node(),
                    Iterator<node>* it = NULL):
    curRoot(root), cameFrom(from), neighbours(it) {}
  ~dfsFreeTreeStruct() {
    if (neighbours)
      delete neighbours;
  }
};

//Determines if the given graph is topologically a tree
bool TreeTest::isFreeTree (const Graph *graph, node curRoot) {
  // do a dfs traversal from curRoot;
  MutableContainer<bool> visited;
  visited.setAll (false);
  stack<dfsFreeTreeStruct> dfsLevels;
  dfsFreeTreeStruct curParams(curRoot, curRoot, graph->getInOutNodes(curRoot));
  dfsLevels.push(curParams);

  while (!dfsLevels.empty()) {
    curParams = dfsLevels.top();
    curRoot = curParams.curRoot;
    node cameFrom = curParams.cameFrom;
    Iterator<node> *neighbours = curParams.neighbours;
    // set neighbours member to NULL
    // to avoid twice deletion on exit
    curParams.neighbours = NULL;

    if (!neighbours->hasNext()) {
      dfsLevels.pop();
    }
    else {
      visited.set (curRoot.id, true);

      // loop on remaining neighbours
      while (neighbours->hasNext()) {
        node curNode = neighbours->next();

        // check self loop
        if (curNode == curRoot) {
          return false;
        }

        if (curNode != cameFrom) {
          if (visited.get(curNode.id)) {
            return false;
          }

          // go deeper in the dfs exploration
          curParams.curRoot = curNode;
          curParams.cameFrom = curRoot;
          curParams.neighbours = graph->getInOutNodes(curNode);
          dfsLevels.push(curParams);
          break;
        }
      }
    }
  }

  return true;
}//end isFreeTree
//====================================================================
// simple structure to implement
// the further makeRootedTree dfs loop
struct dfsMakeRootedTreeStruct {
  node curRoot;
  node cameFrom;
  Iterator<edge>* ioEdges;

  dfsMakeRootedTreeStruct(node root, node from, Iterator<edge>* it):
    curRoot(root), cameFrom(from), ioEdges(it) {}
};

//given that a graph is topologically a tree, The function turns graph
//into a directed tree.
static void makeRootedTree (Graph *graph, node curRoot,
                            vector<edge>* reversedEdges) {
  stack<dfsMakeRootedTreeStruct> dfsLevels;
  dfsMakeRootedTreeStruct curParams(curRoot, curRoot,
                                    graph->getInOutEdges(curRoot));
  dfsLevels.push(curParams);

  // dfs loop
  while (!dfsLevels.empty()) {
    curParams = dfsLevels.top();
    curRoot = curParams.curRoot;
    node cameFrom = curParams.cameFrom;
    Iterator<edge> *ioEdges = curParams.ioEdges;

    if (!ioEdges->hasNext()) {
      delete ioEdges;
      dfsLevels.pop();
    }
    else {
      // loop on remaining ioEdges
      while (ioEdges->hasNext()) {
        edge curEdge = ioEdges->next();
        node opposite = graph->opposite(curEdge, curRoot);

        if (opposite != cameFrom) {
          if (graph->target (curEdge) == curRoot) {
            graph->reverse(curEdge);

            if (reversedEdges)
              reversedEdges->push_back(curEdge);
          }

          // go deeper in the dfs traversal
          curParams.curRoot = opposite;
          curParams.cameFrom = curRoot;
          curParams.ioEdges = graph->getInOutEdges(opposite);
          dfsLevels.push(curParams);
          break;
        }//end if
      }//end while
    }//end else
  }//end while
}//end makeRootedTree
//====================================================================
//Turns a topological tree graph into a directed tree starting at
//the node root.
void TreeTest::makeRootedTree(Graph *graph, node root) {
  if (instance==NULL) instance=new TreeTest();

  graph->removeListener(instance);
  instance->resultsBuffer.erase(graph);

  if (!graph->isElement (root)) {
    tlp::warning() << "makeRootedTree:  Passed root is not element of graph" << endl;
    return;
  }//end if

  if (!TreeTest::isFreeTree (graph)) {
    tlp::warning() << "makeRootedTree:  Graph is not topologically a tree, so rooted "
                   << "tree cannot be made." << endl;
    return;
  }//end if

  ::makeRootedTree(graph, root, NULL);
  assert (TreeTest::isTree (graph));
}//end makeRootedTree

//====================================================================
// this function is for internal purpose only
static Graph* computeTreeInternal(Graph *graph, Graph *rGraph, bool isConnected,
                                  PluginProgress *pluginProgress,
                                  vector<edge>* reversedEdges = NULL) {
  // nothing todo if the graph is already
  if (TreeTest::isTree(graph))
    return graph;

  // if needed, create a clone of the graph
  // as a working copy
  Graph *gClone = graph;

  if (!rGraph) {
    // the graph attribute used to store the clone
#define CLONE_NAME "CloneForTree"
    rGraph = gClone = graph->addCloneSubGraph(CLONE_NAME);
    // the graph attribute used to store added root node
#define CLONE_ROOT "CloneRoot"
    rGraph->setAttribute(CLONE_ROOT, node());
    // the graph attribute used to store the reversed edge
#define REVERSED_EDGES "ReversedEdges"
    reversedEdges = new vector<edge>;
    rGraph->setAttribute(REVERSED_EDGES, reversedEdges);
  }

  // add a node for an empty graph
  if (graph->numberOfNodes() == 0) {
    rGraph->setAttribute(CLONE_ROOT, rGraph->addNode());
    return rGraph;
  }

  // if the graph is topologically a tree, make it rooted
  // using a 'center' of the graph as root
  if (TreeTest::isFreeTree(gClone)) {
    makeRootedTree(gClone, graphCenterHeuristic(gClone), reversedEdges);
    return gClone;
  }

  // if the graph is connected,
  // extract a spanning tree,
  // and make it rooted
  if (isConnected || ConnectedTest::isConnected(gClone)) {
    BooleanProperty treeSelection(gClone);
    selectSpanningTree(gClone, &treeSelection, pluginProgress);

    if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
      return 0;

    return computeTreeInternal(gClone->addSubGraph(&treeSelection),
                               rGraph, true, pluginProgress,
                               reversedEdges);
  }

  // graph is not connected
  // compute the connected components's subgraphs
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(rGraph, components);

  for (unsigned int i = 0; i < components.size(); ++i) {
    rGraph->inducedSubGraph(components[i]);
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
    Graph *sTree = computeTreeInternal(gConn, rGraph, true, pluginProgress,
                                       reversedEdges);

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

// the documented functions
Graph* TreeTest::computeTree(tlp::Graph* graph, PluginProgress* pluginProgress) {
  return computeTreeInternal(graph, NULL, false, pluginProgress);
}

// this one revert the updates due to tree computation
void TreeTest::cleanComputedTree(tlp::Graph *graph, tlp::Graph *tree) {
  if (graph == tree)
    return;

  // get the subgraph clone
  Graph *sg = tree;
  string nameAtt("name");
  string name;
  sg->getAttribute<string>(nameAtt, name);

  while(name != CLONE_NAME) {
    sg = sg->getSuperGraph();
    sg->getAttribute<string>(nameAtt, name);
  }

  Graph* rg = graph->getRoot();
  // get its added root
  node root;
  sg->getAttribute<node>(CLONE_ROOT, root);

  // delete it if needed
  if (root.isValid())
    rg->delNode(root);

  // delete the reversed edges if any
  vector<edge>* reversedEdges = NULL;

  if (sg->getAttribute<vector<edge>*>(REVERSED_EDGES, reversedEdges)) {
    sg->removeAttribute(REVERSED_EDGES);

    for(vector<edge>::iterator ite = reversedEdges->begin();
        ite != reversedEdges->end(); ++ite) {
      rg->reverse(*ite);
    }

    delete reversedEdges;
  }

  // delete the clone
  graph->delAllSubGraphs(sg);
}

//====================================================================
bool TreeTest::compute(const Graph *graph) {
  if (resultsBuffer.find(graph)!=resultsBuffer.end()) {
    return resultsBuffer[graph];
  }

  if (graph->numberOfEdges()!=graph->numberOfNodes()-1) {
    resultsBuffer[graph]=false;
    graph->addListener(this);
    return false;
  }

  bool rootNodeFound=false;
  Iterator<node> *it=graph->getNodes();

  while (it->hasNext()) {
    node tmp=it->next();

    if (graph->indeg(tmp)>1) {
      delete it;
      resultsBuffer[graph]=false;
      graph->addListener(this);
      return false;
    }

    if (graph->indeg(tmp)==0) {
      if (rootNodeFound) {
        delete it;
        resultsBuffer[graph]=false;
        graph->addListener(this);
        return false;
      }
      else
        rootNodeFound=true;
    }
  }

  delete it;

  if (AcyclicTest::isAcyclic(graph)) {
    resultsBuffer[graph]=true;
    graph->addListener(this);
    return true;
  }
  else {
    resultsBuffer[graph]=false;
    graph->addListener(this);
    return false;
  }
}

void TreeTest::treatEvent(const Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_NODE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  }
  else {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* graph = reinterpret_cast<Graph *>(evt.sender());

    if (graph && evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
