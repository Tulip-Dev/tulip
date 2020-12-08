/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

namespace tlp {

class PropertyInterface : tlp::Observable /Abstract/ {
%TypeHeaderCode
#include <tulip/PropertyInterface.h>
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

  virtual bool hasNonDefaultValuatedNodes(const tlp::Graph* subgraph= NULL) const = 0;
 %Docstring
 tlp.PropertyInterface.hasNonDefaultValuatedNodes(subgraph=None)

.. versionadded:: 5.2

 Returns whether it exists nodes whose value is different
 from the default value.

 :param subgraph: a subgraph can be given in parameter. In that case, only the nodes belonging to this subgraph are considered.
 :type subgraph: :class:`tlp.Graph`
 :rtype: boolean
 %End

 //===========================================================================================

  virtual unsigned int numberOfNonDefaultValuatedNodes(const tlp::Graph* subgraph= NULL) const = 0;
 %Docstring
 tlp.PropertyInterface.numberOfNonDefaultValuatedNodes(subgraph=None)

.. versionadded:: 4.8

 Returns the number of nodes whose value is different
 from the default value.

 :param subgraph: a subgraph can be given in parameter. In that case, only the number of nodes in this subgraph whose value is different from the default one is returned.
 :type subgraph: :class:`tlp.Graph`
 :rtype: integer
 %End

//===========================================================================================

  virtual tlp::Iterator<tlp::node>* getNonDefaultValuatedNodes(const tlp::Graph* subgraph= NULL) const = 0 /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.PropertyInterface.getNonDefaultValuatedNodes(subgraph=None)

Returns an iterator on all nodes whose value is different
from the default value. 

:param subgraph: a subgraph can be given in parameter. In that case, only the nodes owned by this subgraph are returned by the iterator.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.IteratorNode`
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getNonDefaultValuatedNodes(a0));
%End

 //===========================================================================================

  virtual bool hasNonDefaultValuatedEdges(const tlp::Graph* subgraph= NULL) const = 0;
 %Docstring
 tlp.PropertyInterface.hasNonDefaultValuatedEdges(subgraph=None)

.. versionadded:: 5.2

 Returns whether it exists edges whose value is different
 from the default value.

 :param subgraph: a subgraph can be given in parameter. In that case, only the edges belonging to this subgraph are considered.
 :type subgraph: :class:`tlp.Graph`
 :rtype: boolean
 %End

 //===========================================================================================

  virtual unsigned int numberOfNonDefaultValuatedEdges(const tlp::Graph* subgraph= NULL) const = 0;
 %Docstring
 tlp.PropertyInterface.numberOfNonDefaultValuatedEdges(subgraph=None)

.. versionadded:: 4.8

 Returns the number of edges whose value is different
 from the default value.

 :param subgraph: a subgraph can be given in parameter. In that case, only the number of edges in this subgraph whose value is different from the default one is returned.
 :type subgraph: :class:`tlp.Graph`
 :rtype: integer
 %End

//===========================================================================================

  virtual tlp::Iterator<tlp::edge>* getNonDefaultValuatedEdges(const tlp::Graph* subgraph= NULL) const = 0 /TransferBack, TypeHint="Iterator[tlp.edge]"/;
%Docstring
tlp.PropertyInterface.getNonDefaultValuatedEdges(subgraph=None)

Returns an iterator on all edges whose value is different
from the default value. 

:param subgraph: a subgraph can be given in parameter. In that case, only the edges owned by this subgraph are returned by the iterator.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.IteratorEdge`
%End

%MethodCode
 sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getNonDefaultValuatedEdges(a0));
%End

//===========================================================================================

  virtual const std::string& getTypename() const = 0;
%Docstring
tlp.PropertyInterface.getTypename()

Returns a string describing the type of the property,
i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...

:rtype: string
%End

//===========================================================================================

