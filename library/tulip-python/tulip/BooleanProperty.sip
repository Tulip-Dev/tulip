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
// | Improved and maintained by Antoine Lambert 			                 |
// | Contact:  antoine.lambert@labri.fr 								     |
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
class BooleanProperty : tlp:: PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/BooleanProperty.h>
%End
public :
	BooleanProperty (tlp::Graph *g, std::string n="");
	
	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
	
	void reverse();
	
	///reverse all the direction of edges of the visible graph which are true in this BooleanProperty
	void reverseEdgeDirection();
	
	tlp::Iterator<tlp::node> *getNodesEqualTo(const bool val, tlp::Graph * = 0);
	
	tlp::Iterator<tlp::edge> *getEdgesEqualTo(const bool val, tlp::Graph * = 0);
	
	bool getNodeDefaultValue();
	
	bool getEdgeDefaultValue();

	const bool getNodeValue(const tlp::node n);

	const bool getEdgeValue(const tlp::edge e);

	void setNodeValue(const tlp::node n, const bool &v);

	void setEdgeValue(const tlp::edge e, const bool &v);

	void setAllNodeValue(const bool &v);

	void setAllEdgeValue(const bool &v);
	
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
};

class BooleanVectorProperty : tlp:: PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/BooleanProperty.h>
%End
public :
	BooleanVectorProperty (tlp::Graph *g, std::string n="");
	
	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
	
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
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const bool &v);
 
 	const bool& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const bool& v);
 
    	void popBackNodeEltValue(const tlp::node n);
  
    	void resizeNodeValue(const tlp::node n, unsigned int size, bool elt = false);
  
 	void setEdgeEltValue(const tlp::edge e, unsigned int i, const bool &v);
  
    	const bool& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
    	void pushBackEdgeEltValue(const tlp::edge e, const bool &v);
  
    	void popBackEdgeEltValue(const tlp::edge e);
  
    	void resizeEdgeValue(const tlp::edge e, unsigned int size, bool elt = false);
};
};


