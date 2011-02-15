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

class ColorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/ColorProperty.h>
%End
public :
	
	ColorProperty (tlp::Graph *g, std::string n="");
	
	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
	
	tlp::Color getNodeDefaultValue();
	
	tlp::Color getEdgeDefaultValue();

	const tlp::Color getNodeValue(const tlp::node n);

	const tlp::Color getEdgeValue(const tlp::edge e);

	void setNodeValue(const tlp::node n, const tlp::Color &v);

	void setEdgeValue(const tlp::edge e, const tlp::Color &v);

	void setAllNodeValue(const tlp::Color &v);

	void setAllEdgeValue(const tlp::Color &v);
	
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

class ColorVectorProperty : tlp::PropertyInterface /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/ColorProperty.h>
%End
public :
	
	ColorVectorProperty (tlp::Graph *g, std::string n="");
	
	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );
	
	const std::vector<tlp::Color> getNodeDefaultValue();
%Docstring
tlp.ColorVectorProperty.getNodeDefaultValue() -> list of tlp.Color
%End
	
	const std::vector<tlp::Color> getEdgeDefaultValue();
%Docstring
tlp.ColorVectorProperty.getEdgeDefaultValue() -> list of tlp.Color
%End

	const std::vector<tlp::Color> getNodeValue(const tlp::node n);
%Docstring
tlp.ColorVectorProperty.getNodeValue(tlp.node) -> list of tlp.Color
%End

	const std::vector<tlp::Color> getEdgeValue(const tlp::edge e);
%Docstring
tlp.ColorVectorProperty.getEdgeValue(tlp.edge) -> list of tlp.Color
%End

	void setNodeValue(const tlp::node n, const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setNodeValue(tlp.node, list of tlp.Color)
%End

	void setEdgeValue(const tlp::edge e, const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setEdgeValue(tlp.edge, list of tlp.Color)
%End

	void setAllNodeValue(const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setAllNodeValue(list of tlp.Color)
%End

	void setAllEdgeValue(const std::vector<tlp::Color> &v);
%Docstring
tlp.ColorVectorProperty.setAllEdgeValue(list of tlp.Color)
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
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const tlp::Color &v);
 
 	const tlp::Color& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const tlp::Color& v);
 
    void popBackNodeEltValue(const tlp::node n);
  
    void resizeNodeValue(const tlp::node n, unsigned int size, tlp::Color elt = tlp::Color());
  
    void setEdgeEltValue(const tlp::edge e, unsigned int i, const tlp::Color &v);
  
    const tlp::Color& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
    void pushBackEdgeEltValue(const tlp::edge e, const tlp::Color &v);
  
    void popBackEdgeEltValue(const tlp::edge e);
  
    void resizeEdgeValue(const tlp::edge e, unsigned int size, tlp::Color elt = tlp::Color());
};
};
