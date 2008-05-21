#ifndef OUTERPLANARTEST_H
#define OUTERPLANARTEST_H

#if (__GNUC__ < 3)
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include "tulip/ObservableGraph.h"

namespace tlp {

class Graph;
class edge;
class node;

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing the outerplanarity of a graph
class TLP_SCOPE OuterPlanarTest  : private GraphObserver {
public:
  static bool isOuterPlanar(Graph *graph);
  
private:
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  bool compute(Graph *graph);
  OuterPlanarTest(){}
  static OuterPlanarTest* instance;
  stdext::hash_map<unsigned long, bool> resultsBuffer;
};
/*@}*/
}

#endif 
