#ifndef QUADTREE2_H
#define QUADTREE2_H

#include <map>
#include <iostream>

#include <tulip/Coord.h>

typedef std::pair<tlp::Coord, tlp::Coord> BBox;

namespace tlp {

class QuadTreeNode {

 public:

  //======================================
  /*
   * build a new Quadtree
   * to work correctly box should be the bounding box
   * of all elements inserted in that QuadTree
   */

 QuadTreeNode(const BBox &box):entitiesInLong(NULL),entitiesInInt(NULL),_box(box) {
    for(int i=0; i<4; ++i)
      children[i] = 0;
  }
  //======================================
  ~QuadTreeNode() {
    for(int i=0; i<4; ++i)
      if (children[i] != 0) delete children[i];
    if(entitiesInLong)
      delete entitiesInLong;
    if(entitiesInInt)
      delete entitiesInInt;
  }
  //======================================
  //insert an element with id id in the Quadtree
  void insert(const BBox &box, const unsigned int id) {
    if(box.first[0]==box.second[0] && box.first[1]==box.second[1])
      return;

    //Check for infini recursion : check if we are on float limit case
    Coord subBox=(_box.first+_box.second)/2.;
    if((subBox[0] != _box.first[0] && subBox[0]!= _box.second[0]) || (subBox[1]!=_box.first[1] && subBox[1]!=_box.second[1])){
      for (int i=0; i<4; ++i) {
	if (isInside(box, getChildBox(i))) {
	  getChild(i)->insert(box, id);
	  return;
	}
      }
    }

    if(!entitiesInInt){
      assert(entitiesInLong==NULL);
      entitiesInInt=new std::vector<unsigned int>();
    }

    entitiesInInt->push_back(id);
  }
  //======================================
  //insert an element with address address in the Quadtree
  void insert(const BBox &box, const unsigned long address) {
    if(box.first[0]==box.second[0] && box.first[1]==box.second[1])
          return;

    //Check for infini recursion : check if we are on float limit case
    Coord subBox=(_box.first+_box.second)/2.;
    if((subBox[0] != _box.first[0] && subBox[0]!= _box.second[0]) || (subBox[1]!=_box.first[1] && subBox[1]!=_box.second[1])){
      for (int i=0; i<4; ++i) {
	if (isInside(box, getChildBox(i))) {
	  getChild(i)->insert(box, address);
	  return;
	}
      }
    }

    if(!entitiesInLong){
      assert(entitiesInInt==NULL);
      entitiesInLong=new std::vector<unsigned long>();
    }

    entitiesInLong->push_back(address);
  }
  //======================================
  /*
   * return all elements that could be in
   * the given box (the function ensures that
   * all elements inside the box are return. However
   * some elements not inside the box can be returned.
   */
  void getElements(const BBox &box, std::vector<unsigned int> *resultForInt, std::vector<unsigned long> *resultForLong) const{
    if (intersect(box, _box)) {
      if(entitiesInInt){
        for (unsigned int i=0; i<entitiesInInt->size(); ++i){
          assert(resultForInt);
          resultForInt->push_back((*entitiesInInt)[i]);
        }
      }
      if(entitiesInLong){
        for (unsigned int i=0; i<entitiesInLong->size(); ++i){
          assert(resultForLong);
          resultForLong->push_back((*entitiesInLong)[i]);
        }
      }

      for (unsigned int i=0; i<4; ++i) {
        if (children[i]!=0)
          children[i]->getElements(box, resultForInt, resultForLong);
      }
    }
  }

  /*
    same as getElements, however if the size of the elements are to small compare
    to the size of the box (equivalent to have severeal item at the same position on the screen)
    only one elements is returned for the small cells.
    The ratio should fixed according to the number of pixels displayed.
    If we have a 1000*800 screen we can merge items of box into a single item if
    the size of box is max(1000,800) times smaller than the box given in parameter.
    so the ratio should be 1000.(merge elements that are 1000 times smaller
  */

  void getElementsWithRatio(const BBox &box, std::vector<unsigned int> *resultForInt, std::vector<unsigned long> *resultForLong, float ratio = 1000.) const{
    if (intersect(box, _box)) {
      float xRatio = (box.second[0] - box.first[0]) / (_box.second[0] - _box.first[0]) ;
      float yRatio = (box.second[1] - box.first[1]) / (_box.second[1] - _box.first[1]);
      //elements are big enough and all of them must be displayed
      if (xRatio < ratio || yRatio < ratio) {
        if(entitiesInInt){
          for (unsigned int i=0; i<entitiesInInt->size(); ++i){
            assert(resultForInt);
            resultForInt->push_back((*entitiesInInt)[i]);
          }
        }
        if(entitiesInLong){
          for (unsigned int i=0; i<entitiesInLong->size(); ++i){
            assert(resultForLong);
            resultForLong->push_back((*entitiesInLong)[i]);
          }
        }
        for (unsigned int i=0; i<4; ++i) {
          if (children[i]!=0)
            children[i]->getElementsWithRatio(box, resultForInt, resultForLong, ratio);
        }
      }
      //elements are too small return only one elements (we must seach it)
      else {
        assert(!(resultForInt && resultForLong));
        bool find=false;
        if(entitiesInInt){
          if (entitiesInInt->size() > 0){
            resultForInt->push_back((*entitiesInInt)[0]);
            find=true;
          }
        }
        if(entitiesInLong){
          if(entitiesInLong->size() > 0){
            resultForLong->push_back((*entitiesInLong)[0]);
            find=true;
          }
        }
        if(!find){
          for (unsigned int i=0; i<4; ++i) {
            if (children[i]!=0 && intersect(box, children[i]->_box)) {
              //if children[i]!=0 we are sure to find an elements in that branch of the tree
              //thus we do not have to explore the other branches.
              children[i]->getElementsWithRatio(box, resultForInt,resultForLong, ratio);
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
      BBox box=getChildBox(i);
      if(box.first==_box.first && box.second==_box.second)
        assert(false);
      children[i] = new QuadTreeNode(box);
    }
    return children[i];
  }
  //======================================
  BBox getChildBox(int i) {
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
    case 0: return BBox(_box.first, F); break;
    case 1: return BBox(I, G); break;
    case 2: return BBox(F, _box.second); break;
    case 3: return BBox(E, H);
    default: std::cerr << "ERROR" << __PRETTY_FUNCTION__ << std::endl; exit(1);
    }
  }
  //======================================
  //reutrn true id p is in box
  bool isInside(const tlp::Coord &p, const BBox &box) const{
    if ((p[0] <= box.second[0]) &&
        (p[0] >= box.first[0]) &&
        (p[1] <= box.second[1]) &&
        (p[1] >= box.first[1]))
      return true;
    return false;
  }

  //return true if box1 is in box2
  bool isInside(const BBox &box1, const BBox &box2) const{
    return isInside(box1.first, box2) && isInside(box1.second, box2);
  }
  //======================================
  //return true if box1 intersect box2
  bool intersect(const BBox &box1, const BBox &box2) const{
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
 private:
  QuadTreeNode *children[4];
  std::vector<unsigned long> *entitiesInLong;
  std::vector<unsigned int> *entitiesInInt;
  BBox _box;
};

}

#endif // QUADTREE2_H

