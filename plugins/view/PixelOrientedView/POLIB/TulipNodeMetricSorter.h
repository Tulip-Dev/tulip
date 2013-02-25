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

#ifndef TULIPNODESORTER_H_
#define TULIPNODESORTER_H_

#include <tulip/Graph.h>
#include <tulip/Observable.h>

#include <map>
#include <vector>

namespace pocore {


template<typename PROPERTYTYPE, typename PROPERTY>
class NodeMetricPropertyOrderRelation {

public :

	NodeMetricPropertyOrderRelation(tlp::Graph *graph, std::string dimName) {
		nodeProperty = graph->getProperty<PROPERTY>(dimName);
	}

	bool operator()(tlp::node n1, tlp::node n2) const {
		typename PROPERTYTYPE::RealType v1 = nodeProperty->getNodeValue(n1);
		typename PROPERTYTYPE::RealType v2 = nodeProperty->getNodeValue(n2);
		return v1 < v2;
	}

private :

	PROPERTY *nodeProperty;
};

class TulipNodeMetricSorter  {

public :

	static TulipNodeMetricSorter *getInstance(tlp::Graph *graph);

	~TulipNodeMetricSorter();

	void sortNodesForProperty(std::string propertyName);
	void cleanupSortNodesForProperty(std::string propertyName);
	unsigned int getNbValuesForProperty(std::string propertyName) ;
	tlp::node getNodeAtRankForProperty(const unsigned int rank, std::string propertyName);
	unsigned int getNodeRankForProperty(tlp::node n, std::string propertyName);

private :

	TulipNodeMetricSorter(tlp::Graph *graph);

	void reset();

	tlp::Graph *graph;
	unsigned int lastNbNodes;
	std::map<std::string, std::vector<tlp::node> > nodeSortingMap;
	std::map<std::string, unsigned int> nbValuesPropertyMap;

	static std::map<tlp::Graph *, TulipNodeMetricSorter *> instances;

};

}

#endif /* TULIPNODESORTER_H_ */
