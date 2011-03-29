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

class BiconnectedTest {
%TypeHeaderCode
#include <tulip/BiconnectedTest.h>
%End
public:
  static bool isBiconnected(tlp::Graph *graph);
  static void makeBiconnected(tlp::Graph *graph, std::vector<tlp::edge>& addedEdges /Out/);
%Docstring
tlp.BiconnectedTest.makeBiconnected(tlp.Graph) -> list of tlp.edge
%End
  
private :

	BiconnectedTest(); 
};

};
