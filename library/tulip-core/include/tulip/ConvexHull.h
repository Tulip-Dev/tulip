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

/**
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
#ifndef TLP_GEO_CONVEX_HULL_H
#define TLP_GEO_CONVEX_HULL_H
#include <vector>
#include <tulip/Coord.h>

namespace tlp {

/**
 * \brief function for computing a 2D convex hull
 *
 * Computes a 2D convex hull using the Qhull library (www.qhull.org)
 *
 * Computes the 2D convex hull and returns a list of indexes for the
 * points on the convex hull in counterclockwise order.
 *
 * The convexHull vector is automatically cleared.
 *
 */
TLP_SCOPE void convexHull(const std::vector<Coord> &points,
                          std::vector<unsigned int> &convexHull);

/**
 * \brief function for computing a 2D/3D convex hull
 *
 * Computes a 2D/3D convex hull using the Qhull library (www.qhull.org)
 *
 * Computes a 2D/3D convex hull and returns a list of the hull facets (segments for 2D, triangles for 3D)
 * and a list of neighbors facets for each facet. A facet is defined by a list of indexes in the points vector.
 * The neighbors facets of a facet are defined by a list of indexes in the convexHullFacets vector
 * (facetNeighbors[i] stores the neighbors of facet convexHullFacets[i]).
 *
 * The convexHullFacets and facetNeighbors vectors are automatically cleared.
 */
TLP_SCOPE void convexHull(const std::vector<Coord> &points,
                          std::vector<std::vector<unsigned int> > &convexHullFacets,
                          std::vector<std::vector<unsigned int> > &facetNeighbors);

/*@}*/

}
#endif
