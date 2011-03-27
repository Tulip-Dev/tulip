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

class SizeProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/SizeProperty.h>
%End
public :
	
	SizeProperty(tlp::Graph *g, std::string n="");
	
	void setAllNodeValue(const tlp::Size &v);
%Docstring
tlp.SizeProperty.setAllNodeValue(tlp.Size)
%End

	void setAllEdgeValue(const tlp::Size &v);
%Docstring
tlp.SizeProperty.setAllEdgeValue(tlp.Size)
%End
	
	tlp::Size getNodeDefaultValue();
%Docstring
tlp.SizeProperty.getNodeDefaultValue() -> tlp.Size
%End	
	
	tlp::Size getEdgeDefaultValue();
%Docstring
tlp.SizeProperty.getEdgeDefaultValue() -> tlp.Size
%End	

	const tlp::Size getNodeValue(const tlp::node n);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new tlp::Size(sipCpp->getNodeValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.SizeProperty.getNodeValue(tlp.node) -> tlp.Size
%End

	const tlp::Size getEdgeValue(const tlp::edge e);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new tlp::Size(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.SizeProperty.getEdgeValue(tlp.edge) -> tlp.Size
%End

	void setNodeValue(const tlp::node n, const tlp::Size &v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.SizeProperty.setNodeValue(tlp.node, tlp.Size)
%End

	void setEdgeValue(const tlp::edge e, const tlp::Size &v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.SizeProperty.setEdgeValue(tlp.edge, tlp.Size)
%End

	tlp::Size __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new tlp::Size(sipCpp->getNodeValue(*a0));
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

	void __setitem__(const tlp::node n, const tlp::Size &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

	tlp::Size __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new tlp::Size(sipCpp->getEdgeValue(*a0));
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

	void __setitem__(const tlp::edge e, const tlp::Size &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setEdgeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    } 
%End
	

	tlp::Size getMax(tlp::Graph *sg=0);
	
	tlp::Size getMin(tlp::Graph *sg=0);
	
	void scale(const tlp::Size&, tlp::Graph *sg=0);
	
	void scale(const tlp::Size&, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);



	
};

};


namespace tlp {

class SizeVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/SizeProperty.h>
%End
public :

	SizeVectorProperty (tlp::Graph *, std::string n="");
  	
 
	const std::vector<tlp::Size> getNodeDefaultValue();
%Docstring
tlp.SizeVectorProperty.getNodeDefaultValue() -> list of tlp.Size
%End
	
	const std::vector<tlp::Size> getEdgeDefaultValue();
%Docstring
tlp.SizeVectorProperty.getEdgeDefaultValue() -> list of tlp.Size
%End

	const std::vector<tlp::Size> getNodeValue(const tlp::node n);
%Docstring
tlp.SizeVectorProperty.getNodeValue(tlp.node) -> list of tlp.Size
%End

	const std::vector<tlp::Size> getEdgeValue(const tlp::edge e);
%Docstring
tlp.SizeVectorProperty.getEdgeValue(tlp.edge) -> list of tlp.Size
%End

	void setNodeValue(const tlp::node n, const std::vector<tlp::Size> &v);
%Docstring
tlp.SizeVectorProperty.setNodeValue(tlp.node, list of tlp.Size)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<tlp::Size> &v);
%Docstring
tlp.SizeVectorProperty.setEdgeValue(tlp.edge, list of tlp.Size)
%End

	void setAllNodeValue(const std::vector<tlp::Size> &v);
%Docstring
tlp.SizeVectorProperty.setAllNodeValue(list of tlp.Size)
%End

	void setAllEdgeValue(const std::vector<tlp::Size> &v);
%Docstring
tlp.SizeVectorProperty.setAllEdgeValue(list of tlp.Size)
%End
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const tlp::Size &v);
 
 	const tlp::Size& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const tlp::Size& v);
 
	void popBackNodeEltValue(const tlp::node n);
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
	void resizeNodeValue(const tlp::node n, unsigned int size, tlp::Size elt);
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const tlp::Size &v);
  
	const tlp::Size& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
	void pushBackEdgeEltValue(const tlp::edge e, const tlp::Size &v);
  
	void popBackEdgeEltValue(const tlp::edge e);
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
	void resizeEdgeValue(const tlp::edge e, unsigned int size, tlp::Size elt);
    
	std::vector<tlp::Size> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<tlp::Size>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<tlp::Size> &v) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<tlp::Size> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<tlp::Size>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<tlp::Size> &v) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Size vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};