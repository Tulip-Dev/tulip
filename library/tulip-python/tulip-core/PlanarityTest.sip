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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {

class PlanarityTest {
%TypeHeaderCode
#include <tulip/PlanarityTest.h>
%End
public:

  static bool isPlanar(tlp::Graph *graph);
%Docstring
tlp.PlanarityTest.isPlanar(graph)

Returns :const:`True` if the graph is planar (i.e. the graph can be drawn in such a way that no edges cross each other),
:const:`False` otherwise.

:param graph: the graph on which to perform the planarity test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End  
  
//===========================================================================================   
  
  static bool isPlanarEmbedding(tlp::Graph *graph);
%Docstring
tlp.PlanarityTest.isPlanarEmbedding(graph)

Returns :const:`True` if the current embedding of the graph is planar, :const:`False` otherwise.

:param graph: the graph on which to perform the planar embedding test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End
  
//===========================================================================================   
  
  static bool planarEmbedding(tlp::Graph *graph);
%Docstring
tlp.PlanarityTest.planarEmbedding(graph)

Changes the order of edges around the nodes in order to make the
embedding planar (the graph must be planar for that).
Returns :const:`True` if the graph is planar, :const:`False` otherwise.

:param graph: the graph on which to compute a planar embedding
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End  
  
//===========================================================================================   
  
  static std::list<tlp::edge> getObstructionsEdges(tlp::Graph *graph);
%Docstring
tlp.PlanarityTest.getObstructionsEdges(graph)

Returns a list of edges that prevents to make the graph planar
(ie. part of the minor of K3,3 or K5).

:param graph: the graph on which to compute the obstruction edges
:type graph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.edge`
%End
    
private :

   PlanarityTest();  
};

};
