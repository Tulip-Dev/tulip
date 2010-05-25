#ifndef Tulip_OCCLUSIONTEST_H
#define Tulip_OCCLUSIONTEST_H
#ifndef DOXYGEN_NOTFOR_DEVEL
#include <tulip/Rectangle.h>

namespace tlp {

struct TLP_GL_SCOPE RectangleInt2D : public tlp::Rectangle<int> {
  RectangleInt2D(int x, int y, int x1, int y1) {
    (*this)[0][0]=x;
    (*this)[0][1]=y;
    (*this)[1][0]=x1;
    (*this)[1][1]=y1;
  }
  RectangleInt2D(const RectangleInt2D &r): tlp::Rectangle<int>(r){}
};

/**
* @brief Manage a set of non overlapping 2D Axis Aligned Bounding Box
*
* That class enables to store a set of non overlapping 2D AABB.
*
* @todo Use the Tulip quadtree to store AABB and thus speedup testRectangle function
*/
struct TLP_GL_SCOPE OcclusionTest {
  std::vector<RectangleInt2D> data;
  /**
  * Remove all 2D AABB previously added.
  */
  void reset() {
    data.clear();
  }
  /**
  * Add a new 2D AABB to the set of non overlapping AABB
  * if that AABB intersect with AABB already inserted, 
  * the AABB is not inserted.
  *
  * @return true if the AABB is inserted else false.
  *
  */
  bool addRectangle(const RectangleInt2D &rec) {
    if (!testRectangle(rec)) {
      data.push_back(rec);
      return false;
    }
    return true;
  }
  /**
  * @brief test wehter or nor the AABB intersect with a AABB already inserted.
  *
  * @return true is the ABBB intersect else false.
  */
  bool testRectangle(const RectangleInt2D &rec) {
    bool intersect=false;
    std::vector<RectangleInt2D>::const_iterator it;
    for (it=data.begin(); it!=data.end(); ++it) {
      if (rec.intersect(*it)) {
	intersect=true;
	break;
      }
    }
    return intersect;
  }

};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
