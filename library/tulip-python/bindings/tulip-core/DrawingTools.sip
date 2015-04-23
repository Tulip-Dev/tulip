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
#include <tulip/DrawingTools.h>
%End

namespace tlp { 

tlp::BoundingBox computeBoundingBox(tlp::Graph *graph, tlp::BooleanProperty *selection = 0);
%Docstring
tlp.computeBoundingBox(graph[, selection = None])

.. versionadded:: 3.8

Computes the bounding box of a graph according to nodes positions and edges bends defined in the default layout property "viewLayout",
nodes z-rotations defined in the default double property "viewRotation" and sizes of elements defined in the default size property "viewSize".

:param graph: the graph on which to compute its bounding box
:type graph: :class:`tlp.Graph`
:param selection: a boolean property to restrain the computation of the bounding box to selected elements (if provided)
:type selection: :class:`tlp.BooleanProperty`
:rtype: :class:`tlp.BoundingBox`
:throws: an exception if the provided boolean property is not attached to the graph
%End

%MethodCode
    if (a1 && !(a0 == a1->getGraph() || a1->getGraph()->isDescendantGraph(a0))) {
        sipIsErr = 1;
        PyErr_SetString(PyExc_Exception, "The provided boolean property is not attached to the graph on which to compute its bounding box.");
    } else {
        sipRes = new tlp::BoundingBox(tlp::computeBoundingBox(a0, a0->getProperty<tlp::LayoutProperty>("viewLayout"),
                                                              a0->getProperty<tlp::SizeProperty>("viewSize"),
                                                              a0->getProperty<tlp::DoubleProperty>("viewRotation"),
                                                              a1));
    }
%End

// ================================================================================================
 
tlp::BoundingBox computeBoundingBox(tlp::Graph *graph,
                                    tlp::LayoutProperty *layout,
                                    tlp::SizeProperty *size,
                                    tlp::DoubleProperty *rotation,
                                    tlp::BooleanProperty *selection = 0);
%Docstring
tlp.computeBoundingBox(graph, layout, size, rotation[, selection = None])

Computes the bounding box of a graph according to nodes positions, edges bends,
nodes z-rotations and sizes of elements.

:param graph: the graph on which to compute its bounding box
:type graph: :class:`tlp.Graph`
:param layout: a layout property containing positions of the graph elements
:type layout: :class:`tlp.LayoutProperty`
:param size: a size property containing the sizes of the graph elements.
:type size: :class:`tlp.SizeProperty`
:param rotation: a double property containing the z-rotations of nodes
:type rotation: :class:`tlp.DoubleProperty`
:param selection: a boolean property to restrain the computation of the bounding box to selected elements (if provided)
:type selection: :class:`tlp.BooleanProperty`
:rtype: :class:`tlp.BoundingBox`
:throws: an exception if the provided properties are not attached to the graph
%End

%MethodCode
	if (!(a0 == a1->getGraph() || a1->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided layout property is not attached to the graph on which to compute its bounding box.");
	} else if (!(a0 == a2->getGraph() || a2->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided size property is not attached to the graph on which to compute its bounding box.");
	} else if (!(a0 == a3->getGraph() || a3->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided double property is not attached to the graph on which to compute its bounding box.");
	} else if (a4 && !(a0 == a4->getGraph() || a4->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided boolean property is not attached to the graph on which to compute its bounding box.");
	} else {
		sipRes = new tlp::BoundingBox(tlp::computeBoundingBox(a0, a1, a2, a3, a4));
	}
%End

// ================================================================================================                             

std::pair<tlp::Coord, tlp::Coord> computeBoundingRadius(tlp::Graph *graph, tlp::BooleanProperty *selection = 0);
%Docstring
tlp.computeBoundingRadius(graph[, selection = None])

.. versionadded:: 3.8

Computes a bounding sphere (or a bounding circle if the graph has a 2D layout) of a graph according to nodes positions and edges bends
defined in the default layout property "viewLayout", nodes z-rotations defined in the default double property "viewRotation"
and sizes of elements defined in the default size property "viewSize".

Returns a tuple of :class:`tlp.Coord` whose first member is the center of the bounding sphere (circle for 2D layout)
and second member is the farthest point from the center (computed from graph elements positions).
To get the bounding radius, you have to compute the distance between the two members of the tuple
(use the :meth:`tlp.Vec3f.dist` method).

:param graph: the graph on which to compute its bounding sphere
:type graph: :class:`tlp.Graph`
:param selection: a boolean property to restrain the computation of the bounding sphere to selected elements (if provided)
:type selection: :class:`tlp.BooleanProperty`
:rtype: (:class:`tlp.Coord`, :class:`tlp.Coord`)
:throws: an exception if the provided boolean property is not attached to the graph

%End

%MethodCode
    if (a1 && (!(a0 == a1->getGraph() || a1->getGraph()->isDescendantGraph(a0)))) {
        sipIsErr = 1;
        PyErr_SetString(PyExc_Exception, "The provided boolean property is not attached to the graph on which to compute its bounding sphere.");
    } else {
        sipRes = new std::pair<tlp::Coord, tlp::Coord>(tlp::computeBoundingRadius(a0, a0->getProperty<tlp::LayoutProperty>("viewLayout"),
                                                                                  a0->getProperty<tlp::SizeProperty>("viewSize"),
                                                                                  a0->getProperty<tlp::DoubleProperty>("viewRotation"),
                                                                                  a1));
    }
%End

// ================================================================================================                             

std::pair<tlp::Coord, tlp::Coord> computeBoundingRadius(tlp::Graph *graph,
                                                        tlp::LayoutProperty *layout,
                                                        tlp::SizeProperty *size,
                                                        tlp::DoubleProperty *rotation,
                                                        tlp::BooleanProperty *selection = 0);
%Docstring
tlp.computeBoundingRadius(graph, layout, size, rotation[, selection = None])

Computes a bounding sphere (or a bounding circle if the graph has a 2D layout) of a graph according to node position edge bends
node z-rotation, and size of elements.

Returns a tuple of :class:`tlp.Coord` whose first member is the center of the bounding sphere (circle for 2D layout)
and second member is the farthest point from the center (computed from graph elements positions).
To get the bounding radius, you have to compute the distance between the two members of the tuple 
(use the :meth:`tlp.Vec3f.dist` method). 

:param graph: the graph on which to compute its bounding sphere
:type graph: :class:`tlp.Graph`
:param layout: a layout property containing positions of the graph elements
:type layout: :class:`tlp.LayoutProperty`
:param size: a size property containing the sizes of the graph elements.
:type size: :class:`tlp.SizeProperty`
:param rotation: a double property containing the z-rotations of nodes
:type rotation: :class:`tlp.DoubleProperty`
:param selection: a boolean property to restrain the computation of the bounding sphere to selected elements (if provided)
:type selection: :class:`tlp.BooleanProperty`
:rtype: (:class:`tlp.Coord`, :class:`tlp.Coord`)
:throws: an exception if the provided properties are not attached to the graph

%End

%MethodCode
	if (!(a0 == a1->getGraph() || a1->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided layout property is not attached to the graph on which to compute its bounding sphere.");
	} else if (!(a0 == a2->getGraph() || a2->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided size property is not attached to the graph on which to compute its bounding sphere.");
	} else if (!(a0 == a3->getGraph() || a3->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided double property is not attached to the graph on which to compute its bounding sphere.");
	} else if (a4 && !(a0 == a4->getGraph() || a4->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided boolean property is not attached to the graph on which to compute its bounding sphere.");
	} else {
		sipRes = new std::pair<tlp::Coord, tlp::Coord>(tlp::computeBoundingRadius(a0, a1, a2, a3, a4));
	}
%End

// ================================================================================================

std::vector<tlp::Coord> computeConvexHull(const std::vector<tlp::Coord> &points);
%Docstring
tlp.computeConvexHull(points)

.. versionadded:: 3.8

Computes a convex hull of a set of points. Only works with 2D layouts.
Returns a list of :class:`tlp.Coord` containing the vertices of the points convex hull correctly ordered.

:param points: the points on which to compute a conbex hull
:type points: :list of class:`tlp.Coord`
:rtype: list of :class:`tlp.Coord`
%End

// ================================================================================================

std::vector<tlp::Coord> computeConvexHull(tlp::Graph *graph, tlp::BooleanProperty *selection = 0);
%Docstring
tlp.computeConvexHull(graph[, selection = None])

.. versionadded:: 3.8

Computes a convex hull of a graph according to nodes positions and edges bends
defined in the default layout property "viewLayout", nodes z-rotations defined in the default double property "viewRotation"
and sizes of elements defined in the default size property "viewSize". Only works with 2D layouts.
Returns a list of :class:`tlp.Coord` containing the vertices of the graph convex hull correctly ordered.

:param graph: the graph on which to compute its convex hull
:type graph: :class:`tlp.Graph`
:param selection: a boolean property to restrain the computation of the convex hull to selected elements (if provided)
:type selection: :class:`tlp.BooleanProperty`
:rtype: list of :class:`tlp.Coord`
:throws: an exception if the provided boolean property is not attached to the graph
%End

%MethodCode
    if (a1 && (!(a0 == a1->getGraph() || a1->getGraph()->isDescendantGraph(a0)))) {
        sipIsErr = 1;
        PyErr_SetString(PyExc_Exception, "The provided boolean property is not attached to the graph on which to compute its convex hull.");
    } else {
        sipRes = new std::vector<tlp::Coord>(tlp::computeConvexHull(a0, a0->getProperty<tlp::LayoutProperty>("viewLayout"),
                                                                    a0->getProperty<tlp::SizeProperty>("viewSize"),
                                                                    a0->getProperty<tlp::DoubleProperty>("viewRotation"),
                                                                    a1));
    }
%End

// ================================================================================================

 std::vector<tlp::Coord> computeConvexHull(tlp::Graph *graph,
                                           tlp::LayoutProperty *layout,
                                           tlp::SizeProperty *size,
                                           tlp::DoubleProperty *rotation,
                                           tlp::BooleanProperty *selection = 0);	
%Docstring
tlp.computeConvexHull(graph, layout, size, rotation[, selection = None])

Computes a convex hull of a graph according to nodes positions, edges bends,
nodes z-rotations, and sizes of elements. Only works with 2D layouts.
Returns a list of :class:`tlp.Coord` containing the vertices of the graph convex hull correctly ordered.

:param graph: the graph on which to compute its convex hull
:type graph: :class:`tlp.Graph`
:param layout: a layout property containing positions of the graph elements
:type layout: :class:`tlp.LayoutProperty`
:param size: a size property containing the sizes of the graph elements.
:type size: :class:`tlp.SizeProperty`
:param rotation: a double property containing the z-rotations of nodes
:type rotation: :class:`tlp.DoubleProperty`
:param selection: a boolean property to restrain the computation of the convex hull to selected elements (if provided)
:type selection: :class:`tlp.BooleanProperty`
:rtype: list of :class:`tlp.Coord`
:throws: an exception if the provided properties are not attached to the graph
%End

%MethodCode
	if (!(a0 == a1->getGraph() || a1->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided layout property is not attached to the graph on which to compute its convex hull.");
	} else if (!(a0 == a2->getGraph() || a2->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided size property is not attached to the graph on which to compute its convex hull.");
	} else if (!(a0 == a3->getGraph() || a3->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided double property is not attached to the graph on which to compute its convex hull.");
	} else if (a4 && !(a0 == a4->getGraph() || a4->getGraph()->isDescendantGraph(a0))) {
		sipIsErr = 1;
		PyErr_SetString(PyExc_Exception, "The provided boolean property is not attached to the graph on which to compute its convex hull.");
	} else {
		sipRes = new std::vector<tlp::Coord>(tlp::computeConvexHull(a0, a1, a2, a3, a4));
	}
%End

// ================================================================================================

bool computeLinesIntersection(const std::pair<tlp::Coord, tlp::Coord> &line1,
                              const std::pair<tlp::Coord, tlp::Coord> &line2,
                              tlp::Coord &intersectionPoint /Out/);

%Docstring
tlp.computeLinesIntersection(line1, line2)

.. versionadded:: 3.8

Computes the intersection point (if any) of two 3d lines.
Returns a tuple whose first member is a boolean indicating
if the lines intersect (if :const:`False` it means that lines are parallel or skew)
and second member is the intersection point (if any).

:param line1: the coordinates of the two points defining the first line
:type line1: (:class:`tlp.Coord` , :class:`tlp.Coord`)
:param line2: the coordinates of the two points defining the second line
:type line2: (:class:`tlp.Coord` , :class:`tlp.Coord`)
:rtype: (boolean, :class:`tlp.Coord`)

%End

// ================================================================================================

tlp::Coord computePolygonCentroid(const std::vector<tlp::Coord> &points);
%Docstring
tlp.computePolygonCentroid(points)

.. versionadded:: 3.8

Computes the centroid of a polygon.

:param points: the vertices of the polygon correctly ordered
:type points: list of :class:`tlp.Coord`
:rtype: :class:`tlp.Coord`
%End


};
