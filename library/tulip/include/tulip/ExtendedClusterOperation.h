#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <vector>
#include <string>
#include <tulip/Node.h>
#include <tulip/Graph.h>
#include <tulip/GraphProperty.h>

namespace tlp {
  /**
   * Function to open a metanode and replace all edges between that
   * meta node and other nodes in the graph.
   */
  inline void openMetaNode(Graph *graph, node n) {
    graph->openMetaNode(n);
  }

  /**
   * Function to close a subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   */
  inline node createMetaNode(Graph *graph, const std::set<node> &nodeSet) {
    return graph->createMetaNode(nodeSet);
  }

  /**
   *  Return the subgraph induced by a set of nodes
   */
  inline Graph *inducedSubGraph(Graph* graph, const std::set<node> &nodeSet) {
    return graph->inducedSubGraph(nodeSet);
  }
}

#endif
