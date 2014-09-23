/**
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
#ifndef __OCTTREE_H__
#define __OCTTREE_H__

#include <tulip/Node.h>
#include <tulip/Coord.h>
#include <tulip/DoubleProperty.h>
#include <vector>

using namespace tlp;

/**
 * Octtree for graph nodes with positions in 3D space.
 * Contains all graph nodes that are located in a given cuboid in 3D space.
 * From Andreas Noack's java implementation.
 */

class OctTree {
  friend class LinLogLayout;

protected:
  bool isLeaf;
  bool firstNode;

  //Maximum depth of tree nodes.
  unsigned int MAX_DEPTH;// = 18;
  unsigned int MAX_CHILDREN;

  //For leafs, the corresponding graph node; for non-leafs <code>null</code>.
  tlp::node node;

  //Children of this tree node.
  //    std::vector<OctTree*>* children ;//= new OctTree[8];
  OctTree** _children;

  //Number of children of this tree node.
  unsigned int childCount;// = 0;
  //unsigned int childrenSize;

  //Barycenter of the contained graph nodes.
  Coord position;

  //Total weight of the contained graph nodes.
  double weight;

  //Minimum coordinates of the cuboid in each of the 3 dimensions.
  Coord minPos;

  //Maximum coordinates of the cuboid in each of the 3 dimensions.
  Coord maxPos;

  //The weight metric
  tlp::DoubleProperty *linLogWeight;

public:
  //constructor
  OctTree (tlp::node node,
           Coord position,
           Coord minPos,
           Coord maxPos,
           tlp::DoubleProperty* _linLogWeight,
           bool _fistNode);
  //desctructor
  ~OctTree ();

  //Adds a graph node to the octtree
  void addNode (tlp::node newNode,
                Coord newPos,
                unsigned int depth);

  //Adds a graph node to the OctTree, without changing the position and weight of the root
  void addNode2 (tlp::node newNode,
                 Coord newPos,
                 unsigned int depth);

  // Removes a graph node from the octtree
  void removeNode (tlp::node oldNode,
                   Coord oldPos,
                   unsigned int depth);
  // Returns the maximum extension of the octtree
  double width ();

  // Returns the height of the octtree
  int getHeight();

  // Returns the current node
  tlp::node getNode ();

  // Sets the maximum number of children for a branch of the octtree
  void setMaxChildren (unsigned int);

  // Prints the tree on a console output
  void printTree (unsigned int);

};

#endif // __OCTTREE_H__
