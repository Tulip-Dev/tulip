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
#include <algorithm>
#include <sstream>
#include <string>
#include <queue>
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
  bool inflate(double r, unsigned int k, node n, bool equal/*, bool noprune*/);
  void prune(node n);
  //void pruneT(node n);

  bool equal();
  void init();
  void power(node n);

  VectorGraph g;
  EdgeProperty<double> inW, outW;
  NodeProperty<node> tlpNodes;
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
void MCLClustering::power(node n) {
  edge e1;
  
  stableForEach(e1, g.getOutEdges(n)) {
    double v1 = inW[e1];

    if (v1 > epsilon) {
      edge e2;
      stableForEach(e2, g.getOutEdges(g.target(e1))) {
        double v2 = inW[e2] * v1;

        if (v2 > epsilon) {
	  node tgt = g.target(e2);
          edge ne = g.existEdge(n, tgt, true);
	  if (ne.isValid())
	    outW[ne] += v2;
	  else {
	    ne = g.addEdge(n, tgt);
	    inW[ne] = 0.;
	    outW[ne] = v2;
	  }
        }
      }
    }
  }
}
//==================================================
struct pvectCmp {
  bool operator()(const std::pair<double, edge>& p1,
		  const std::pair<double, edge>& p2) {
    return p1.first < p2.first;
  }   
};

void MCLClustering::prune(node n) {
  unsigned int outdeg = g.outdeg(n);
  if (outdeg == 0) return;

  // we use a specific vector to hold out edges needed infos
  // in order to
  // - improve the locality of reference
  // - ease the sort of the out edges according to their outW value
  // - avoid a costly stableForEach when deleting edges
  std::vector<pair<double, edge> > pvect;
  pvect.reserve(outdeg);
  edge e;
  forEach(e, g.getOutEdges(n)) {
    pvect.push_back(pair<double, edge>(outW[e], e));
  }

  std::sort(pvect.begin(), pvect.end(), pvectCmp());
  double t = pvect[outdeg - 1].first;
  for (unsigned int i = 0; i < outdeg; ++i) {
    pair<double, edge>& p = pvect[i];
    if (p.first < t || inW[p.second] < epsilon)
      g.delEdge(p.second);
  }
}
//=================================================
bool MCLClustering::inflate(double r, unsigned int k, node n, bool equal
			    /*, bool noprune */) {
  unsigned int sz = g.outdeg(n);
  // we use a specific vector to hold out edges needed infos
  // in order to
  // - improve the locality of reference
  // - ease the sort of the out edges according to their outW value
  // - avoid a costly stableForEach when deleting edges
  std::vector<pair<double, edge> > pvect;
  pvect.reserve(sz);

  edge e;
  double sum  = 0.;
  forEach(e, g.getOutEdges(n)) {
    double outVal = outW[e];
    sum += pow(outVal, r);
    pvect.push_back(pair<double, edge>(outVal, e));
  }

  if (sum > 0.) {
    for(unsigned int i = 0; i < sz; ++i) {
      pair<double, edge>& p = pvect[i];
      p.first = outW[p.second] = pow(p.first, r) / sum;
    }
  }

  /*if (noprune)
    return;*/

  // pruneK step
  std::sort(pvect.begin(), pvect.end(), pvectCmp());
  double t = pvect[sz - 1].first;
  --k;
  unsigned int outdeg = sz;
  for (int i = sz - 2; i > 0; --i) {
    pair<double, edge>& p = pvect[i];
    if (k) {
      if (p.first < t) {
	--k;
	t = p.first;
      }
    }
    else if (p.first < t) {
      e = p.second;
      inW[e]  = 0.;
      outW[e] = 0.;
      g.delEdge(e);
      // put an invalid edge
      // to avoid any further computation
      // for this pvect elt
      p.second = edge();
      --outdeg;
    }
  }

  // makeStoc step
  sum = 0.;
  for (unsigned int i = 0; i < sz; ++i) {
    pair<double, edge>& p = pvect[i];
    if (p.second.isValid())
      sum += p.first;
  }

  if (sum > 0.) {
    for (unsigned int i = 0; i < sz; ++i) {
      pair<double, edge>& p = pvect[i];
      e = p.second;
      if (e.isValid()) {
	double outVal = outW[e] = p.first / sum;
	if (equal && (fabs(outVal - inW[e]) > epsilon))
	  // more iteration needed
	  equal = false;
      }
    }
  }
  else {
    for (unsigned int i = 0; i < sz; ++i) {
      pair<double, edge>& p = pvect[i];
      e = p.second;
      if (e.isValid()) {
	double outVal = outW[e] = 1. / double(outdeg);
	if (equal && (fabs(outVal - inW[e]) > epsilon))
	  // more iteration needed
	  equal = false;
      }
    }
  }
  return equal;
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
  node newNode;
  forEach(n, graph->getNodes()) {
    newNode = g.addNode();
    nodeMapping.set(n.id, newNode);
    tlpNodes[newNode] = n;
  }

  edge e;
  forEach(e, graph->getEdges()) {
    std::pair<node, node> eEnds = graph->ends(e);
    node src = eEnds.first = nodeMapping.get(eEnds.first.id);
    node tgt = eEnds.second = nodeMapping.get(eEnds.second.id);
    edge tmp = g.addEdge(src, tgt);
    edgeMapping.set(e.id, tmp);

    double weight = (weights != NULL) ? weights->getEdgeDoubleValue(e) : 1.0;
    inW[tmp] = weight;
    outW[tmp] = 0.;
    // add reverse edge
    tmp = g.addEdge(tgt, src);
    inW[tmp] = weight;
    outW[tmp] = 0.;
  }

  // add loops (Set the maximum of out-edges weights to self-loops weight)
  unsigned int nbNodes = g.numberOfNodes();
  for (unsigned int i = 0; i < nbNodes; ++i) {
    n = g[i];
    edge tmp = g.addEdge(n, n);
    edge e;
    double sum = 0.;
    outW[tmp] = 0.;

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
    else {
      inW[tmp] = 1.;
      sum=double(g.outdeg(n));
    }

    forEach(e, g.getOutEdges(n))
      inW[e] /= sum;
  }
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
    bool equal = true;
    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = g[i];
      power(n);
      // comment the next line to have exact MCL
      if (inflate(_r, _k,  n, equal /*, false*/) == false)
	equal = false;
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

    if (equal) break;

    outW.setAll(0.);
  }

  // outW is no longer needed
  g.free(outW);
  
  outW = inW;

  for (unsigned int i = 0; i < nbNodes; ++i) {
    prune(g[i]);
  }

  DegreeSort sortFunc(g);
  g.sortNodes(sortFunc); //sort nodes in decreasing order of their degree

  NodeProperty<bool> visited;
  g.alloc(visited);
  visited.setAll(false);

  double curVal = 0.;
  // connected component loop
  // set the same value to all connected nodes 
  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = g[i];

    if (!visited[n]) {
      queue<node> fifo;
      fifo.push(n);
      visited[n] = true;

      while(!fifo.empty()) {
	node n = fifo.front();
	result->setNodeValue(tlpNodes[n], curVal);
	fifo.pop();
	const std::vector<node> &neighbours = g.adj(n);
	unsigned int nbNeighbours = neighbours.size();

	for (unsigned int i = 0; i <  nbNeighbours; ++i) {
	  node ni = neighbours[i];

	  if(!visited[ni]) {
	    fifo.push(ni);
	    visited[ni] = true;
	  }
	}
      }
      curVal += 1.;
    }
  }

  return true;
}
