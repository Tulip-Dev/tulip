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
#include <map>
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
  ~LouvainClustering();
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
  double measure_gain(const tlp::node &toMove,const tlp::node &dest,double interWeight) const;
  /**
  * Compute the modularity (according to comToInfo map)
  * \return the Q modularity
  **/
  double modularity() const;
  /**
  * Get the differents clusters in the neighborhood of a given node (and its own clusters)
  * \param tlp::node toMove
  * \return std::map<tlp::node,double> a map with Clusters representative nodes as keys
  * and the weight of the relations as values
  **/
  std::map<tlp::node,double> clustersNeighborhood(const tlp::node &toMove) const;
  /**
  * Performs an local modularity optimisation of the quotient graph
  * \return double the new Q modularity
  */
  double oneLevel();

  tlp::Graph* quotient;                          //A quotient graph of the original graph
  std::map<tlp::node,std::pair<double,double> > comToInfo;      //Clusters representative node to Internal and Total edges weights

  tlp::MutableContainer<tlp::node> nodeMapping;  //Original graph nodes to Clusters representative  in quotient
  tlp::MutableContainer<tlp::node> qclusters;         //Quortient graph nodes to Clusters representative nodes

  tlp::DoubleProperty *internalWeight;                 //Sum of Internal edges weights
  tlp::DoubleProperty *externalWeight;                 //Sum of External edges weights

  tlp::NumericProperty *metric;                        //Edge Weights
  double m;                                      //Sum of edge weights for the original graph
};
/*@}*/

static double precision = 0.000001;

//========================================================================================
PLUGIN(LouvainClustering)
//========================================================================================
void LouvainClustering::createQuotient() {
  quotient = tlp::newGraph();
  internalWeight = new DoubleProperty(quotient);
  externalWeight = new DoubleProperty(quotient);

  //Copy the graph into quotient graph
  node n;
  forEach(n,graph->getNodes()) {
    node t = quotient->addNode();
    nodeMapping.set(n.id,t);
    //Sum of weights on adjacents edges and test for self-loops
    double deg=0.0;
    edge e;

    if (metric) {
      forEach(e,graph->getInOutEdges(n)) {
        if(graph->opposite(e,n)==n)
          internalWeight->setNodeValue(t, metric->getEdgeDoubleValue(e));
        else
          deg+=metric->getEdgeDoubleValue(e);
      }
    }
    else {
      forEach(e,graph->getInOutEdges(n)) {
        if(graph->opposite(e,n)==n)
          internalWeight->setNodeValue(t, 1.0);
        else
          deg+=1.0;
      }
    }

    externalWeight->setNodeValue(t,deg);
    //Update comToInfo for modularity computing
    comToInfo.insert(make_pair(t,make_pair(internalWeight->getNodeValue(t),2.0*internalWeight->getNodeValue(t)+deg)));
  }

  //Copy Edges and the sum of edge weights
  m=0.0;
  edge e;

  if (metric) {
    forEach(e,graph->getEdges()) {
      const std::pair<node, node>& eEnds = graph->ends(e);
      edge ne = quotient->addEdge(nodeMapping.get(eEnds.first.id),
                                  nodeMapping.get(eEnds.second.id));
      externalWeight->setEdgeValue(ne,metric->getEdgeDoubleValue(e));
      m+=metric->getEdgeDoubleValue(e);
    }
  }
  else {
    forEach(e,graph->getEdges()) {
      const std::pair<node, node>& eEnds = graph->ends(e);
      edge ne = quotient->addEdge(nodeMapping.get(eEnds.first.id),
                                  nodeMapping.get(eEnds.second.id));
      externalWeight->setEdgeValue(ne, 1.0);
      m+=1.0;
    }
  }
}
//========================================================================================
void LouvainClustering::trackClustering() {
  node n;
  forEach(n,graph->getNodes()) {
    result->setNodeValue(n,qclusters.get(nodeMapping.get(n.id)).id);
    nodeMapping.set(n.id,qclusters.get(nodeMapping.get(n.id)));
  }
}

