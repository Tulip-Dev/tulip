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
#include <tulip/tuliphash.h>
#include <tulip/GraphTools.h>
#include <tulip/GraphMeasure.h>
#include <tulip/AcyclicTest.h>
#include <tulip/TreeTest.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/IntegerProperty.h>
#include <tulip/NumericProperty.h>
#include <tulip/ForEach.h>
#include <tulip/ConnectedTest.h>
#include <tulip/Ordering.h>
#include <tulip/MutableContainer.h>

#include <queue>
#include <stack>
#include <cmath>

using namespace std;

namespace tlp {

//======================================================================
void makeProperDag(Graph* graph,list<node> &addedNodes,
                   TLP_HASH_MAP<edge,edge> &replacedEdges,
                   IntegerProperty *edgeLength) {
  if (TreeTest::isTree(graph)) return;

  assert(AcyclicTest::isAcyclic(graph));
  //We compute the dag level metric on resulting sg.
  MutableContainer<unsigned int> dLevel;
  dagLevel(graph, dLevel);
  //we now transform the dag in a proper Dag, two linked nodes of a proper dag
  //must have a difference of one of dag level metric.
  node tmp1,tmp2;
  vector<edge> sgEdges(graph->numberOfEdges());
  Iterator<edge> *itE=graph->getEdges();

  for (int i=0; itE->hasNext(); ++i) {
    sgEdges[i]=itE->next();
  }

  delete itE;

  if (edgeLength)
    edgeLength->setAllEdgeValue(1);

  for (vector<edge>::const_iterator itEdge=sgEdges.begin(); itEdge!=sgEdges.end(); ++itEdge) {
    edge ite=*itEdge;
    pair<node, node> eEnds = graph->ends(ite);
    int delta=dLevel.get(eEnds.second.id)- dLevel.get(eEnds.first.id);

    if (delta>1) {
      tmp1=graph->addNode();
      replacedEdges[ite]=graph->addEdge(eEnds.first, tmp1);
      addedNodes.push_back(tmp1);
      dLevel.set(tmp1.id, dLevel.get(eEnds.first.id) + 1);

      if (delta>2) {
        tmp2=graph->addNode();
        addedNodes.push_back(tmp2);
        edge e=graph->addEdge(tmp1, tmp2);

        if (edgeLength)
          edgeLength->setEdgeValue(e, delta-2);

        dLevel.set(tmp2.id, dLevel.get(eEnds.second.id) - 1);
        tmp1=tmp2;
      }

      graph->addEdge(tmp1, eEnds.second);
    }
  }

  for (TLP_HASH_MAP<edge,edge>::const_iterator it=replacedEdges.begin(); it!=replacedEdges.end(); ++it)
    graph->delEdge((*it).first);

  assert(AcyclicTest::isAcyclic(graph));
}
//======================================================================
node makeSimpleSource(Graph* graph) {
  assert(AcyclicTest::isAcyclic(graph));
  node startNode=graph->addNode();
  Iterator<node> *itN=graph->getNodes();

  while (itN->hasNext()) {
    node itn=itN->next();

    if ((graph->indeg(itn)==0) && (itn!=startNode)) {
      graph->addEdge(startNode,itn);
    }
  }

  delete itN;
  assert(AcyclicTest::isAcyclic(graph));
  return startNode;
}
//======================================================================
vector<vector<node> > computeCanonicalOrdering(PlanarConMap *carte, std::vector<edge>  *dummyEdges,PluginProgress* pluginProgress) {
  Ordering o(carte, pluginProgress, 0, 100, 100); // feedback (0% -> 100%)

  if (dummyEdges!=NULL)
    *dummyEdges = o.getDummyEdges();

  vector<vector<node> > res;
  int nbMax = o.size()-1;

  for (int i=nbMax; i>=0; i--) {
    res.push_back(o[i]);
  }

  return res;
}
//======================================================================
std::vector<node> computeGraphCenters(Graph * graph) {
  assert(ConnectedTest::isConnected(graph));
  MutableContainer<unsigned int> _dist;
  node n;
  unsigned minD = UINT_MAX;
  forEach(n, graph->getNodes()) {
    MutableContainer<unsigned int> tmp;
    unsigned int maxD = maxDistance(graph, n, tmp, UNDIRECTED);
    _dist.set(n.id, maxD);
    minD = std::min(minD, maxD);
  }

  vector<node> result;
  forEach(n, graph->getNodes()) {
    if (_dist.get(n.id) == minD)
      result.push_back(n);
  }

  return result;
}
//======================================================================
node graphCenterHeuristic(Graph * graph,
                          PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));
  node result;
  unsigned int cDist = UINT_MAX - 2;
  MutableContainer<bool> toTreat;
  toTreat.setAll(true);
  MutableContainer<unsigned int> dist;
  unsigned int i = 0;
  node n = graph->getOneNode();
  unsigned int nbTry = 2 + sqrt(graph->numberOfNodes());
  unsigned int maxTries = nbTry;
  bool stop = false;

  while (nbTry>0 && !stop) {
    --nbTry;

    if (pluginProgress) {
      pluginProgress->setComment("Computing graph center...");

      if (maxTries - nbTry % 200 == 0 )
        pluginProgress->progress(maxTries - nbTry, maxTries);
    }

    if (toTreat.get(n.id)) {
      ++i;
      unsigned int di = tlp::maxDistance(graph, n, dist);
      toTreat.set(n.id, false);

      if (di < cDist) {
        result = n;
        cDist = di;
      }
      else {
        unsigned int delta = di - cDist;
        node v;
        forEach(v, graph->getNodes()) {
          if (dist.get(v.id) < delta) { //all the nodes at distance less than delta can't be center
            toTreat.set(v.id, false);
          }
        }
      }

      unsigned int nextMax = 0;
      node v;
      forEach(v, graph->getNodes()) {
        if (dist.get(v.id) > (di/2 + di%2) )
          toTreat.set(v.id, false);
        else {
          if (toTreat.get(v.id)) {
            if (dist.get(v.id) > nextMax) {
              n = v;
              nextMax = dist.get(v.id);
            }
          }
        }
      }

      if (nextMax == 0) stop = true;
    }
  }

  if (pluginProgress) {
    pluginProgress->setComment("Graph center computed");
    pluginProgress->progress(100, 100);
  }

  return result;
}

