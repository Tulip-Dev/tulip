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

#include <tulip/Vector.h>
#include <tulip/tulipconf.h>

namespace tlp {
/**
 * \brief This class represents the 3D bounding box of an object.
 * It is mostly used to determine whether or not two object are in a state of collision.
 *
 * It is composed of two voxels, the first one (A) being the lowest point, the second (B) being the highest.
 * As a bounding box is a mathematical entity describing the lowest and highest points, whether these points are in the top-left corner or
 * lower-right corner depends on the axes we use.
 * Below is a crude ASCII-art description of tthe axes we use in our 3D world and the points where the min and max are thus positioned.
 * Through the rest of this class's documentation, it will be assumed that this is the convention.
 *
 *     y
 *    |
 *    |
 *    |_____ x
 *   /
 *  /
 * z
 *
 *    _________ B
 *   /        /|
 *  /        / |
 * /________/  |
 * |        |  |
 * |        |  |
 * |        | /
 * |________|/
 * A
 *
 *
 * Author : <a href="www.tulip-software.org">Tulip team</a>
 */
struct TLP_SCOPE BoundingBox : public Array<Vec3f, 2> {


	/**
	 * @brief Creates an invalid boundig box.
	 * The minimum is in (1, 1, 1) and the maximum in (-1, -1, -1).
	 *
	 **/
	BoundingBox() {
		(*this)[0].fill(1);
		(*this)[1].fill(-1);
		assert(!isValid());
	}

	/**
	 * @brief Creates a bounding box that must be valid.
	 * Validity is checked in debug mode by an assert.
	 *
	 * @param min The lower left closest point of the box.
	 * @param max The higher left most farther point of the box.
	 **/
	BoundingBox(const tlp::Vec3f& min, const tlp::Vec3f& max) {
		(*this)[0] = min;
		(*this)[1] = max;
		assert(isValid());
	}

	/**
	 * @brief Returns the geometrical center of the bounding box.
	 * An assertion is raised in debug mode if the BoundingBox is not valid.
	 *
	 * @return The center of the bounding box :Vec3f
	 **/
	Vec3f center() const {
		assert(isValid());
		return ((*this)[0] + (*this)[1]) / 2.f;
	}

	/**
	 * @brief Returns the width of the bounding box
	 * An assertion is raised in debug mode if the BoundingBox is not valid.
	 *
	 **/
	float width() const {
		assert(isValid());
		return ((*this)[1][0] - (*this)[0][0]);
	}

	/**
	 * @brief Returns the height of the bounding box
	 * An assertion is raised in debug mode if the BoundingBox is not valid.
	 *
	 **/
	float height() const {
		assert(isValid());
		return ((*this)[1][1] - (*this)[0][1]);
	}

	/**
	 * @brief Returns the depth of the bounding box
	 * An assertion is raised in debug mode if the BoundingBox is not valid.
	 *
	 **/
	float depth() const {
		assert(isValid());
		return ((*this)[1][2] - (*this)[0][2]);
	}

	/**
	 * @brief Expands the bounding box to one containing the vector passed as param.
	 * If the parameter is inside the bounding box, it remains unchanged.
	 *
	 * @param coord A point in the 3D space we want the bounding box to encompass.
	 * @return void
	 **/
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
	 * @brief Translates the bounding box of the distance given by the parameter vector.
	 *
	 * @param vec The distance in 3D space to translate this bounding box by.
	 * @return void
	 **/
	void translate(const tlp::Vec3f& vec) {
		(*this)[0] += vec;
		(*this)[1] += vec;
	}

	/**
	 * @brief Checks whether this bounding box's lowest point is less than it's highest point.
	 * "Less Than" means axis-by-axis comparison, e.g. x1 < x2 && y1 < y2 && z1 < z3.
	 *
	 * @return bool Whether this bounding box is valid.
	 **/
	bool isValid() const {
		return (*this)[0][0] <= (*this)[1][0] && (*this)[0][1] <= (*this)[1][1] && (*this)[0][2] <= (*this)[1][2];
	}

	/**
	 * @brief The vector passed as param is modified to contain the 8 points of the bounding box.
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
	 *   6_________ 7
	 *   /|       /|
	 *  / |      / |
	 *3/__|_____/2 |
	 * |  |_____|__|
	 * |  /4    |  /5
	 * | /      | /
	 * |/_______|/
	 * 0        1
	 *
	 * @param bb A vector in which to put the points of the bounding box.
	 * @return void
	 **/
	void getCompleteBB(Vec3f bb[8]) const {
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
