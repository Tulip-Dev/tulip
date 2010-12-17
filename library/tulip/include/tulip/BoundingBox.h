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
#ifndef Tulip_BOUNDINGBOX_H
#define Tulip_BOUNDINGBOX_H

#include <utility>
#include <limits>

#include "tulip/Vector.h"

namespace tlp {
    /**
     * \brief class for 3D BoundingBox
     *
     * Enables to both create and manipulate a 3D Axis Aligned box
     *
     * Author : <a href="www.tulip-software.org">Tulip team</a>
     */
  struct BoundingBox : public Array<Vec3f, 2> {
  
      /**
        * create an invalid boundig box
        */
      BoundingBox() {
          (*this)[0].fill(1);
          (*this)[1].fill(-1);
          assert(!isValid());
      }
      /**
       * create a valid bounding
       * validity is test in debug mode
       */
      BoundingBox(const tlp::Vec3f& min, const tlp::Vec3f& max) {
        (*this)[0] = min;
        (*this)[1] = max;
        assert(isValid());
     }
      /**
      * assertion is raised in debug if the BoundingBox is not Valid
      * @return center of the bounding
      */
      Vec3f center() const {
          assert(isValid());
          return ((*this)[0] + (*this)[1]) / 2.f;
      }

    /**
      * @brief recompute the current Bounding to ensure that coord belongs to it
      *
      */
    void expand(const tlp::Vec3f& coord) {
      if(!isValid()) {
          (*this)[0] = coord;
          (*this)[1] = coord;
      } else {
          (*this)[0] = tlp::minVector((*this)[0], coord);
          (*this)[1] = tlp::maxVector((*this)[1], coord);
      }
    }
    /**
      * @brief translate the bounding
      *
      */
    void translate(const tlp::Vec3f& vec) {
        (*this)[0] += vec;
        (*this)[1] += vec;
    }
    /**
    * @return true if the Rectangle is well define [0] min corner, [1] max corner.
    */
    bool isValid() const {
        return (*this)[0][0] <= (*this)[1][0] && (*this)[0][1] <= (*this)[1][1] && (*this)[0][2] <= (*this)[1][2];
    }
    /**
      * Build the 8 points of the bounding box
      */
    void getCompleteBB(Vec3f bb[8]) const{
      bb[0] = (*this)[0];
      bb[1] = (*this)[0];
      bb[1][0] = (*this)[1][0];
      bb[2] = bb[1];
      bb[2][1] = (*this)[1][1];
      bb[3] = (*this)[0];
      bb[3][1] = (*this)[1][1];
      bb[4] = bb[0];
      bb[4][2] = (*this)[1][2];
      bb[5] = bb[1];
      bb[5][2] = (*this)[1][2];
      bb[6] = bb[2];
      bb[6][2] = (*this)[1][2];
      bb[7] = bb[3];
      bb[7][2] = (*this)[1][2];
    }

};
  
}

#endif // Tulip_BOUNDINGBOX_H