//======================================================================
void selectSpanningForest(Graph* graph, BooleanProperty *selectionProperty,
                          PluginProgress *pluginProgress) {
  list<node> fifo;

  BooleanProperty nodeFlag(graph);

  unsigned int nbSelectedNodes = 0;
  unsigned int nbNodes = 0;
  // get previously selected nodes
  Iterator<node> *itN=graph->getNodes();

  for (; itN->hasNext();) {
    ++ nbNodes;
    node itn=itN->next();

    if (selectionProperty->getNodeValue(itn)==true) {
      fifo.push_back(itn);
      nodeFlag.setNodeValue(itn,true);
      ++nbSelectedNodes;
    }
  }

  delete itN;

  selectionProperty->setAllEdgeValue(true);
  selectionProperty->setAllNodeValue(true);

  bool ok=true;
  node tmp1;
  unsigned int edgeCount = 0;

  while (ok) {
    while (!fifo.empty()) {
      tmp1=fifo.front();
      fifo.pop_front();
      Iterator<edge> *itE=graph->getOutEdges(tmp1);

      for(; itE->hasNext();) {
        edge adjit=itE->next();

        if (!nodeFlag.getNodeValue(graph->target(adjit))) {
          nodeFlag.setNodeValue(graph->target(adjit),true);
          ++nbSelectedNodes;
          fifo.push_back(graph->target(adjit));
        }
        else
          selectionProperty->setEdgeValue(adjit,false);

        if (pluginProgress) {
          pluginProgress->setComment("Computing a spanning forest...");
          ++edgeCount;

          if (edgeCount == 200 ) {
            if (pluginProgress->progress(nbSelectedNodes*100/nbNodes, 100) != TLP_CONTINUE) {
              return;
            }

            edgeCount = 0;
          }
        }
      }

      delete itE;
    }

    ok=false;
    bool degZ=false;
    node goodNode;
    Iterator<node> *itN=graph->getNodes();

    for(; itN->hasNext();) {
      node itn=itN->next();

      if (!nodeFlag.getNodeValue(itn)) {
        if (!ok) {
          goodNode=itn;
          ok=true;
        }

        if (graph->indeg(itn)==0) {
          fifo.push_back(itn);
          nodeFlag.setNodeValue(itn,true);
          ++nbSelectedNodes;
          degZ=true;
        }

        if (!degZ) {
          if (graph->indeg(itn)<graph->indeg(goodNode))
            goodNode=itn;
          else {
            if (graph->indeg(itn)==graph->indeg(goodNode))
              if (graph->outdeg(itn)>graph->outdeg(goodNode))
                goodNode=itn;
          }
        }
      }
    }

    delete itN;

    if (ok && (!degZ)) {
      fifo.push_back(goodNode);
      nodeFlag.setNodeValue(goodNode,true);
      ++nbSelectedNodes;
    }
  }
}

