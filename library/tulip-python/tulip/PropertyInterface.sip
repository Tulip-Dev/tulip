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

class PropertyInterface /Abstract/ {
%TypeHeaderCode
#include <tulip/PropertyInterface.h>
%End

%ConvertToSubClassCode
    if (dynamic_cast<tlp::BooleanProperty *>(sipCpp)) {
        sipType = sipType_tlp_BooleanProperty;
    } else if (dynamic_cast<tlp::ColorProperty *>(sipCpp)) {
        sipType = sipType_tlp_ColorProperty;
    } else if (dynamic_cast<tlp::DoubleProperty *>(sipCpp)) {
        sipType = sipType_tlp_DoubleProperty;
    } else if (dynamic_cast<tlp::GraphProperty *>(sipCpp)) {
        sipType = sipType_tlp_GraphProperty;
    } else if (dynamic_cast<tlp::IntegerProperty *>(sipCpp)) {
        sipType = sipType_tlp_IntegerProperty;
    } else if (dynamic_cast<tlp::LayoutProperty *>(sipCpp)) {
        sipType = sipType_tlp_LayoutProperty;
    } else if (dynamic_cast<tlp::SizeProperty *>(sipCpp)) {
        sipType = sipType_tlp_SizeProperty;
    } else if (dynamic_cast<tlp::StringProperty *>(sipCpp)) {
        sipType = sipType_tlp_StringProperty;

    } else if (dynamic_cast<tlp::BooleanVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_BooleanVectorProperty;
    } else if (dynamic_cast<tlp::ColorVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_ColorVectorProperty;
    } else if (dynamic_cast<tlp::DoubleVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_DoubleVectorProperty;
    } else if (dynamic_cast<tlp::IntegerVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_IntegerVectorProperty;
    } else if (dynamic_cast<tlp::CoordVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_CoordVectorProperty;
    } else if (dynamic_cast<tlp::SizeVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_SizeVectorProperty;
    } else if (dynamic_cast<tlp::StringVectorProperty *>(sipCpp)) {
        sipType = sipType_tlp_StringVectorProperty;
    } else {
        sipType = NULL;
    }
%End

public:
 
 	tlp::Graph * getGraph();
%Docstring
tlp.PropertyInterface.getGraph()

Returns the graph on which the property has been defined.

:rtype: :class:`tlp.Graph`

.. warning:: If the property is inherited the graph could be different
             that the one used to get that property.

%End  

//===========================================================================================

	virtual tlp::Iterator<tlp::node>* getNonDefaultValuatedNodes(const tlp::Graph* subgraph= NULL) const = 0;
%Docstring
tlp.PropertyInterface.getNonDefaultValuatedNodes([subgraph = None])

Returns an iterator on all nodes whose value is different
from the default value. 

:param subgraph: a sub-graph can be given in parameter. In that case, only the nodes owned by this sub-graph are returned by the iterator.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.IteratorNode`
%End	

//===========================================================================================
	
	virtual tlp::Iterator<tlp::edge>* getNonDefaultValuatedEdges(const tlp::Graph* subgraph= NULL) const = 0;
%Docstring
tlp.PropertyInterface.getNonDefaultValuatedEdges([subgraph = None])

Returns an iterator on all edges whose value is different
from the default value. 

:param subgraph: a sub-graph can be given in parameter. In that case, only the edges owned by this sub-graph are returned by the iterator.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.IteratorEdge`
%End	
	
//===========================================================================================	
	
	virtual std::string getTypename() const = 0;
%Docstring
tlp.PropertyInterface.getTypename()

Returns a string describing the type of the property,
i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...

:rtype: string
%End

//===========================================================================================

	const std::string& getName() const = 0;
%Docstring
tlp.PropertyInterface.getName()

Returns the name of the property.

:rtype: string
%End

//===========================================================================================

	virtual std::string getNodeDefaultStringValue() const = 0;
%Docstring
tlp.PropertyInterface.getNodeDefaultStringValue()

Returns a string representation of the node default value.

:rtype: string
%End
	
//===========================================================================================	
	
	virtual std::string getEdgeDefaultStringValue() const = 0;
%Docstring
tlp.PropertyInterface.getEdgeDefaultStringValue()

Returns a string representation of the edge default value.

:rtype: string
%End	
//===========================================================================================	
	
