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

class IntegerProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/IntegerProperty.h>
%End
public :
	
	IntegerProperty(tlp::Graph *g, std::string n="");
	
	void setAllNodeValue(const int v);
%Docstring
tlp.IntegerProperty.setAllNodeValue(int)
%End

	void setAllEdgeValue(const int v);
%Docstring
tlp.IntegerProperty.setAllEdgeValue(int)
%End
	
	int getNodeDefaultValue();
%Docstring
tlp.IntegerProperty.getNodeDefaultValue() -> int
%End	
	
	int getEdgeDefaultValue();
%Docstring
tlp.IntegerProperty.getEdgeDefaultValue() -> int
%End

	const int getNodeValue(const tlp::node n);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getNodeValue(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.IntegerProperty.getNodeValue(tlp.node) -> int
%End

	const int getEdgeValue(const tlp::edge e);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getEdgeValue(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.IntegerProperty.getEdgeValue(tlp.edge) -> int
%End

	void setNodeValue(const tlp::node n, const int v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.IntegerProperty.setNodeValue(tlp.node, @PYTHON_NODE_TYPE)
%End

	void setEdgeValue(const tlp::edge e, const int v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.IntegerProperty.setEdgeValue(tlp.edge, @PYTHON_EDGE_TYPE)
%End

	int __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = sipCpp->getNodeValue(*a0);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

	void __setitem__(const tlp::node n, const int v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

	int __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = sipCpp->getEdgeValue(*a0);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

	void __setitem__(const tlp::edge e, const int v) const;
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
	oss << "<Integer property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
	
	int  getNodeMin(tlp::Graph *sg=0);

	int  getNodeMax(tlp::Graph *sg=0);

	int  getEdgeMin(tlp::Graph *sg=0);

	int  getEdgeMax(tlp::Graph *sg=0);


	
};

};

namespace tlp {

class IntegerVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/IntegerProperty.h>
%End
public :

	IntegerVectorProperty (tlp::Graph *, std::string n="");
  	
 
	const std::vector<int> getNodeDefaultValue();
%Docstring
tlp.IntegerVectorProperty.getNodeDefaultValue() -> list of int
%End
	
	const std::vector<int> getEdgeDefaultValue();
%Docstring
tlp.IntegerVectorProperty.getEdgeDefaultValue() -> list of int
%End

	const std::vector<int> getNodeValue(const tlp::node n);
%Docstring
tlp.IntegerVectorProperty.getNodeValue(tlp.node) -> list of int
%End

	const std::vector<int> getEdgeValue(const tlp::edge e);
%Docstring
tlp.IntegerVectorProperty.getEdgeValue(tlp.edge) -> list of int
%End

	void setNodeValue(const tlp::node n, const std::vector<int> &v);
%Docstring
tlp.IntegerVectorProperty.setNodeValue(tlp.node, list of int)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<int> &v);
%Docstring
tlp.IntegerVectorProperty.setEdgeValue(tlp.edge, list of int)
%End

	void setAllNodeValue(const std::vector<int> &v);
%Docstring
tlp.IntegerVectorProperty.setAllNodeValue(list of int)
%End

	void setAllEdgeValue(const std::vector<int> &v);
%Docstring
tlp.IntegerVectorProperty.setAllEdgeValue(list of int)
%End
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const int &v);
 
 	const int& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const int& v);
 
	void popBackNodeEltValue(const tlp::node n);
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
	void resizeNodeValue(const tlp::node n, unsigned int size, int elt);
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const int &v);
  
	const int& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
	void pushBackEdgeEltValue(const tlp::edge e, const int &v);
  
	void popBackEdgeEltValue(const tlp::edge e);
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
	void resizeEdgeValue(const tlp::edge e, unsigned int size, int elt);
    
	std::vector<int> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<int>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<int> &v) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<int> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<int>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<int> &v) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Integer vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};