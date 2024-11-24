/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <algorithm>
#include <sstream>
#include <string>
#include <queue>
#include <tulip/DoubleProperty.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/GraphMeasure.h>
#include <tulip/vectorgraph.h>

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
 *
 **/
class MCLClustering : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "MCL Clustering", "D. Auber & R. Bourqui", "10/10/2005",
      "Nodes partitioning measure of Markov Cluster algorithm<br/>used for community detection."
      "This is an implementation of the MCL algorithm first published as:<br/>"
      "<b>Graph Clustering by Flow Simulation</b>,<br/>Stijn van Dongen PhD Thesis, University of "
      "Utrecht (2000).",
      "1.1", "Clustering")

  MCLClustering(const tlp::PluginContext *);
  ~MCLClustering() override;
  bool run() override;
  bool inflate(double r, unsigned int k, node n, bool equal /*, bool noprune*/);
  void prune(node n);
  // void pruneT(node n);

  bool equal();
  void init();
  void power(node n);

  VectorGraph g;
  EdgeProperty<double> inW, outW;
  NumericProperty *weights;
  double _r;
  unsigned int _k;
};
/*@}*/

const double epsilon = 1E-9;

//=================================================
void MCLClustering::power(node n) {
  tlp_hash_map<node, double> newTargets;

  for (auto &adjn : g.adj(n)) {
    if (!adjn.isOut())
      continue;
    double vn = inW[adjn.link()];

    if (vn > epsilon) {
      for (auto &adjo : g.adj(adjn.opposite())) {
        if (!adjo.isOut())
          continue;
        double v2 = inW[adjo.link()] * vn;

        if (v2 > epsilon) {
          node tgt = adjo.opposite();
          edge ne = g.existEdge(n, tgt, true);

          if (ne.isValid())
            outW[ne] += v2;
          else {
            tlp_hash_map<node, double>::iterator it = newTargets.find(tgt);

            if (it != newTargets.end())
              // newTargets[tgt] += v2;
              it->second += v2;
            else
              newTargets[tgt] = v2;
          }
        }
      }
    }
  }

  for (tlp_hash_map<node, double>::iterator it = newTargets.begin(); it != newTargets.end(); ++it) {
    edge ne;
    ne = g.addEdge(n, it->first);
    inW[ne] = 0.;
    outW[ne] = it->second;
  }
}
//==================================================
struct pvectCmp {
  bool operator()(const std::pair<double, edge> &p1, const std::pair<double, edge> &p2) {
    return p1.first < p2.first;
  }
};

