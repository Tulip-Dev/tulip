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

class LayoutProperty: tlp::PropertyInterface {
%TypeHeaderCode
#include <tulip/LayoutProperty.h>
%End
public:
	
	LayoutProperty (tlp::Graph *, std::string n="", bool updateOnEdgeReversal = true);

	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );

	tlp::Coord getMax(tlp::Graph *graph = 0);
	
	tlp::Coord getMin(tlp::Graph *graph = 0);

	void translate(const tlp::Vec3f& v, tlp::Graph *graph=0);
	
	void translate(const tlp::Vec3f& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void scale(const tlp::Vec3f& v, tlp::Graph *graph=0);
	
	void scale(const tlp::Vec3f& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateZ(const double& alpha, tlp::Graph *graph=0);

	void rotateX(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateY(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void rotateZ(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
	
	void center(tlp::Graph *graph = 0);
	
	void normalize(tlp::Graph *graph = 0);
	
	void perfectAspectRatio();
	
	void resetBoundingBox();

	double edgeLength(tlp::edge e);

	double averageAngularResolution(tlp::Graph *graph=0);

	double averageAngularResolution(const tlp::node n, tlp::Graph *graph=0);

	std::vector<double> angularResolutions(const tlp::node n, tlp::Graph *graph=0);

	unsigned int crossingNumber();

	void computeEmbedding(tlp::Graph *sg = 0);
  
  	void computeEmbedding(const tlp::node n, tlp::Graph *sg = 0);

	void reverseEdge(tlp::Graph *graph, const tlp::edge e);

	tlp::Coord getNodeDefaultValue();
	
	std::vector<tlp::Coord> getEdgeDefaultValue();
%Docstring
tlp.LayoutProperty.getEdgeDefaultValue() -> list of tlp.Coord
%End

	const tlp::Coord getNodeValue(const tlp::node n);

	const std::vector<tlp::Coord> getEdgeValue(const tlp::edge e);
%Docstring
tlp.LayoutProperty.getEdgeValue(tlp.edge) -> list of tlp.Coord
%End

	void setNodeValue(const tlp::node n, const tlp::Coord &v);

	void setEdgeValue(const tlp::edge e, const std::vector<tlp::Coord> &v);
%Docstring
tlp.LayoutProperty.setEdgeValue(tlp.edge, list of tlp.Coord) 
%End

	void setAllNodeValue(const tlp::Coord &v);

	void setAllEdgeValue(const std::vector<tlp::Coord> &v);
%Docstring
tlp.LayoutProperty.setAllEdgeValue(list of tlp.Coord) 
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
	
private:

	LayoutProperty(const tlp::LayoutProperty &);
	
};

class CoordVectorProperty: tlp::PropertyInterface {
%TypeHeaderCode
#include <tulip/LayoutProperty.h>
%End
public:
	
	CoordVectorProperty (tlp::Graph *, std::string n="");

	tlp::PropertyInterface* clonePrototype(tlp::Graph *, const std::string&);
	
	void copy( const tlp::node, const tlp::node, tlp::PropertyInterface * );
	
	void copy( const tlp::edge, const tlp::edge, tlp::PropertyInterface * );

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
	
	void setNodeEltValue(const tlp::node n, unsigned int i, const tlp::Coord &v);
 
 	const tlp::Coord& getNodeEltValue(const tlp::node n, unsigned int i) const;
  
  	void pushBackNodeEltValue(const tlp::node n, const tlp::Coord& v);
 
    void popBackNodeEltValue(const tlp::node n);
  
    void resizeNodeValue(const tlp::node n, unsigned int size, tlp::Coord elt = tlp::Coord());
  
    void setEdgeEltValue(const tlp::edge e, unsigned int i, const tlp::Coord &v);
  
    const tlp::Coord& getEdgeEltValue(const tlp::edge n, unsigned int i) const;
  
    void pushBackEdgeEltValue(const tlp::edge e, const tlp::Coord &v);
  
    void popBackEdgeEltValue(const tlp::edge e);
  
    void resizeEdgeValue(const tlp::edge e, unsigned int size, tlp::Coord elt = tlp::Coord());
    
private:

	CoordVectorProperty(const tlp::CoordVectorProperty &);
};

};
