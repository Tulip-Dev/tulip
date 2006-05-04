#ifndef _KRUSKAL_H
#define _KRUSKAL_H

#include <tulip/TulipPlugin.h>
#include <map>
#include <iostream>

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
class Kruskal:public BooleanAlgorithm { 
  
public:
  Kruskal(const PropertyContext &);
  ~Kruskal();
  bool run();
  bool check(std::string &);

private:

  struct ltEdge {
    DoubleProperty *m;
    ltEdge(DoubleProperty *m) : m(m) {}
    bool operator()(const edge &e1, const edge &e2) const
    {
      return (m->getEdgeValue(e1) < m->getEdgeValue(e2));
    } 
  };

  std::map<int,int> *classes;
  int numClasses;
  int getClass(const int i);
  int makeUnion(const int p, const int q);
  bool edgeOk(const edge &e);
};
/*@}*/
#endif
