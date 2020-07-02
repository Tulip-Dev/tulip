/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <stack>
#include <queue>
#include <unordered_map>
#include <tulip/DoubleProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/GraphTools.h>
#include <tulip/StringCollection.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // directed
    "Indicates if the graph should be considered as directed or not.",

    // norm
    "If true the node measure will be normalized<br>"
    " - if not directed: m(n) = 2*c(n) / (#V - 1)(#V - 2)<br>"
    " - if directed    : m(n) = c(n) / (#V - 1)(#V - 2)<br>"
    "If true the edge measure will be normalized<br>"
    " - if not directed: m(e) = 2*c(e) / (#V / 2)(#V / 2)<br>"
    " - if directed    : m(e) = c(e) / (#V / 2)(#V / 2)",

    // weight
    "An existing edge weight metric property.",

    // Average path length
    "The computed average path length",
    // target
    "Indicates whether the metric is computed only for nodes, edges, or both."};

#define TARGET_TYPE "target"
#define TARGET_TYPES "both;nodes;edges"
#define NODES_TARGET 1
#define EDGES_TARGET 2
#define BOTH_TARGET 0

/** \addtogroup metric */

/** This plugin is an implementation of betweenness centrality parameter.
 *  (see http://en.wikipedia.org/wiki/Centrality#Betweenness_centrality for more details)
 *
 *  Algorithm published by:
 *
 *  U. Brandes, \n
 *  "A Faster Algorithm for Betweenness Centrality", \n
 *  "Journal of Mathematical Sociology" \n
 *  "2001", \n
 *  volume 25, \n
 *  pages 163-177
 *
 *  The edge betweenness centrality is also computed, it is described in:
 *
 *  Newman, M. E. J. and Girvan, M. \n
 *  "Finding and evaluating community structure in networks",  \n
 *  "Physics Reviews E",  \n
 *  "2004",  \n
 *  volume 69
 *
 *
 *  \note The complexity of the algorithm is O(|V| * |E|) in time
 *        on unweighted graphs and O(|V||E| + |V|^2 log |V|) on
 *        weighted graphs.
 *
 *  <b>HISTORY</b>
 *
 *  - 26/04/19 Version 1.3: Weighted version
 *  - 16/02/11 Version 1.2: Edge betweenness computation added
 *  - 08/02/11 Version 1.1: Normalisation option added
 *  - 03/01/05 Version 1.0: Initial release
 *
 */
