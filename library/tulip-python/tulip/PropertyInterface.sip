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

class PropertyInterface /Abstract/ {
%TypeHeaderCode
#include <tulip/PropertyInterface.h>
%End
public:
	virtual void erase(const tlp::node) = 0;
	
	virtual void erase(const tlp::edge) = 0;
	
	virtual tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&) = 0;

	virtual std::string getTypename() const = 0;

	const std::string& getName() const;

	virtual std::string getNodeDefaultStringValue() const = 0;
	
	virtual std::string getEdgeDefaultStringValue() const = 0;
	
	virtual std::string getNodeStringValue( const tlp::node n ) const = 0;
	
	virtual std::string getEdgeStringValue( const tlp::edge e ) const = 0;
	
	virtual bool setNodeStringValue( const tlp::node n, const std::string & v ) = 0;
	
	virtual bool setEdgeStringValue( const tlp::edge e, const std::string & v ) = 0;
	
	virtual bool setAllNodeStringValue( const std::string & v ) = 0;
	
	virtual bool setAllEdgeStringValue( const std::string & v ) = 0;
	
	virtual tlp::Iterator<tlp::node>* getNonDefaultValuatedNodes(const tlp::Graph* = NULL) const = 0;
	
	virtual tlp::Iterator<tlp::edge>* getNonDefaultValuatedEdges(const tlp::Graph* = NULL) const = 0;
	
  	virtual void copy(tlp::node, tlp::node, tlp::PropertyInterface*, bool) = 0;
	
	virtual void copy(tlp::edge, tlp::edge, tlp::PropertyInterface*, bool) = 0;
		
  	
};

};
