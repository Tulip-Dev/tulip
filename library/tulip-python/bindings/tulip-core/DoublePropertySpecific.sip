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

	double  getNodeMin(tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getNodeMin([subgraph = None])

Returns the minimum value on the nodes for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End	
	
	
//===========================================================================================	
	
	double  getNodeMax(tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getNodeMax([subgraph = None])

Returns the maximum value on the nodes for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
		
//===========================================================================================	
	
	double  getEdgeMin(tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getEdgeMin([subgraph = None])

Returns the minimum value on the edges for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
	
//===========================================================================================	
	
	double  getEdgeMax(tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getEdgeMax([subgraph = None])

Returns the maximum value on the edges for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
	
//===========================================================================================	
	
	void uniformQuantification(unsigned int n);
%Docstring
tlp.DoubleProperty.uniformQuantification(n)

Performs an uniform quantification on nodes and edges based on the values stored in that double property.
An uniform quantification tries to separate the nodes and edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes and edges will be erased and replaced by the id of the class they belong to.
%End	
	
//===========================================================================================	
	
	void nodesUniformQuantification(unsigned int n);
%Docstring
tlp.DoubleProperty.nodesUniformQuantification(n)

Performs an uniform quantification on nodes based on the values stored in that double property.
An uniform quantification tries to separate the nodes in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes will be erased and replaced by the id of the class they belong to.
%End
	
//===========================================================================================	
	
	void edgesUniformQuantification(unsigned int n);
%Docstring
tlp.DoubleProperty.edgesUniformQuantification(n)

Performs an uniform quantification on edges based on the values stored in that double property.
An uniform quantification tries to separate the edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on edges will be erased and replaced by the id of the class they belong to.
%End

private:

  void treatEvent(const tlp::Event&);
