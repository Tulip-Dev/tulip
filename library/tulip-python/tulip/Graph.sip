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

%ModuleHeaderCode
#include <tulip/StableIterator.h>
namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;
};
%End

namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;

enum ElementType {NODE=0, EDGE};

class Graph /Abstract/ {
%TypeHeaderCode
#include <tulip/Graph.h>
%End

public:
	Graph();

	void clear();

	tlp::Graph *addSubGraph(tlp::BooleanProperty *selection=0);

	tlp::Graph *inducedSubGraph(const std::set<tlp::node>& nodeSet);
%Docstring
tlp.Graph.inducedSubGraph(list of tlp.node) -> tlp.Graph
%End

	void delSubGraph(tlp::Graph *);

	void delAllSubGraphs(tlp::Graph *);

	tlp::Graph* getSuperGraph()const ;
	
	tlp::Graph* getRoot() const;

	void setSuperGraph(tlp::Graph *);
	
	tlp::IteratorGraph *getSubGraphs() const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::Graph *>(sipCpp->getSubGraphs());
%End		

	bool isSubGraph(tlp::Graph* sg) const;

	bool isDescendantGraph(tlp::Graph* sg) const;

	tlp::Graph* getSubGraph(unsigned int id) const;

	tlp::Graph* getDescendantGraph(unsigned int id) const;

	tlp::node addNode();

	void addNode(const tlp::node);

	void delNode(const tlp::node);

	void delAllNode(const tlp::node);

	tlp::edge addEdge(const tlp::node, const tlp::node );

	void addEdge(const tlp::edge );

	 void delEdge(const tlp::edge );

	 void delAllEdge(const tlp::edge);

	void setEdgeOrder(const tlp::node,const std::vector<tlp::edge> &);
%Docstring
tlp.Graph.setEdgeOrder(tlp.node, list of tlp.edge)
%End

	void swapEdgeOrder(const tlp::node,const tlp::edge , const tlp::edge );

	void reverse(const tlp::edge);

	tlp::node getOneNode() const ;

	tlp::Iterator<tlp::node>* getNodes() const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getNodes());
%End	

	tlp::node getInNode(const tlp::node,unsigned int )const ;

	tlp::Iterator<tlp::node>* getInNodes(const tlp::node) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getInNodes(*a0));
%End		

	tlp::node getOutNode(const tlp::node,unsigned int) const ;

	tlp::Iterator<tlp::node>* getOutNodes(const tlp::node) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getOutNodes(*a0));
%End			

	tlp::Iterator<tlp::node>* getInOutNodes(const tlp::node) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getInOutNodes(*a0));
%End			

	tlp::Graph* getNodeMetaInfo(const tlp::node) const;

	tlp::Iterator<tlp::edge>* getEdges() const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getEdges());
%End		

	tlp::edge getOneEdge() const ;

	tlp::Iterator<tlp::edge>* getOutEdges(const tlp::node) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getOutEdges(*a0));
%End		

	tlp::Iterator<tlp::edge>* getInOutEdges(const tlp::node) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getInOutEdges(*a0));
%End			

	tlp::Iterator<tlp::edge>* getInEdges(const tlp::node) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getInEdges(*a0));
%End		

	tlp::Iterator<tlp::edge>* getEdgeMetaInfo(const tlp::edge) const;
%MethodCode
	sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getEdgeMetaInfo(*a0));
%End		
	unsigned int getId() const;

	unsigned int numberOfNodes()const ;

	unsigned int numberOfEdges()const ;

	unsigned int deg(const tlp::node)const ;

	unsigned int indeg(const tlp::node)const ;

	unsigned int outdeg(const tlp::node)const ;

	tlp::node source(const tlp::edge)const ;

	tlp::node target(const tlp::edge)const ;

	tlp::node opposite(const tlp::edge, const tlp::node)const ;

	bool isElement(const tlp::node ) const ;

	bool isMetaNode(const tlp::node ) const ;

	bool isElement(const tlp::edge ) const ;

	bool isMetaEdge(const tlp::edge ) const ;

	tlp::edge existEdge(const tlp::node , const tlp::node) const ;
	
	const tlp::DataSet & getAttributes();
	 
	std::string getStringAttribute(const std::string &name);
%MethodCode
	std::string s;
	sipCpp->getAttribute(*a0, s);
	sipRes = new std::string(s);
%End

	
	void setStringAttribute(const std::string &name, const std::string &value);
%MethodCode
	sipCpp->setAttribute<std::string>(*a0, *a1);
%End			 
	 
	void removeAttribute(const std::string &name);

	bool attributeExist(const std::string &name);

	tlp::BooleanProperty *getBooleanProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::BooleanProperty>(*a0);
%End
	tlp::BooleanProperty *getLocalBooleanProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::BooleanProperty>(*a0);
%End

	tlp::BooleanVectorProperty *getBooleanVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::BooleanVectorProperty>(*a0);
%End
	tlp::BooleanVectorProperty *getLocalBooleanVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::BooleanVectorProperty>(*a0);
%End

	tlp::LayoutProperty *getLayoutProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::LayoutProperty>(*a0);
%End
	tlp::LayoutProperty *getLocalLayoutProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::LayoutProperty>(*a0);
%End

tlp::CoordVectorProperty *getCoordVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::CoordVectorProperty>(*a0);
%End
	tlp::CoordVectorProperty *getLocalCoordVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::CoordVectorProperty>(*a0);
%End

	tlp::ColorProperty *getColorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::ColorProperty>(*a0);
%End
	tlp::ColorProperty *getLocalColorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::ColorProperty>(*a0);
%End

	tlp::ColorVectorProperty *getColorVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::ColorVectorProperty>(*a0);
