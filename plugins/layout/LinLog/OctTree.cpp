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
#include <math.h>
#include "OctTree.h"

using namespace tlp;


/**
 * Creates an OctTree containing exactly one graph node.
 *
 * @param node     graph node
 * @param position position of the graph node
 * @param minPos   minimum coordinates of the cuboid
 * @param maxPos   maximum coordinates of the cuboid
 */
OctTree::OctTree (tlp::node _node,
                  Coord _position,
                  Coord _minPos,
                  Coord _maxPos,
                  tlp::DoubleProperty* _linLogWeight,
                  bool _firstNode)
  : position (_position[0],_position[1],_position[2]),
    minPos (_minPos[0],_minPos[1],_minPos[2]),
    maxPos (_maxPos[0],_maxPos[1],_maxPos[2]) {

  firstNode = _firstNode;
  linLogWeight = _linLogWeight;
  MAX_DEPTH = 8;
  MAX_CHILDREN = 8;
  //node = NULL;
  //if(_node != NULL)
  node = _node;
  isLeaf = true;

  if (firstNode) {
    isLeaf = false;
  }

  //childrenSize = 0;
  childCount = 0;
  _children = NULL; //new OctTree*[8]; //init à NULL?
  this->weight = 0.0;

  /*
    for(int i=0;i<8;++i)
    {
    _children[i] = NULL;
    }
  */
  // if (_node != NULL)
  // {
  if(!firstNode)
    this->weight = linLogWeight->getNodeValue(_node);

  // }else{
  //   std::cerr<<"assert: creating a an octtree with a null node\n";
  // }
}

/**
 * Deletes the an OctTree.
 */
OctTree::~OctTree () {
  //std::cerr<<"deletion of an octtree\n";
  if (_children!=NULL) {

    for (unsigned int i=0; i<MAX_CHILDREN; ++i) {
      if(_children[i]!=NULL)
        delete _children[i];

      _children[i] = NULL;
    }

    delete[] _children;
    _children = NULL;
  }
}

/**
 *  Sets the maximum number of children in the OctTree.
 *
 *  @param max maximum nimber of children
 */

void
OctTree::setMaxChildren (unsigned int max) {
  MAX_CHILDREN = max;
}

/**
 * Adds a graph node to the octtree.
 *
 * @param newNode  graph node
 * @param newPos   position of the graph node
 * @param depth    depth of this tree node in the octtree
 */
void
OctTree::addNode (tlp::node newNode,
                  Coord newPos,
                  unsigned int depth) {
  if (depth > MAX_DEPTH-1) {
    std::cerr<<"assert: adding a node at a depth deeper than the max depth (add1)\n";
    return;
  }

  /*
  if(newNode == NULL)
  {
  std::cerr<<"assert: adding a null node\n";
  return;
  }
  */

  double nnWeight = linLogWeight->getNodeValue(newNode);

  if (nnWeight == 0.0) {
    //std::cerr<<"assert: new node has a null weight, cannot add\n";
    return;
  }

  //on passe de feuille a branche
  //if (node != NULL)
  //{
  if (isLeaf) {
    addNode2 (node, position, depth); //!depth
    isLeaf = false;
  }

  //  node = NULL;
  //}

  for (int d = 0; d < 3; ++d) {
    position[d] = (weight*position[d] + nnWeight * newPos[d])
                  / (weight+nnWeight);
  }

  weight += nnWeight;

  addNode2 (newNode, newPos, depth); //!depth
}

/**
 * Returns the current node of this OctTree
 */

tlp::node
OctTree::getNode() {
  return node;
}

/**
 * Adds a graph node to the OctTree,
 * without changing the position and weight of the root.
 *
 * @param newNode  graph node
 * @param newPos   position of the graph node
 * @param depth    depth of this tree node in the octtree
 */