  virtual const std::string& getCppClassName() const = 0;
%Docstring
tlp.PropertyInterface.getCppClassName()

Returns a string giving the name of a the PropertyInterface subclass
e.g, "tlp::BooleanProperty", "tlp::DoubleProperty", ...

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

  virtual tlp::PropertyInterface* clonePrototype(tlp::Graph* g, const std::string & name) const = 0;
%Docstring
tlp.PropertyInterface.clonePrototype(g, name)

Returns a newly created property of the same type in the graph g.

:param g: the graph in with the new property will be added
:type g: :class:`tlp.Graph`
:param name: the name of the property to create and return
:type name: string
:rtype: :class:`tlp.PropertyInterface`
%End

//===========================================================================================

virtual bool setNodeDefaultStringValue( const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setNodeDefaultStringValue(str)

.. versionadded:: 5.0

Sets the value assigned as the default one to the future added nodes from a string representation.

Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representing the new value to set on future added nodes.
:type str: string
:rtype: boolean
%End

//===========================================================================================

 virtual bool setAllNodeStringValue( const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setAllNodeStringValue(str)

Sets all the nodes value to the value represented by a string.
All previous values are lost and the represented value is assigned as the default one to the future added nodes.

Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representation of a value
:type str: string
:rtype: boolean
%End

//===========================================================================================

 virtual bool setStringValueToGraphNodes( const std::string & strVal , const tlp::Graph *graph ) = 0;
%Docstring
tlp.PropertyInterface.setStringValueToGraphNodes(str, graph)

.. versionadded:: 5.0

Sets the same value for all nodes in a graph in converting the given string representation of a value.
Only the nodes from that graph will have their value modified in the property
and the default node value will not be modified.

.. warning:: If the provided graph is not a descendant of the one associated to that property, no node value will be modified in it.

Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representation of a value
:type str: string
:param graph: a graph that defines a set of nodes
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End

//===========================================================================================

virtual bool setEdgeDefaultStringValue( const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setEdgeDefaultStringValue(str)

.. versionadded:: 5.0

Sets the value assigned as the default one to the future added edges from a string representation.

Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representing the new value to set on future added edges.
:type str: string
:rtype: boolean
%End

//===========================================================================================

virtual bool setAllEdgeStringValue( const std::string & strVal ) = 0;
%Docstring
tlp.PropertyInterface.setAllEdgeStringValue(str)

Sets all the edges value to the value represented by a string.
All previous values are lost and the represented value is assigned as the default one to the future added edges.

Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representation of a value
:type str: string
:rtype: boolean
%End

//===========================================================================================

virtual bool setStringValueToGraphEdges( const std::string & strVal , const tlp::Graph *graph ) = 0;
%Docstring
tlp.PropertyInterface.setStringValueToGraphEdges(str, graph)

.. versionadded:: 5.0

Sets the same value for all edges in a graph in converting the given string representation of a value.
Only the edges from that descendant graph will have their value modified in the property
and the default edge value will not be modified.

.. warning:: If the provided graph is not a descendant of the one associated to that property, no edge value will be modified in it.

Returns :const:`True` if the given string representation has been successfully converted to a value, :const:`False` otherwise.

:param str: a string representation of a value
:type str: string
:param graph: a graph that defines a set of edges
:type graph: :class:`tlp.Graph`
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

  virtual std::string getNodeStringValue( const tlp::node node ) const = 0;
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

  virtual std::string getEdgeStringValue( const tlp::edge edge ) const = 0;
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

  std::string __getitem__(const tlp::node node);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipRes = new std::string(sipCpp->getNodeStringValue(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

//===========================================================================================

  void __setitem__(const tlp::node node, const std::string &strVal);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeStringValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

//===========================================================================================

  std::string __getitem__(const tlp::edge edge);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipRes = new std::string(sipCpp->getEdgeStringValue(*a0));
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End


//===========================================================================================

  void __setitem__(const tlp::edge edge, const std::string &strVal);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeStringValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End 
  
};

};
