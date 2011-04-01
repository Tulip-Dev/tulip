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
typedef std::set<tlp::node> std_set_0100tlp_node;
%End

namespace tlp {

class ConnectedTest {
%TypeHeaderCode
#include <tulip/ConnectedTest.h>
%End

public:

  static bool isConnected(const tlp::Graph*graph);
 
  static void makeConnected(tlp::Graph *graph, std::vector<tlp::edge>& addedEdges /Out/);
%Docstring
tlp.ConnectedTest.makeConnected(tlp.Graph) -> list of tlp.edge
%End
  
  static unsigned int numberOfConnectedComponents(const tlp::Graph*graph);
  
  static void computeConnectedComponents(tlp::Graph *graph, std::vector<std_set_0100tlp_node>& components /Out/);
  
private : 

	ConnectedTest();
};

};
