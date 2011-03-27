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

class DoubleProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/DoubleProperty.h>
%End
public :
	
	DoubleProperty(tlp::Graph *g, std::string n="");
	
	void setAllNodeValue(const double v);
%Docstring
tlp.DoubleProperty.setAllNodeValue(double)
%End

	void setAllEdgeValue(const double v);
%Docstring
tlp.DoubleProperty.setAllEdgeValue(double)
%End
	
	double getNodeDefaultValue();
%Docstring
tlp.DoubleProperty.getNodeDefaultValue() -> double
%End	
	
	double getEdgeDefaultValue();
%Docstring
tlp.DoubleProperty.getEdgeDefaultValue() -> double
%End

	const double getNodeValue(const tlp::node n);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getNodeValue(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.DoubleProperty.getNodeValue(tlp.node) -> double
%End

	const double getEdgeValue(const tlp::edge e);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->getEdgeValue(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.DoubleProperty.getEdgeValue(tlp.edge) -> double
%End

	void setNodeValue(const tlp::node n, const double v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.DoubleProperty.setNodeValue(tlp.node, @PYTHON_NODE_TYPE)
%End

	void setEdgeValue(const tlp::edge e, const double v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.DoubleProperty.setEdgeValue(tlp.edge, @PYTHON_EDGE_TYPE)
%End

	double __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = sipCpp->getNodeValue(*a0);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

	void __setitem__(const tlp::node n, const double v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

	double __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = sipCpp->getEdgeValue(*a0);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

	void __setitem__(const tlp::edge e, const double v) const;
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
	oss << "<Double property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	

	double  getNodeMin(tlp::Graph *sg=0);
	
	double  getNodeMax(tlp::Graph *sg=0);
	
	double  getEdgeMin(tlp::Graph *sg=0);
	
	double  getEdgeMax(tlp::Graph *sg=0);
	
	void uniformQuantification(unsigned int);
	
	void nodesUniformQuantification(unsigned int);
	
	void edgesUniformQuantification(unsigned int);


	
	
};

};

namespace tlp {

class DoubleVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/DoubleProperty.h>
%End
public :

	DoubleVectorProperty (tlp::Graph *, std::string n="");
  	
 
	const std::vector<double> getNodeDefaultValue();
%Docstring
tlp.DoubleVectorProperty.getNodeDefaultValue() -> list of double
%End
	
	const std::vector<double> getEdgeDefaultValue();
%Docstring
tlp.DoubleVectorProperty.getEdgeDefaultValue() -> list of double
%End

	const std::vector<double> getNodeValue(const tlp::node n);
%Docstring
tlp.DoubleVectorProperty.getNodeValue(tlp.node) -> list of double
%End

	const std::vector<double> getEdgeValue(const tlp::edge e);
%Docstring
tlp.DoubleVectorProperty.getEdgeValue(tlp.edge) -> list of double
%End

	void setNodeValue(const tlp::node n, const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setNodeValue(tlp.node, list of double)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setEdgeValue(tlp.edge, list of double)
%End

	void setAllNodeValue(const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setAllNodeValue(list of double)
%End

	void setAllEdgeValue(const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setAllEdgeValue(list of double)
%End
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const double &v);
 
 	const double& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const double& v);
 
	void popBackNodeEltValue(const tlp::node n);
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
	void resizeNodeValue(const tlp::node n, unsigned int size, double elt);
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const double &v);
  
	const double& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
	void pushBackEdgeEltValue(const tlp::edge e, const double &v);
  
	void popBackEdgeEltValue(const tlp::edge e);
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
	void resizeEdgeValue(const tlp::edge e, unsigned int size, double elt);
    
	std::vector<double> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<double>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<double> &v) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<double> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<double>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<double> &v) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Double vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};