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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

  void reverse(const tlp::Graph* graph = NULL);
%Docstring
tlp.BooleanProperty.reverse([graph = None])

Reverses all values associated to graph elements,
i.e :const:`True` => :const:`False`, :const:`False` => :const:`True`.

:param graph: a sub-graph can be given in parameter, in that case returns an iterator on nodes only belonging to that sub-graph
:type graph: :class:`tlp.Graph`
%End

//===========================================================================================

  void reverseEdgeDirection(tlp::Graph* graph = NULL);
%Docstring
tlp.BooleanProperty.reverseEdgeDirection([graph = None])

Reverses all the direction of edges in the attached graph
which are set to :const:`True` in this boolean property.

:param graph: a sub-graph can be given in parameter, in that case returns an iterator on nodes only belonging to that sub-graph
:type graph: :class:`tlp.Graph`

%End