class BetweennessCentrality : public DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Betweenness Centrality", "David Auber", "03/01/2005",
      "Computes the betweeness centrality as described for<ul>"
      "<li>nodes in <b>A Faster Algorithm for Betweenness Centrality</b>, U. Brandes, Journal of "
      "Mathematical Sociology volume 25, pages 163-177 (2001)</li>"
      "<li>edges in <b>Finding and evaluating community structure in networks</b>, M. E. J. Newman "
      "and M. Girvan, Physics Reviews E, volume 69 (2004).</li></ul>"
      "The average path length is alo computed.",
      "1.4", "Graph")
  BetweennessCentrality(const PluginContext *context) : DoubleAlgorithm(context) {
    addInParameter<bool>("directed", paramHelp[0], "false");
    addInParameter<bool>("norm", paramHelp[1], "false", false);
    addInParameter<NumericProperty *>("weight", paramHelp[2], "", false);
    addOutParameter<double>("average path length", paramHelp[3], "");
    addInParameter<StringCollection>(TARGET_TYPE, paramHelp[4], TARGET_TYPES, true,
                                     "both <br> nodes <br> edges");
    // result needs to be an inout parameter
    // in order to preserve the original values of non targeted elements
    // i.e if "target" = "nodes", the values of edges must be preserved
    // and if "target" = "edges", the values of nodes must be preserved
    parameters.setDirection("result", INOUT_PARAM);
  }

  bool run() override {
    bool directed = false;
    bool norm = false;
    NumericProperty *weight = nullptr;
    bool nodes(true), edges(true);

    if (dataSet != nullptr) {
      dataSet->get("directed", directed);
      dataSet->get("norm", norm);
      dataSet->get("weight", weight);
      StringCollection targetType;
      dataSet->get(TARGET_TYPE, targetType);

      if (targetType.getCurrent() == NODES_TARGET) {
        edges = false;
        nodes = true;
      } else if (targetType.getCurrent() == EDGES_TARGET) {
        edges = true;
        nodes = false;
      } else {
        edges = true;
        nodes = true;
      }
    }
    if (nodes)
      result->setAllNodeValue(0.0);
    if (edges)
      result->setAllEdgeValue(0.0);

    // Metric is 0 in this case
    if (graph->numberOfNodes() <= 2)
      return true;

    // Edges weights should be positive
    if (weight && weight->getEdgeDoubleMin() <= 0) {
      pluginProgress->setError("Edges weights should be positive.");
      return false;
    }

    double avg_path_length = 0.;
    unsigned int nbNodes = graph->numberOfNodes();
    unsigned int count = 0;

    pluginProgress->showPreview(false);

    for (auto s : graph->nodes()) {

      if (((++count % 50) == 0) && (pluginProgress->progress(count, nbNodes) != TLP_CONTINUE))
        break;

      stack<node> S;
      unordered_map<node, list<node>> P;
      MutableContainer<int> sigma;

      if (weight)
        computeDijkstra(s, directed, weight, S, P, sigma);
      else
        computeBFS(s, directed, S, P, sigma);

      MutableContainer<double> delta;
      delta.setAll(0.0);

      while (!S.empty()) {
        node w = S.top();
        double wD = delta.get(w.id);
        S.pop();

        for (auto v : P[w]) {
          double vd = double(sigma.get(v.id)) / double(sigma.get(w.id)) * (1.0 + wD);
          delta.add(v.id, vd);
          edge e = graph->existEdge(v, w, directed);

          if (e.isValid()) {
            if (edges)
              result->setEdgeValue(e, result->getEdgeValue(e) + vd);
            if (weight)
              avg_path_length += vd * weight->getEdgeDoubleValue(e);
            else
              avg_path_length += vd;
          }
        }

        if (nodes && w != s)
          result->setNodeValue(w, result->getNodeValue(w) + wD);
      }
    }

    if (pluginProgress->state() != TLP_CONTINUE)
      return pluginProgress->state() != TLP_CANCEL;

    // Normalization
    if (norm || !directed) {
      auto n = graph->numberOfNodes();
      double factor = norm ? (1.0 / ((n - 1) * (n - 2))) : 1.0;
      if (!directed)
        // In the undirected case, the metric must be divided by two, then
        factor *= 0.5;

      if (nodes) {
        for (auto n : graph->nodes())
          result->setNodeValue(n, result->getNodeValue(n) * factor);
      }
      if (edges) {
        factor = norm ? (4.0 / (n * n)) : 1.0;
        if (!directed)
          // In the undirected case, the metric must be divided by two, then
          factor *= 0.5;

        for (auto e : graph->edges())
          result->setEdgeValue(e, result->getEdgeValue(e) * factor);
      }
    }
    avg_path_length /= (nbNodes * (nbNodes - 1.));
    dataSet->set("average path length", avg_path_length);

    return pluginProgress->state() != TLP_CANCEL;
  }

private:
  void computeBFS(node s, bool directed, stack<node> &S, unordered_map<node, list<node>> &P,
                  MutableContainer<int> &sigma) {
    sigma.setAll(0);
    sigma.set(s.id, 1);
    MutableContainer<int> d;
    d.setAll(-1);
    d.set(s.id, 0);
    queue<node> Q;
    Q.push(s);

    while (!Q.empty()) {
      node v = Q.front();
      int vd = d.get(v.id);
      int vs = sigma.get(v.id);
      Q.pop();
      S.push(v);

      for (auto w : (directed ? graph->getOutNodes(v) : graph->getInOutNodes(v))) {
        int wd = d.get(w.id);

        if (wd < 0) {
          Q.push(w);
          d.set(w.id, wd = vd + 1);
        }

        if (wd == vd + 1) {
          sigma.add(w.id, vs);
          P[w].push_back(v);
        }
      }
    }
  }

  void computeDijkstra(node s, bool directed, NumericProperty *weight, stack<node> &S,
                       unordered_map<node, list<node>> &P, MutableContainer<int> &sigma) {
    EdgeStaticProperty<double> eWeights(graph);
    eWeights.copyFromNumericProperty(weight);
    NodeStaticProperty<double> nodeDistance(graph);
    tlp::computeDijkstra(graph, s, eWeights, nodeDistance, directed ? DIRECTED : UNDIRECTED, P, &S,
                         &sigma);
  }
};

PLUGIN(BetweennessCentrality)
