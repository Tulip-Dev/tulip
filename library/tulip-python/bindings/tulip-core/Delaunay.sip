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
#include <tulip/Delaunay.h>
typedef std::pair<unsigned int, unsigned int> pairUintUint;
typedef std::vector<unsigned int> vectorUint;
typedef std::vector<pairUintUint> vector_pairUintUint;
typedef std::vector<vectorUint> vector_vectorUint;
%End

typedef std::pair<unsigned int, unsigned int> pairUintUint;
typedef std::vector<unsigned int> vectorUint;
typedef std::vector<pairUintUint> vector_pairUintUint;
typedef std::vector<vectorUint> vector_vectorUint;

namespace tlp {

std::pair<vector_pairUintUint, vector_vectorUint> delaunayTriangulation(std::vector<tlp::Coord> &points);
%Docstring
tlp.delaunayTriangulation(points)

.. versionadded:: 3.8

Computes the delaunay triangulation of a set of points and returns the set of delaunay edges and simplices (triangles in 2d, tetrahedra in 3d).
Returns a tuple whose first member is the list of edges and second member is the list of simplices, or :const:`None` if
the triangulation failed.
Edges and simplices are defined using indexes into the original list of points.

:param points: a list of points on which to compute a delaunay triangulation
:type points: list of :class:`tlp.Coord`
:rtype: (list of (integer, integer), list of integer) or :const:`None`
%End

%MethodCode
    std::vector<std::pair<unsigned int, unsigned int> > edges;
    std::vector<std::vector<unsigned int> > simplices;
    if (tlp::delaunayTriangulation(*a0, edges, simplices)) {
        sipRes = new std::pair<std::vector<std::pair<unsigned int, unsigned int> >, std::vector<std::vector<unsigned int> > >(edges, simplices);
    } else {
        sipRes = NULL;
    }
%End

class VoronoiDiagram /Abstract/ {
%TypeHeaderCode
#include <tulip/Delaunay.h>
%End

%Docstring
.. versionadded:: 3.8

That class defines a voronoi diagram. Use the :func:`tlp.voronoiDiagram` function
to get an instance of it.
%End

  unsigned int nbSites() const;
%Docstring
tlp.VoronoiDiagram.nbSites()

Returns the number of voronoi sites.

:rtype: integer
%End

  unsigned int nbVertices() const;
%Docstring
tlp.VoronoiDiagram.nbVertices()

Returns the number of voronoi vertices.

:rtype: integer
%End

  unsigned int nbEdges() const;
%Docstring
tlp.VoronoiDiagram.nbEdges()

Returns the number of voronoi edges.

:rtype: integer
%End

  const tlp::Coord &site(const unsigned int siteIdx);
%Docstring
tlp.VoronoiDiagram.site(siteIdx)

Returns the ith voronoi site.

:param siteIdx: the index of the site to return
:type siteIdx: integer
:rtype: :class:`tlp.Coord`
%End

  const tlp::Coord &vertex(const unsigned int vertexIdx);
%Docstring
tlp.VoronoiDiagram.vertex(vertexIdx)

Returns the ith voronoi vertex.

:param vertexIdx: the index of the vertex to return
:type vertexIdx: integer
:rtype: :class:`tlp.Coord`
%End

  const pairUintUint &edge(const unsigned int edgeIdx);
%Docstring
tlp.VoronoiDiagram.edge(edgeIdx)

Returns the ith voronoi edge. An edge is defined by two indexes in the voronoi vertices.

:param edgeIdx: the index of the edge to return
:type edgeIdx: integer
:rtype: (integer, integer)
%End

  const std::set<unsigned int> &cell(const unsigned int cellIdx);
%Docstring
tlp.VoronoiDiagram.cell(cellIdx)

Returns the ith voronoi cell. A cell is defined by a list of indexes in the voronoi vertices.

:param cellIdx: the index of the cell to return
:type cellIdx: integer
:rtype: set of integer
%End

  unsigned int degreeOfVertex(const unsigned int vertexIdx);
%Docstring
tlp.VoronoiDiagram.degreeOfVertex(vertexIdx)

Returns the degree (number of incident edges) of the ith voronoi vertex

:param vertexIdx: the index of the vertex on which to return the degree
:type vertexIdx: integer
:rtype: integer
%End

  std::vector<pairUintUint> voronoiEdgesForSite(const unsigned int siteIdx);
%Docstring
tlp.VoronoiDiagram.voronoiEdgesForSite(siteIdx)

Returns the voronoi edges surrounding the ith site.

:param siteIdx: the index of the site on which to return edges
:type siteIdx: integer
:rtype: list of (integer, integer)
%End

  const std::set<unsigned int> &voronoiCellForSite(const unsigned int siteIdx);
%Docstring
tlp.VoronoiDiagram.voronoiCellForSite(siteIdx)

Returns the cell associated to the ith site.

:param siteIdx: the index of the site on which to return the cell
:type siteIdx: integer
:rtype: set of integer
%End

};

tlp::VoronoiDiagram voronoiDiagram(std::vector<tlp::Coord> &sites);
%Docstring
tlp.voronoiDiagram(sites)

.. versionadded:: 3.8

Computes the voronoi diagram of a set of sites (for 2d and 3d layouts).
It automatically computes the set of all voronoi
vertices, edges and cells. In order to not have to deal with infinite
voronoi rays, the input layout is enclosed in its convex hull in 2d or
in its bounding box in 3d. It enables to have a connected voronoi cell
for each input site.

:param sites: the sites from which to compute the voronoi diagram
:type sites: list of :class:`tlp.Coord`
:rtype: :class:`tlp.VoronoiDiagram`
%End

%MethodCode
    tlp::VoronoiDiagram voronoiDiagram;
    if (tlp::voronoiDiagram(*a0, voronoiDiagram)) {
        sipRes = new tlp::VoronoiDiagram(voronoiDiagram);
    } else {
        sipRes = NULL;
    }
%End

};
