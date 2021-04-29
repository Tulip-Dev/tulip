/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef _DRAWINGTOOLS_H
#define _DRAWINGTOOLS_H

#include <vector>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>
#include <tulip/Matrix.h>
#include <tulip/Size.h>

namespace tlp {

template <class itType>
struct Iterator;

class Graph;
class LayoutProperty;
class SizeProperty;
class DoubleProperty;
class BooleanProperty;

typedef Matrix<float, 3> Mat3f;

/**
 *
 * Computes the bounding box of a graph according to nodes positions, edges bends,
 * nodes z-rotations and sizes of elements.
 *
 */
TLP_SCOPE BoundingBox computeBoundingBox(const Graph *graph, const LayoutProperty *layout,
                                         const SizeProperty *size, const DoubleProperty *rotation,
                                         const BooleanProperty *selection = nullptr);

//======================================================================================================

/**
 * Compute the bounding box of graph elements in corresponding vectors according to node positions,
 * edges bends,
 * nodes z-rotations and sizes of elements.
 *
 */
TLP_SCOPE BoundingBox computeBoundingBox(const std::vector<node> &nodes,
                                         const std::vector<edge> &edges,
                                         const LayoutProperty *layout, const SizeProperty *size,
                                         const DoubleProperty *rotation,
                                         const BooleanProperty *selection = nullptr);

//======================================================================================================

/**
 *
 * Computes a bounding sphere (or a bounding circle if the graph has a 2D layout) of a graph
 * according to nodes positions, edges bends,
 * nodes z-rotations and sizes of elements.
 *
 * Returns a pair of tlp::Coord whose first member is the center of the bounding sphere (circle for
 * 2D layout)
 * and second member is the farthest point from the center (computed from graph elements
 * positions).
 * To get the bounding radius, you have to compute the distance between the two members of the pair
 * (use the dist method from tlp::Coord).
 *
 */

TLP_SCOPE std::pair<Coord, Coord>
computeBoundingRadius(const Graph *graph, const LayoutProperty *layout, const SizeProperty *size,
                      const DoubleProperty *rotation, const BooleanProperty *selection = nullptr);

//======================================================================================================

/**
 *
 * Computes a convex hull of a graph according to nodes positions, edges bends,
 * nodes z-rotations, and sizes of elements. Only works with 2D layouts.
 *
 * Returns a vector of tlp::Coord containing the vertices of the graph convex hull correctly
 * ordered.
 *
 */
TLP_SCOPE std::vector<Coord> computeConvexHull(const Graph *graph, const LayoutProperty *layout,
                                               const SizeProperty *size,
                                               const DoubleProperty *rotation,
                                               const BooleanProperty *selection = nullptr);

//======================================================================================================

/**
 *
 * Computes a convex hull of a set of points,
 * Only works with 2D layouts.
 *
 * Returns a vector of tlp::Coord containing the vertices of the points convex hull correctly
 * ordered.
 *
 */
TLP_SCOPE std::vector<Coord> computeConvexHull(const std::vector<tlp::Coord> &points);

//======================================================================================================

/**
 * Computes the intersection point (if any) of two 3d lines.
 * Returns true if the line intersects, false otherwise (parallel or skew lines).
 *
 */
TLP_SCOPE bool computeLinesIntersection(const std::pair<tlp::Coord, tlp::Coord> &line1,
                                        const std::pair<tlp::Coord, tlp::Coord> &line2,
                                        tlp::Coord &intersectionPoint);

//======================================================================================================

/**
 * Computes the centroid of a polygon.
 * Polygon vertices must be provided correctly ordered in the points vector.
 *
 */
TLP_SCOPE tlp::Coord computePolygonCentroid(const std::vector<tlp::Coord> &points);

//======================================================================================================

/**
 * Checks if a layout is co-planar, returns true if so.
 * If the layout is co-planar, the inverse transform matrix is also returned
 * in order to project the layout in the z=0 plane.
 *
 */
TLP_SCOPE bool isLayoutCoPlanar(const std::vector<Coord> &points, Mat3f &invTransformMatrix);

//======================================================================================================

/**
 * Computes the vertices of a regular polygon.
 * A regular polygon is a polygon that is equiangular (all angles are equal in measure)
 * and equilateral (all sides have the same length).
 *
 * @since Tulip 4.8
 *
 * @param numberOfSides the number of sides of the polygon (minimum value is 3)
 * @param center the center point of the polygon
 * @param size the size of the rectangle enclosing the polygon in the form (width/2, height/2)
 * @param startAngle the start angle when computing the polygon vertices
 * @return a vector filled with the numberOfSides polygon vertices
 *
 */
TLP_SCOPE std::vector<tlp::Coord> computeRegularPolygon(unsigned int numberOfSides,
                                                        const tlp::Coord &center,
                                                        const tlp::Size &size,
                                                        float startAngle = 0);
} // namespace tlp

#endif
///@endcond
