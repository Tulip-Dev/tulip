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
#include <ctime>
#include <tulip/tuliphash.h>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

/** \addtogroup clustering */
/*@{*/
/** \file
* \brief  An implementation of the Louvain clustering algorithm
*
* This plugin is an implementation of the Louvain clustering algorithm
* first published as:
*
* Blondel, V.D. and Guillaume, J.L. and Lambiotte, R. and Lefebvre, E., \n
* "Fast unfolding of communities in large networks", \n
* "Journal of Statistical Mechanics: Theory and Experiment, P10008",\n
* 2008. \n
*
* <b> HISTORY</b>
*
* - 25/02/2011 Version 1.0: Initial release
* - 13/05/2011 Version 2.0 (Bruno Pinaud): Change plugin type from General Algorithm to DoubleAlgorithm, code cleaning and fix some memory leaks.
*
* \note A threshold for modularity improvement is used here, its value is 0.000001
*
* \author François QUEYROI, Labri, Email : queyroi@labri.fr
*
*  <b>LICENCE</b>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
**/
class LouvainClustering : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Louvain","François Queyroi","25/02/11","Nodes partitioning measure used for community detection.","2.0","Clustering")
  LouvainClustering(const tlp::PluginContext*);
  bool run();
private:
  /**
  * Initialisation of the quotient graph
  */
  void createQuotient();
  /**
  * Update the quotient graph according to current clustering (qclusters) and
  * connections (commToIntraDeg)
  */
  void updateQuotient();
  /**
  * Save the current clustering (qclusters)
  * according to the mapping of the graph node (nodeMapping)
  */
  void trackClustering();
  /**
  * Compute the modularity gain when inserting a node in a given cluster
  * \param tlp::node toMove the inserted node
  * \param tlp::node dest the destination cluster representative node
  * \param double interWeight the weight of the relations between toMove and dest
  * \return the increase of modularity
  **/
  double measure_gain(double deg, tlp::node dest, double interWeight) ;
  /**
  * Compute the modularity (according to comToInfo map)
  * \return the Q modularity
  **/
  double modularity() const;
  /**
  * Get the differents clusters in the neighborhood of a given node (and its own clusters)
  * \param tlp::node toMove
  * \param TLP_HASH_MAP<tlp::node,double> a map with Clusters representative nodes as keys
  * and the weight of the relations as values
  **/
  void clustersNeighborhood(const tlp::node &toMove, TLP_HASH_MAP<tlp::node,double>&) const;
  /**
  * Performs an local modularity optimisation of the quotient graph
  * \return double the new Q modularity
  */
  double oneLevel();

  VectorGraph quotient;                          //A quotient graph of the original graph

  TLP_HASH_MAP<tlp::node, std::pair<double,double> > comToInfo;      //Clusters representative node to Internal and Total edges weights

  tlp::MutableContainer<tlp::node> nodeMapping;  //Original graph nodes to Clusters representative  in quotient
  tlp::MutableContainer<tlp::node> qclusters;         //Quotient graph nodes to Clusters representative nodes

  NodeProperty<double> iW;                 //Sum of Internal edges weights
  NodeProperty<double> nodeEW;             //Sum of External edges weights
  EdgeProperty<double> edgeEW;             //Sum of External edges weights

  tlp::NumericProperty *metric;                        //Edge Weights
  double m;                                      //Sum of edge weights for the original graph
};
/*@}*/

static double precision = 0.000001;

