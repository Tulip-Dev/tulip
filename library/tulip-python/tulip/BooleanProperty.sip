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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert                              |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

%Include PropertyInterface.sip

namespace tlp {

class BooleanProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/BooleanProperty.h>
%End
public :
	
	BooleanProperty(tlp::Graph *g, std::string n="");
	
	void setAllNodeValue(const bool v);
%Docstring
tlp.BooleanProperty.setAllNodeValue(bool)
%End

	void setAllEdgeValue(const bool v);
%Docstring
tlp.BooleanProperty.setAllEdgeValue(bool)
%End
	
	bool getNodeDefaultValue();
%Docstring
tlp.BooleanProperty.getNodeDefaultValue() -> bool
%End	
	
	bool getEdgeDefaultValue();
%Docstring
tlp.BooleanProperty.getEdgeDefaultValue() -> bool
%End

	const bool getNodeValue(const tlp::node n);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getNodeValue(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.BooleanProperty.getNodeValue(tlp.node) -> bool
%End

	const bool getEdgeValue(const tlp::edge e);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getEdgeValue(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.BooleanProperty.getEdgeValue(tlp.edge) -> bool
%End

	void setNodeValue(const tlp::node n, const bool v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.BooleanProperty.setNodeValue(tlp.node, @PYTHON_NODE_TYPE)
%End

	void setEdgeValue(const tlp::edge e, const bool v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.BooleanProperty.setEdgeValue(tlp.edge, @PYTHON_EDGE_TYPE)
%End

	bool __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = sipCpp->getNodeValue(*a0);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

	void __setitem__(const tlp::node n, const bool v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

	bool __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = sipCpp->getEdgeValue(*a0);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

	void __setitem__(const tlp::edge e, const bool v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setEdgeValue(*a0, a1);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    } 
%End
	
	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Boolean property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	

	void reverse();

	void reverseEdgeDirection();
	
	tlp::Iterator<tlp::node> *getNodesEqualTo(const bool val, tlp::Graph *g = 0);
		
	tlp::Iterator<tlp::edge> *getEdgesEqualTo(const bool val, tlp::Graph *g = 0);
	

	
};

};

namespace tlp {

class BooleanVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/BooleanProperty.h>
%End
public :

	BooleanVectorProperty (tlp::Graph *, std::string n="");
  	
 
	const std::vector<bool> getNodeDefaultValue();
%Docstring
tlp.BooleanVectorProperty.getNodeDefaultValue() -> list of bool
%End
	
	const std::vector<bool> getEdgeDefaultValue();
%Docstring
tlp.BooleanVectorProperty.getEdgeDefaultValue() -> list of bool
%End

	const std::vector<bool> getNodeValue(const tlp::node n);
%Docstring
tlp.BooleanVectorProperty.getNodeValue(tlp.node) -> list of bool
%End

	const std::vector<bool> getEdgeValue(const tlp::edge e);
%Docstring
tlp.BooleanVectorProperty.getEdgeValue(tlp.edge) -> list of bool
%End

	void setNodeValue(const tlp::node n, const std::vector<bool> &v);
%Docstring
tlp.BooleanVectorProperty.setNodeValue(tlp.node, list of bool)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<bool> &v);
%Docstring
tlp.BooleanVectorProperty.setEdgeValue(tlp.edge, list of bool)
%End

	void setAllNodeValue(const std::vector<bool> &v);
%Docstring
tlp.BooleanVectorProperty.setAllNodeValue(list of bool)
%End

	void setAllEdgeValue(const std::vector<bool> &v);
%Docstring
tlp.BooleanVectorProperty.setAllEdgeValue(list of bool)
%End
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const bool &v);
 
 	const bool& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const bool& v);
 
	void popBackNodeEltValue(const tlp::node n);
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
	void resizeNodeValue(const tlp::node n, unsigned int size, bool elt);
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const bool &v);
  
	const bool& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
	void pushBackEdgeEltValue(const tlp::edge e, const bool &v);
  
	void popBackEdgeEltValue(const tlp::edge e);
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
	void resizeEdgeValue(const tlp::edge e, unsigned int size, bool elt);
    
	std::vector<bool> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<bool>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<bool> &v) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<bool> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<bool>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<bool> &v) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Boolean vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};