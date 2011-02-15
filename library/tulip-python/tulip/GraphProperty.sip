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

class GraphProperty  /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/GraphProperty.h>
%End
public:

	GraphProperty(tlp::Graph *, std::string n="");
	
	virtual ~GraphProperty();
		
	tlp::Graph* getNodeDefaultValue();
	
	std::set<tlp::edge> getEdgeDefaultValue();
%Docstring
tlp.GraphProperty.getEdgeDefaultValue() -> list of tlp.edge
%End

	tlp::Graph* getNodeValue(const tlp::node n);

	const std::set<tlp::edge> getEdgeValue(const tlp::edge e);
%Docstring
tlp.GraphProperty.getEdgeValue(tlp.edge) -> list of tlp.edge
%End	

	void setNodeValue(const tlp::node n, tlp::Graph* v);

	void setEdgeValue(const tlp::edge e, const std::set<tlp::edge> &v);
%Docstring
tlp.GraphProperty.setEdgeValue(tlp.edge, list of tlp.edge)
%End
	void setAllNodeValue(tlp::Graph* v);

	void setAllEdgeValue(const std::set<tlp::edge> &v);
%Docstring
tlp.GraphProperty.setAllEdgeValue(list of tlp.edge)
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
	
};
};
