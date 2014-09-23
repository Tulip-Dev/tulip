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

#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

#include <tulip/Rectangle.h>
#include <tulip/Coord.h>

namespace tlp {

/** \brief QuadTree template class
 *
 * This class provide QuadTree system
 */
template <class TYPE> class QuadTreeNode {

public:

  //======================================
  /*
     * build a new Quadtree
     * to work correctly box should be the bounding box
     * of all elements inserted in that QuadTree
     */
  /**
   * Contructor, you have to put the global bounding box of the quadtree
   */
  QuadTreeNode(const tlp::Rectangle<float> &box):_box(box) {
    assert(_box.isValid());

    for(int i=0; i<4; ++i)
      children[i] = 0;
  }
  /**
   * Basic destructor
   */
  ~QuadTreeNode() {
    for(int i=0; i<4; ++i)
      if (children[i] != NULL) delete children[i];
  }
  /**
   * Insert an element in the quadtree
   */
  void insert(const tlp::Rectangle<float> &box, const TYPE id) {
    assert(box.isValid());
    assert(_box.isValid());

    if (box[0]==box[1])
      return;

    //Check for infini recursion : check if we are on float limit case
    Vec2f subBox((_box[0]+_box[1])/2.f);

    if( !((subBox == _box[0]) || (subBox == _box[1]))) {
      for (int i=0; i<4; ++i) {
        if (getChildBox(i).isInside(box)) {
          QuadTreeNode *child=getChild(i);

          if(child)
            child->insert(box, id);
          else
            entities.push_back(id);

          return;
        }
      }
    }

    entities.push_back(id);
  }
  /**
   * return all elements that could be in
   * the given box (the function ensures that
   * all elements inside the box are return. However
   * some elements not inside the box can be returned.
   */
  void getElements(const tlp::Rectangle<float> &box, std::vector<TYPE> &result) const {
    assert(box.isValid());
    assert(_box.isValid());

    if (_box.intersect(box)) {
      for (size_t i=0; i<entities.size(); ++i) {
        result.push_back(entities[i]);
      }

      for (unsigned int i=0; i<4; ++i) {
        if (children[i]!=NULL)
          children[i]->getElements(box, result);
      }
    }
  }

  /**
   * Return all elements of the quadtree
   */
  void getElements(std::vector<TYPE> &result) const {
    for (size_t i=0; i<entities.size(); ++i) {
      result.push_back(entities[i]);
    }

    for (unsigned int i=0; i<4; ++i) {
      if (children[i]!=NULL)
        children[i]->getElements(result);
    }
  }

  /**
   * same as getElements, however if the size of the elements are to small compare
   * to the size of the box (equivalent to have severeal item at the same position on the screen)
   * only one elements is returned for the small cells.
   * The ratio should fixed according to the number of pixels displayed.
   * If we have a 1000*800 screen we can merge items of box into a single item if
   * the size of box is max(1000,800) times smaller than the box given in parameter.
   * so the ratio should be 1000.(merge elements that are 1000 times smaller
   */
  void getElementsWithRatio(const tlp::Rectangle<float> &box, std::vector<TYPE> &result, float ratio = 1000.) const {
    assert(_box.isValid());
    assert(box.isValid());

    if (_box.intersect(box)) {
      float xRatio = (box[1][0] - box[0][0]) / (_box[1][0] - _box[0][0]) ;
      float yRatio = (box[1][1] - box[0][1]) / (_box[1][1] - _box[0][1]);

      //elements are big enough and all of them must be displayed
      if (xRatio < ratio || yRatio < ratio) {
        for (size_t i=0; i<entities.size(); ++i) {
          result.push_back(entities[i]);
        }

        for (unsigned int i=0; i<4; ++i) {
          if (children[i]!=NULL)
            children[i]->getElementsWithRatio(box, result, ratio);
        }
      }
      //elements are too small return only one elements (we must seach it)
      else {
        bool find=false;

        if (entities.size() > 0) {
          result.push_back(entities[0]);
          find=true;
        }

        if(!find) {
          for (unsigned int i=0; i<4; ++i) {
            if (children[i]!=NULL && children[i]->_box.intersect(box)) {
              //if children[i]!=NULL we are sure to find an elements in that branch of the tree
              //thus we do not have to explore the other branches.
              children[i]->getElementsWithRatio(box, result, ratio);
              break;
            }
          }
        }
      }
    }
  }

private:
  //======================================
  QuadTreeNode* getChild(int i) {
    if (children[i] == 0) {
      Rectangle<float> box (getChildBox(i));

      if(box[0] ==_box[0] && box[1]==_box[1])
        return NULL;

      children[i] = new QuadTreeNode<TYPE>(box);
    }

    return children[i];
  }
  //======================================
  Rectangle<float> getChildBox(int i) {
    assert(_box.isValid());
    // A***I***B
    // *-------*
    // E---F---G
    // *-------*
    // *-------*
    // D***H***C
    // 0 => AIFE
    // 1 => IBGF
    // 2 => FGCH
    // 3 => FHDE
    Vec2f I;
    I[0] = (_box[0][0] + _box[1][0]) / 2.;
    I[1] = _box[0][1];
    Vec2f E;
    E[0] =  _box[0][0];
    E[1] = (_box[0][1] + _box[1][1]) / 2.;
    Vec2f F;
    F[0] = I[0];
    F[1] = E[1];
    Vec2f G;
    G[0] = _box[1][0];
    G[1] = F[1];
    Vec2f H;
    H[0] = F[0];
    H[1] = _box[1][1];

    switch(i) {
    case 0:
      return Rectangle<float>(_box[0], F);
      break;

    case 1:
      return Rectangle<float>(I, G);
      break;

    case 2:
      return Rectangle<float>(F, _box[1]);
      break;

    case 3:
      return Rectangle<float>(E, H);

    default:
      tlp::error() << "ERROR" << __PRETTY_FUNCTION__  << std::endl;
      exit(1);
    }
  }
  //======================================
  QuadTreeNode *children[4];
  std::vector<TYPE> entities;
  tlp::Rectangle<float> _box;
};

}

#endif // QUADTREE_H

///@endcond
