/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/GraphTools.h>
#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;

//=================================================================
class TreeTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  std::unordered_map<const Graph *, bool> resultsBuffer;
};

void TreeTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_DEL_NODE:
    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_DEL_EDGE:
    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
//=================================================================
static TreeTestListener instance;
//====================================================================
static bool treeTest(const Graph *graph) {
  if (graph->numberOfEdges() != graph->numberOfNodes() - 1)
    return false;

  bool rootNodeFound = false;

  for (auto tmp : graph->nodes()) {
    if (graph->indeg(tmp) > 1)
      return false;

    if (graph->indeg(tmp) == 0) {
      if (rootNodeFound)
        return false;
      else
        rootNodeFound = true;
    }
  }

  return AcyclicTest::acyclicTest(graph);
}
//====================================================================
bool TreeTest::isTree(const tlp::Graph *graph) {
  auto it = instance.resultsBuffer.find(graph);
  if (it != instance.resultsBuffer.end())
    return it->second;

  graph->addListener(instance);
  return instance.resultsBuffer[graph] = treeTest(graph);
}
//====================================================================
// Determines if a graph is a topological tree.  This means that
// if the graph was undirected, there would be no cycle
bool TreeTest::isFreeTree(const tlp::Graph *graph) {
  auto nbNodes = graph->numberOfNodes();
  return (nbNodes && (graph->numberOfEdges() == nbNodes - 1)) ? ConnectedTest::isConnected(graph)
                                                              : false;
}
//====================================================================
// simple structure to implement
// the further makeRootedTree dfs loop
struct dfsMakeRootedTreeStruct {
  node curRoot;
  node cameFrom;
  Iterator<edge> *ioEdges;

  dfsMakeRootedTreeStruct(node root, node from, Iterator<edge> *it)
      : curRoot(root), cameFrom(from), ioEdges(it) {}
};

// given that a graph is topologically a tree, The function turns graph
// into a directed tree.
static void makeRootedTree(Graph *graph, node curRoot, vector<edge> *reversedEdges) {
  stack<dfsMakeRootedTreeStruct> dfsLevels;
  dfsMakeRootedTreeStruct curParams(curRoot, curRoot, graph->getInOutEdges(curRoot));
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
    } else {
      // loop on remaining ioEdges
      while (ioEdges->hasNext()) {
        edge curEdge = ioEdges->next();
        node opposite = graph->opposite(curEdge, curRoot);

        if (opposite != cameFrom) {
          if (graph->target(curEdge) == curRoot) {
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
        } // end if
      }   // end while
    }     // end else
  }       // end while
}
//====================================================================
// Turns a topological tree graph into a directed tree starting at
// the node root.
void TreeTest::makeRootedTree(Graph *graph, node root) {
  graph->removeListener(instance);
  instance.resultsBuffer.erase(graph);

  if (!graph->isElement(root)) {
    tlp::warning() << "makeRootedTree:  Passed root is not an element of the graph" << endl;
    return;
  } // end if

  if (!TreeTest::isFreeTree(graph)) {
    tlp::warning()
        << "makeRootedTree: The graph is not topologically a tree, so rooted tree cannot be made."
        << endl;
    return;
  } // end if

  ::makeRootedTree(graph, root, nullptr);
  assert(treeTest(graph));
} // end makeRootedTree

//====================================================================
// this function is for internal purpose only
static Graph *computeTreeInternal(Graph *graph, Graph *rGraph, bool isConnected,
                                  PluginProgress *pluginProgress,
                                  vector<edge> *reversedEdges = nullptr) {
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
  if (graph->isEmpty()) {
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

    if (pluginProgress && pluginProgress->state() != TLP_CONTINUE)
      return nullptr;

    return computeTreeInternal(gClone->addSubGraph(&treeSelection), rGraph, true, pluginProgress,
                               reversedEdges);
  }

  // graph is not connected
  // compute the connected components's subgraphs
  std::vector<std::vector<node>> components;
  ConnectedTest::computeConnectedComponents(rGraph, components);

  for (unsigned int i = 0; i < components.size(); ++i) {
    rGraph->inducedSubGraph(components[i]);
  }

  // create a new subgraph for the tree
  Graph *tree = rGraph->addSubGraph();
  node root = tree->addNode();
  rGraph->setAttribute(CLONE_ROOT, root);

  // connected components subgraphs loop
  for (Graph *gConn : rGraph->subGraphs()) {
    if (gConn == tree)
      continue;

    // compute a tree for each subgraph
    // add each element of that tree
    // to our main tree
    // and connect the main root to each
    // subtree root
    Graph *sTree = computeTreeInternal(gConn, rGraph, true, pluginProgress, reversedEdges);

    if (pluginProgress && pluginProgress->state() != TLP_CONTINUE)
      return nullptr;

    for (auto n : sTree->nodes()) {
      tree->addNode(n);

      if (sTree->indeg(n) == 0)
        tree->addEdge(root, n);
    }
    tree->addEdges(sTree->edges());
  }
  assert(treeTest(tree));
  return tree;
}

// the documented functions
Graph *TreeTest::computeTree(tlp::Graph *graph, PluginProgress *pluginProgress) {
  return computeTreeInternal(graph, nullptr, false, pluginProgress);
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

  while (name != CLONE_NAME) {
    sg = sg->getSuperGraph();
    sg->getAttribute<string>(nameAtt, name);
  }

  Graph *rg = graph->getRoot();
  // get its added root
  node root;
  sg->getAttribute<node>(CLONE_ROOT, root);

  // delete it if needed
  if (root.isValid())
    rg->delNode(root);

  // delete the reversed edges if any
  vector<edge> *reversedEdges = nullptr;

  if (sg->getAttribute<vector<edge> *>(REVERSED_EDGES, reversedEdges)) {
    sg->removeAttribute(REVERSED_EDGES);

    for (auto e : *reversedEdges) {
      rg->reverse(e);
    }

    delete reversedEdges;
  }

  // delete the clone
  graph->delAllSubGraphs(sg);
}
