//-*-c++-*-
#ifndef _TLPGRAPHTOOLS_H
#define _TLPGRAPHTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <set>
#include <list>

class Graph;
class IntegerProperty;

namespace tlp {

  TLP_SCOPE node makeSimpleSource(Graph* graph);

  TLP_SCOPE void makeProperDag(Graph* graph,std::list<node> &addedNodes, 
		     stdext::hash_map<edge,edge> &replacedEdges,IntegerProperty* edgeLength);

};
#endif