	virtual bool setAllNodeStringValue( const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setAllNodeStringValue(str)

Clears all nodes registered values, and sets a new node default value
in converting the given string representation of a value.
Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representation of a value
:type str: string
:rtype: boolean
%End
	
//===========================================================================================	
	
	virtual bool setAllEdgeStringValue( const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setAllEdgeStringValue(str)

Clears all edges registered values, and sets a new edge default value
in converting the given string representation of a value.
Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representation of a value
:type str: string
:rtype: boolean
%End
//===========================================================================================

	virtual void erase(const tlp::node node) = 0;
%Docstring
tlp.PropertyInterface.erase(node)

Removes the value stored for the node given in parameter.
The new value for the node is the default one.

:param node: an existing node
:type node: :class:`tlp.node`
:throws: an exception if the node does not belong to the graph attached to the property
%End	
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->erase(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
//===========================================================================================	
	
	virtual void erase(const tlp::edge edge) = 0;
%Docstring
tlp.PropertyInterface.erase(edge)

Removes the value stored for the edge given in parameter.
The new value for the edge is the default one.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:throws: an exception if the edge does not belong to the graph attached to the property
%End	
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->erase(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

    virtual std::string getNodeStringValue( const tlp::node node )  = 0;
%Docstring
tlp.PropertyInterface.getNodeStringValue(node)

Returns a string conversion of the value registered for the given node.

:param node: an existing node
:type node: :class:`tlp.node`
:rtype: string
:throws: an exception if the node does not belong to the graph attached to the property
%End	
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getNodeStringValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
//===========================================================================================	
	
    virtual std::string getEdgeStringValue( const tlp::edge edge )  = 0;
%Docstring
tlp.PropertyInterface.getEdgeStringValue(edge)

Returns a string conversion of the value registered for the given edge.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:rtype: string
:throws: an exception if the edge does not belong to the graph attached to the property
%End	
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getEdgeStringValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End
	
//===========================================================================================	
	
	virtual bool setNodeStringValue( const tlp::node node, const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setNodeStringValue(node, str)

Registers a new value for the given node in converting
the given string representation.
Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param node: an existing node
:type node: :class:`tlp.node`
:param str: a string representation of a value
:type str: string
:rtype: boolean
:throws: an exception if the node does not belong to the graph attached to the property
%End	
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeStringValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
//===========================================================================================	
	
	virtual bool setEdgeStringValue( const tlp::edge edge, const std::string & strVal) = 0;
%Docstring
tlp.PropertyInterface.setEdgeStringValue(edge, str)

Registers a new value for the given edge in converting
the given string representation.
Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param str: a string representation of a value
:type str: string
:rtype: boolean
:throws: an exception if the edge does not belong to the graph attached to the property
%End	
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeStringValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

    void addPropertyObserver(tlp::PropertyObserver *pObs);
%Docstring
tlp.PropertyInterface.addPropertyObserver(propertyObserver)

.. versionadded:: 3.8

Registers a new property observer object
that will receive notifications when the property is modified.

:param propertyObserver: the new property observer to register
:type propertyObserver: :class:`tlp.PropertyObserver`
%End

//===========================================================================================

    void removePropertyObserver(tlp::PropertyObserver *pObs);
%Docstring
tlp.PropertyInterface.removePropertyObserver(propertyObserver)

.. versionadded:: 3.8

Removes a registered property observer object.

:param propertyObserver: the property observer to remove
:type propertyObserver: :class:`tlp.PropertyObserver`
%End

//===========================================================================================

    unsigned int countPropertyObservers() const;
%Docstring
tlp.PropertyInterface.countPropertyObservers()

.. versionadded:: 3.8

Returns the number of registered property observers.

:rtype: integer
%End

//===========================================================================================	
		
    std::string __getitem__(const tlp::node node) ;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getNodeStringValue(*a0));     	
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End


//===========================================================================================

    void __setitem__(const tlp::node node, const std::string &strVal) ;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeStringValue(*a0, *a1);     	
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

//===========================================================================================	
		
    std::string __getitem__(const tlp::edge edge) ;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getEdgeStringValue(*a0));     	
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
%End


//===========================================================================================

    void __setitem__(const tlp::edge edge, const std::string &strVal) ;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeStringValue(*a0, *a1);     	
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
%End 
  	
};

};
