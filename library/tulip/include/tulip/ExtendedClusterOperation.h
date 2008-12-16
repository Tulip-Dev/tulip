#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <string>
#include <tulip/Node.h>
#include <tulip/Graph.h>
#include <tulip/GraphProperty.h>

namespace tlp {
  /**
   * Function to open a metanode and replace all edges between that
   * meta node and other nodes in the graph.
   *
   * \param graph The graph where the metanode is contained and where
   * the subgraph will be replaced.
   * \param n The meta node to open.
   * \param groupUnderSubgraph The subgraph where the groups are placed 
   * \param metaGraphProperty The metagraph property used to associate 
   * the subgraph to metanodes
   */
  TLP_SCOPE void openMetaNode(Graph *graph, node metaNode,
			      Graph *groupUnderSubGraph,
			      GraphProperty *metaGraphProperty);

  /**
   * Function to open a metanode and replace all edges between that
   * meta node and other nodes in the graph.  Provided for backwards
   * compatability with groupUnderSubGraph set to graph->getSuperGraph(),
   * and metaGraphs set to viewMetaGraph.
   */
  TLP_SCOPE void openMetaNode(Graph *graph, node n);

  /**
   * Function to close an existing subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   *
   * \param graph: The graph where the meta node will be placed.
   * \param subGraph: an existing subgraph
   * \param groupUnderSubgraph: The subgraph where the groups are placed. 
   * the subgraph to the new metanode.
   */
  TLP_SCOPE node createMetaNode(Graph *graph, std::set<node> &subGraph,
				Graph *groupUnderSubGraph);

  /**
   * Function to close a subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   * Version for backwards compatibility with groupUnderSubGraph set
   * to graph->getSuperGraph()
   */
  TLP_SCOPE node createMetaNode(Graph *graph, 
				std::set<node> &subGraph);
  /**
   *  Return the subgraph induced by a set of nodes
   */
  TLP_SCOPE Graph *inducedSubGraph(Graph *, const std::set<node> &);

  /**
   * Update the layout of metanode in cluster according to the layout of underlying nodes in graph
   *
   */
  TLP_SCOPE void updateGroupLayout(Graph *graph, Graph *cluster, node metanode);
}

#endif
