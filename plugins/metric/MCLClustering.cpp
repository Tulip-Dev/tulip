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
#include <math.h>
#include <sstream>
#include <string>
#include <map>
#include <tulip/MutableContainer.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/GraphMeasure.h>
#include <tulip/StableIterator.h>
#include <tulip/SimpleTest.h>
#include <tulip/ForEach.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/StableIterator.h>

using namespace tlp;
using namespace std;

/** \addtogroup clustering */
/*@{*/
/** \file
* \brief  An implementation of the MCL clustering algorithm
*
* This plugin is an implementation of the MCL algorithm
* first published as:
*
* Stijn van Dongen \n
* PhD Thesis "Graph Clustering by Flow Simulation", \n
* University of Utrecht,\n
* 2000. \n
*
* <b> HISTORY</b>
*
* - 16/09/2011 Version 1.0: Initial release
*
* \author David Auber, Labri, Email : auber@labri.fr
*
*  <b>LICENCE</b>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
**/
class MCLClustering:public tlp::DoubleAlgorithm {
public:

  PLUGININFORMATION("MCL Clustering", "D. Auber & R. Bourqui","10/10/2005","Nodes partitioning measure of Markov Cluster algorithm<br/>used for community detection.","1.0","Clustering")

  MCLClustering(const tlp::PluginContext *);
  ~MCLClustering();
  bool run();
  void inflate(double r, unsigned int k, node n, bool);
  void pruneK(node n, unsigned int k);
  //void pruneT(node n);
  void makeStoc(node n);
  void bfs(node n, double value);
  double connectedComponent();

  bool equal();
  void init();
  void power(node n);
  edge getEdge(node, node);

  VectorGraph g;
  EdgeProperty<double> inW, outW;
  NodeProperty<node> tlpNodes;
  NodeProperty<double> resultN;
  map<pair<node, node>, edge > existEdge;
  MutableContainer<node> nodeMapping;
  MutableContainer<edge> edgeMapping;
  NumericProperty *weights;
  double _r;
  unsigned int _k;
};
/*@}*/

PLUGIN(MCLClustering)

const double epsilon = 1E-9;
//=================================================
edge MCLClustering::getEdge(node src, node tgt) {
  pair<node, node> e(src, tgt);
  map<pair<node, node>, edge>::const_iterator ite = existEdge.find(e);
  if (ite != existEdge.end())
    return ite->second;
  else {
    edge ne = g.addEdge(src, tgt);
    existEdge[e] = ne;
    inW[ne] = 0.;
    outW[ne] = 0.;
    return ne;
  }
}
//=================================================
void MCLClustering::power(node n) {
  edge e1;
  stableForEach(e1, g.getOutEdges(n)) {
    double v1 = inW[e1];

    if (v1 > epsilon) {
      edge e2;
      stableForEach(e2, g.getOutEdges(g.target(e1))) {
        double v2 = inW[e2] * v1;

        if (v2 > epsilon) {
          edge ne = getEdge(n, g.target(e2));
          outW[ne] += v2;
        }
      }
    }
  }
}
//==================================================
void MCLClustering::makeStoc(node n) {
  double sum = 0.;
  edge e;
  forEach(e, g.getOutEdges(n)) {
    sum += outW[e];
  }

  if (sum > 0.) {
    forEach(e, g.getOutEdges(n)) {
      outW[e] = outW[e] / sum;
    }
  }
  else {
    //cout << "ERROR" << endl;
    forEach(e, g.getOutEdges(n)) {
      outW[e] = 1. / double(g.outdeg(n));
    }
  }
}
//==================================================
void MCLClustering::pruneK(node n, unsigned int k) {
  if (g.outdeg(n) < k) return;

  set<double> orderedVal;
  edge e;
  forEach(e, g.getOutEdges(n)) {
    orderedVal.insert(outW[e]);
  }

  set<double>::reverse_iterator it  = orderedVal.rbegin();

  while(--k) ++it;

  double t = *it;
  stableForEach(e, g.getOutEdges(n)) {
    if (outW[e] < t) {
      existEdge.erase(g.ends(e));
      inW[e]  = 0.;
      outW[e] = 0.;
      g.delEdge(e);
    }
  }
}
//==================================================
/*void MCLClustering::pruneT(node n) {
  double maxV = 0.;
  //double sum = 0.;
  edge   e;
  forEach(e, g.getOutEdges(n)) {
    maxV = std::max(outW[e], maxV);
    //sum += outW[e];
  }
  stableForEach(e, g.getOutEdges(n)) {
    if (outW[e] < maxV / (2. * double(g.outdeg(n) + 1))) {
      //if (outW[e] < epsilon) {
      const std::pair<node, node>& eEnds = g.ends(e);
      pair<unsigned int, unsigned int> edgeM(eEnds.first.id, eEnds.second.id);
      existEdge.erase(edgeM);
      inW[e]  = 0.;
      outW[e] = 0.;
      g.delEdge(e);
    }
  }
  }*/
