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


#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef _TLP_PLANE_H_
#define _TLP_PLANE_H_

#include <tulip/Coord.h>

namespace tlp {
/** \brief Class used to describe a plane
 *
 * This class is used to represent a plane with it's coordinates.
 * it follows the basic plane equation aX + bY + cZ + d = 0
 *
 * \author Delorme Maxime
 * \date 23/07/2005
 * \version 1.0
 */
class TLP_SCOPE Plane {
public:

  float a, b, c, d;

  /**
   * Constructs a Plane
   * \attention a = b = c = d = 1.0f
   */
  Plane();

  /**
   * Constructs a plane with given coordinates
   */
  Plane(float a, float b, float c, float d);

  /**
   * Destructs a plane
   */
  ~Plane();

  /**
   * Static function used to determine the last coordinate of a point in order to have it on the plane
   * It follows the equation : Z = (aX + bY + d) / c
   */
  static float planeValue(float a, float b, float c, float d, float x, float y);

  /**
   * Computes a quad from the plane
   */
  bool computePlane(Coord &p1, Coord &p2, Coord &p3, Coord &p4);

  /**
   * Given a point, this function returns aX + bY + cZ + d.
   * This is mainly used to determine if a point is above or under the plane.
   */
  float planePointValue(const Coord &pos);
};

}
#endif
#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