%End
	tlp::ColorVectorProperty *getLocalColorVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::ColorVectorProperty>(*a0);
%End

	tlp::DoubleProperty *getDoubleProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::DoubleProperty>(*a0);
%End
	tlp::DoubleProperty *getLocalDoubleProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::DoubleProperty>(*a0);
%End

	tlp::DoubleVectorProperty *getDoubleVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::DoubleVectorProperty>(*a0);
%End
	tlp::DoubleVectorProperty *getLocalDoubleVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::DoubleVectorProperty>(*a0);
%End

	tlp::StringProperty *getStringProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::StringProperty>(*a0);
%End
	tlp::StringProperty *getLocalStringProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::StringProperty>(*a0);
%End

	tlp::StringVectorProperty *getStringVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::StringVectorProperty>(*a0);
%End
	tlp::StringVectorProperty *getLocalStringVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::StringVectorProperty>(*a0);
%End

	tlp::SizeProperty *getSizeProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::SizeProperty>(*a0);
%End

	tlp::SizeProperty *getLocalSizeProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::SizeProperty>(*a0);
%End

	tlp::SizeVectorProperty *getSizeVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::SizeVectorProperty>(*a0);
%End

	tlp::SizeVectorProperty *getLocalSizeVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::SizeVectorProperty>(*a0);
%End

	tlp::IntegerProperty *getIntegerProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::IntegerProperty>(*a0);
%End
	tlp::IntegerProperty *getLocalIntegerProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::IntegerProperty>(*a0);
%End

	tlp::IntegerVectorProperty *getIntegerVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::IntegerVectorProperty>(*a0);
%End
	tlp::IntegerVectorProperty *getLocalIntegerVectorProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::IntegerVectorProperty>(*a0);
%End

	tlp::GraphProperty *getGraphProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getProperty<tlp::GraphProperty>(*a0);
%End

	tlp::GraphProperty *getLocalGraphProperty(const std::string &name);
%MethodCode
	sipRes = sipCpp->getLocalProperty<tlp::GraphProperty>(*a0);
%End

	tlp::PropertyInterface* getProperty(const std::string& name);

	 bool existProperty(const std::string& name);

	 bool existLocalProperty(const std::string& name);

	 void delLocalProperty(const std::string& name);

	tlp::Iterator<std::string>* getLocalProperties();
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getLocalProperties());
%End		

	tlp::Iterator<std::string>* getInheritedProperties();
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getInheritedProperties());
%End		

	tlp::Iterator<std::string>* getProperties();
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getProperties());
%End			
	

	bool computeIntegerProperty(const std::string &algorithm, tlp::IntegerProperty* result, tlp::DataSet *data=0);
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::IntegerProperty>(*a0, a1, s, NULL, a2);
%End

	bool computeDoubleProperty(const std::string &algorithm, tlp::DoubleProperty* result, tlp::DataSet *data=0);
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::DoubleProperty>(*a0, a1, s, NULL, a2);
%End

	bool computeLayoutProperty(const std::string &algorithm, tlp::LayoutProperty* result, tlp::DataSet *data=0);
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::LayoutProperty>(*a0, a1, s, NULL, a2);
%End

	bool computeBooleanProperty(const std::string &algorithm, tlp::BooleanProperty* result, tlp::DataSet *data=0);
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::BooleanProperty>(*a0, a1, s, NULL, a2);
%End

	bool computeSizeProperty(const std::string &algorithm, tlp::SizeProperty* result, tlp::DataSet *data=0);
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::SizeProperty>(*a0, a1, s, NULL, a2);
%End

bool computeColorProperty(const std::string &algorithm, tlp::ColorProperty* result, tlp::DataSet *data=0);
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::ColorProperty>(*a0, a1, s, NULL, a2);
%End


	// updates management
	void push(bool unpopAllowed = true);
	void pop(bool unpopAllowed = true);
	
	void unpop();
	bool canPop();
	bool canUnpop();

	tlp::node createMetaNode(const std::set<tlp::node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(list of tlp.node, bool multiEdges=True, bool delAllEdge=True) -> tlp.node
%End

	void createMetaNodes(tlp::IteratorGraph *itS, tlp::Graph *quotientGraph, std::vector<tlp::node>& metaNodes /Out/);
%Docstring
tlp.Graph.createMetaNodes(tlp.IteratorGraph, tlp.Graph) -> list of tlp.node
%End
		
	tlp::node createMetaNode(tlp::Graph* subGraph, bool multiEdges = true, bool delAllEdge = true);

	void openMetaNode(tlp::node n);
};

};

namespace tlp {

tlp::Graph *newGraph();
tlp::Graph *loadGraph(const std::string &filename);
bool saveGraph(tlp::Graph *, const std::string &filename);
tlp::Graph *importGraph(const std::string &alg, tlp::DataSet &dataSet, tlp::Graph *newGraph=0);
%MethodCode
	sipRes = tlp::importGraph(*a0, *a1, 0, a2);
%End

bool applyAlgorithm(tlp::Graph *graph, std::string &errMsg /Out/, tlp::DataSet *dataSet =0, const std::string &alg="any");

tlp::Graph *newSubGraph(tlp::Graph *, std::string name = "unnamed");
tlp::Graph *newCloneSubGraph(tlp::Graph *, std::string name = "unnamed");
bool getSource(tlp::Graph *, tlp::node &n);
void copyToGraph(tlp::Graph *outG, tlp::Graph *inG, tlp::BooleanProperty* inSelection=0, tlp::BooleanProperty* outSelection=0 );
void removeFromGraph(tlp::Graph *ioG, tlp::BooleanProperty *inSelection = 0);


};

