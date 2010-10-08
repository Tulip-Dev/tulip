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
#include "tulip/Graph.h"
#include "tulip/ConnectedTest.h"
#include "tulip/MutableContainer.h"
#include "tulip/ForEach.h"

using namespace std;
using namespace tlp;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
ConnectedTest * ConnectedTest::instance=0;
#endif
#else
ConnectedTest * ConnectedTest::instance=0;
#endif
//=================================================================
bool ConnectedTest::isConnected(const tlp::Graph*const graph) {
	if (instance==0)
		instance=new ConnectedTest();
	return instance->compute(graph);
}
//=================================================================
void ConnectedTest::makeConnected(Graph *graph, vector<edge> &addedEdges) {
	//  cerr << __PRETTY_FUNCTION__ << " : " << graph->getAttribute<string>("name") << endl;
	if (instance==0)
		instance=new ConnectedTest();
	graph->removeGraphObserver(instance);
	instance->resultsBuffer.erase((unsigned long)graph);
	vector<node> toLink;
	instance->connect(graph, toLink);
	for (unsigned int i = 1; i < toLink.size(); ++i)
		addedEdges.push_back(graph->addEdge(toLink[i-1], toLink[i]));
	assert(ConnectedTest::isConnected(graph));
}
//=================================================================
unsigned int ConnectedTest::numberOfConnectedComponents(const tlp::Graph* const graph) {
	if (graph->numberOfNodes()==0) return 0u;
	if (instance==0)
		instance=new ConnectedTest();
	graph->removeGraphObserver(instance);
	vector<node> toLink;
	instance->connect(graph, toLink);
	unsigned int result;
	if (!toLink.empty())
		result =  toLink.size();
	else
		result = 1u;
	instance->resultsBuffer[(unsigned long)graph] = (result == 1u);
	graph->addGraphObserver(instance);
	return result;
}
//======================================================================
static void dfsAddNodesToComponent(Graph *graph, node n, MutableContainer<bool> &flag,
		std::set<node>& component) {
	if (flag.get(n.id)) return;
	flag.set(n.id, true);
	component.insert(n);
	node itn;
	forEach(itn, graph->getInOutNodes(n))
	dfsAddNodesToComponent(graph, itn, flag, component);
}

void ConnectedTest::computeConnectedComponents(Graph *graph, std::vector<std::set<node> >& components) {
	MutableContainer<bool> flag;
	flag.setAll(false);
	node itn;
	forEach(itn, graph->getNodes()) {
		if (!flag.get(itn.id)) {
			components.push_back(std::set<node>());
			dfsAddNodesToComponent(graph, itn, flag, components.back());
		}
	}
}

//=================================================================
void connectedTest(const Graph * const graph, node n, 
		MutableContainer<bool> &visited,
		unsigned int &count) {

	unsigned int i = 0;
	vector<node> next_roots;
	next_roots.push_back(n);
	while(i < next_roots.size()) {
		node r = next_roots[i];
		Iterator<node> * itn = graph->getInOutNodes(r);
		while(itn->hasNext()){
			node n = itn->next();
			if(!visited.get(n.id)) {
				visited.set(n.id,true);
				next_roots.push_back(n);
				count++;
			}
		} delete itn;
		i++;
	}
}
//=================================================================
ConnectedTest::ConnectedTest(){}
//=================================================================
bool ConnectedTest::compute(const tlp::Graph* const graph) {
	if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
		return resultsBuffer[(unsigned long)graph];
	if (graph->numberOfNodes()==0) return true;
	MutableContainer<bool> visited;
	visited.setAll(false);
	unsigned int count = 0;
	connectedTest(graph, graph->getOneNode(), visited, count);
	bool result = (count == graph->numberOfNodes());
	resultsBuffer[(unsigned long)graph]=result;
	graph->addGraphObserver(this);
	return result;
}
//=================================================================
void ConnectedTest::connect(const tlp::Graph* const graph, vector< node >& toLink) {
	if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) {
		if (resultsBuffer[(unsigned long)graph])
			return;
	}
	if (graph->numberOfNodes()==0) return;
	MutableContainer<bool> visited;
	visited.setAll(false);
	unsigned int count = 0;
	Iterator<node> *itN = graph->getNodes();
	while(itN->hasNext()) {
		node n = itN->next();
		if (!visited.get(n.id)) {
			toLink.push_back(n);
			connectedTest(graph, n, visited, count);
		}
	} delete itN;
}
//=================================================================
void ConnectedTest::addEdge(Graph *graph,const edge) {
	if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
		if (resultsBuffer[(unsigned long)graph]) return;
	graph->removeGraphObserver(this);
	resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void ConnectedTest::delEdge(Graph *graph,const edge) {
	//  cerr << __PRETTY_FUNCTION__ << endl;
	if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
		if (!resultsBuffer[(unsigned long)graph]) return;
	graph->removeGraphObserver(this);
	resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void ConnectedTest::reverseEdge(Graph *,const edge) {
}
//=================================================================
void ConnectedTest::addNode(Graph *graph,const node) {
	resultsBuffer[(unsigned long)graph]=false;
}
//=================================================================
void ConnectedTest::delNode(Graph *graph,const node) {
	graph->removeGraphObserver(this);
	resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void ConnectedTest::destroy(Graph *graph) {
	//  cerr << __PRETTY_FUNCTION__ << endl;
	graph->removeGraphObserver(this);
	resultsBuffer.erase((unsigned long)graph);
}
//=================================================================

