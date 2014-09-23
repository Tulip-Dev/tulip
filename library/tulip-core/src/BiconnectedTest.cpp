/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include<stack>

#include <tulip/Graph.h>
#include <tulip/StableIterator.h>
#include <tulip/BiconnectedTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/MutableContainer.h>

using namespace std;
using namespace tlp;
//=================================================================
BiconnectedTest * BiconnectedTest::instance = NULL;
//=================================================================
// structure below is used to implement dfs loop
struct dfsBiconnectStruct {
  node from, u, first;
  unsigned int depth;
  Iterator<node>* inOutNodes;

  dfsBiconnectStruct(Graph* graph, node n, unsigned int d = 0, node u = node(), node first = node()):
    from(n), u(u), first(first), depth(d), inOutNodes(new StableIterator<node>(graph->getInOutNodes(from))) {}
};

static void makeBiconnectedDFS(Graph *graph, vector<edge> &addedEdges) {
  // the graph is already connected
  // so get any node to begin
  node from = graph->getOneNode();

  if (!from.isValid())
    return;

  MutableContainer<int> low;
  MutableContainer<int> depth;
  depth.setAll(-1);
  MutableContainer<node> supergraph;
  supergraph.setAll(node());

  // dfs loop
  stack<dfsBiconnectStruct> dfsLevels;
  dfsBiconnectStruct dfsParams(graph, from);
  dfsLevels.push(dfsParams);
  depth.set(from.id, 0);
  low.set(from.id, 0);

  while(!dfsLevels.empty()) {
    dfsParams = dfsLevels.top();
    from = dfsParams.from;
    node u = dfsParams.first;

    //for every node connected to from
    Iterator<node>* itN = dfsParams.inOutNodes;

    while (itN->hasNext()) {
      node to = itN->next();

      //if there is a loop, ignore it
      if (from == to) {
        continue;
      }

      if (!u.isValid()) {
        dfsLevels.top().first = u = to;
      }

      //if the destination node has not been visited, visit it
      if (depth.get(to.id) == -1) {
        supergraph.set(to.id, from);
        dfsParams.from = to;
        dfsParams.first = node();
        dfsParams.u = u;
        unsigned int currentDepth = dfsParams.depth + 1;
        dfsParams.depth = currentDepth;
        depth.set(to.id, currentDepth);
        low.set(to.id, currentDepth);
        dfsParams.inOutNodes = new StableIterator<node>(graph->getInOutNodes(to));
        break;
      }
      else {
        low.set(from.id, std::min(low.get(from.id), depth.get(to.id)));
      }
    }

    if (from != dfsParams.from) {
      dfsLevels.push(dfsParams);
      continue;
    }

    delete itN;

    // pop the current dfsParams
    node to = dfsParams.from;
    from = supergraph.get(to.id);
    u = dfsParams.u;

    if (low.get(to.id) == depth.get(from.id)) {
      if (to == u && supergraph.get(from.id).isValid())
        addedEdges.push_back(graph->addEdge(to, supergraph.get(from.id)));

      if (to != u)
        addedEdges.push_back(graph->addEdge(u, to));
    }

    low.set(from.id, std::min(low.get(from.id), low.get(to.id)));

    dfsLevels.pop();
  }
}

