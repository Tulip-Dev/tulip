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

#include <stack>
#include <queue>
#include <tulip/tuliphash.h>
#include <tulip/DoubleProperty.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
  // directed
  "Indicates if the graph should be considered as directed or not.",

  // norm
  "If true the node measure will be normalized<br>"
  " - if not directed : m(n) = 2*c(n) / (#V - 1)(#V - 2)<br>"
  " - if directed     : m(n) = c(n) / (#V - 1)(#V - 2)<br>"
  "If true the edge measure will be normalized<br>"
  " - if not directed : m(e) = 2*c(e) / (#V / 2)(#V / 2)<br>"
  " - if directed     : m(e) = c(e) / (#V / 2)(#V / 2)"
};

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
 *  The edge betweenness centrality is also computed, it is described in :
 *
 *  Newman, M. E. J. and Girvan, M. \n
 *  "Finding and evaluating community structure in networks",  \n
 *  "Physics Reviews E",  \n
 *  "2004",  \n
 *  volume 69
 *
 *
 *  \note The complexity of the algorithm is O(|V| * |E|) in time
 *  <b>HISTORY</b>
 *
 *  - 16/02/11 Version 1.2: Edge betweenness computation added
 *  - 08/02/11 Version 1.1: Normalisation option added
 *  - 03/01/05 Version 1.0: Initial release
 *
 */
class BetweennessCentrality:public DoubleAlgorithm {
public:
  PLUGININFORMATION("Betweenness Centrality","David Auber","03/01/2005","Computes the betweenness centrality.","1.2","Graph")
  BetweennessCentrality(const PluginContext* context):DoubleAlgorithm(context) {
    addInParameter<bool>("directed", paramHelp[0], "false");
    addInParameter<bool>("norm", paramHelp[1], "false", false);
  }
  bool run() {
    result->setAllNodeValue(0.0);
    result->setAllEdgeValue(0.0);
    bool directed = false;
    bool norm = false;

    if ( dataSet!=NULL ) {
      dataSet->get("directed",directed);
      dataSet->get("norm", norm);
    }

    //Metric is 0 in this case
    if(graph->numberOfNodes()<=2) return true;

    Iterator<node> *it = graph->getNodes();
    unsigned int nbNodes = graph->numberOfNodes();
    unsigned int count = 0;

    pluginProgress->showPreview(false);

    while(it->hasNext()) {
      if (((++count % 50) == 0) &&
          (pluginProgress->progress(count, nbNodes)!=TLP_CONTINUE))
        break;

      node s = it->next();
      stack<node> S;
      TLP_HASH_MAP<node, list<node> > P;
      MutableContainer<int> sigma;
      sigma.setAll(0);
      sigma.set(s.id,1);
      MutableContainer<int> d;
      d.setAll(-1);
      d.set(s.id, 0);
      queue<node> Q;
      Q.push(s);

      while(!Q.empty()) {
        node v = Q.front();
        int vd = d.get(v.id);
        int vs = sigma.get(v.id);
        Q.pop();
        S.push(v);
        Iterator<node> *it2;

        if (directed)
          it2 = graph->getOutNodes(v);
        else
          it2 = graph->getInOutNodes(v);

        while (it2->hasNext()) {
          node w = it2->next();
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

        delete it2;
      }

      MutableContainer<double> delta;
      delta.setAll(0.0);

      while (!S.empty()) {
        node w = S.top();
        double wD = delta.get(w.id);
        S.pop();
        list<node>::const_iterator itn = P[w].begin();

        for (; itn!=P[w].end(); ++itn) {
          node v = *itn;
          double vd =
            double(sigma.get(v.id))/double(sigma.get(w.id)) * (1.0 + wD);
          delta.add(v.id, vd);
          edge e = graph->existEdge(v,w,directed);

          if(e.isValid())
            result->setEdgeValue(e, result->getEdgeValue(e) + vd);
        }

        if (w != s)
          result->setNodeValue(w, result->getNodeValue(w) + wD);
      }
    }

    delete it;

    //Normalization
    if(norm || !directed) {
      double n = graph->numberOfNodes();
      const double nNormFactor = 1./(double)((n - 1) * (n - 2));
      it = graph->getNodes();

      while(it->hasNext()) {
        node s = it->next();

        //In the undirected case, the metric must be divided by two, then
        if(norm)
          result->setNodeValue(s,result->getNodeValue(s) * nNormFactor);

        if(!directed)
          result->setNodeValue(s,result->getNodeValue(s) * 0.5);
      }

      delete it;

      Iterator<edge> *itE = graph->getEdges();
      const double eNormFactor = 4./(double)(n * n);

      while(itE->hasNext()) {
        edge e = itE->next();

        if(norm)
          result->setEdgeValue(e,result->getEdgeValue(e) * eNormFactor);

        if(!directed)
          result->setEdgeValue(e,result->getEdgeValue(e) * 0.5);
      }

      delete itE;
    }


    return pluginProgress->state()!=TLP_CANCEL;
  }
};

PLUGIN(BetweennessCentrality)
