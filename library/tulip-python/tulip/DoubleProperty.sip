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
class DoubleProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/DoubleProperty.h>
%End
public :

	DoubleProperty (tlp::Graph *, std::string n="");
	
  	double getNodeMin(tlp::Graph *sg=0);
  	
  	double getNodeMax(tlp::Graph *sg=0);
  	
  	double getEdgeMin(tlp::Graph *sg=0);
  	
  	double getEdgeMax(tlp::Graph *sg=0);
  	
  	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string& );
  	
  	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
  	
  	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
  	
  	void uniformQuantification(unsigned int);

	double getNodeDefaultValue();
	
	double getEdgeDefaultValue();

	const double getNodeValue(const tlp::node n);

	const double getEdgeValue(const tlp::edge e);

	void setNodeValue(const tlp::node n, const double &v);

	void setEdgeValue(const tlp::edge e, const double &v);

	void setAllNodeValue(const double &v);

	void setAllEdgeValue(const double &v);	
	
	virtual void erase(const tlp::node) ;
	
	virtual void erase(const tlp::edge) ;

	virtual std::string getTypename() const ;

	const std::string& getName() const;

	virtual std::string getNodeDefaultStringValue() const ;
	
	virtual std::string getEdgeDefaultStringValue() const ;
	
	virtual std::string getNodeStringValue( const tlp::node n ) const ;
	
	virtual std::string getEdgeStringValue( const tlp::edge e ) const ;
	
	virtual bool setNodeStringValue( const tlp::node n, const std::string & v ) ;
	
	virtual bool setEdgeStringValue( const tlp::edge e, const std::string & v ) ;
	
	virtual bool setAllNodeStringValue( const std::string & v ) ;
	
	virtual bool setAllEdgeStringValue( const std::string & v ) ;
	
	virtual tlp::Iterator<tlp::node>* getNonDefaultValuatedNodes(const tlp::Graph* = NULL) const ;
	
	virtual tlp::Iterator<tlp::edge>* getNonDefaultValuatedEdges(const tlp::Graph* = NULL) const ;
	
  	virtual void copy(tlp::node, tlp::node, tlp::PropertyInterface*, bool) ;
	
	virtual void copy(tlp::edge, tlp::edge, tlp::PropertyInterface*, bool) ;
	
	double __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = sipCpp->getNodeValue(*a0);
%End

	void __setitem__(const tlp::node n, const double d) const;
%MethodCode
     sipCpp->setNodeValue(*a0, a1);
%End

	double __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = sipCpp->getEdgeValue(*a0);
%End

	void __setitem__(const tlp::edge e, const double d) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<double property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};

class DoubleVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/DoubleProperty.h>
%End
public :

	DoubleVectorProperty (tlp::Graph *, std::string n="");
  	
  	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string& );
  	
  	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
  	
  	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );

	const std::vector<double> getNodeDefaultValue();
%Docstring
tlp.DoubleVectorProperty.getNodeDefaultValue() -> list of float
%End
	
	const std::vector<double> getEdgeDefaultValue();
%Docstring
tlp.DoubleVectorProperty.getEdgeDefaultValue() -> list of float
%End

	const std::vector<double> getNodeValue(const tlp::node n);
%Docstring
tlp.DoubleVectorProperty.getNodeValue(tlp.node) -> list of float
%End

	const std::vector<double> getEdgeValue(const tlp::edge e);
%Docstring
tlp.DoubleVectorProperty.getEdgeValue(tlp.edge) -> list of float
%End

	void setNodeValue(const tlp::node n, const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setNodeValue(tlp.node, list of float)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setEdgeValue(tlp.edge, list of float)
%End

	void setAllNodeValue(const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setAllNodeValue(list of float)
%End

	void setAllEdgeValue(const std::vector<double> &v);
%Docstring
tlp.DoubleVectorProperty.setAllEdgeValue(list of float)
%End
	
	virtual void erase(const tlp::node) ;
	
	virtual void erase(const tlp::edge) ;

	virtual std::string getTypename() const ;

	const std::string& getName() const;

	virtual std::string getNodeDefaultStringValue() const ;
	
	virtual std::string getEdgeDefaultStringValue() const ;
	
	virtual std::string getNodeStringValue( const tlp::node n ) const ;
	
	virtual std::string getEdgeStringValue( const tlp::edge e ) const ;
	
	virtual bool setNodeStringValue( const tlp::node n, const std::string & v ) ;
	
	virtual bool setEdgeStringValue( const tlp::edge e, const std::string & v ) ;
	
	virtual bool setAllNodeStringValue( const std::string & v ) ;
	
	virtual bool setAllEdgeStringValue( const std::string & v ) ;
	
	virtual tlp::Iterator<tlp::node>* getNonDefaultValuatedNodes(const tlp::Graph* = NULL) const ;
	
	virtual tlp::Iterator<tlp::edge>* getNonDefaultValuatedEdges(const tlp::Graph* = NULL) const ;
	
  	virtual void copy(tlp::node, tlp::node, tlp::PropertyInterface*, bool) ;
	
	virtual void copy(tlp::edge, tlp::edge, tlp::PropertyInterface*, bool) ;
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const double &v);
 
 	const double& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const double& v);
 
    void popBackNodeEltValue(const tlp::node n);
  
    void resizeNodeValue(const tlp::node n, unsigned int size, double elt = 0);
  
    void setEdgeEltValue(const tlp::edge e, unsigned int i, const double &v);
  
    const double& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
    void pushBackEdgeEltValue(const tlp::edge e, const double &v);
  
    void popBackEdgeEltValue(const tlp::edge e);
  
    void resizeEdgeValue(const tlp::edge e, unsigned int size, double elt = 0);
    
    std::vector<double> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<double>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<double> &vd) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<double> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<double>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<double> &vd) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<double vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
	
};
};