//======================================================================
void selectSpanningTree(Graph* graph, BooleanProperty *selection,
                        PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  node root = graphCenterHeuristic(graph, pluginProgress);
  unsigned int size = graph->numberOfNodes();
  unsigned int nbNodes = 1, edgeCount = 0;
  vector<node> roots;
  unsigned int i = 0;
  selection->setNodeValue(root, true);
  roots.push_back(root);

  while(nbNodes != size) {
    root = roots[i];
    Iterator<edge> * ite = graph->getInOutEdges(root);

    while(ite->hasNext()) {
      edge e = ite->next();

      if(!selection->getEdgeValue(e)) {
        node neighbour = graph->opposite(e, root);

        if(!selection->getNodeValue(neighbour)) {
          selection->setNodeValue(neighbour, true);
          roots.push_back(neighbour);
          nbNodes++;
          selection->setEdgeValue(e, true);

          if (pluginProgress) {
            pluginProgress->setComment("Computing spanning tree...");
            ++edgeCount;

            if (edgeCount % 200  == 0) {
              if (pluginProgress->progress(edgeCount, graph->numberOfEdges()) != TLP_CONTINUE)
                return;
            }
          }
        }
      }
    }

    delete ite;
    i++;
  }

  if (pluginProgress) {
    pluginProgress->setComment("Spanning tree computed");
    pluginProgress->progress(100, 100);
  }

}

//======================================================================
struct ltEdge {
  NumericProperty *m;
  ltEdge(NumericProperty *metric) : m(metric) {}
  bool operator()(const edge &e1, const edge &e2) const {
    return (m->getEdgeDoubleValue(e1) < m->getEdgeDoubleValue(e2));
  }
};

void selectMinimumSpanningTree(Graph* graph, BooleanProperty *selection,
                               NumericProperty *edgeWeight,
                               PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));

  if (!edgeWeight)
    // no weight return a spanning tree
    return selectSpanningTree(graph, selection, pluginProgress);

  selection->setAllNodeValue(true);
  selection->setAllEdgeValue(false);

  unsigned int numClasses = 0;
  map<int, int> classes;

  Iterator<node> *itN = graph->getNodes();

  while (itN->hasNext()) {
    node n=itN->next();
    classes[n.id] = numClasses;
    ++numClasses;
  }

  delete itN;

  unsigned int maxCount = numClasses;
  unsigned int edgeCount = 0;

  std::list<edge> sortedEdges;
  Iterator<edge> *itE = graph->getEdges();

  while (itE->hasNext()) {
    edge e=itE->next();
    sortedEdges.push_back(e);
  }

  delete itE;

  sortedEdges.sort<ltEdge>(ltEdge(edgeWeight));

  while(numClasses > 1) {
    edge cur;
    pair<node, node> curEnds;

    do {
      curEnds = graph->ends(cur = sortedEdges.front());
      sortedEdges.pop_front();
    }
    while(!(classes[curEnds.first.id] !=  classes[curEnds.second.id]));

    selection->setEdgeValue(cur, true);

    if (pluginProgress) {
      pluginProgress->setComment("Computing minimum spanning tree..." );
      ++edgeCount;

      if (edgeCount == 200 ) {
        if (pluginProgress->progress((maxCount - numClasses)*100/maxCount, 100) != TLP_CONTINUE)
          return;

        edgeCount = 0;
      }
    }

    int x = classes[curEnds.first.id];
    int y = classes[curEnds.second.id];

    Iterator<node> *itN = graph->getNodes();

    while (itN->hasNext()) {
      node n=itN->next();

      if(classes[n.id] == y)
        classes[n.id] = x;
    }

    delete itN;
    numClasses--;
  }
}

