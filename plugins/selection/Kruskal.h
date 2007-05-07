#ifndef _KRUSKAL_H
#define _KRUSKAL_H

#include <tulip/TulipPlugin.h>

/** \addtogroup selection */
/*@{*/

/// This selection plugins implements the so called Kruskal algorithm. This algorithm enables to find a minimum spanning tree in a connected graph.
/**
 * This selection plugins enables to find all nodes and 
 * edges at a fixed distance of a set of nodes.
 *
 * This working work on undirected graphs, (ie. the orientation of edges is ommitted).
 *
 * It takes one parameter :
 * - <DoubleProperty> edge weight, this parameter defines the weight of each edge in the graph. 
 *
 *  \author Anthony Don, LaBRI University Bordeaux I France: 
 */
class Kruskal:public tlp::BooleanAlgorithm { 
  
public:
  Kruskal(const tlp::PropertyContext &);
  ~Kruskal();
  bool run();
  bool check(std::string &);
};
/*@}*/
#endif
