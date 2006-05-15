#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <tulip/Node.h>

namespace tlp {

class Graph;

  TLP_SCOPE void openMetaNode(Graph *sg, node n);
  TLP_SCOPE node createMetaNode(Graph *sg, std::set<node> &subGraph);
  /**
   *  Return a subsg induced by a set of nodes
   */
  TLP_SCOPE Graph *inducedSubGraph(Graph *, const std::set<node> &, std::string name = "induced subsg");
}

#endif
