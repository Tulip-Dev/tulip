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
class TriconnectedTest {
%TypeHeaderCode
#include <tulip/TriconnectedTest.h>
%End
public:

  static bool isTriconnected(tlp::Graph *graph);
%Docstring
tlp.TriconnectedTest.isTriconnected(graph)

Returns :const:`True` if the graph is triconnected (i.e. a connected graph such that deleting any two nodes (and incident edges)
results in a graph that is still connected), :const:`False` otherwise.

:param graph: the graph on which to perform the triconnected test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End
  
private :

   TriconnectedTest();  
};

};