void makeBiconnectedDFS(Graph *graph, node from,
                        MutableContainer<int> &low,
                        MutableContainer<int> &depth,
                        MutableContainer<node> &supergraph,
                        unsigned int &currentDepth,
                        vector<edge> &addedEdges) {
  node u;
  depth.set(from.id, currentDepth++);
  low.set(from.id, depth.get(from.id));

  //for every node connected to this one, call this function so it runs on every node.
  StableIterator<node> itN(graph->getInOutNodes(from));

  while (itN.hasNext()) {
    node to = itN.next();

    //if there is a loop, ignore it
    if (from == to) {
      continue;
    }

    if (!u.isValid()) {
      u = to;
    }

    //if the destination node has not been visited, visit it
    if (depth.get(to.id) == -1) {
      supergraph.set(to.id, from);
      makeBiconnectedDFS(graph, to, low, depth, supergraph, currentDepth, addedEdges);

      if (low.get(to.id) == depth.get(from.id)) {
        if (to == u && supergraph.get(from.id).isValid())
          addedEdges.push_back(graph->addEdge(to, supergraph.get(from.id)));

        if (to != u)
          addedEdges.push_back(graph->addEdge(u, to));
      }

      low.set(from.id, std::min(low.get(from.id), low.get(to.id)));
    }
    else {
      low.set(from.id, std::min(low.get(from.id), depth.get(to.id)));
    }
  }
}
//=================================================================
bool biconnectedTest(const Graph *graph, node v,
                     MutableContainer<bool> &mark,
                     MutableContainer<unsigned int> &low,
                     MutableContainer<unsigned int> &dfsNumber,
                     MutableContainer<node> &supergraph,
                     unsigned int &count) {
  mark.set(v.id,true);
  dfsNumber.set(v.id,count);
  low.set(v.id,count);
  ++count;
  Iterator<node> *it=graph->getInOutNodes(v);

  while (it->hasNext()) {
    node w=it->next();

    if (!mark.get(w.id)) {
      if (dfsNumber.get(v.id)==1) {
        if (count != 2) {
          delete it;
          return false;
        }
      }

      supergraph.set(w.id,v);

      if (!biconnectedTest(graph,w,mark,low,dfsNumber,supergraph,count)) {
        delete it;
        return false;
      }

      if (dfsNumber.get(v.id)!=1) {
        if (low.get(w.id)>=dfsNumber.get(v.id)) {
          delete it;
          return false;
        }
        else
          low.set(v.id, std::min(low.get(v.id), low.get(w.id)));
      }
    }
    else if (supergraph.get(v.id)!=w) {
      low.set(v.id, std::min(low.get(v.id), dfsNumber.get(w.id)));
    }
  }

  delete it;
  return true;
}
//=================================================================
BiconnectedTest::BiconnectedTest() {
}
//=================================================================
bool BiconnectedTest::isBiconnected(const tlp::Graph* graph) {
  if (instance == NULL) {
    instance = new BiconnectedTest();
  }

  return instance->compute(graph);
}
//=================================================================
void BiconnectedTest::makeBiconnected(Graph *graph, vector<edge> &addedEdges) {
  if (instance == NULL) {
    instance = new BiconnectedTest();
  }

  graph->removeListener(instance);
  instance->resultsBuffer.erase(graph);
  instance->connect(graph, addedEdges);
  assert(BiconnectedTest::isBiconnected(graph));
}
//=================================================================
void BiconnectedTest::connect(Graph *graph, vector<edge> &addedEdges) {
  ConnectedTest::makeConnected(graph, addedEdges);
  makeBiconnectedDFS(graph, addedEdges);
}
//=================================================================
bool BiconnectedTest::compute(const tlp::Graph* graph) {
  if(graph->numberOfNodes() == 0) {
    return true;
  }

  if (resultsBuffer.find(graph)!=resultsBuffer.end())
    return resultsBuffer[graph];

  MutableContainer<bool> mark;
  mark.setAll(false);
  MutableContainer<unsigned int> low;
  MutableContainer<unsigned int> dfsNumber;
  MutableContainer<node> supergraph;
  unsigned int count = 1;
  bool result = false;
  Iterator<node> *it=graph->getNodes();

  if (it->hasNext())
    result=(biconnectedTest(graph,it->next(),mark,low,dfsNumber,supergraph,count));

  delete it;

  if (count!=graph->numberOfNodes()+1) {
    result=false;
  } //connected test

  resultsBuffer[graph]=result;
  graph->addListener(this);
  return result;
}
//=================================================================
void BiconnectedTest::treatEvent(const Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      resultsBuffer[graph]=false;
      break;

    case GraphEvent::TLP_DEL_NODE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (!resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      // we don't care about other events
      break;
    }
  }
  else {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* graph = reinterpret_cast<Graph *>(evt.sender());

    if (graph && evt.type() == Event::TLP_DELETE) {
      resultsBuffer.erase(graph);
    }
  }
}

