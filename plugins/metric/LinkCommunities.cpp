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
#ifdef _OPENMP
#include <omp.h>
#endif
#include <set>
#include <tulip/DoubleProperty.h>
#include <tulip/vectorgraph.h>
#include <tulip/vectorgraphproperty.h>
#include <tulip/ForEach.h>

using namespace std;
using namespace tlp;

/** \addtogroup clustering */
/*@{*/
/** \file
* @brief This plugin is an implementation of a fuzzy clustering procedure. First introduced in :
*
* Ahn, Y.Y. and Bagrow, J.P. and Lehmann, S., \n
* "Link communities reveal multiscale complexity in networks", \n
* in Nature vol:466, \n
* pages 761--764, \n
* 2010 \n
*
* The result of this procedure is saved as an edge metric : two edges share the same value
* if they are part of the same group.
* The result for a node shows the number of groups to which it belongs.
*
* @note To create subgraphs using the result of this algortihm use "Equal Value" with parameter Type="edges".
*
* @todo Deal with directed graphs.
*
**/
class LinkCommunities : public tlp::DoubleAlgorithm {
public:

  PLUGININFORMATION("Link Communities","François Queyroi","25/02/11",
                    "Edges partitioning measure used for community detection.<br>"
                    "It is an implementation of a fuzzy clustering procedure. First introduced in :<br>"
                    " <b>Link communities reveal multiscale complexity in networks</b>, Ahn, Y.Y. and Bagrow, J.P. and Lehmann, S., Nature vol:466, 761--764 (2010)",
                    "1.0","Clustering")

  LinkCommunities(const tlp::PluginContext *);
  ~LinkCommunities();
  bool run();
private:
  /**
  * @brief Create the dual (as VectorGraph) of the graph
  * in order to store Similarity value between two edges
  * Edges are represented by nodes linked according to edges' neighborhood.
  **/
  void createDualGraph(const std::vector<edge>& edges);
  /**
  * @brief Compute all similarities between all pairs of adjacent edges.
  **/
  void computeSimilarities(const std::vector<edge>& edges);
  /**
  * @brief Compute similarity (Jaccard) between graph->edges()[source(e).id] and graph->edges()[target(e).id]
  **/
  double getSimilarity(tlp::edge e, const std::vector<edge>& edges);
  /**
  * @brief Compute weighted (Tanimoto) similarity between graph->edges()[source(e).id] and graph->edges()[target(e).id]
  **/
  double getWeightedSimilarity(tlp::edge e, const std::vector<edge>& edges);
  /**
  * @brief Perform #(step) single linkage clustering in order to find the partition
  * which maximise the average density
  **/
  double findBestThreshold(unsigned int, const std::vector<edge>& edges);
  /**
  * @brief Compute the partition of dual node for the given threshold value
  * and return average density of this edge partition
  **/
  double computeAverageDensity(double, const std::vector<edge>& edges);
  /**
  * @brief set edge values according the partition corresponding
  * to the best threshold
  **/
  void setEdgeValues(double, bool, const std::vector<edge>& edges);

  tlp::VectorGraph dual; // Dual Node -> Graph Edges; Dual Edge -> indicates that the linked Graph Edges have a same end.
  tlp::MutableContainer<tlp::node> mapKeystone;
  tlp::EdgeProperty<double> similarity;

  tlp::NumericProperty* metric;
};
/*@}*/

