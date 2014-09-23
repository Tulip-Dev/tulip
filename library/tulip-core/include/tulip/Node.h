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

#ifndef Tulip_NODE_H
#define Tulip_NODE_H
#include <climits>
#include <tulip/tuliphash.h>

namespace tlp {

/**
 * @ingroup Graph
 * @brief The node struct represents a node in a Graph object.
 *
 * This structure only contains an identifier, and a function to check if the node is valid.
 * A node is considered invalid when its id has the UINT_MAX value.
 *
 * Most operations performed on a node (getting out edges etc) are available into the tlp::Graph object.
 *
 * @see tlp::edge
 * @see tlp::Graph
 */
struct node {
  /**
   * @brief id The identifier of the node.
   */
  unsigned int id;

  /**
   * @brief node creates an invalid node.
   */
  node():id(UINT_MAX) {}

  /**
   * @brief node Create a node of given identifier.
   * It is your responsibility to make sure a node of this ID exists when you create the node.
   * If you want to make sure this node exists, use Graph::isElement(), as isValid() will only tell is the node was correctly initialized.
   *
   * @param j the identifier this node will use.
   */
  explicit node(unsigned int j):id(j) {}

  /**
   * @brief operator unsigned int A convenience function to get the id of a node.
   */
  operator unsigned int() const {
    return id;
  }

  /**
   * @brief operator != Compares two nodes, checking that they are different..
   * @param n The other node to compare this one to.
   * @return Whether or not the two nodes are different.
   */
  bool operator!=(const node n) const {
    return id!=n.id;
  }

  /**
   * @brief operator != Compares two nodes, checking that they are identical.
   * @param n The other node to compare this one to.
   * @return Whether or not the two nodes are the same.
   */
  bool operator==(const node n) const {
    return id==n.id;
  }

  /**
   * @brief isValid checks if the node is valid.
   * An invalid node is a node whose id is UINT_MAX.
   *
   * @return whether the node is valid or not.
   */
  bool isValid() const {
    return id!=UINT_MAX;
  }
};

}

#ifndef DOXYGEN_NOTFOR_DEVEL
//these three functions allow to use tlp::node as a key in a hash-based data structure (e.g. hashmap).
TLP_BEGIN_HASH_NAMESPACE {
  template<> struct hash<tlp::node> {
    size_t operator()(const tlp::node n) const {return n.id;}
  };
} TLP_END_HASH_NAMESPACE


namespace std {
template<> struct equal_to<tlp::node> {
  size_t operator()(const tlp::node n,const tlp::node n2) const {
    return n.id==n2.id;
  }
};
template<> struct less<tlp::node> {
  size_t operator()(const tlp::node n,const tlp::node n2) const {
    return n.id<n2.id;
  }
};
}
#endif // DOXYGEN_NOTFOR_DEVEL

#endif
