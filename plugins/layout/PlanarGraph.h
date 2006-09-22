#include <vector>
class PlanarConMap;
class SuperGraph;

namespace tlp {
  /**
   * This function triangulate a planar graph. (ie. add edges in the graph until 
   * each face of the planar map contains only 3 edges.
   * The input graph must be planr in order to call this function.
   */
  void triangulate(SuperGraph *map, std::vector<edge>& addedEdges);
  /**
   * Build the canonical ordering of a planar triangulated graph. For information
   * about canonical ordering the user must reffer to the planar graph drawing book.
   */
  void canonicalOrdering(SuperGraph *graph, std::vector<node> &order);
}

