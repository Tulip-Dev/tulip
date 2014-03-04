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

%ModuleHeaderCode
#include <tulip/ConvexHull.h>
%End

namespace tlp {

void convexHull(const std::vector<tlp::Coord> &points,
                std::vector<unsigned int> &convexHull /Out/) /PyName=convexHull2D/;
%Docstring
tlp.convexHull2D(points)

.. versionadded:: 4.2

Computes the 2D convex hull of a list of points (the z-coordinate is not taken into acccount)
using the Qhull_ library.
Returns a list of indexes in the list of points defining the hull polygon in counterclockwise order.

:param points: the points on which to compute the 2D convex hull
:type points: list of :class:`tlp.Coord`
:rtype: list of integers

.. _Qhull: http://www.qhull.org
%End


void convexHull(const std::vector<tlp::Coord> &points,
                std::vector<vectorUint> &convexHullFacets /Out/,
                std::vector<vectorUint> &facetNeighbors /Out/);

%Docstring
tlp.convexHull(points)

.. versionadded:: 4.2

Computes the 2D/3D convex hull of a list of points using the Qhull_ library.
Returns a tuple whose first member is the list of facets defining the
convex hull (segments for 2D, triangles for 3D) and second member is the list
of neighbors facets for each facet. A facet is defined by a list of indexes
in the list of points. The ith element of the neighbors list is a list
of indexes in the facets list corresponding to the neighbors facets of the ith facet.

:param points: the points on which to compute the 2D/3D convex hull
:type points: list of :class:`tlp.Coord`
:rtype: (list of list of integers, list of list of integers)

.. _Qhull: http://www.qhull.org
%End

};
