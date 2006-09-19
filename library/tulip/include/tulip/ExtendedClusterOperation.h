#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <tulip/Node.h>

namespace tlp {

class Graph;
  TLP_SCOPE void openMetaNode(Graph *graph, node n,
			      Graph *groupUnderSubGraph);
  TLP_SCOPE inline void openMetaNode(Graph *graph, node n) {
    return openMetaNode (graph, n, graph->getFather());
  }
  TLP_SCOPE node createMetaNode(Graph *graph, std::set<node> &subGraph,
				Graph *groupUnderSubGraph);
  TLP_SCOPE inline node createMetaNode(Graph *graph, 
				       std::set<node> &subGraph) {
    return createMetaNode(graph, subGraph, graph->getFather());
  }//end create meta node
  /**
   *  Return a subgraph induced by a set of nodes
   */
  TLP_SCOPE Graph *inducedSubGraph(Graph *, const std::set<node> &, std::string name = "induced subgraph");
}

#endif
