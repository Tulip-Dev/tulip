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
#include <tulip/TulipPluginHeaders.h>
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
  void createDualGraph();
  /**
  * @brief Compute all similarities between all pairs of adjacent edges.
  **/
  void computeSimilarities();
  /**
  * @brief Compute similarity (Jaccard) between edges mapDNtoE[source(e)] and mapDNtoE[target(e)]
  **/
  double getSimilarity(tlp::edge e);
  /**
  * @brief Compute weighted (Tanimoto) similarity between edges mapDNtoE[source(e)] and mapDNtoE[target(e)]
  **/
  double getWeightedSimilarity(tlp::edge e);
  /**
  * @brief Perform #(step) single linkage clustering in order to find the partition
  * which maximise the average density
  **/
  double findBestThreshold(unsigned int);
  /**
  * @brief Compute the partition of dual node for the given threshold value
  * and return average density of this edge partition
  **/
  double computeAverageDensity(double);
  /**
  * @brief set edge values according the partition corresponding
  * to the best threshold
  **/
  void setEdgeValues(double, bool);

  tlp::VectorGraph dual; // Dual Node -> Graph Edges; Dual Edge -> indicates that the linked Graph Edges have a same end.
  tlp::NodeProperty<tlp::edge> mapDNtoE;
  tlp::MutableContainer<tlp::node> mapKeystone;
  tlp::EdgeProperty<double> similarity;


  tlp::NumericProperty* metric;
};
/*@}*/