//========================================================================================
PLUGIN(LouvainClustering)
//========================================================================================
void LouvainClustering::createQuotient() {
  quotient.alloc(iW);
  quotient.alloc(nodeEW);
  quotient.alloc(edgeEW);

  //Copy the graph into quotient graph
  node n;
  forEach(n,graph->getNodes()) {
    node t = quotient.addNode();
    iW[t] = 0.0;
    nodeMapping.set(n.id, t);
    //Sum of weights on adjacents edges and test for self-loops
    double deg=0.0;
    edge e;

    if (metric) {
      forEach(e,graph->getInOutEdges(n)) {
        if(graph->opposite(e,n)==n)
          iW[t] = metric->getEdgeDoubleValue(e);
        else
          deg+=metric->getEdgeDoubleValue(e);
      }
    }
    else {
      forEach(e,graph->getInOutEdges(n)) {
        if(graph->opposite(e,n)==n)
          iW[t] = 1.0;
        else
          deg+=1.0;
      }
    }

    nodeEW[t] = deg;
    //Update comToInfo for modularity computing
    comToInfo[t] = make_pair(iW[t], 2.0*iW[t] + deg);
  }

  //Copy Edges and the sum of edge weights
  m=0.0;
  edge e;

  if (metric) {
    forEach(e,graph->getEdges()) {
      const std::pair<node, node>& eEnds = graph->ends(e);
      edge ne = quotient.addEdge(nodeMapping.get(eEnds.first.id),
                                 nodeMapping.get(eEnds.second.id));
      edgeEW[ne] = metric->getEdgeDoubleValue(e);
      m+=metric->getEdgeDoubleValue(e);
    }
  }
  else {
    forEach(e,graph->getEdges()) {
      const std::pair<node, node>& eEnds = graph->ends(e);
      edge ne = quotient.addEdge(nodeMapping.get(eEnds.first.id),
                                 nodeMapping.get(eEnds.second.id));
      edgeEW[ne] = 1.0;
      m+=1.0;
    }
  }
}
//========================================================================================
void LouvainClustering::trackClustering() {
  node n;
  forEach(n, graph->getNodes()) {
    node ncluster = qclusters.get(nodeMapping.get(n.id));
    result->setNodeValue(n, ncluster.id);
    nodeMapping.set(n.id, ncluster);
  }
}

//========================================================================================
void LouvainClustering::updateQuotient() {
  NodeProperty<bool> subNodes;
  EdgeProperty<bool> subEdges;
  quotient.alloc(subNodes);
  quotient.alloc(subEdges);

  edge e;
  unsigned int nbElts = quotient.numberOfEdges();

  for (unsigned int i = nbElts; i > 0;) {
    edge e = quotient(--i);
    const std::pair<node, node>& eEnds = quotient.ends(e);
    node qsrc = qclusters.get(eEnds.first.id);

    if (subNodes[qsrc.id] == false) {
      subNodes[qsrc.id] = true;
      std::pair<double, double>& weights = comToInfo[qsrc];
      iW[qsrc] = weights.first;
      nodeEW[qsrc] = weights.second - 2 * weights.first;
    }

    node qtgt = qclusters.get(eEnds.second.id);

    if (qsrc!=qtgt) {
      if (subNodes[qtgt.id] == false) {
        subNodes[qsrc.id] = true;
        std::pair<double, double>& weights = comToInfo[qtgt];
        iW[qtgt] = weights.first;
        nodeEW[qtgt] = weights.second - 2 * weights.first;
      }

      edge me = quotient.existEdge(qsrc, qtgt, false);

      if (!me.isValid() || subEdges[me] == false) {
        me = quotient.addEdge(qsrc, qtgt);
        edgeEW[me] = edgeEW[e];
        subEdges[me] = true;
      }
      else
        edgeEW[me] += edgeEW[e];
    }

    quotient.delEdge(e);
  }

  node n;
  stableForEach(n, quotient.getNodes()) {
    if (subNodes[n.id] == false)
      quotient.delNode(n);
  }
  quotient.free(subNodes);
  quotient.free(subEdges);
}
//========================================================================================
inline double LouvainClustering::measure_gain(double deg, tlp::node dest, double interWeight) {
  double degtot = comToInfo[dest].second;
  return (interWeight - degtot*deg/(2.0*m));
}