//========================================================================================
void LouvainClustering::updateQuotient() {
  Graph* sub = quotient->addSubGraph();
  edge e;
  stableForEach(e,quotient->getEdges()) {
    const std::pair<node, node>& eEnds = quotient->ends(e);
    node qsrc = qclusters.get(eEnds.first.id);

    if(!sub->isElement(qsrc)) {
      sub->addNode(qsrc);
      internalWeight->setNodeValue(qsrc,comToInfo[qsrc].first);
      externalWeight->setNodeValue(qsrc,comToInfo[qsrc].second-comToInfo[qsrc].first*2.0);
    }

    node qtgt = qclusters.get(eEnds.second.id);

    if(qsrc!=qtgt) {
      if(!sub->isElement(qtgt)) {
        sub->addNode(qtgt);
        internalWeight->setNodeValue(qtgt,comToInfo[qtgt].first);
        externalWeight->setNodeValue(qtgt,comToInfo[qtgt].second-comToInfo[qtgt].first*2.0);
      }

      edge me = sub->existEdge(qsrc,qtgt,false);

      if(!me.isValid())
        me = sub->addEdge(qsrc,qtgt);

      externalWeight->setEdgeValue(me,externalWeight->getEdgeValue(me)+externalWeight->getEdgeValue(e));
    }

    quotient->delEdge(e);
  }

  node n;
  stableForEach(n,quotient->getNodes()) {
    if(!sub->isElement(n))
      quotient->delNode(n);
  }
  quotient->delSubGraph(sub);
}
//========================================================================================
double LouvainClustering::measure_gain(const node &toMove,const node &dest,double interWeight) const {
  double deg = 2.0*internalWeight->getNodeValue(toMove)+externalWeight->getNodeValue(toMove);
  double degtot = (comToInfo.find(dest)->second).second;
  return (interWeight - degtot*deg/(2.0*m)) ;
}

//========================================================================================
double LouvainClustering::modularity() const {
  double q=0.;

  for (map<node,pair<double,double> >::const_iterator it=comToInfo.begin() ; it!=comToInfo.end() ; ++it) {
    double in=(it->second).first;
    double tot=(it->second).second;

    //Test if the node is a cluster representative
    if(tot>0)
      q+=in/m - (tot/(2.0*m))*(tot/(2.0*m));
  }

  return q;
}
//========================================================================================
map<node,double> LouvainClustering::clustersNeighborhood(const node &toMove) const {
  map<node,double> neigh;
  //Insert node's own clusters
  node comm = qclusters.get(toMove.id);
  neigh.insert(make_pair(comm,0.0));
  edge e;
  forEach(e,quotient->getInOutEdges(toMove)) {
    node qn = quotient->opposite(e,toMove);
    node cInd = qclusters.get(qn.id);

    if(neigh.find(cInd)==neigh.end())
      neigh.insert(make_pair(cInd,0.0));

    neigh[cInd]+=externalWeight->getEdgeValue(e);
  }
  return neigh;
}
//========================================================================================
double LouvainClustering::oneLevel() {
  //Random schuffle of the quotient nodes
  vector<node> random_nodes;
  random_nodes.resize(quotient->numberOfNodes());
  unsigned int i=0;
  node n;
  forEach(n,quotient->getNodes()) {
    random_nodes[i]=n;
    i++;
    qclusters.set(n.id,n);
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
      n  = random_nodes[i];
      node best_comm        = qclusters.get(n.id);
      //Compute reachable clusters
      map<node,double> neigh = clustersNeighborhood(n);
      //Remove n from its own community
      comToInfo[best_comm].first -= (neigh.find(best_comm))->second+internalWeight->getNodeValue(n);
      comToInfo[best_comm].second-= internalWeight->getNodeValue(n)*2.+externalWeight->getNodeValue(n);

      double best_increase = 0.;
      node previous_com = best_comm;

      //Find the best move among node clusters neighborhood
      for (map<node,double>::iterator it=neigh.begin() ; it!=neigh.end() ; ++it) {
        double increase = measure_gain(n,it->first,it->second);

        if(increase>best_increase) {
          best_increase=increase;
          best_comm = it->first;
        }
      }

      //Insert n in its new cluster
      comToInfo[best_comm].first  += (neigh.find(best_comm))->second+internalWeight->getNodeValue(n);
      comToInfo[best_comm].second += internalWeight->getNodeValue(n)*2.+externalWeight->getNodeValue(n);
      qclusters.set(n.id,best_comm);

      if (best_comm!=previous_com) {
        qimprove=true;
      }
    }

    //Compute new modularity
    new_mod = modularity();
  }
  while (qimprove && new_mod-cur_mod>precision);

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
LouvainClustering::LouvainClustering(const tlp::PluginContext* context): DoubleAlgorithm(context), quotient(NULL), internalWeight(NULL), externalWeight(NULL), metric(NULL), m(0.0) {
  addInParameter<NumericProperty*>("metric",paramHelp[0], "",false);
}
//========================================================================================
LouvainClustering::~LouvainClustering() {
  if(quotient) {
    delete quotient;
    delete internalWeight;
    delete externalWeight;
  }
}
//========================================================================================
bool LouvainClustering::run() {
  if(dataSet!=NULL)
    dataSet->get("metric",metric);

  // initialize a random sequence according the given seed
  tlp::initRandomSequence();
  //Init Quotient graph
  createQuotient();

  //Test if there is ane edges in the graph
  if(m==0) return false;

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
  map<unsigned int,unsigned int> mapIndex;
  node n;
  unsigned int ind=0;
  forEach(n,graph->getNodes()) {
    unsigned int i = result->getNodeValue(n);

    if(mapIndex.find(i)==mapIndex.end()) {
      mapIndex.insert(make_pair(i,ind));
      ind++;
    }

    result->setNodeValue(n,mapIndex[i]);
  }

  return true;
}
