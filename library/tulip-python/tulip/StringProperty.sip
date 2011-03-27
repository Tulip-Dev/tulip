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

class StringProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/StringProperty.h>
%End
public :
	
	StringProperty(tlp::Graph *g, std::string n="");
	
	void setAllNodeValue(const std::string &v);
%Docstring
tlp.StringProperty.setAllNodeValue(string)
%End

	void setAllEdgeValue(const std::string &v);
%Docstring
tlp.StringProperty.setAllEdgeValue(string)
%End
	
	std::string getNodeDefaultValue();
%Docstring
tlp.StringProperty.getNodeDefaultValue() -> string
%End	
	
	std::string getEdgeDefaultValue();
%Docstring
tlp.StringProperty.getEdgeDefaultValue() -> string
%End	

	const std::string getNodeValue(const tlp::node n);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getNodeValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.StringProperty.getNodeValue(tlp.node) -> string
%End

	const std::string getEdgeValue(const tlp::edge e);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::string(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.StringProperty.getEdgeValue(tlp.edge) -> string
%End

	void setNodeValue(const tlp::node n, const std::string &v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.StringProperty.setNodeValue(tlp.node, string)
%End

	void setEdgeValue(const tlp::edge e, const std::string &v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.StringProperty.setEdgeValue(tlp.edge, string)
%End

	std::string __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new std::string(sipCpp->getNodeValue(*a0));
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

	void __setitem__(const tlp::node n, const std::string &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

	std::string __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new std::string(sipCpp->getEdgeValue(*a0));
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

	void __setitem__(const tlp::edge e, const std::string &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setEdgeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    } 
%End
	

	
};

};


namespace tlp {

class StringVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/StringProperty.h>
%End
public :

	StringVectorProperty (tlp::Graph *, std::string n="");
  	
 
	const std::vector<std::string> getNodeDefaultValue();
%Docstring
tlp.StringVectorProperty.getNodeDefaultValue() -> list of string
%End
	
	const std::vector<std::string> getEdgeDefaultValue();
%Docstring
tlp.StringVectorProperty.getEdgeDefaultValue() -> list of string
%End

	const std::vector<std::string> getNodeValue(const tlp::node n);
%Docstring
tlp.StringVectorProperty.getNodeValue(tlp.node) -> list of string
%End

	const std::vector<std::string> getEdgeValue(const tlp::edge e);
%Docstring
tlp.StringVectorProperty.getEdgeValue(tlp.edge) -> list of string
%End

	void setNodeValue(const tlp::node n, const std::vector<std::string> &v);
%Docstring
tlp.StringVectorProperty.setNodeValue(tlp.node, list of string)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<std::string> &v);
%Docstring
tlp.StringVectorProperty.setEdgeValue(tlp.edge, list of string)
%End

	void setAllNodeValue(const std::vector<std::string> &v);
%Docstring
tlp.StringVectorProperty.setAllNodeValue(list of string)
%End

	void setAllEdgeValue(const std::vector<std::string> &v);
%Docstring
tlp.StringVectorProperty.setAllEdgeValue(list of string)
%End
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const std::string &v);
 
 	const std::string& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const std::string& v);
 
	void popBackNodeEltValue(const tlp::node n);
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
	void resizeNodeValue(const tlp::node n, unsigned int size, std::string elt);
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const std::string &v);
  
	const std::string& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
	void pushBackEdgeEltValue(const tlp::edge e, const std::string &v);
  
	void popBackEdgeEltValue(const tlp::edge e);
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
	void resizeEdgeValue(const tlp::edge e, unsigned int size, std::string elt);
    
	std::vector<std::string> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<std::string>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<std::string> &v) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<std::string> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<std::string>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<std::string> &v) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<String vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};