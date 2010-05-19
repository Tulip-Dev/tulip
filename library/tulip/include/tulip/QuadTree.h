#ifndef QUADTREE_H
#define QUADTREE_H

#include <map>
#include <iostream>

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
    QuadTreeNode(const BoundingBox &box):_box(box) {
      for(int i=0; i<4; ++i)
        children[i] = 0;
    }
    /**
     * Basic destructor
     */
    ~QuadTreeNode() {
      for(int i=0; i<4; ++i)
        if (children[i] != 0) delete children[i];
    }
    /**
     * Insert an element in the quadtree
     */
    void insert(const BoundingBox &box, const TYPE id) {
      if(box.first[0]==box.second[0] && box.first[1]==box.second[1])
        return;

      //Check for infini recursion : check if we are on float limit case
      Coord subBox=(_box.first+_box.second)/2.f;
      if((subBox[0] != _box.first[0] && subBox[0]!= _box.second[0]) || (subBox[1]!=_box.first[1] && subBox[1]!=_box.second[1])){
        for (int i=0; i<4; ++i) {
          if (isInside(box, getChildBox(i))) {
            getChild(i)->insert(box, id);
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
    void getElements(const BoundingBox &box, std::vector<TYPE> &result) const{
      if (intersect(box, _box)) {
        for (size_t i=0; i<entities.size(); ++i){
          result.push_back(entities[i]);
        }

        for (unsigned int i=0; i<4; ++i) {
          if (children[i]!=0)
            children[i]->getElements(box, result);
        }
      }
    }

    /**
     * Return all elements of the quadtree
     */
    void getElements(std::vector<TYPE> &result) const{
      for (size_t i=0; i<entities.size(); ++i){
        result.push_back(entities[i]);
      }
      for (unsigned int i=0; i<4; ++i) {
        if (children[i]!=0)
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
    void getElementsWithRatio(const BoundingBox &box, std::vector<TYPE> &result, float ratio = 1000.) const{
      if (intersect(box, _box)) {
        float xRatio = (box.second[0] - box.first[0]) / (_box.second[0] - _box.first[0]) ;
        float yRatio = (box.second[1] - box.first[1]) / (_box.second[1] - _box.first[1]);
        //elements are big enough and all of them must be displayed
        if (xRatio < ratio || yRatio < ratio) {
          for (size_t i=0; i<entities.size(); ++i){
            result.push_back(entities[i]);
          }
          for (unsigned int i=0; i<4; ++i) {
            if (children[i]!=0)
              children[i]->getElementsWithRatio(box, result, ratio);
          }
        }
        //elements are too small return only one elements (we must seach it)
        else {
          bool find=false;
          if (entities.size() > 0){
            result.push_back(entities[0]);
            find=true;
          }
          if(!find){
            for (unsigned int i=0; i<4; ++i) {
              if (children[i]!=0 && intersect(box, children[i]->_box)) {
                //if children[i]!=0 we are sure to find an elements in that branch of the tree
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
        BoundingBox box=getChildBox(i);
        if(box.first==_box.first && box.second==_box.second)
          assert(false);
        children[i] = new QuadTreeNode<TYPE>(box);
      }
      return children[i];
    }
    //======================================
    BoundingBox getChildBox(int i) {
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
      tlp::Coord I( (_box.first[0] + _box.second[0]) / 2.,
                    _box.first[1] ,
                    0);
      tlp::Coord E( _box.first[0],
                    (_box.first[1] + _box.second[1]) / 2.,
                    0);
      tlp::Coord F(I[0], E[1], 0);
      tlp::Coord G(_box.second[0],
                   F[1],
                   0);
      tlp::Coord H(F[0], _box.second[1], 0);
      switch(i) {
      case 0: return BoundingBox(_box.first, F); break;
      case 1: return BoundingBox(I, G); break;
      case 2: return BoundingBox(F, _box.second); break;
      case 3: return BoundingBox(E, H);
      default: std::cerr << "ERROR" << __PRETTY_FUNCTION__ << std::endl; exit(1);
      }
    }
    //======================================
    //reutrn true id p is in box
    bool isInside(const tlp::Coord &p, const BoundingBox &box) const{
      if ((p[0] <= box.second[0]) &&
          (p[0] >= box.first[0]) &&
          (p[1] <= box.second[1]) &&
          (p[1] >= box.first[1]))
        return true;
      return false;
    }

    //return true if box1 is in box2
    bool isInside(const BoundingBox &box1, const BoundingBox &box2) const{
      return isInside(box1.first, box2) && isInside(box1.second, box2);
    }
    //======================================
    //return true if box1 intersect box2
    bool intersect(const BoundingBox &box1, const BoundingBox &box2) const{
      bool inside=(isInside(box1.first, box2) ||
                   isInside(box1.second, box2) ||
                   isInside(Coord(box1.first[0],box1.second[1],0.), box2) ||
                   isInside(Coord(box1.second[0],box1.first[1],0.), box2) ||
                   isInside(box2.first, box1) ||
                   isInside(box2.second, box1) ||
                   isInside(Coord(box2.first[0],box2.second[1],0.), box1) ||
                   isInside(Coord(box2.second[0],box2.first[1],0.), box1));
      if(inside)
        return true;

      return ((box1.first[0] < box2.first[0] && box1.second[0] > box2.second[0] && box1.first[1] > box2.first[1] && box1.second[1] < box2.second[1]) ||
              (box1.first[1] < box2.first[1] && box1.second[1] > box2.second[1] && box1.first[2] > box2.first[2] && box1.second[2] < box2.second[2]) ||
              (box2.first[0] < box1.first[0] && box2.second[0] > box1.second[0] && box2.first[1] > box1.first[1] && box2.second[1] < box1.second[1]) ||
              (box2.first[1] < box1.first[1] && box2.second[1] > box1.second[1] && box2.first[2] > box1.first[2] && box2.second[2] < box1.second[2]));
    }
    //======================================
    QuadTreeNode *children[4];
    std::vector<TYPE> entities;
    BoundingBox _box;
  };

}

#endif // QUADTREE_H

