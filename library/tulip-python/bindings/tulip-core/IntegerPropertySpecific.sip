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

  int getNodeMin(const tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getNodeMin(subgraph=None)

Returns the minimum value on the nodes for that integer property.

:param subgraph: a subgraph can be given in parameter, in that case return the minimum value on the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

//===========================================================================================

  int getNodeMax(const tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getNodeMax(subgraph=None)

Returns the maximum value on the nodes for that integer property.

:param subgraph: a subgraph can be given in parameter, in that case return the maximum value on the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

//===========================================================================================

  int getEdgeMin(const tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getEdgeMin(subgraph=None)

Returns the minimum value on the edges for that integer property.

:param subgraph: a subgraph can be given in parameter, in that case return the minimum value on the edges belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

//===========================================================================================

  int getEdgeMax(const tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getEdgeMax(subgraph=None)

Returns the maximum value on the edges for that integer property.

:param subgraph: a subgraph can be given in parameter, in that case return the maximum value on the edges belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

private:

  void treatEvent(const tlp::Event&);
