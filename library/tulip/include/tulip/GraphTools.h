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

class BooleanProperty;
class DoubleProperty;
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
   * Find all the graph centers, that version does not manage edge weight.
   * complexity O(n * m). Only works on connected graphs.
   */
  TLP_SCOPE std::vector<node> computeGraphCenters(Graph * graph);
  /**
   * return a node that can be considered as the graph center. 
   * It is an heuristic, thus it is not absolutely sure that this 
   * node is a graph center. Only works on connected graphs.
   */
  TLP_SCOPE node graphCenterHeuristic(Graph * graph);
  /**
   * return a new node connected to all previously
   * existing nodes which had a null indegree
   */
  TLP_SCOPE node makeSimpleSource(Graph* graph);

  TLP_SCOPE void makeProperDag(Graph* graph,std::list<node> &addedNodes, 
			       stdext::hash_map<edge,edge> &replacedEdges,
			       IntegerProperty* edgeLength = 0);

  /**
   * Select a spanning forest of the graph,
   * i.e for all graph elements (nodes or edges) belonging to that forest
   * the selectionProperty associated value is true. The value is false
   * for the other elements
   */
  TLP_SCOPE void selectSpanningForest(Graph* graph, BooleanProperty *selectionProperty,
				      PluginProgress *pluginProgress = 0);

  /**
   * Select the minimum spanning tree (Kruskal algorithm) of a weighted graph,
   * i.e for all graph elements (nodes or edges) belonging to that tree
   * the selectionProperty associated value is true. The value is false
   * for the other elements
   */
  TLP_SCOPE void selectMinimumSpanningTree(Graph* graph, BooleanProperty *selectionProperty,
					   DoubleProperty *weight = 0,
					   PluginProgress *pluginProgress = 0);

  /**
   * Compute a directed tree from the graph.
   * The algorithm is the following
   * - if the graph is a directed tree, return the graph
   * - if the graph is a free tree, return a directed copy
   * - if the graph is connected, make a copy
   *   return a directed spanning tree of that copy
   * - if the graph is not connected, make a copy,
   *   compute a tree for each of its connected components,
   *   add a simple source and return the copy.
   */  
  TLP_SCOPE Graph *computeTree(Graph* graph, Graph* rootGraph = 0, bool isConnected=false,
			       PluginProgress *pluginProgress = 0);

  /**
   * Clean the graph from a tree previously computed
   * with the computTree function
   */
  TLP_SCOPE void cleanComputedTree(Graph *graph, Graph *tree);
}
#endif

