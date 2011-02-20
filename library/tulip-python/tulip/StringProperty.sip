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

namespace tlp {

class StringProperty :  tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/StringProperty.h>
%End
public :
	StringProperty (tlp::Graph *g, std::string n=""); 
	
	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
	
	std::string getNodeDefaultValue();
	
	std::string getEdgeDefaultValue();

	const std::string getNodeValue(const tlp::node n);

	const std::string getEdgeValue(const tlp::edge e);

	void setNodeValue(const tlp::node n, const std::string &v);

	void setEdgeValue(const tlp::edge e, const std::string &v);

	void setAllNodeValue(const std::string &v);

	void setAllEdgeValue(const std::string &v);
	
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
	
	std::string __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::string(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::string &s) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::string __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::string(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::string &s) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<string property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
};

class StringVectorProperty :  tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/StringProperty.h>
%End
public :
	StringVectorProperty (tlp::Graph *g, std::string n=""); 
	
	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
	
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
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const std::string &v);
 
 	const std::string& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const std::string& v);
 
    void popBackNodeEltValue(const tlp::node n);
  
    void resizeNodeValue(const tlp::node n, unsigned int size, std::string elt = "");
  
    void setEdgeEltValue(const tlp::edge e, unsigned int i, const std::string &v);
  
    const std::string& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
    void pushBackEdgeEltValue(const tlp::edge e, const std::string &v);
  
    void popBackEdgeEltValue(const tlp::edge e);
  
    void resizeEdgeValue(const tlp::edge e, unsigned int size, std::string elt = 0);
    
    std::vector<std::string> __getitem__(const tlp::node n) const;
%MethodCode
     sipRes = new std::vector<std::string>(sipCpp->getNodeValue(*a0));
%End

	void __setitem__(const tlp::node n, const std::vector<std::string> &s) const;
%MethodCode
     sipCpp->setNodeValue(*a0, *a1);
%End

	std::vector<std::string> __getitem__(const tlp::edge e) const;
%MethodCode
     sipRes = new std::vector<std::string>(sipCpp->getEdgeValue(*a0));
%End

	void __setitem__(const tlp::edge e, const std::vector<std::string> &s) const;
%MethodCode
     sipCpp->setEdgeValue(*a0, *a1);
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::ostringstream oss;
	oss << "<string vector property " << sipCpp->getName() << ">";
	std::string s = oss.str();
	sipRes = PyString_FromString(s.c_str());
%End
};
};
