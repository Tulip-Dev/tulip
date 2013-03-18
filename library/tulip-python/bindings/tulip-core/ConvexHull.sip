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
