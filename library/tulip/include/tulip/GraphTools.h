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
class IntProxy;

namespace tlp {
  struct SelfLoops {
    SelfLoops(node n1,node n2,edge e1, edge e2,edge e3,edge old):
      n1(n1),n2(n2),e1(e1),e2(e2),e3(e3),old(old){
    }
    node n1,n2;
    edge e1,e2,e3,old;  
  };


  TLP_SCOPE void makeAcyclic(SuperGraph* graph,std::set<edge> &reversed,std::list<tlp::SelfLoops> &selfLoops);

  TLP_SCOPE node makeSimpleSource(SuperGraph* superGraph);

  TLP_SCOPE void makeProperDag(SuperGraph* superGraph,std::list<node> &addedNodes, 
		     stdext::hash_map<edge,edge> &replacedEdges,IntProxy *edgeLength);

};
#endif