//======================================================================

vector<node> bfs(const Graph *graph, node root) {
  std::vector<tlp::node> ret;

  if (graph->numberOfNodes() > 0) {
    if (!root.isValid()) {
      root = graph->getSource();

      if (!root.isValid()) {
        root = graph->getOneNode();
      }
    }

    assert(graph->isElement(root));
    std::queue<tlp::node> nodesToVisit;
    MutableContainer<bool> markedNodes;
    markedNodes.setAll(false);
    nodesToVisit.push(root);
    markedNodes.set(root, true);

    while (!nodesToVisit.empty()) {
      node curNode = nodesToVisit.front();
      nodesToVisit.pop();
      ret.push_back(curNode);
      node neigh;
      forEach(neigh, graph->getInOutNodes(curNode)) {
        if (!markedNodes.get(neigh)) {
          markedNodes.set(neigh, true);
          nodesToVisit.push(neigh);
        }
      }
    }
  }

  return ret;
}

//======================================================================

vector<node> dfs(const Graph *graph, node root) {
  std::vector<tlp::node> ret;

  if (graph->numberOfNodes() > 0) {
    if (!root.isValid()) {
      root = graph->getSource();

      if (!root.isValid()) {
        root = graph->getOneNode();
      }
    }

    assert(graph->isElement(root));
    std::stack<tlp::node> nodesToVisit;
    MutableContainer<bool> markedNodes;
    markedNodes.setAll(false);
    nodesToVisit.push(root);
    markedNodes.set(root, true);

    while (!nodesToVisit.empty()) {
      node curNode = nodesToVisit.top();
      nodesToVisit.pop();
      ret.push_back(curNode);
      std::vector<tlp::node> neighs;
      node neigh;
      forEach(neigh, graph->getInOutNodes(curNode)) {
        neighs.push_back(neigh);
      }

      for (std::vector<tlp::node>::reverse_iterator it = neighs.rbegin() ; it != neighs.rend() ; ++it) {
        if (!markedNodes.get(*it)) {
          markedNodes.set(*it, true);
          nodesToVisit.push(*it);
        }
      }
    }
  }

  return ret;
}

//==================================================
void buildNodesUniformQuantification(const Graph* graph,
                                     const NumericProperty* prop,
                                     unsigned int k,
                                     std::map<double, int>& nodeMapping) {
  //build the histogram of node values
  map<double,int> histogram;
  Iterator<node> *itN=graph->getNodes();

  while (itN->hasNext()) {
    node itn=itN->next();
    double nodeValue=prop->getNodeDoubleValue(itn);

    if (histogram.find(nodeValue)==histogram.end())
      histogram[nodeValue]=1;
    else
      histogram[nodeValue]+=1;
  }

  delete itN;

  //Build the color map
  map<double,int>::iterator it;
  double sum=0;
  double cK=double(graph->numberOfNodes())/double(k);
  int k2=0;

  for (it=histogram.begin(); it!=histogram.end(); ++it) {
    sum+=(*it).second;
    nodeMapping[(*it).first]=k2;

    while (sum>cK*double(k2+1)) ++k2;
  }
}
//==================================================
void buildEdgesUniformQuantification(const Graph* graph,
                                     const NumericProperty* prop,
                                     unsigned int k,
                                     std::map<double, int>& edgeMapping) {
  //build the histogram of edges values
  map<double,int> histogram;
  Iterator<edge> *itE=graph->getEdges();

  while (itE->hasNext()) {
    edge ite=itE->next();
    double value=prop->getEdgeDoubleValue(ite);

    if (histogram.find(value)==histogram.end())
      histogram[value]=1;
    else
      histogram[value]+=1;
  }

  delete itE;
  //===============================================================
  //Build the color map
  map<double,int>::iterator it;
  double sum=0;
  double cK=double(graph->numberOfEdges())/double(k);
  int k2=0;

  for (it=histogram.begin(); it!=histogram.end(); ++it) {
    sum+=(*it).second;
    edgeMapping[(*it).first]=k2;

    while (sum>cK*double(k2+1)) ++k2;
  }
}

}
