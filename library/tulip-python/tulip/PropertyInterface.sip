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
public:

	virtual tlp::Iterator<tlp::node>* getNonDefaultValuatedNodes(const tlp::Graph* = NULL) const = 0;
	
	virtual tlp::Iterator<tlp::edge>* getNonDefaultValuatedEdges(const tlp::Graph* = NULL) const = 0;
	
	virtual std::string getTypename() const = 0;

	const std::string& getName() const = 0;

	virtual std::string getNodeDefaultStringValue() const = 0;
	
	virtual std::string getEdgeDefaultStringValue() const = 0;
	
	virtual bool setAllNodeStringValue( const std::string & v ) = 0;
	
	virtual bool setAllEdgeStringValue( const std::string & v ) = 0;

	virtual void erase(const tlp::node n) = 0;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->erase(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
	virtual void erase(const tlp::edge) = 0;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->erase(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

	virtual std::string getNodeStringValue( const tlp::node n ) const = 0;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getNodeStringValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
	virtual std::string getEdgeStringValue( const tlp::edge e ) const = 0;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getEdgeStringValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End
	
	virtual bool setNodeStringValue( const tlp::node n, const std::string & v ) = 0;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeStringValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End	
	
	virtual bool setEdgeStringValue( const tlp::edge e, const std::string & v ) = 0;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeStringValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End
		
  	
};

};
