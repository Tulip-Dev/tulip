/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/Iterator.h>
#include <tulip/Graph.h>
#include <tulip/tulipconf.h>
#include <list>
#include <vector>

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TULIP_NODEMAPITERATOR_H
#define TULIP_NODEMAPITERATOR_H

namespace tlp {

/** \addtogroup graphs */ 
/*@{*/

TLP_SCOPE edge nextFaceEdge(Graph* g, edge e, node n);

/**
 *
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
 *
 */
struct TLP_SCOPE EdgeMapIterator : public Iterator<edge> {
  ///
  EdgeMapIterator(Graph *sg, edge source, node target);
  ///
  ~EdgeMapIterator();
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
/*@}*/

}
#endif
#endif //DOXYGEN_NOTFOR_DEVEL
