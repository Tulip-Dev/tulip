%ModuleHeaderCode
#include <tulip/GraphMeasure.h>
%End

namespace tlp {

  	enum EDGE_TYPE {DIRECTED = 0, INV_DIRECTED = 1, UNDIRECTED = 2};

	double averagePathLength(const tlp::Graph* g, tlp::PluginProgress* = NULL);
	
  	double averageClusteringCoefficient(const tlp::Graph *, tlp::PluginProgress * = 0);
  
  	void clusteringCoefficient(const tlp::Graph *g, tlp::DoubleProperty *result, unsigned int maxDepth = 1, tlp::PluginProgress* = NULL);
%MethodCode
	tlp::MutableContainer<double> result;
	tlp::clusteringCoefficient(a0, result, a2, a3);
	tlp::node n;
	forEach(n, a0->getNodes()) {
		a1->setNodeValue(n, result.get(n));
	}
%End  	
  	
  	
  void dagLevel(const tlp::Graph *graph, tlp::IntegerProperty* level, tlp::PluginProgress* = NULL);
%MethodCode
	tlp::MutableContainer<unsigned int> result;
	tlp::dagLevel(a0, result, a2);
	tlp::node n;
	forEach(n, a0->getNodes()) {
		a1->setNodeValue(n, static_cast<int>(result.get(n)));
	}
%End  
  
	unsigned int maxDegree(const tlp::Graph *);
  
	unsigned int minDegree(const tlp::Graph *);
  
  	unsigned int maxDistance(const tlp::Graph *graph, const tlp::node n, tlp::IntegerProperty *distance, tlp::EDGE_TYPE direction = tlp::UNDIRECTED);
%MethodCode
	tlp::MutableContainer<unsigned int> result;
	tlp::maxDistance(a0, *a1, result, a3);
	tlp::node n;
	forEach(n, a0->getNodes()) {
		a2->setNodeValue(n, static_cast<int>(result.get(n)));
	}
%End  	

  	void reachableNodes(const tlp::Graph *graph, const tlp::node startNode, std::set<tlp::node> &result /Out/, unsigned int maxDistance, tlp::EDGE_TYPE direction = tlp::UNDIRECTED);
%Docstring
tlp.reachableNodes(tlp.Graph, tlp.node, int, tlp.EDGE_TYPE = tlp.UNDIRECTED) -> list of tlp.node 
%End  	
};
