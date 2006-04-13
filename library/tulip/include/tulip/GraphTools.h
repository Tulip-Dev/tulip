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

class SuperGraph;
class Int;

namespace tlp {

  TLP_SCOPE node makeSimpleSource(SuperGraph* superGraph);

  TLP_SCOPE void makeProperDag(SuperGraph* superGraph,std::list<node> &addedNodes, 
		     stdext::hash_map<edge,edge> &replacedEdges,Int* edgeLength);

};
#endif

