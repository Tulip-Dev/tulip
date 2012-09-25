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
#ifndef TLP_GEO_CONVEX_HULL_H
#define TLP_GEO_CONVEX_HULL_H
#include <vector>
#include <tulip/Coord.h>

namespace tlp {

/**
 * @ingroup Graph
 * \brief function for convex hull manipulation
 *
 * \author : Daniel Archambault archam@cs.ubc.ca,
 *
 * \version 0.0.3 26/06/2006
 *
 * Compute the convex hull and return a list of indexes for the
 * points on the convex hull in counterclockwise order.  The convexHull
 * vector is automatically cleared.  The algorithm runs in O(nlgn) time.
 */
TLP_SCOPE void convexHull (const std::vector<Coord> &points,
                           std::vector<unsigned int> &convexHull);



}
#endif