void
OctTree::addNode2 (tlp::node newNode,
                   Coord newPos,
                   unsigned int depth) {
  if (depth > MAX_DEPTH-1) {
    std::cerr<<"assert: adding a node at a depth deeper than the max depth! (add2)\n";
    return;
  }

  if (depth == MAX_DEPTH-1) {
    if (childCount == MAX_CHILDREN) {
      OctTree** _oldChildren = _children;
      _children = new OctTree*[2*MAX_CHILDREN];

      for (unsigned int i=0; i<MAX_CHILDREN; ++i) {
        _children[i] = _oldChildren[i];
      }

      for (unsigned int i=MAX_CHILDREN; i<MAX_CHILDREN*2; ++i) {
        _children[i] = NULL;
      }

      MAX_CHILDREN *= 2;
    }

    if (childCount == 0 || _children == NULL) {
      _children = new OctTree*[MAX_CHILDREN];

      for (unsigned int i = 0; i<MAX_CHILDREN; ++i)
        _children[i] = NULL;
    }

    _children[childCount++] = new OctTree(newNode, newPos, newPos, newPos,linLogWeight, false);

    return;
  }

  //on localise le noeud
  int childIndex = 0;

  for (int d = 0; d < 3; ++d) {
    if (newPos[d] > (minPos[d]+maxPos[d])/2) {
      childIndex += 1 << d;
    }
  }

  if (childCount == 0 || _children == NULL) {
    _children = new OctTree*[MAX_CHILDREN];

    for (unsigned int i = 0; i<MAX_CHILDREN; ++i)
      _children[i] = NULL;
  }

  //si la place est vide
  if (_children[childIndex] == NULL) {

    Coord newMinPos;
    Coord newMaxPos;

    for (int d = 0; d < 3; ++d) {
      if ((childIndex & 1<<d) == 0) {
        newMinPos[d] = minPos[d];
        newMaxPos[d] = (minPos[d] + maxPos[d]) / 2;
      }
      else {
        newMinPos[d] = (minPos[d] + maxPos[d]) / 2;
        newMaxPos[d] = maxPos[d];
      }
    }

    childCount++;
    _children[childIndex] = new OctTree(newNode, newPos, newMinPos, newMaxPos, linLogWeight, false);
  }
  else {
    _children[childIndex]->addNode(newNode, newPos, depth+1);
  }
}

/**
 * Prints the OctTree on a console output at the desired depth
 *
 * @param depth the desired depth
 */

void OctTree::printTree (unsigned int depth) {
  std::cerr<<"\n";

  for (unsigned int i=0; i<depth; ++i) {
    std::cerr<<"\t";;
  }

  std::cerr<<"[d("<<depth<<"),w("<<weight<<"),n("<<node.id<<"),l("<<isLeaf<<"),p("<<position[0]<<","<<position[1]<<","<<position[2]<<"),";

  if(_children!=NULL) {
    for (unsigned int i=0; i<MAX_CHILDREN; ++i) {
      if(_children[i] ==NULL) {
        std::cerr<<"X,";
      }
      else {
        std::cerr<<"O,";
      }
    }

    for (unsigned int i=0; i<MAX_CHILDREN; ++i)
      if(_children[i] !=NULL)
        if (depth < MAX_DEPTH)
          _children[i]->printTree(depth+1);

  }

  std::cerr<<"]\n";

}

/**
 * Removes a graph node from the octtree.
 *
 * @param oldNode  graph node
 * @param oldPos   position of the graph node
 * @param depth    current depth
 */
