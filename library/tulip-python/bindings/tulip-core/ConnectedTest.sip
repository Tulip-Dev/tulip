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

%ModuleHeaderCode
typedef std::set<tlp::node> setNodes;
%End

typedef std::set<tlp::node> setNodes;

namespace tlp {

class ConnectedTest {
%TypeHeaderCode
#include <tulip/ConnectedTest.h>
%End

public:

  static bool isConnected(const tlp::Graph *graph);
%Docstring
tlp.ConnectedTest.isConnected(graph)

Returns :const:`True` if the graph is connected (ie. it exists an undirected path 
between each pair of nodes), :const:`False` otherwise.

:param graph: the graph on which to perform the connected test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End

//=========================================================================================== 
 
  static void makeConnected(tlp::Graph *graph, std::vector<tlp::edge>& addedEdges /Out/);
%Docstring
tlp.ConnectedTest.makeConnected(graph)

If the graph is not connected, adds edges in order to make the graph
connected. Returns a list of added edges.

:param graph: the graph to make connected
:type graph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.edge`
%End

//===========================================================================================   


  static unsigned int numberOfConnectedComponents(const tlp::Graph *graph);
%Docstring
tlp.ConnectedTest.numberOfConnectedComponents(graph)

Returns the number of connected components in the graph.

:param graph: the graph on which to compute the number of connected components
:type graph: :class:`tlp.Graph`
:rtype: integer
%End 
  
//===========================================================================================   
  
  static void computeConnectedComponents(tlp::Graph *graph, std::vector<setNodes>& components /Out/) ;
%Docstring
tlp.ConnectedTest.computeConnectedComponents(graph)

Computes the sets of connected components. Returns a list of connected components.

:param graph: the graph on which to compute the sets of connected components
:type graph: :class:`tlp.Graph`
:rtype: list of list of :class:`tlp.node`
%End
  
private : 

	ConnectedTest();
};

};
