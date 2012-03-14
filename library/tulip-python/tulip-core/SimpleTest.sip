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

class SimpleTest   {
%TypeHeaderCode
#include <tulip/SimpleTest.h>
%End

public: 

  static bool isSimple(tlp::Graph *graph);
%Docstring
tlp.SimpleTest.isSimple(graph)

Returns :const:`True` if the graph is simple (i.e. it contains no self loops or parallel edges),
:const:`False` otherwise.

:param graph: the graph on which to perform the simple test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End  
  
//===========================================================================================   
  
  static void makeSimple(tlp::Graph* graph, std::vector<tlp::edge> &removed /Out/) ;
%Docstring
tlp.SimpleTest.makeSimple(graph)

Makes the graph  simple by removing self loops and parallel edges if any.
Returns a list of removed edges.

:param graph: the graph to make simple
:type graph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.edge`
%End
 
//===========================================================================================  
 
 static bool simpleTest(tlp::Graph *graph, std::vector<tlp::edge> *multipleEdges /Out/, std::vector<tlp::edge> *loops /Out/);
%Docstring
tlp.SimpleTest.simpleTest(graph)

Performs a simple test and returns a tuple with 3 elements. The first element is a boolean indicating the
result of the test. The second element is a list of found parallel edges. The third element is a list of
found self loops.

:param graph: the graph on which to perform the simple test
:type graph: :class:`tlp.Graph`
:rtype: (boolean, list of :class:`tlp.edge`, list of :class:`tlp.edge`)
%End
  	
private:

  SimpleTest();
  
};

};