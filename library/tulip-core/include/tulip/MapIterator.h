/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#include <tulip/Iterator.h>
#include <tulip/tulipconf.h>
#include <tulip/Edge.h>
#include <list>
#include <vector>

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TULIP_NODEMAPITERATOR_H
#define TULIP_NODEMAPITERATOR_H

namespace tlp {

struct node;
class Graph;

/**
  * That function enables to obtain the next edge on a face of the embedding. It uses
  * the EdgeMapIterators.
  *
  * @see NodeMapIterator
  * @see EdgeMapIterator
  * @see PlanarConMap
  */
TLP_SCOPE edge nextFaceEdge(Graph* g, edge source, node target);

/**
  * @class NodeMapIterator
  * @brief Iterator that enables to traverse the graph taking into account the ordering of edges aroung nodes
  * @param sg the considered graph
  * @param source the node from witch one arrives on target
  * @param target the node the considered node (one will obtain an iterator on the neighboors of that node)
  *
  * Since Tulip enables to order the edges around nodes, it is possible to traverse the nodes according
  * to that ordering. It is necessary to use that function if one wants to take into account the embedding
  * of the graph. Such functionnality is really useful when dealing with planar graphs. However if one wants
  * more efficient data structure for planar graphs one should consider using PlanarConMap.
  *
  * @see EdgeMapIterator
  * @see PlanarConMap
  */
struct TLP_SCOPE NodeMapIterator : public Iterator<node> {
  ///
  NodeMapIterator(Graph *sg, node source, node target);
  ~NodeMapIterator();
  ///Return the next element
  node next();
  ///Return true if it exist a next element
  bool hasNext();

private :
  std::list<node> cloneIt;
  std::list<node>::iterator itStl;
};

/**
  * @class EdgeMapIterator
  * @brief Iterator that enables to traverse the graph taking into account the ordering of edges aroung nodes
  * @param sg the considered graph
  * @param source the edge from witch one arrives on target
  * @param target the node the considered node (one will obtain an iterator on the neighboors of that node)
  *
  * Since Tulip enables to order the edges around nodes, it is possible to traverse the nodes according
  * to that ordering. It is necessary to use that function if one wants to take into account the embedding
  * of the graph. Such functionnality is really useful when dealing with planar graphs. However if one wants
  * more efficient data structure for planar graphs one should consider using PlanarConMap.
  *
  * @see EdgeMapIterator
  * @see PlanarConMap
  */
struct TLP_SCOPE EdgeMapIterator : public Iterator<edge> {
  ///
  EdgeMapIterator(const Graph *sg, edge source, node target);
  ///Return the next element
  edge next();
  ///Return true if it exist a next element
  bool hasNext();

private :
  std::vector<edge> adj;
  edge start;
  int treat;
  unsigned int pos;
  bool finished;
};


}
#endif
#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