void MCLClustering::prune(node n) {
  unsigned int outdeg = g.outdeg(n);

  if (outdeg == 0)
    return;

  // we use a specific vector to hold out edges needed info
  // in order to
  // - improve the locality of reference
  // - ease the sort of the out edges according to their outW value
  // - avoid a costly stable iteration when deleting edges
  std::vector<pair<double, edge>> pvect;
  pvect.reserve(outdeg);
  for (auto &adj : g.adj(n)) {
    if (!adj.isOut())
      continue;
    auto e = adj.link();
    pvect.push_back(pair<double, edge>(outW[e], e));
  }

  std::sort(pvect.begin(), pvect.end(), pvectCmp());
  double t = pvect[outdeg - 1].first;

  for (unsigned int i = 0; i < outdeg; ++i) {
    pair<double, edge> p = pvect[i];

    if (p.first < t || inW[p.second] < epsilon)
      g.delEdge(p.second);
  }
}
//=================================================
bool MCLClustering::inflate(double r, unsigned int k, node n, bool equal
                            /*, bool noprune */) {
  unsigned int sz = g.outdeg(n);
  // we use a specific vector to hold out edges needed info
  // in order to
  // - improve the locality of reference
  // - ease the sort of the out edges according to their outW value
  // - avoid a costly stable iteration when deleting edges
  std::vector<pair<double, edge>> pvect;
  pvect.reserve(sz);

  double sum = 0.;
  for (auto &adj : g.adj(n)) {
    if (!adj.isOut())
      continue;
    auto e = adj.link();
    double outVal = outW[e];
    sum += pow(outVal, r);
    pvect.push_back(pair<double, edge>(outVal, e));
  }

  if (sum > 0.) {
    double oos = 1. / sum;

    for (unsigned int i = 0; i < sz; ++i) {
      pair<double, edge> &p = pvect[i];
      p.first = outW[p.second] = pow(p.first, r) * oos;
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
    pair<double, edge> &p = pvect[i];

    if (k) {
      if (p.first < t) {
        --k;
        t = p.first;
      }
    } else if (p.first < t) {
      edge e = p.second;
      inW[e] = 0.;
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
    pair<double, edge> &p = pvect[i];

    if (p.second.isValid())
      sum += p.first;
  }

  if (sum > 0.) {
    double oos = 1. / sum;

    for (unsigned int i = 0; i < sz; ++i) {
      pair<double, edge> &p = pvect[i];
      edge e = p.second;

      if (e.isValid()) {
        double outVal = outW[e] = p.first * oos;

        if (equal && (fabs(outVal - inW[e]) > epsilon))
          // more iteration needed
          equal = false;
      }
    }
  } else {
    double ood = 1. / outdeg;

    for (unsigned int i = 0; i < sz; ++i) {
      edge e = pvect[i].second;

      if (e.isValid()) {
        double outVal = outW[e] = ood;

        if (equal && (fabs(outVal - inW[e]) > epsilon))
          // more iteration needed
          equal = false;
      }
    }
  }

  return equal;
}
//=================================================
static const char *paramHelp[] = {
    // inflate
    "Determines the random walk length at each step.",

    // weights
    "Defines the metric used for edge weights.",

    // pruning
    "Determines, for each node, the number of strongest link kept at each iteration."};
//=================================================
MCLClustering::MCLClustering(const tlp::PluginContext *context)
    : DoubleAlgorithm(context), weights(nullptr), _r(2.0), _k(5) {
  addInParameter<double>("inflate", paramHelp[0], "2.", false);
  addInParameter<NumericProperty *>("metric", paramHelp[1], "", false);
  addInParameter<unsigned int>("pruning", paramHelp[2], "5", false);
}
//===================================================================================
MCLClustering::~MCLClustering() {}
//================================================================================
struct DegreeSort {
  DegreeSort(VectorGraph &g) : g(g) {}
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

  weights = nullptr;
  _r = 2.;
  _k = 5;

  if (dataSet != nullptr) {
    dataSet->get("metric", weights);
    dataSet->get("inflate", _r);
    dataSet->get("pruning", _k);
  }

  NodeStaticProperty<node> nodeMapping(graph);
  const std::vector<node> &tlpNodes = graph->nodes();
  unsigned int nbNodes = tlpNodes.size();
  g.addNodes(nbNodes);
  g.alloc(inW);
  g.alloc(outW);

  // add nodes to g
  TLP_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
    g.reserveAdj(nodeMapping[i] = node(i), 2 * graph->deg(n) + 1);
  });

  for (auto e : graph->edges()) {
    auto eEnds = graph->ends(e);
    node src = nodeMapping[graph->nodePos(eEnds.first)];
    node tgt = nodeMapping[graph->nodePos(eEnds.second)];
    edge tmp = g.addEdge(src, tgt);

    double weight = (weights != nullptr) ? weights->getEdgeDoubleValue(e) : 1.0;
    inW[tmp] = weight;
    outW[tmp] = 0.;
    // add reverse edge
    tmp = g.addEdge(tgt, src);
    inW[tmp] = weight;
    outW[tmp] = 0.;
  }

  // add loops (Set the maximum of out-edges weights to self-loops weight)
  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = g[i];
    edge tmp = g.addEdge(n, n);
    double sum = 0.;
    outW[tmp] = 0.;

    if (weights != nullptr) {
      double tmpVal = inW[tmp] = 0.;
      for (auto &adj : g.adj(n)) {
        if (!adj.isOut())
          continue;
        auto e = adj.link();
        double eVal = inW[e];
        sum += eVal;

        if (eVal > tmpVal)
          tmpVal = eVal;
      }
      sum += (inW[tmp] = tmpVal);
    } else {
      inW[tmp] = 1.;
      sum = double(g.outdeg(n));
    }

    double oos = 1. / sum;
    for (auto &adj : g.adj(n)) {
      if (!adj.isOut())
        continue;
      inW[adj.link()] *= oos;
    }
  }

  // output for mcl
  /*
    for(const edge &e : graph->edges()) {
    cout << graph->source(e).id << "\t" << graph->target(e).id << endl;
    }
  */

  int iteration = 15. * log1p(g.numberOfNodes());

  while (iteration-- > 0) {
    bool equal = true;

    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = g[i];
      power(n);

      // comment the next line to have exact MCL
      if (inflate(_r, _k, n, equal /*, false*/) == false)
        equal = false;
    }

    /* exact MCL should inflate after because we share the same graphs tructure,
     * or we should only remove edges created during the power and delay the
     * deletion of edge that does exist in the previous graph
     * however that impletenation doesn't change the result too much.
     */
    // uncomment that block to have correct MCL

    //        for(const node &n : g.nodes()) {
    //            inflate(_r, _k,  n, false);
    //        }

    inW.swap(outW);

    if (equal)
      break;

    outW.setAll(0.);
  }

  // outW is no longer needed
  g.free(outW);

  outW = inW;

  for (auto n : g.nodes()) {
    prune(n);
  }

  // outW is no longer needed
  g.free(outW);

  DegreeSort sortFunc(g);
  g.sortNodes(sortFunc); // sort nodes in decreasing order of their degree

  NodeProperty<bool> visited;
  g.alloc(visited);
  visited.setAll(false);

  double curVal = 0.;

  // connected component loop
  // set the same value to all connected nodes
  for (auto n : g.nodes()) {
    if (!visited[n]) {
      queue<node> fifo;
      fifo.push(n);
      visited[n] = true;

      while (!fifo.empty()) {
        node nq = fifo.front();
        result->setNodeValue(tlpNodes[nq.id], curVal);
        fifo.pop();
        for (auto &adj : g.adj(nq)) {
          auto ni = adj.opposite();
          if (!visited[ni]) {
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
//==============================================================================
PLUGIN(MCLClustering)
