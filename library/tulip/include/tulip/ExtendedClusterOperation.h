#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <tulip/Node.h>

class SuperGraph;

namespace tlp {
  TLP_SCOPE void openMetaNode(SuperGraph *graph, node n,
			      SuperGraph *groupUnderSubGraph,
			      const std::string &metagraphProperty);
  TLP_SCOPE inline void openMetaNode(SuperGraph *graph, node n) {
    const std::string viewMetaGraph ("viewMetaGraph");
    return openMetaNode (graph, n, graph->getFather(), viewMetaGraph);
  }
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
