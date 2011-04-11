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

%Include PropertyInterface.sip

namespace tlp {

class ColorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/ColorProperty.h>
%End
public :

//===========================================================================================
	
	ColorProperty(tlp::Graph *g, std::string n="");
	
//===========================================================================================
	
	void setAllNodeValue(const tlp::Color &v);
%Docstring
tlp.ColorProperty.setAllNodeValue(val)

Sets the same value on the whole set of nodes.

:param val: the value to set on the nodes.
:type val: :class:`tlp.Color`
%End

//===========================================================================================

	void setAllEdgeValue(const tlp::Color &v);
%Docstring
tlp.ColorProperty.setAllEdgeValue(val)

Sets the same value on the whole set of edges.

:param val: the value to set on the edges.
:type val: :class:`tlp.Color`
%End
	
//===========================================================================================	
	
	tlp::Color getNodeDefaultValue();
%Docstring
tlp.ColorProperty.getNodeDefaultValue()

Returns the default value associated to the nodes.

:rtype: :class:`tlp.Color`
%End	
	
//===========================================================================================	
	
	tlp::Color getEdgeDefaultValue();
%Docstring
tlp.ColorProperty.getEdgeDefaultValue()

Returns the default value associated to the edges.

:rtype: :class:`tlp.Color`
%End	

//===========================================================================================