//=================================================
void MCLClustering::inflate(double r, unsigned int k, node n, bool noprune) {
  edge e;
  double sum  = 0.;
  forEach(e, g.getOutEdges(n)) {
    sum += pow(outW[e], r);
  }

  if (sum > 0.) {
    forEach(e, g.getOutEdges(n)) {
      outW[e] = pow(outW[e], r) / sum;
    }
  }

  if (noprune)
    return;

  pruneK(n, k);
  makeStoc(n);
}
//=================================================
bool MCLClustering::equal() {
  //cout << __PRETTY_FUNCTION__ << endl << flush;
  edge e;
  forEach(e, g.getEdges()) {
    if (fabs(inW[e] - outW[e]) > epsilon)
      return false;
  }
  return true;
}
//=================================================
static const char * paramHelp[] = {
  // number of clusters
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_BODY() \
  "Determines the random walk length at each step" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "NumericProperty" ) \
  HTML_HELP_BODY() \
  "Edge weights to use" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_BODY() \
  "Determines, for each node, the number of strongest link kept at each iteration" \
  HTML_HELP_CLOSE(),
};
//=================================================
MCLClustering::MCLClustering(const tlp::PluginContext *context):DoubleAlgorithm(context), weights(NULL), _r(2.0), _k(5) {
  addInParameter<double>("inflate", paramHelp[0], "2.", false);
  addInParameter<NumericProperty*>("weights", paramHelp[1], "", false);
  addInParameter<unsigned int>("pruning", paramHelp[2], "5", false);
}
//===================================================================================
MCLClustering::~MCLClustering() {
}
//================================================================================
void MCLClustering::init() {
  node n;
  forEach(n, graph->getNodes()) {
    node newNode = g.addNode();
    nodeMapping.set(n.id, newNode);
    tlpNodes[newNode] = n;
  }
  edge e;
  forEach(e, graph->getEdges()) {
    const std::pair<node, node> &eEnds = graph->ends(e);
    node src = nodeMapping.get(eEnds.first.id);
    node tgt = nodeMapping.get(eEnds.second.id);
    edge tmp = g.addEdge(src, tgt);
    existEdge[eEnds] = tmp;
    edgeMapping.set(e.id, tmp);

    if (weights != NULL) {
      inW[tmp] = weights->getEdgeDoubleValue(e);
    }
    else {
      inW[tmp] = 1.0;
    }
  }
  //add reverse edges
  // new edges of g are added at the end of its edges underlying vector
  // so there is no need of a costly stableForEach
  unsigned int nbEdges = g.numberOfEdges();

  for (unsigned int i = 0; i < nbEdges; ++i) {
    e = g(i);
    const std::pair<node, node>& eEnds = g.ends(e);
    edge tmp = g.addEdge(eEnds.second, eEnds.first);
    existEdge[pair<node, node>(eEnds.second, eEnds.first)] = tmp;
    inW[tmp] = inW[e];
  }

  //add loops (Set the maximum of out-edges weights to self-loops weight)
  unsigned int nbNodes = g.numberOfNodes();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    n = g[i];
    edge tmp = g.addEdge(n, n);
    existEdge[pair<node, node>(n, n)] = tmp;
    edge e;
    double sum = 0.;
    inW[tmp]=1.;

    if(weights!=0) {
      double tmpVal = inW[tmp]=0.;
      forEach(e, g.getOutEdges(n)) {
        double eVal = inW[e];
        sum += eVal;

        if (eVal > tmpVal)
          tmpVal = eVal;
      }
      sum += (inW[tmp] = tmpVal);
    }
    else
      sum=double(g.outdeg(n));

    forEach(e, g.getOutEdges(n))
    inW[e] /= sum;
  }

  nbEdges = g.numberOfEdges();

  for (unsigned int i = 0; i < nbEdges; ++i) {
    outW[g(i)] = 0.;
  }

}
//================================================================================
void MCLClustering::bfs(node n, double value) {
  deque<node> fifo;
  MutableContainer<bool> flag;
  flag.setAll(false);
  fifo.push_back(n);
  flag.set(n, true);

  while(!fifo.empty()) {
    node n = fifo.front();
    resultN[n] = value;
    fifo.pop_front();
    const std::vector<node> &neighbours = g.adj(n);
    unsigned int nbNeighbours = neighbours.size();

    for (unsigned int i = 0; i <  nbNeighbours; ++i) {
      node ni = neighbours[i];

      if(!flag.get(ni)) {
        fifo.push_back(ni);
        flag.set(ni, true);
      }
    }
  }
}
//================================================================================
double MCLClustering::connectedComponent() {
  unsigned int nbNodes = g.numberOfNodes();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    resultN[g[i]] = -1.;
  }

  double curVal = 0.;

  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = g[i];

    if (resultN[n] < 0) {
      bfs(n, curVal);
      curVal += 1.;
    }
  }

  return curVal;
}
//================================================================================
struct DegreeSort {
  DegreeSort(VectorGraph &g):g(g) {}
  bool operator()(node a, node b) {
    unsigned int da = g.deg(a), db = g.deg(b);

    if (da == db)
      return a.id > b.id;

    return da > db;
  }
  VectorGraph &g;
};
//==============================================================================
bool MCLClustering::run() {


  g.alloc(inW);
  g.alloc(outW);
  g.alloc(tlpNodes);

  weights = NULL;
  _r = 2.;
  _k = 5;

  if(dataSet!=0) {
    dataSet->get("weights", weights);
    dataSet->get("inflate", _r);
    dataSet->get("pruning", _k);
  }

  init();
  unsigned int nbNodes = g.numberOfNodes();

  edge e;
  //output for mcl
  /*
  forEach(e, graph->getEdges()) {
      cout << graph->source(e).id << "\t" << graph->target(e).id << endl;
  }
  */

  int iteration = 15. * log(g.numberOfNodes() + 1);

  while(iteration-- > 0) {
    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = g[i];
      power(n);
      // comment the next line to have exact MCL
      inflate(_r, _k,  n, false);
    }

    /* exact MCL should inflate after because we share the same graphs tructure,
    * or we should only remove edges created during the power and delay the
    * deletion of edge that does exist in the previous graph
    * however that impletenation doesn't change the result too much.
    */
    //uncomment that block to have correct MCL

//        forEach(n, g.getNodes()) {
//            inflate(_r, _k,  n, false);
//        }

    EdgeProperty<double> tmp;
    tmp = inW;
    inW = outW;
    outW = tmp;

    if (equal()) break;

    unsigned int nbEdges = g.numberOfEdges();

    for (unsigned int i = 0; i < nbEdges; ++i)
      outW[g(i)] = 0.;
  }

  outW = inW;

  for (unsigned int i = 0; i < nbNodes; ++i) {
    pruneK(g[i], 1);
  }

  stableForEach(e, g.getEdges()) {
    if (inW[e] < epsilon) {
      g.delEdge(e);
    }
  }

  DegreeSort sortFunc(g);
  g.sortNodes(sortFunc); //sort nodes in decreasing order of their degree
  //node n;
  g.alloc(resultN);
  connectedComponent();
//    cout << "#clust : " << connectedComponnent() << endl << flush;


  //compute clusters
  //double piv = 0;
  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = g[i];
    //if(g.deg(n) > 1) piv += 1.;

    result->setNodeValue(tlpNodes[n], resultN[n]);
  }

//    cout << "#pivot : " << piv << endl << flush;

  return true;
}