//==============================================================================================================
static const char *paramHelp[] = {
  // metric
  "An existing edge metric property.",

  // Group isthmus
  "This parameter indicates whether the single-link clusters should be merged or not.",

  // Number of steps
  "This parameter indicates the number of thresholds to be compared."
};
//==============================================================================================================
LinkCommunities::LinkCommunities(const tlp::PluginContext *context) : DoubleAlgorithm(context), metric(NULL) {
  addInParameter<NumericProperty*>("metric", paramHelp[0],"",false);
  addInParameter<bool>("Group isthmus", paramHelp[1],"true",true);
  addInParameter<unsigned int>("Number of steps",paramHelp[2],"200",true);
}
//==============================================================================================================
LinkCommunities::~LinkCommunities() {}
//==============================================================================================================
bool LinkCommunities::run() {
  metric = NULL;
  bool group_isthmus = true;
  unsigned int nb_steps = 200;

  if(dataSet!=0) {
    dataSet->get("metric",metric);
    dataSet->get("Group isthmus",group_isthmus);
    dataSet->get("Number of steps",nb_steps);
  }

  const std::vector<edge>& edges = graph->edges();
  createDualGraph(edges);

  dual.alloc(similarity);
  computeSimilarities(edges);

  result->setAllNodeValue(0);
  result->setAllEdgeValue(0);
  double th = findBestThreshold(nb_steps, edges);

  setEdgeValues(th, group_isthmus, edges);

  dual.free(similarity);
  dual.clear();

  const std::vector<node>& nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = nodes[i];
    std::set<double> around;
    edge e;
    forEach(e,graph->getInOutEdges(n)) {
      double val = result->getEdgeValue(e);

      if (val)
        around.insert(val);
    }
    result->setNodeValue(n,around.size());
  }

  return true;
}
//==============================================================================================================
void LinkCommunities::createDualGraph(const std::vector<edge>& edges) {
  unsigned int nbEdges = edges.size();
  dual.reserveNodes(nbEdges);

  for (unsigned int i = 0; i < nbEdges; ++i) {
    node dn = dual.addNode();
    const std::pair<node, node> eEnds = graph->ends(edges[i]);
    node src = eEnds.first;
    node tgt = eEnds.second;
    edge ee;
    forEach(ee, graph->getInOutEdges(src)) {
      unsigned int eePos = graph->edgePos(ee);

      if (eePos < i) {
        if(!dual.existEdge(dn, dual[eePos], false).isValid()) {
          edge de = dual.addEdge(dn, dual[eePos]);
          mapKeystone.set(de.id, src);
        }
      }
    }
    forEach(ee, graph->getInOutEdges(tgt)) {
      unsigned int eePos = graph->edgePos(ee);

      if (eePos < i) {
        if(!dual.existEdge(dn, dual[eePos], false).isValid()) {
          edge de = dual.addEdge(dn, dual[eePos]);
          mapKeystone.set(de.id,tgt);
        }
      }
    }
  }
}
//==============================================================================================================
void LinkCommunities::computeSimilarities(const std::vector<edge>& edges) {
  if(metric == NULL) {
#ifdef _OPENMP
    #pragma omp parallel for
#endif

    for(int i=0; i<(int)dual.numberOfEdges(); ++i) { //use int for MSVS2010 compilation
      edge e = dual(i);
      similarity[e]=getSimilarity(e, edges);
    }
  }
  else
#ifdef _OPENMP
    #pragma omp parallel for
#endif
    for(int i=0; i<(int)dual.numberOfEdges(); ++i) { //use int for MSVS2010 compilation
      edge e = dual(i);
      similarity[e]=getWeightedSimilarity(e, edges);
    }
}
//==============================================================================================================
double LinkCommunities::getSimilarity(edge ee, const std::vector<edge>& edges) {
  node key = mapKeystone.get(ee.id);
  const std::pair<node, node> eeEnds = dual.ends(ee);
  edge e1=edges[eeEnds.first.id];
  edge e2=edges[eeEnds.second.id];
  const std::pair<node, node> e1Ends = graph->ends(e1);
  node n1 = (e1Ends.first != key) ? e1Ends.first : e1Ends.second;
  const std::pair<node, node> e2Ends = graph->ends(e2);
  node n2 = (e2Ends.first != key) ? e2Ends.first : e2Ends.second;
  unsigned int wuv=0, m=0;
  node n;
  forEach(n,graph->getInOutNodes(n1)) {
    if(graph->existEdge(n2,n,true).isValid())
      wuv+=1;

    if(graph->existEdge(n,n2,true).isValid())
      wuv+=1;

    m+=1.0;
  }

  forEach(n,graph->getInOutNodes(n2)) {
    if(!graph->existEdge(n1,n,false).isValid())
      m+=1;
  }

  if(graph->existEdge(n1,n2,false).isValid())
    wuv+=2;

  if(m>0)
    return (double)wuv / (double)m;
  else
    return 0.0;
}
//==============================================================================================================
double LinkCommunities::getWeightedSimilarity(tlp::edge ee,
    const std::vector<edge>& edges) {
  node key = mapKeystone.get(ee.id);
  const std::pair<node, node> eeEnds = dual.ends(ee);
  edge e1=edges[eeEnds.first.id];
  edge e2=edges[eeEnds.second.id];
  const std::pair<node, node>& e1Ends = graph->ends(e1);
  node n1 = (e1Ends.first != key) ? e1Ends.first : e1Ends.second;
  const std::pair<node, node>& e2Ends = graph->ends(e2);
  node n2 = (e2Ends.first != key) ? e2Ends.first : e2Ends.second;

  if(graph->deg(n1)>graph->deg(n2)) {
    node tmp=n1;
    n1=n2;
    n2=tmp;
  }

  double a1a2=0.0;
  double a1=0.0,a2=0.0;
  double a11=0.0,a22=0.0;
  edge e;
  forEach(e,graph->getInEdges(n1)) {
    double val = metric->getEdgeDoubleValue(e);
    node n=graph->source(e);
    edge me = graph->existEdge(n2,n,true);

    if(me.isValid())
      a1a2+=val*metric->getEdgeDoubleValue(me);

    me = graph->existEdge(n,n2,true);

    if(me.isValid())
      a1a2+=val*metric->getEdgeDoubleValue(me);

    a1+=val;
    a11+=val*val;
  }

  forEach(e,graph->getOutEdges(n1)) {
    double val = metric->getEdgeDoubleValue(e);
    node n=graph->target(e);
    edge me = graph->existEdge(n2,n,true);

    if(me.isValid())
      a1a2+=val*metric->getEdgeDoubleValue(me);

    me = graph->existEdge(n,n2,true);

    if(me.isValid())
      a1a2+=val*metric->getEdgeDoubleValue(me);

    a1+=val;
    a11+=val*val;
  }

  forEach(e,graph->getInOutEdges(n2)) {
    double val = metric->getEdgeDoubleValue(e);
    a2+=val;
    a22+=val*val;
  }
  a1/=graph->deg(n1);
  a11+=a1*a1;
  a2/=graph->deg(n2);
  a22+=a2*a2;

  e = graph->existEdge(n1,n2,false);

  if(e.isValid())
    a1a2+=metric->getEdgeDoubleValue(e)*(a1+a2);

  double m=a11+a22-a1a2;

  if(m<0.0)
    return 0.0;
  else
    return a1a2/m;
}
//==============================================================================================================
double LinkCommunities::computeAverageDensity(double threshold,
    const std::vector<edge>& edges) {
  double d = 0.0;
  NodeProperty<bool> dn_visited;
#ifdef _OPENMP
  #pragma omp critical(DN_VISITED)
#endif
  dual.alloc(dn_visited);
  dn_visited.setAll(false);

  unsigned int sz = dual.numberOfNodes();

  for(unsigned int i = 0; i < sz; ++i) {
    node dn = dual[i];

    if(!dn_visited[dn]) {
      unsigned int nbDNodes = 1;
      dn_visited[dn] = true;
      edge re = edges[dn.id];
      MutableContainer<bool> visited;
      unsigned int nbNodes = 1;
      const std::pair<node, node> reEnds = graph->ends(re);
      visited.set(reEnds.first.id, true);

      if (reEnds.first != reEnds.second) {
        visited.set(reEnds.second.id, true);
        nbNodes = 2;
      }

      list<node> dnToVisit;
      dnToVisit.push_front(dn);

      while (!dnToVisit.empty()) {
        dn=dnToVisit.front();
        dnToVisit.pop_front();
        const std::vector<edge>& curEdges = dual.star(dn);
        unsigned int eSz = curEdges.size();

        for(unsigned int j = 0; j < eSz; ++j) {
          edge e = curEdges[j];

          if(similarity[e]>threshold) {
            node neighbour = dual.opposite(e,dn);

            if (!dn_visited[neighbour]) {
              dn_visited[neighbour] = true;
              dnToVisit.push_back(neighbour);
              ++nbDNodes;
              edge re = edges[neighbour.id];
              const std::pair<node, node> reEnds = graph->ends(re);

              if (!visited.get(reEnds.first.id)) {
                visited.set(reEnds.first.id, true);
                ++nbNodes;
              }

              if (!visited.get(reEnds.second.id)) {
                visited.set(reEnds.second.id, true);
                ++nbNodes;
              }
            }
          }
        }
      }

      if (nbNodes >= 3) {
        double mc = nbDNodes;
        double nc = nbNodes;
        double density = (mc-nc+1)/(nc*(nc-1)/2.0-nc+1);
        d += nbDNodes * density;
      }
    }
  }

#ifdef _OPENMP
  #pragma omp critical(DN_VISITED)
#endif
  dual.free(dn_visited);

  return 2.0*d/(graph->numberOfEdges());
}
//==============================================================================================================
void LinkCommunities::setEdgeValues(double threshold, bool group_isthmus, const std::vector<edge>& edges) {
  NodeProperty<bool> dn_visited;
  dual.alloc(dn_visited);
  dn_visited.setAll(false);

  double val = 1;
  unsigned int sz = dual.numberOfNodes();

  for(unsigned int i = 0; i < sz; ++i) {
    node dn = dual[i];

    if(!dn_visited[dn]) {
      dn_visited[dn] = true;
      vector<node> component;
      component.push_back(dn);

      list<node> dnToVisit;
      dnToVisit.push_front(dn);

      while (!dnToVisit.empty()) {
        dn=dnToVisit.front();
        dnToVisit.pop_front();
        const std::vector<edge>& curEdges = dual.star(dn);
        unsigned int eSz = curEdges.size();

        for(unsigned int j = 0; j < eSz; ++j) {
          edge e = curEdges[j];

          if(similarity[e]>threshold) {
            node neighbour = dual.opposite(e,dn);

            if (!dn_visited[neighbour]) {
              dn_visited[neighbour] = true;
              dnToVisit.push_back(neighbour);
              component.push_back(neighbour);
            }
          }
        }
      }

      if (component.size() >= 2 || group_isthmus == false) {
        vector<node>::const_iterator ite;

        for(ite=component.begin(); ite!=component.end(); ++ite) {
          edge re = edges[ite->id];
          result->setEdgeValue(re, val);
        }
      }

      val += 1;
    }
  }

  dual.free(dn_visited);
}
//==============================================================================================================
double LinkCommunities::findBestThreshold(unsigned int numberOfSteps,
    const std::vector<edge>& edges) {
  double maxD=-2;
  double threshold = 0.0;

  double min = 1.1;
  double max = -1.0;

  unsigned int sz = dual.numberOfEdges();

  for(unsigned int i = 0; i < sz; ++i) {
    double value = similarity[dual(i)];

    if (value < min)
      min = value;
    else if (value > max)
      max = value;
  }

  double deltaThreshold = (max-min)/double(numberOfSteps);

#ifdef _OPENMP
  #pragma omp parallel for
#endif

  for (unsigned int i=0; i< numberOfSteps; i++) {
    double step = min + i * deltaThreshold;
    double d = computeAverageDensity(step, edges);
#ifdef _OPENMP
    #pragma omp critical
#endif
    {
      if ( d > maxD) {
        threshold=step;
        maxD=d;
      }
    }
  }

  return threshold;
}
//==============================================================================================================
PLUGIN(LinkCommunities)
