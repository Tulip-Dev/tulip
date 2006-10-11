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

#include <tulip/PlanarConMap.h>

namespace tlp {

class Graph;
class IntegerProperty;
  

  /**
   *  This ordering was first introduced by C. Gutwenger and P. Mutzel in \n
   *  "Grid embeddings of biconnected planar graphs", \n
   *  "Extended Abstract, Max-Planck-Institut für Informatik," \n
   *  "Saarbrücken, Germany, 1997" \n
   *  Let n be the number of nodes, the original algorithm complexity is in O(n).\n
   *  But the implementation of the canonical ordering has not been made in O(n).\n 
   */  
  TLP_SCOPE std::vector<std::vector<node> > computeCanonicalOrdering(PlanarConMap *, 
								     std::vector<edge>  *dummyEdges = 0,
								     PluginProgress *pluginProgress = 0);
  /**
   * Find all the graph centers, that version do not manage edge weight.
   * complexity O(n * m). Only works on connected graphs.
   */
  TLP_SCOPE std::vector<node> computeGraphCenters(Graph * graph);
  /**
   * return a node that can be considered as the graph center. 
   * It is a heurictic thus, it is not absolutly sure that this 
   * node is a graph center. Only works on connected graphs.
   */
  TLP_SCOPE node graphCenterHeuristic(Graph * graph);
    


  TLP_SCOPE node makeSimpleSource(Graph* graph);
  TLP_SCOPE void makeProperDag(Graph* graph,std::list<node> &addedNodes, 
			       stdext::hash_map<edge,edge> &replacedEdges,
			       IntegerProperty* edgeLength);
}
#endif

