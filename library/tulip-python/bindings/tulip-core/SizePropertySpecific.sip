/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

  tlp::Size getMax(const tlp::Graph *sg=0);
%Docstring
tlp.SizeProperty.getMax(subgraph=None)

Returns the maximum value on the nodes for that size property.

:param subgraph: a subgraph can be given in parameter, in that case return the maximum value on the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Size`
%End

//===========================================================================================

  tlp::Size getMin(const tlp::Graph *sg=0);
%Docstring
tlp.SizeProperty.getMin(subgraph=None)

Returns the minimum value on the nodes for that size property.

:param subgraph: a subgraph can be given in parameter, in that case return the minimum value on the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Size`
%End

//===========================================================================================

  void scale(const tlp::Size&, const tlp::Graph *sg=0);
%Docstring
tlp.SizeProperty.scale(scaleFactors, subgraph=None)

Scales the sizes of the nodes and edges according to a scale factors vector (sx, sy, sz).

:param scaleFactors: a vector of scales factor
:type scaleFactors: :class:`tlp.Size`
:param subgraph: a subgraph can be given in parameter, in that case scales the size of the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
%End


//===========================================================================================

  void scale(const tlp::Size&, tlp::Iterator<tlp::node> * /TypeHint="Iterator[tlp.node]"/, tlp::Iterator<tlp::edge> * /TypeHint="Iterator[tlp.edge]"/);
%Docstring
tlp.SizeProperty.scale(scaleFactors, itNodes, itEdges)

Scales the sizes of the nodes and edges provided through iterators according to a scale factors vector (sx, sy, sz).

:param scaleFactors: a vector of scale factors
:type scaleFactors: :class:`tlp.Size`
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End