void OctTree::removeNode (tlp::node oldNode,
                          Coord oldPos,
                          unsigned int depth) {
  //std::cerr<<"depth: "<<depth<<"\n";

  /*
    if(_children!=NULL)
    {
    std::cerr<<"[";

    for (int i=0; i<MAX_CHILDREN; ++i)
    {
    if(_children[i] ==NULL)
    {
    std::cerr<<" NULL |";
    }
    else
    std::cerr<<" "<<_children[i]<<" |";
    }
    std::cerr<<"\n";
    }*/


  if (depth > MAX_DEPTH-1) {
    std::cerr<<"assert: remove a node at a depth deeper than the max depth: "<<depth<<" / "<<MAX_DEPTH-1<<"\n";
    return;
  }

  double onWeight = linLogWeight->getNodeValue(oldNode);

  if (onWeight == 0.0) {
    //std::cerr<<"assert: removing a node with a weight of 0\n";
    return;
  }

  if (weight <= onWeight) {
    weight = 0.0;
    //    node = NULL;

    for (unsigned int i = 0; i < childCount; ++i) {
      delete _children[i];
      _children[i] = NULL;
    }

    delete[] _children;
    _children = NULL;

    childCount = 0;
    return;
  }

  for (int d = 0; d < 3; ++d) {
    position[d] = (weight * position[d] - onWeight * oldPos[d])
                  / (weight - onWeight);
  }

  weight -= onWeight;


  if (depth == MAX_DEPTH-1) {
    if(childCount>0) {
      //std::cerr<<"childCount: "<<childCount<<"\n";
      unsigned int childIndex = 0;

      /*
      if((_children[childIndex])->node == oldNode)
      {
      std::cerr<<"Identical nodes :) before while\n";
      }
      */

      bool endwhile = false;

      while (endwhile == false) {
        if(childIndex < MAX_CHILDREN) {
          if (_children[childIndex] == NULL) {
            std::cerr<<"this part of the tree is null\n";
            childIndex++;
          }
          /*          else if ((_children[childIndex])->node == NULL || oldNode == NULL)
                {
                //std::cerr<<"this tree node or the searched one is NULL: "<<oldNode<<" and "<<(_children[childIndex])->node<<"\n";
                childIndex++;
                }
          */
          else if((_children[childIndex])->node.id != oldNode.id) {
            //std::cerr<<"this tree node is different from the searched one: "<<oldNode.id<<" and "<<(_children[childIndex])->node.id<<"\n";
            childIndex++;
          }
          else {
            //std::cerr<<"node found :)\n";
            endwhile = true;
          }

        }
        else {
          std::cerr<<"we're stopping at the end of the table: "<<childIndex<<"\n";
          endwhile = true;
        }
      }

      if (childIndex == MAX_CHILDREN) {
        std::cerr<<"assert: removing a non existant node in the tree\n";
        return;
      }

      /*
      if((_children[childIndex])->node == oldNode)
      {
      //  std::cerr<<"Identical nodes :)\n";
      }
      */

      delete _children[childIndex];
      _children[childIndex] = NULL;

      for (unsigned int i = childIndex; i < childCount-1; ++i) {
        _children[i] = _children[i+1];
      }

      //delete _children[childCount];
      _children[childCount-1] = NULL; //delete

      --childCount;

    }
    else {
      std::cerr<<"assert ChildCount <= 0: "<< childCount<< "\n";
    }

  }
  else {
    int childIndex = 0;

    //on localise le noeud
    for (int d = 0; d < 3; ++d) {
      if (oldPos[d] > (minPos[d]+maxPos[d])/2) {
        childIndex += 1 << d;
      }
    }

    if(_children[childIndex]!=NULL) {

      _children[childIndex]->removeNode(oldNode, oldPos, depth+1);

      if (_children[childIndex]->weight == 0.0) {
        delete _children[childIndex];
        _children[childIndex] = NULL;
        --childCount;
      }
    }
    else {
      std::cerr<<"assert: the selected child it is not supposed to be NULL!\n";
      return;
    }
  }
}

/**
 * Returns the maximum extension of the octtree.
 *
 * @return maximum over all dimensions of the extension of the octtree
 */
double OctTree::width () {
  double width = 0.0;

  for (int d = 0; d < 3; ++d) {
    if (maxPos[d] - minPos[d] > width) {
      width = maxPos[d] - minPos[d];
    }
  }

  return width;
}

/**
 * Returns the height of the octtree.
 *
 * @return height of the octtree
 */
int
OctTree::getHeight () {
  int height = -1;

  for (unsigned int i=0; i<childCount; ++i) {
    OctTree* aChild = _children[i];

    if (aChild != NULL) {
      height = std::max(height, aChild->getHeight());
    }
  }

  return height+1;
}