//==============================================================================================================
PLUGIN(LinkCommunities)
//==============================================================================================================
namespace {
const char * paramHelp[] = {
  // metric
  HTML_HELP_OPEN()              \
  HTML_HELP_DEF( "type", "NumericProperty" )       \
  HTML_HELP_DEF( "value", "An existing edge metric" )                 \
  HTML_HELP_BODY()              \
  "An existing edge metric property"\
  HTML_HELP_CLOSE(),
  // Group isthmus
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "values", "[true, false]" ) \
  HTML_HELP_DEF( "default", "true" ) \
  HTML_HELP_BODY() \
  "This parameter indicates whether the single-link clusters should be merged or not." \
  HTML_HELP_CLOSE(),
  // Number of steps
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_DEF( "default", "200" ) \
  HTML_HELP_BODY() \
  "This parameter indicates the number of thresholds to be compared" \
  HTML_HELP_CLOSE(),
};
}
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

  dual.alloc(mapDNtoE);
  createDualGraph();

  dual.alloc(similarity);
  computeSimilarities();

  result->setAllNodeValue(0);
  result->setAllEdgeValue(0);
  double th = findBestThreshold(nb_steps);

  setEdgeValues(th, group_isthmus);

  dual.free(mapDNtoE);
  dual.free(similarity);
  dual.clear();

  node n;
  forEach(n,graph->getNodes()) {
    std::set<double> around;
    edge e;
    forEach(e,graph->getInOutEdges(n)) {
      double val = result->getEdgeValue(e);
      if (val && around.find(val)==around.end()) {
        around.insert(val);
      }
    }
    result->setNodeValue(n,around.size());
  }
  return true;
}
//==============================================================================================================
void LinkCommunities::createDualGraph() {
  dual.reserveNodes(graph->numberOfEdges());
  tlp::MutableContainer<node> mapEtoN;
  mapEtoN.setAll(node());
  edge e;
  forEach(e,graph->getEdges()) {
    node dn = dual.addNode();
    mapDNtoE[dn.id] = e;
    mapEtoN.set(e.id,dn);
    const std::pair<node, node> eEnds = graph->ends(e);
    node src = eEnds.first;
    node tgt = eEnds.second;
    edge ee;
    forEach(ee,graph->getInOutEdges(src)) {
      if(ee!=e) {
        if(mapEtoN.get(ee.id).isValid()) {
          if(!dual.existEdge(dn,mapEtoN.get(ee.id),false).isValid()) {
            edge de = dual.addEdge(dn,mapEtoN.get(ee.id));
            mapKeystone.set(de.id,src);
          }
        }
      }
    }
    forEach(ee,graph->getInOutEdges(tgt)) {
      if(ee!=e) {
        if(mapEtoN.get(ee.id).isValid()) {
          if(!dual.existEdge(dn,mapEtoN.get(ee.id),false).isValid()) {
            edge de = dual.addEdge(dn,mapEtoN.get(ee.id));
            mapKeystone.set(de.id,tgt);
          }
        }
      }
    }
  }
}
//==============================================================================================================
void LinkCommunities::computeSimilarities() {
  if(metric == NULL) {
#ifdef _OPENMP
    #pragma omp parallel for
#endif

    for(int i=0; i<(int)dual.numberOfEdges(); ++i) { //use int for MSVS2010 compilation
      edge e = dual(i);
      similarity[e]=getSimilarity(e);
    }
  }
  else
#ifdef _OPENMP
    #pragma omp parallel for
#endif
    for(int i=0; i<(int)dual.numberOfEdges(); ++i) { //use int for MSVS2010 compilation
      edge e = dual(i);
      similarity[e]=getWeightedSimilarity(e);
    }
}
//==============================================================================================================
double LinkCommunities::getSimilarity(edge ee) {
  node key = mapKeystone.get(ee.id);
  const std::pair<node, node> eeEnds = dual.ends(ee);
  edge e1=mapDNtoE[eeEnds.first];
  edge e2=mapDNtoE[eeEnds.second];
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
double LinkCommunities::getWeightedSimilarity(tlp::edge ee) {
  node key = mapKeystone.get(ee.id);
  const std::pair<node, node> eeEnds = dual.ends(ee);
  edge e1=mapDNtoE[eeEnds.first];
  edge e2=mapDNtoE[eeEnds.second];
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
double LinkCommunities::computeAverageDensity(double threshold) {
  double d = 0.0;
  tlp::MutableContainer<bool> dn_visited;
  dn_visited.setAll(false);

  unsigned int sz = dual.numberOfNodes();

  for(unsigned int i = 0; i < sz; ++i) {
    node dn = dual[i];

    if(!(dn_visited.get(dn))) {
      unsigned int nbDNodes = 1;
      dn_visited.set(dn.id, true);
      edge re = mapDNtoE[dn];
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

            if (!(dn_visited.get(neighbour.id))) {
              dn_visited.set(neighbour.id, true);
              dnToVisit.push_back(neighbour);
	      ++nbDNodes;
	      edge re = mapDNtoE[neighbour];
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
  return 2.0*d/(graph->numberOfEdges());
}
//==============================================================================================================
void LinkCommunities::setEdgeValues(double threshold, bool group_isthmus) {
  tlp::MutableContainer<bool> dn_visited;
  dn_visited.setAll(false);

  double val = 1;
  unsigned int sz = dual.numberOfNodes();

  for(unsigned int i = 0; i < sz; ++i) {
    node dn = dual[i];

    if(!(dn_visited.get(dn.id))) {
      dn_visited.set(dn.id,true);
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

            if (!(dn_visited.get(neighbour.id))) {
              dn_visited.set(neighbour.id, true);
              dnToVisit.push_back(neighbour);
	      component.push_back(neighbour);
	    }
	  }
	}
      }
      if (component.size() >= 2 || group_isthmus == false) {
	vector<node>::const_iterator ite;
	for(ite=component.begin(); ite!=component.end(); ++ite) {
	  edge re = mapDNtoE[*ite];
	  result->setEdgeValue(re, val);
	}
      }
      val += 1;
    }
  }
}
//==============================================================================================================
double LinkCommunities::findBestThreshold(unsigned int numberOfSteps) {
  double maxD=-2;
  double threshold = 0.0;

  double min = 1.1;
  double max = -1.0;

  int sz = dual.numberOfEdges();
#ifdef _OPENMP
  //#pragma omp parallel for
#endif
  for(int i = 0; i < sz; ++i) {
    double value = similarity[dual(i)];
#ifdef _OPENMP
    //#pragma omp critical
#endif
    {
      if (value < min)
        min = value;
      else if (value > max)
        max = value;
    }
  }

  double deltaThreshold = (max-min)/double(numberOfSteps);

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int i=0; i<(int)numberOfSteps; i++) {    //use int for msvs2010 compilation
    double step = min + i * deltaThreshold;
    double d = computeAverageDensity(step);
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
