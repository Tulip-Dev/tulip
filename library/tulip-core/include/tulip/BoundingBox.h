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

#ifndef Tulip_BOUNDINGBOX_H
#define Tulip_BOUNDINGBOX_H

#include <tulip/Vector.h>
#include <tulip/tulipconf.h>

namespace tlp {
/**
 * \ingroup Structures
 * \brief This class represents the 3D bounding box of an object.
 * It is mostly used to determine whether or not two object are in a state of collision.
 *
 * It is defined by two 3d points, the first one (A) being the lowest point, the second (B) being the highest.
 * As a bounding box is a mathematical entity describing the lowest and highest points, whether these points are in the top-left corner or
 * lower-right corner depends on the axes we use.
 * Below is a crude ASCII-art description of the axes we use in our 3D world and the points where the min and max are thus positioned.
 * Through the rest of this class's documentation, it will be assumed that this is the convention.
 *
 *
 * @verbatim
      y
     |
     |
     |_____ x
    /
   /
  z

     _________ B
    /        /|
   /        / |
  /________/  |
  |        |  |
  |        |  |
  |        | /
  |________|/
  A

  @endverbatim
 *
 *
 * Author : <a href="www.tulip-software.org">Tulip team</a>
 */
struct TLP_SCOPE BoundingBox : public Array<Vec3f, 2> {


  /**
  * @brief Creates an invalid boundig box.
  * The minimum is (1, 1, 1) and the maximum is (-1, -1, -1).
  *
  **/
  BoundingBox();

  /**
  * @brief Creates a bounding box that must be valid.
  * Validity is checked in debug mode by an assert.
  *
  * @param min The lower left closest point of the box.
  * @param max The higher right most farther point of the box.
  **/
  BoundingBox(const tlp::Vec3f& min, const tlp::Vec3f& max);

  /**
  * @brief Returns the geometrical center of the bounding box.
  * An assertion is raised in debug mode if the BoundingBox is not valid.
  *
  * @return The center of the bounding box :Vec3f
  **/
  Vec3f center() const;

  /**
  * @brief Returns the width of the bounding box
  * An assertion is raised in debug mode if the BoundingBox is not valid.
  *
  **/
  float width() const;

  /**
  * @brief Returns the height of the bounding box
  * An assertion is raised in debug mode if the bounding box is not valid.
  *
  **/
  float height() const;

  /**
  * @brief Returns the depth of the bounding box
  * An assertion is raised in debug mode if the bounding box is not valid.
  *
  **/
  float depth() const;


  /**
  * @brief Expands the bounding box to one containing the vector passed as parameter.
  * If the parameter is inside the bounding box, it remains unchanged.
  *
  * @param coord A point in the 3D space we want the bounding box to encompass.
  * @return void
  **/
  void expand(const tlp::Vec3f& coord);

  /**
  * @brief Translates the bounding box by the displacement given by the vector passed as parameter.
  *
  * @param vec The displacement vector in 3D space to translate this bounding box by.
  * @return void
  **/
  void translate(const tlp::Vec3f& vec);

  /**
  * @brief Scales the bounding box, i.e. multiplying its components by a vector passed as parameter.
  *
  * @param factor The factor vector to scale this bounding box by.
  * @return void
  **/
  void scale(const tlp::Vec3f& factor);

  /**
  * @brief Checks whether the bounding box's lowest point is less than it's highest point.
  * "Less Than" means axis-by-axis comparison, i.e. x1 < x2 && y1 < y2 && z1 < z2.
  *
  * @return bool Whether this bounding box is valid.
  **/
  bool isValid() const;

  /**
  * @brief Checks if the given vector is inside the current bounding box. If the bounding box is invalid the result is always false.
  * @param coord A point in the 3D space.
  * @return bool Wether coord is in the bounding box.
  **/
  bool contains(const tlp::Vec3f& coord) const;

  /**
  * @brief Checks if the given bounding box is inside the current bounding box. If one of the bounding boxes is invalid the result is always false.
  * @param boundingBox The bounding box to test inclusion
  * @return bool Wether boundingBox is in the bounding box.
  **/
  bool contains(const tlp::BoundingBox& boundingBox) const;

  /**
  * @brief Checks if the given bounding box intersect the current one. If one of the bounding box is invalid return false.
  * @param boundingBox The bounding box to compare with.
  * @return bool Wether the bounding boxes intersect.
  **/
  bool intersect(const tlp::BoundingBox& boundingBox) const;

  /**
  * @brief Checks if the bounding box intersects a given line segment. If the bounding box is invalid the result is always false.
  * @param segStart the start point of the line segment on which to check intersection
  * @param segEnd the end point of the line segment on which to check intersection
  * @return bool Wether the line segment intersects the bounding box
  **/
  bool intersect(const Vec3f& segStart, const Vec3f& segEnd) const;


  /**
  * @brief The vector passed as parameter is modified to contain the 8 points of the bounding box.
  * The points are, in order :
  * 0: lower leftmost closest point (the bounding box's minimum)
  * 1: lower rightmost closest point
  * 2: highest rightmost closest point
  * 3: highest leftmost closest point
  * 4: lower rightmost farthest point
  * 5: lower rightmost farthest point
  * 6: highest rightmost farthest point
  * 7: highest leftmost farthest point
  *
  * Crude ASCII art again, sorry for your eyes.
  *
  * @verbatim

     6_________ 7
     /|       /|
    / |      / |
  3/__|_____/2 |
   |  |_____|__|
   |  /4    |  /5
   | /      | /
   |/_______|/
   0        1

   @endverbatim
  *
  * @param bb A vector in which to put the points of the bounding box.
  * @return void
  **/
  void getCompleteBB(Vec3f bb[8]) const;
};

}

#endif // Tulip_BOUNDINGBOX_H