	const tlp::Color getNodeValue(const tlp::node n);
%Docstring
tlp.ColorProperty.getNodeValue(node)

Returns the value associated to a particular node.

:param node: an existing node
:type node: :class:`tlp.node`
:rtype: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End	
		
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new tlp::Color(sipCpp->getNodeValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

	const tlp::Color getEdgeValue(const tlp::edge e);
%Docstring
tlp.ColorProperty.getEdgeValue(edge) 

Returns the value associated to a particular edge.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:rtype: :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new tlp::Color(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

	void setNodeValue(const tlp::node n, const tlp::Color &v);
%Docstring
tlp.ColorProperty.setNodeValue(node, val)

Sets the value of a particular node.

:param node: an existing node
:type node: :class:`tlp.node`
:param val: the value to set on the node
:type val: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

	void setEdgeValue(const tlp::edge e, const tlp::Color &v);
%Docstring
tlp.ColorProperty.setEdgeValue(edge, val)

Sets the value of a particular edge.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param val: the value to set on the edge
:type val: :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

	tlp::Color __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new tlp::Color(sipCpp->getNodeValue(*a0));
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

//===========================================================================================

	void __setitem__(const tlp::node n, const tlp::Color &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

//===========================================================================================

	tlp::Color __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new tlp::Color(sipCpp->getEdgeValue(*a0));
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

//===========================================================================================

	void __setitem__(const tlp::edge e, const tlp::Color &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setEdgeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    } 
%End

//===========================================================================================

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Color property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
//===========================================================================================	
	

	
};

class ColorVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/ColorProperty.h>
%End
public :

//===========================================================================================

	ColorVectorProperty (tlp::Graph *, std::string n="");
  	
//===========================================================================================  	
  	
	const std::vector<tlp::Color> getNodeDefaultValue();
%Docstring
tlp.ColorVectorProperty.getNodeDefaultValue()

Returns the default list of values associated to the nodes.

:rtype: list of :class:`tlp.Color`
%End
	
//===========================================================================================	
	
	const std::vector<tlp::Color> getEdgeDefaultValue();
%Docstring
tlp.ColorVectorProperty.getEdgeDefaultValue()

Returns the default list of values associated to the edges.

:rtype: list of :class:`tlp.Color`
%End

//===========================================================================================

	const std::vector<tlp::Color> getNodeValue(const tlp::node n);
%Docstring
tlp.ColorVectorProperty.getNodeValue(node)

Returns the list of values associated to a particular node.

:param node: an existing node
:type node: :class:`tlp.node`
:rtype: list of :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::vector<tlp::Color>(sipCpp->getNodeValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End
	
//===========================================================================================	
	
	const std::vector<tlp::Color> getEdgeValue(const tlp::edge e);
%Docstring
tlp.ColorVectorProperty.getEdgeValue(edge)

Returns the list of values associated to a particular edge.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:rtype: list of :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::vector<tlp::Color>(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End	

//===========================================================================================

	void setNodeValue(const tlp::node n, const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setNodeValue(node, val)

Sets the list of values of a particular node.

:param node: an existing node
:type node: :class:`tlp.node`
:param val: the list of values to set on the node
:type val: list of :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End
	
//===========================================================================================	
	
	void setEdgeValue(const tlp::edge e, const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setEdgeValue(edge, val)

Sets the list of values of a particular edge.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param val: the list of values to set on the edge
:type val: list of :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End	

//===========================================================================================

	void setAllNodeValue(const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setAllNodeValue(val)

Sets the same list of values on the whole set of nodes.

:param val: the list of values to set on the nodes
:type val: list of :class:`tlp.Color` 
%End

//===========================================================================================

	void setAllEdgeValue(const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setAllEdgeValue(val)

Sets the same list of values on the whole set of edges.

:param val: the list of values to set on the edges
:type val: list of :class:`tlp.Color` 
%End
	
//===========================================================================================	
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const tlp::Color &v);
%Docstring	
tlp.ColorVectorProperty.setNodeEltValue(node, index, val)

Sets the value in the list attached to a particular node at the given index.

:param node: an existing node
:type node: :class:`tlp.node`
:param index: the index in the list
:type index: integer
:param val: the value to set in the list
:type val: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property or if the list attached to the node has a size smaller than the given index.
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		const std::vector<tlp::Color> &v = sipCpp->getNodeValue(*a0);
		if (a1 < v.size()) {
			sipCpp->setNodeEltValue(*a0, a1, *a2);
		} else {
			std::ostringstream oss;
			oss << "vector associated to node " << a0->id << " for vector property \"" << sipCpp->getName() << "\" has a size of " << v.size() << " and the requested index is " << a1 ;
			PyErr_SetString(PyExc_Exception, oss.str().c_str());
			sipIsErr = -1;
		}	
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End
 
//=========================================================================================== 
 
 	const tlp::Color& getNodeEltValue(const tlp::node n, unsigned int i) const;
%Docstring	
tlp.ColorVectorProperty.getNodeEltValue(node, index)

Returns the value in the list attached to a particular node at the given index.

:param node: an existing node
:type node: :class:`tlp.node`
:param index: the index in the list
:type index: integer
:rtype: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property or if the list attached to the node has a size smaller than the given index.
%End
 	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		const std::vector<tlp::Color> &v = sipCpp->getNodeValue(*a0);
		if (a1 < v.size()) {
			sipRes = new tlp::Color(sipCpp->getNodeEltValue(*a0, a1));
		} else {
			std::ostringstream oss;
			oss << "vector associated to node " << a0->id << " for vector property \"" << sipCpp->getName() << "\" has a size of " << v.size() << " and the requested index is " << a1 ;
			PyErr_SetString(PyExc_Exception, oss.str().c_str());
			sipIsErr = -1;
		}	
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End
  
//===========================================================================================  
  
  	void pushBackNodeEltValue(const tlp::node n, const tlp::Color& v);
%Docstring	
tlp.ColorVectorProperty.pushBackNodeEltValue(node, val)

Inserts a value at the end of the list attached to a particular node. The size of the list is incremented by one.

:param node: an existing node
:type node: :class:`tlp.node`
:param val: the value to add in the list
:type val: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End
  	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->pushBackNodeEltValue(*a0, *a1);	
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End
 
//=========================================================================================== 
 
	void popBackNodeEltValue(const tlp::node n);
%Docstring	
tlp.ColorVectorProperty.popBackNodeEltValue(node)

Removes the value at the end of the list (if not empty) attached to a particular node. The size of the list is decremented by one.

:param node: an existing node
:type node: :class:`tlp.node`
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->popBackNodeEltValue(*a0);	
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
//===========================================================================================	
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
%Docstring	
tlp.ColorVectorProperty.resizeNodeValue(node, size)

Resizes the list attached to a particular node. A default value will be assigned to new elements (if any).

:param node: an existing node
:type node: :class:`tlp.node`
:param size: the new size of the list
:type size: integer
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->resizeNodeValue(*a0, a1);	
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End
	
//===========================================================================================	
	
	void resizeNodeValue(const tlp::node n, unsigned int size, tlp::Color elt);
%Docstring	
tlp.ColorVectorProperty.resizeNodeValue(node, size, val)

Resizes the list attached to a particular node. The value given in paramater will be assigned to new elements (if any).

:param node: an existing node
:type node: :class:`tlp.node`
:param size: the new size of the list
:type size: integer
:param val: the value to assign on new elements in the list
:type val: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->resizeNodeValue(*a0, a1, *a2);	
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
//===========================================================================================	
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const tlp::Color &v);
%Docstring	
tlp.ColorVectorProperty.setEdgeEltValue(edge, index, val)

Sets the value in the list attached to a particular edge at the given index.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param index: the index in the list
:type index: integer
:param val: the value to set in the list
:type val: :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property or if the list attached to the edge has a size smaller than the given index.
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		const std::vector<tlp::Color> &v = sipCpp->getEdgeValue(*a0);
		if (a1 < v.size()) {
			sipCpp->setEdgeEltValue(*a0, a1, *a2);
		} else {
			std::ostringstream oss;
			oss << "vector associated to edge " << a0->id << " for vector property \"" << sipCpp->getName() << "\" has a size of " << v.size() << " and the requested index is " << a1 ;
			PyErr_SetString(PyExc_Exception, oss.str().c_str());
			sipIsErr = -1;
		}	
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End  

//===========================================================================================

	const tlp::Color& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
%Docstring	
tlp.ColorVectorProperty.getEdgeEltValue(edge, index)

Returns the value in the list attached to a particular edge at the given index.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param index: the index in the list
:type index: integer
:rtype: :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property or if the list attached to the edge has a size smaller than the given index.
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		const std::vector<tlp::Color> &v = sipCpp->getEdgeValue(*a0);
		if (a1 < v.size()) {
			sipRes = new tlp::Color(sipCpp->getEdgeEltValue(*a0, a1));
		} else {
			std::ostringstream oss;
			oss << "vector associated to edge " << a0->id << " for vector property \"" << sipCpp->getName() << "\" has a size of " << v.size() << " and the requested index is " << a1 ;
			PyErr_SetString(PyExc_Exception, oss.str().c_str());
			sipIsErr = -1;
		}	
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End
  
//===========================================================================================  
  
	void pushBackEdgeEltValue(const tlp::edge e, const tlp::Color &v);
%Docstring	
tlp.ColorVectorProperty.pushBackEdgeEltValue(edge, val)

Inserts a value at the end of the list attached to a particular edge. The size of the list is incremented by one.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param val: the value to add in the list
:type val: :class:`tlp.Color`
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->pushBackEdgeEltValue(*a0, *a1);	
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End
  
//===========================================================================================  
  
	void popBackEdgeEltValue(const tlp::edge e);
%Docstring	
tlp.ColorVectorProperty.popBackEdgeEltValue(edge)

Removes the value at the end of the list (if not empty) attached to a particular edge. The size of the list is decremented by one.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:throws: an exception if the edge does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->popBackEdgeEltValue(*a0);	
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End
  
//===========================================================================================  
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
%Docstring	
tlp.ColorVectorProperty.resizeEdgeValue(edge, size)

Resizes the list attached to a particular edge. A default value will be assigned to new elements (if any).

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param size: the new size of the list
:type size: integer
:throws: an exception if the edge does not belong to the graph attached to the property
%End
  	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->resizeEdgeValue(*a0, a1);	
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End  	
  	
//===========================================================================================  	
  	
	void resizeEdgeValue(const tlp::edge e, unsigned int size, tlp::Color elt);
%Docstring	
tlp.ColorVectorProperty.resizeEdgeValue(edge, size, val)

Resizes the list attached to a particular edge. The value given in paramater will be assigned to new elements (if any).

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:param size: the new size of the list
:type size: integer
:param val: the value to assign on new elements in the list
:type val: :class:`tlp.Color`
:throws: an exception if the node does not belong to the graph attached to the property
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->resizeEdgeValue(*a0, a1, *a2);	
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End 
    
//===========================================================================================    
    
	std::vector<tlp::Color> __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
     	sipRes = new std::vector<tlp::Color>(sipCpp->getNodeValue(*a0));
     } else {
     	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
     }
%End

//===========================================================================================

	void __setitem__(const tlp::node n, const std::vector<tlp::Color> &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
     	sipCpp->setNodeValue(*a0, *a1);
     } else {
     	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
     }
%End

//===========================================================================================

	std::vector<tlp::Color> __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
     	sipRes = new std::vector<tlp::Color>(sipCpp->getEdgeValue(*a0));
     } else {
     	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
     }
%End

//===========================================================================================

	void __setitem__(const tlp::edge e, const std::vector<tlp::Color> &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
     	sipCpp->setEdgeValue(*a0, *a1);
     } else {
     	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
     }
%End

//===========================================================================================

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Color vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};

