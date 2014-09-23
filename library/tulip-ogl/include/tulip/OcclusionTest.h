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

#ifndef Tulip_OCCLUSIONTEST_H
#define Tulip_OCCLUSIONTEST_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <vector>

#include <tulip/Rectangle.h>

namespace tlp {

typedef Rectangle<int> RectangleInt2D;

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
  void clear() {
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
      return true;
    }

    return false;
  }
  /**
  * @brief test wehter or nor the AABB intersect with a AABB already inserted.
  *
  * @return true is the ABBB intersect else false.
  */
  bool testRectangle(const RectangleInt2D &rec) {
    bool intersect=false;

    for (std::vector<RectangleInt2D>::const_iterator it=data.begin(); it!=data.end(); ++it) {
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
///@endcond
