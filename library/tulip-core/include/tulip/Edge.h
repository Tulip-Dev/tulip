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

#ifndef Tulip_EDGE_H
#define Tulip_EDGE_H
#include <climits>
#include <tulip/tuliphash.h>

namespace tlp {

/**
 * @ingroup Graph
 * @brief The edge struct represents an edge in a Graph object.
 *
 * This structure only contains an identifier, and a function to check if the edge is valid.
 * A edge is considered invalid when its id has the UINT_MAX value.
 *
 * Most operations performed on an edge (getting the source, target, etc) are available into the tlp::Graph object.
 *
 * @see tlp::node
 * @see tlp::Graph
 */
struct edge {
  /**
   * @brief id The identifier of the edge.
   */
  unsigned int id;

  /**
   * @brief edge creates an invalid edge.
   */
  edge():id(UINT_MAX) {}

  /**
   * @brief edge Create an edge of given identifier.
   * It is your responsibility to make sure an edge of this ID exists when you create the edge.
   * If you want to make sure this edge exists, use Graph::isElement(), as isValid() will only tell is the edge was correctly initialized.
   *
   * @param j the identifier this edge will use.
   */
  explicit edge(unsigned int j):id(j) {}

  /**
   * @brief operator unsigned int A convenience function to get the id of an edge.
   */
  operator unsigned int() const {
    return id;
  }

  /**
   * @brief operator != Compares two edges, checking that they are different..
   * @param n The other edge to compare this one to.
   * @return Whether or not the two edges are different.
   */
  bool operator==(const edge e) const {
    return id==e.id;
  }

  /**
   * @brief operator != Compares two edges, checking that they are identical.
   * @param n The other edge to compare this one to.
   * @return Whether or not the two edges are the same.
   */
  bool operator!=(const edge e) const {
    return id!=e.id;
  }

  /**
   * @brief isValid checks if the edge is valid.
   * An invalid edge is an edge whose id is UINT_MAX.
   *
   * @return whether the edge is valid or not.
   */
  bool isValid() const {
    return id!=UINT_MAX;
  }
};

}

#ifndef DOXYGEN_NOTFOR_DEVEL
//these three functions allow to use tlp::edge as a key in a hash-based data structure (e.g. hashmap).
TLP_BEGIN_HASH_NAMESPACE {
  template<>
  struct hash<tlp::edge>{
    size_t operator()(const tlp::edge e) const {return e.id;}
  };
} TLP_END_HASH_NAMESPACE

namespace std {
template<>
struct equal_to<tlp::edge> {
  size_t operator()(const tlp::edge e,const tlp::edge e2) const {
    return e.id==e2.id;
  }
};
template<>
struct less<tlp::edge> {
  size_t operator()(const tlp::edge e,const tlp::edge e2) const {
    return e.id<e2.id;
  }
};
}
#endif
#endif
