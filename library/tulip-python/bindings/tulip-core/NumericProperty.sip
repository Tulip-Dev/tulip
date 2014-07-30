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

class NumericProperty : tlp::PropertyInterface /Abstract/ {
%TypeHeaderCode
#include <tulip/NumericProperty.h>
%End

%ConvertToSubClassCode
    if (dynamic_cast<tlp::DoubleProperty *>(sipCpp)) {
        sipType = sipFindType("tlp::DoubleProperty");
    } else if (dynamic_cast<tlp::IntegerProperty *>(sipCpp)) {
        sipType = sipFindType("tlp::IntegerProperty");
    } else {
        sipType = NULL;
    }
%End

public:
 
//===========================================================================================	
	
	double getNodeDoubleDefaultValue();
%Docstring
tlp.NumericProperty.getNodeDoubleDefaultValue()

Returns the default value associated to the nodes.

:rtype: float
%End	
	
//===========================================================================================	
	
	double getEdgeDoubleDefaultValue();
%Docstring
tlp.NumericProperty.getEdgeDoubleDefaultValue()

Returns the default value associated to the edges.

:rtype: float
%End

//===========================================================================================

	const double getNodeDoubleValue(const tlp::node node);
%Docstring
tlp.NumericProperty.getNodeDoubleValue(node)

Returns the value associated to a particular node.

:param node: an existing node
:type node: :class:`tlp.node`
:rtype: float
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getNodeDoubleValue(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

	const double getEdgeDoubleValue(const tlp::edge edge);
%Docstring
tlp.NumericProperty.getEdgeDoubleValue(edge) 

Returns the value associated to a particular edge.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:rtype: float
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getEdgeDoubleValue(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

double  getNodeDoubleMin(tlp::Graph *subgraph=0);
%Docstring
tlp.NumericProperty.getNodeDoubleMin([subgraph = None])

Returns the minimum value on the nodes for that property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End	
	
	
//===========================================================================================	
	
	double  getNodeDoubleMax(tlp::Graph *subgraph=0);
%Docstring
tlp.NumericProperty.getNodeDoubleMax([subgraph = None])

Returns the maximum value on the nodes for that property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
		
//===========================================================================================	
	
	double  getEdgeDoubleMin(tlp::Graph *subgraph=0);
%Docstring
tlp.NumericProperty.getEdgeDoubleMin([subgraph = None])

Returns the minimum value on the edges for that property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
	
//===========================================================================================	
	
	double  getEdgeDoubleMax(tlp::Graph *subgraph=0);
%Docstring
tlp.NumericProperty.getEdgeDoubleMax([subgraph = None])

Returns the maximum value on the edges for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
	
//===========================================================================================	
	
	void uniformQuantification(unsigned int n);
%Docstring
tlp.NumericProperty.uniformQuantification(n)

Performs an uniform quantification on nodes and edges based on the values stored in that property.
An uniform quantification tries to separate the nodes and edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes and edges will be erased and replaced by the id of the class they belong to.
%End	
	
//===========================================================================================	
	
	void nodesUniformQuantification(unsigned int n);
%Docstring
tlp.NumericProperty.nodesUniformQuantification(n)

Performs an uniform quantification on nodes based on the values stored in that property.
An uniform quantification tries to separate the nodes in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes will be erased and replaced by the id of the class they belong to.
%End
	
//===========================================================================================	
	
	void edgesUniformQuantification(unsigned int n);
%Docstring
tlp.NumericProperty.edgesUniformQuantification(n)

Performs an uniform quantification on edges based on the values stored in that double property.
An uniform quantification tries to separate the edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on edges will be erased and replaced by the id of the class they belong to.
%End
};

};
