#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <tulip/Node.h>

class SuperGraph;

namespace tlp {
  /**
   * Function to open a metanode and replace all edges between that
   * meta node and other nodes in the graph.
   *
   * \param graph The graph where the metanode is contained and where
   * the subgraph will be replaced.
   * \param n The meta node to open.
   * \param groupUnderSubgraph The subgraph where the groups are placed 
   * \param metagraphProperty The name of the metagraph property used
   * to store the subgraphs.
   */
  TLP_SCOPE void openMetaNode(SuperGraph *graph, node n,
			      SuperGraph *groupUnderSubGraph,
			      const std::string &metagraphProperty);
  TLP_SCOPE inline void openMetaNode(SuperGraph *graph, node n) {
    const std::string viewMetaGraph ("viewMetaGraph");
    return openMetaNode (graph, n, graph->getFather(), viewMetaGraph);
  }
  /**
   * Function to close a subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   *
   * \param graph The supergraph where the meta node will be placed.
   * \param subGraph The set of nodes to be contracted into a metanode.
   * \param groupUnderSubgraph The subgraph where the groups are placed. 
   * \param multiEdges When true, the multiplicity of the edges from the
   * metanode to the other node n is equal to the number of edges between
   * the nodes in the subgraph and n.  Otherwise one edge is placed
   * \param metagraphProperty The name of the metagraph property used
   * to store the subgraphs.
   */
  TLP_SCOPE node createMetaNode(SuperGraph *graph, std::set<node> &subGraph,
				SuperGraph *groupUnderSubGraph,
				bool multiEdges,
				const std::string &metagraphProperty);
  TLP_SCOPE inline node createMetaNode(SuperGraph *graph, 
				       std::set<node> &subGraph) {
    const std::string viewMetaGraph ("viewMetaGraph");
    return createMetaNode(graph, subGraph, graph->getFather(), 
			  true, viewMetaGraph);
  }//end create meta node
  /**
   *  Return a subgraph induced by a set of nodes
   */
  TLP_SCOPE SuperGraph *inducedSubGraph(SuperGraph *, const std::set<node> &, std::string name = "induced subgraph");
};

#endif
