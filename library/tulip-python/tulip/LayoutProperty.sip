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

class LayoutProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/LayoutProperty.h>
%End
public :
	
	LayoutProperty(tlp::Graph *g, std::string n="");
	
	void setAllNodeValue(const tlp::Coord &v);
%Docstring
tlp.LayoutProperty.setAllNodeValue(tlp.Coord)
%End

	void setAllEdgeValue(const std::vector<tlp::Coord> &v);
%Docstring
tlp.LayoutProperty.setAllEdgeValue(list of tlp.Coord)
%End
	
	tlp::Coord getNodeDefaultValue();
%Docstring
tlp.LayoutProperty.getNodeDefaultValue() -> tlp.Coord
%End	
	
	std::vector<tlp::Coord> getEdgeDefaultValue();
%Docstring
tlp.LayoutProperty.getEdgeDefaultValue() -> list of tlp.Coord
%End	

	const tlp::Coord getNodeValue(const tlp::node n);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new tlp::Coord(sipCpp->getNodeValue(*a0));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.LayoutProperty.getNodeValue(tlp.node) -> tlp.Coord
%End

	const std::vector<tlp::Coord> getEdgeValue(const tlp::edge e);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = new std::vector<tlp::Coord>(sipCpp->getEdgeValue(*a0));
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.LayoutProperty.getEdgeValue(tlp.edge) -> list of tlp.Coord
%End

	void setNodeValue(const tlp::node n, const tlp::Coord &v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setNodeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.LayoutProperty.setNodeValue(tlp.node, tlp.Coord)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<tlp::Coord> &v);
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipCpp->setEdgeValue(*a0, *a1);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

%Docstring
tlp.LayoutProperty.setEdgeValue(tlp.edge, list of tlp.Coord)
%End

	tlp::Coord __getitem__(const tlp::node n) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new tlp::Coord(sipCpp->getNodeValue(*a0));
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    }
%End

	void __setitem__(const tlp::node n, const tlp::Coord &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setNodeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
    } 
%End

	std::vector<tlp::Coord> __getitem__(const tlp::edge e) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipRes = new std::vector<tlp::Coord>(sipCpp->getEdgeValue(*a0));
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    }
     
%End

	void __setitem__(const tlp::edge e, const std::vector<tlp::Coord> &v) const;
%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {	
     	sipCpp->setEdgeValue(*a0, *a1);
    } else {
    	sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
    } 
%End
	

	tlp::Coord getMax(tlp::Graph *graph=0);
	
	tlp::Coord getMin(tlp::Graph *graph=0);

	void translate(const tlp::Coord& v, tlp::Graph *graph=0);
 
	void translate(const tlp::Coord& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void scale(const tlp::Coord& v, tlp::Graph *graph=0);
	
	void scale(const tlp::Coord& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
  
	void rotateZ(const double& alpha, tlp::Graph *graph=0);

	void rotateX(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateY(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateZ(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void center(tlp::Graph *graph = 0);
	
	void normalize(tlp::Graph *graph = 0);
	
	void perfectAspectRatio();
	
	void resetBoundingBox();
  
	double edgeLength(const tlp::edge e) const;
  
	double averageAngularResolution(const tlp::Graph *graph = 0) const;
  
  	double averageAngularResolution(const tlp::node n, const tlp::Graph *graph = 0) const;
  
  	std::vector<double> angularResolutions(const tlp::node n, const tlp::Graph *graph = 0) const;

	void computeEmbedding(tlp::Graph *sg = 0);
  
  	void computeEmbedding(const tlp::node n, tlp::Graph *sg = 0);
  
  	unsigned int crossingNumber() const;
  	
  	
	
};

};


namespace tlp {

class CoordVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/LayoutProperty.h>
%End
public :

	CoordVectorProperty (tlp::Graph *, std::string n="");
  	
 
	const std::vector<tlp::Coord> getNodeDefaultValue();
%Docstring
tlp.CoordVectorProperty.getNodeDefaultValue() -> list of tlp.Coord
%End
	
	const std::vector<tlp::Coord> getEdgeDefaultValue();
%Docstring
tlp.CoordVectorProperty.getEdgeDefaultValue() -> list of tlp.Coord
%End

	const std::vector<tlp::Coord> getNodeValue(const tlp::node n);
%Docstring
tlp.CoordVectorProperty.getNodeValue(tlp.node) -> list of tlp.Coord
%End

	const std::vector<tlp::Coord> getEdgeValue(const tlp::edge e);
%Docstring
tlp.CoordVectorProperty.getEdgeValue(tlp.edge) -> list of tlp.Coord
%End

	void setNodeValue(const tlp::node n, const std::vector<tlp::Coord> &v);
%Docstring
tlp.CoordVectorProperty.setNodeValue(tlp.node, list of tlp.Coord)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<tlp::Coord> &v);
%Docstring
tlp.CoordVectorProperty.setEdgeValue(tlp.edge, list of tlp.Coord)
%End

	void setAllNodeValue(const std::vector<tlp::Coord> &v);
%Docstring
tlp.CoordVectorProperty.setAllNodeValue(list of tlp.Coord)
%End

	void setAllEdgeValue(const std::vector<tlp::Coord> &v);
%Docstring
tlp.CoordVectorProperty.setAllEdgeValue(list of tlp.Coord)
%End
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const tlp::Coord &v);
 
 	const tlp::Coord& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const tlp::Coord& v);
 
	void popBackNodeEltValue(const tlp::node n);
	
	void resizeNodeValue(const tlp::node n, unsigned int size);
	void resizeNodeValue(const tlp::node n, unsigned int size, tlp::Coord elt);
	
	void setEdgeEltValue(const tlp::edge e, unsigned int i, const tlp::Coord &v);
  
	const tlp::Coord& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
	void pushBackEdgeEltValue(const tlp::edge e, const tlp::Coord &v);
  
	void popBackEdgeEltValue(const tlp::edge e);
  
  	void resizeEdgeValue(const tlp::edge e, unsigned int size);
	void resizeEdgeValue(const tlp::edge e, unsigned int size, tlp::Coord elt);
    
	std::vector<tlp::Coord> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<tlp::Coord>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<tlp::Coord> &v) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<tlp::Coord> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<tlp::Coord>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<tlp::Coord> &v) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<Coord vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

};