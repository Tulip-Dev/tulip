/*
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
 * \author : Francois Queyroi queyroi@labri.fr,
 *
 * \version 0.0.1 31/01/2013
 *
 * Compute the convex hull using the QuickHull algorithm in
 *
 * "The quickhull algorithm for convex hulls",
 * Barber, C.B. and Dobkin, D.P. and Huhdanpaa, H.
 * ACM Transactions on Mathematical Software (TOMS), 1996
 *
 *
 * Compute the convex hull and return a list of indexes for the
 * points on the convex hull in counterclockwise order.  The convexHull
 * vector is automatically cleared.
 *
 * The algorithm runs in O(nlgn) time where n is the
 * number of points.
 */
TLP_SCOPE void convexHull (const std::vector<Coord> &points,
                           std::vector<unsigned int> &convexHull);

/*@}*/

}
#endif