//========================================================================================
double LouvainClustering::modularity() const {
  double q=0.;

  for (TLP_HASH_MAP<node,pair<double,double> >::const_iterator it=comToInfo.begin() ; it!=comToInfo.end() ; ++it) {
    double in=(it->second).first;
    double tot=(it->second).second;

    //Test if the node is a cluster representative
    if(tot>0)
      q+=in/m - (tot/(2.0*m))*(tot/(2.0*m));
  }

  return q;
}
//========================================================================================
void LouvainClustering::clustersNeighborhood(const node &toMove, TLP_HASH_MAP<node,double>& neigh) const {
  //Insert node's own clusters
  node comm = qclusters.get(toMove.id);
  neigh[comm] = 0.0;
  const std::vector<edge> neighEdges = quotient.star(toMove);
  unsigned int nbNeigh = neighEdges.size();

  for (unsigned int i = 0; i < nbNeigh; ++i) {
    edge e = neighEdges[i];
    node qn = quotient.opposite(e, toMove);
    node cInd = qclusters.get(qn.id);

    if (neigh.find(cInd)==neigh.end())
      neigh[cInd] = edgeEW[e];
    else
      neigh[cInd]+= edgeEW[e];
  }
}
//========================================================================================
double LouvainClustering::oneLevel() {
  //Random schuffle of the quotient nodes
  const std::vector<node>& nodes = quotient.nodes();
  unsigned int nbNodes = quotient.numberOfNodes();
  std::vector<node> random_nodes(nbNodes);

  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = random_nodes[i] = nodes[i];
    qclusters.set(n.id, n);
  }

  random_shuffle(random_nodes.begin(),random_nodes.end());

  //Init modularity
  bool qimprove = false;
  double new_mod   = modularity();
  double cur_mod   = new_mod;

  //While the modularity increases
  do {
    cur_mod = new_mod;
    qimprove = false;

    for(unsigned int i=0; i<random_nodes.size(); ++i) {
      node n  = random_nodes[i];
      node best_comm = qclusters.get(n.id);
      //Compute reachable clusters
      TLP_HASH_MAP<node,double> neigh;
      clustersNeighborhood(n, neigh);
      //Remove n from its own community
      std::pair<double, double> *weights = &comToInfo[best_comm];
      double iWVal = iW[n];
      double deg = 2 * iWVal + nodeEW[n];
      weights->first -= neigh[best_comm] + iWVal;
      weights->second -= deg;

      double best_increase = 0.;
      node previous_com = best_comm;

      //Find the best move among node clusters neighborhood
      for (TLP_HASH_MAP<node,double>::iterator it=neigh.begin(); it!=neigh.end(); ++it) {
        double increase = measure_gain(deg, it->first, it->second);

        if (increase > best_increase) {
          best_increase = increase;
          best_comm = it->first;
        }
        // as enumeration ordering in TLP_HASH_MAP is not predicatable
        // keep the best cluster with the minimum id
        else if (increase == best_increase && it->first < best_comm)
          best_comm = it->first;
      }

      //Insert n in its new cluster
      weights = &comToInfo[best_comm];
      weights->first  += neigh[best_comm] + iWVal;
      weights->second += deg;
      qclusters.set(n.id, best_comm);

      if (best_comm!=previous_com) {
        qimprove=true;
      }
    }

    //Compute new modularity
    new_mod = modularity();
  }
  while (qimprove && (new_mod - cur_mod > precision));

  return new_mod;
}
//========================================================================================
namespace {
const char * paramHelp[] = {
  // metric
  HTML_HELP_OPEN()              \
  HTML_HELP_DEF( "type", "NumericProperty" )       \
  HTML_HELP_DEF( "value", "An existing edge metric" )                 \
  HTML_HELP_BODY()              \
  "An existing edge metric property"\
  HTML_HELP_CLOSE()
};
}
//========================================================================================
LouvainClustering::LouvainClustering(const tlp::PluginContext* context): DoubleAlgorithm(context), metric(NULL), m(0.0) {
  addInParameter<NumericProperty*>("metric",paramHelp[0], "",false);
}
//========================================================================================
bool LouvainClustering::run() {
  if(dataSet!=NULL)
    dataSet->get("metric",metric);

  // initialize a random sequence according the given seed
  tlp::initRandomSequence();
  //Init Quotient graph
  createQuotient();

  //Test if there is an edge in the graph
  if(m==0) {
    pluginProgress->setError("Graph contains no edges");
    return false;
  }

  //Compute initial modularity
  double mod=modularity();
  //Compute first level
  double new_mod = oneLevel();

  //While the modularity increases, re-apply the maximisation heuristic on the quotient  graph
  while(new_mod-mod>precision) {
    mod=new_mod;
    //Saving previous clustering
    trackClustering();
    //Compute the quotient graph induced by the previous clustering
    updateQuotient();
    //Re-apply the heuristic
    new_mod = oneLevel();
  }

  //Rename cluster indexes
  TLP_HASH_MAP<unsigned int,unsigned int> mapIndex;
  node n;
  unsigned int ind=0;
  forEach(n,graph->getNodes()) {
    unsigned int i = result->getNodeValue(n);

    if(mapIndex.find(i)==mapIndex.end()) {
      mapIndex.insert(make_pair(i,ind));
      ++ind;
    }

    result->setNodeValue(n,mapIndex[i]);
  }

  return true;
}
