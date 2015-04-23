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

class GraphProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/GraphProperty.h>
%End
public:

	GraphProperty(tlp::Graph *graph, std::string name="");
	
	virtual ~GraphProperty();
		
	tlp::Graph* getNodeDefaultValue();
	
	std::set<tlp::edge> getEdgeDefaultValue() ;
%Docstring
tlp.GraphProperty.getEdgeDefaultValue() -> list of tlp.edge
%End

	tlp::Graph* getNodeValue(const tlp::node node);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getNodeValue(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

	const std::set<tlp::edge> getEdgeValue(const tlp::edge edge) ;
%Docstring
tlp.GraphProperty.getEdgeValue(tlp.edge) -> set of tlp.edge
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::set<tlp::edge>(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End	

	void setNodeValue(const tlp::node node, tlp::Graph *val);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End


	void setEdgeValue(const tlp::edge edge, const std::set<tlp::edge> &val);
%Docstring
tlp.GraphProperty.setEdgeValue(tlp.edge, set of tlp.edge)
%End
	
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End	

	void setAllNodeValue(tlp::Graph *val);

	void setAllEdgeValue(const std::set<tlp::edge> &val);
%Docstring
tlp.GraphProperty.setAllEdgeValue(set of tlp.edge)
%End
	
	
	tlp::Graph* __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getNodeValue(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

	void __setitem__(const tlp::node n, tlp::Graph* g) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

	std::set<tlp::edge> __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::set<tlp::edge>(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

	void __setitem__(const tlp::edge e, const std::set<tlp::edge> &se) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<graph property " << sipCpp->getName() << ">";
	std::string s = oss.str();
#if PY_MAJOR_VERSION >= 3
    sipRes = PyUnicode_FromString(s.c_str());
#else
	sipRes = PyString_FromString(s.c_str());
#endif
%End
	
};
};
