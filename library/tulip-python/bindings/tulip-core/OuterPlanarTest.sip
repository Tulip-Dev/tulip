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

class OuterPlanarTest {
%TypeHeaderCode
#include <tulip/OuterPlanarTest.h>
%End
public:

  static bool isOuterPlanar(tlp::Graph *graph);
%Docstring
tlp.OuterPlanarTest.isOuterPlanar(graph)

Returns :const:`True` if the graph is outerplanar (i.e. a graph with an embedding
in the plane such that all vertices belong to the unbounded face of the embedding),
:const:`False` otherwise.

:param graph: the graph on which to perform the outerplanarity test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End
  
private:

	OuterPlanarTest();  
  
};

};
 
