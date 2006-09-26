#include <math.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <ext/hash_map>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/GraphMeasure.h>
#include <tulip/StableIterator.h>
#include <tulip/ForEach.h>
#include "StrengthClustering.h"


using namespace std;
using namespace tlp;
using namespace stdext;

CLUSTERINGPLUGIN(StrengthClustering,"Strength","David Auber","27/01/2003","Alpha","0","1");

//================================================================================
StrengthClustering::~StrengthClustering() {}
//==============================================================================
double StrengthClustering::computeMQValue(const vector<set<node> > & partition, Graph *sg) {

  vector<unsigned int> nbIntraEdges(partition.size());
  for (unsigned int i = 0; i<partition.size(); ++i)
    nbIntraEdges[i] = 0;

  map<pair<unsigned int, unsigned int>, unsigned int > nbExtraEdges;

  MutableContainer<unsigned int> clusterId;  
  vector< set<node> >::const_iterator itPart = partition.begin();
  for (unsigned int i=0; itPart!=partition.end(); ++itPart, ++i) {
    set<node>::const_iterator itSet = itPart->begin();
    for (; itSet!=itPart->end(); ++itSet) {
      clusterId.set(itSet->id, i);
    }
  }
  
  Iterator<edge> *itE = sg->getEdges();
  while(itE->hasNext()) {
    edge e = itE->next();
    node n1; 
    node n2;
    if (sg->source(e).id < sg->target(e).id) {
      n1 = sg->source(e);
      n2 = sg->target(e);
    }
    else {
      n1 = sg->target(e);
      n2 = sg->source(e);
    }

    unsigned int n1ClustId = clusterId.get(n1.id);
    unsigned int n2ClustId = clusterId.get(n2.id);
    if ( n1ClustId == n2ClustId)
      nbIntraEdges[n1ClustId] += 1;
    else {
      pair<unsigned int, unsigned int> pp(n1ClustId, n2ClustId);
      if (nbExtraEdges.find(pp) != nbExtraEdges.end()) {
	nbExtraEdges[pp] += 1;
      }
      else {
	nbExtraEdges[pp] = 1;
      }
    }
  } delete itE;
  
  double positive = 0;
  for (unsigned int i=0; i<partition.size(); ++i) {
    if (partition[i].size() > 1)
      positive += 2.0 * double(nbIntraEdges[i]) / double(partition[i].size() * (partition[i].size() - 1));
  }
  positive /= double(partition.size());
  
  double negative=0;
  map<pair<unsigned int, unsigned int>, unsigned int >::const_iterator itMap = nbExtraEdges.begin();
  for (; itMap != nbExtraEdges.end(); ++itMap) {
    pair<unsigned int, unsigned int> pp = itMap->first;
    unsigned int val = itMap->second;
    if ( (partition[pp.first].size()!=0) && (partition[pp.second].size()!=0))
      negative += double(val) / double(partition[pp.first].size() * partition[pp.second].size());
  }
  if (partition.size()>1)
    negative /= double(partition.size()*(partition.size()-1)) / 2.0;
  double result = positive - negative;
  return result;
}


//==============================================================================
vector< set<node> > StrengthClustering::computeNodePartition(double threshold) {
  //cerr << __PRETTY_FUNCTION__ << endl;
  string errMsg;
  Graph *tmpGraph=tlp::newCloneSubGraph(graph);
  StableIterator<edge> itE(graph->getEdges());
  while (itE.hasNext()) {
    edge ite=itE.next();
    if (values->getEdgeValue(ite)<threshold) {
      if (graph->deg(graph->source(ite))>1 && graph->deg(graph->target(ite))>1)
	tmpGraph->delEdge(ite);
    }
  }

  // Select SubGraph singleton in graph
  set<node> singleton;
  StableIterator<node> itN(tmpGraph->getNodes());
  while (itN.hasNext()) {
    node itn=itN.next();
    if (tmpGraph->deg(itn)==0) singleton.insert(itn);
  }
  
  // restore edges to reconnect singleton by computing induced subgraph 
  StableIterator<edge> itE2(graph->getEdges());
  while (itE2.hasNext()) {
    edge ite=itE2.next();
    if (singleton.find(graph->source(ite))!=singleton.end() && 
	singleton.find(graph->target(ite))!=singleton.end()) {
      tmpGraph->addEdge(ite);
    }
  }

  //Extract connected componnent
  DoubleProperty *connected= new DoubleProperty(tmpGraph); 
  tmpGraph->computeProperty("Connected Component", connected, errMsg);

  //Put isolated nodes in the same cluster
  double val=-1;
  Iterator<node> *itN2 = tmpGraph->getNodes();
  while (itN2->hasNext()) {
    node itn=itN2->next();
    if (tmpGraph->deg(itn)==0) {
      if (val==-1) val=connected->getNodeValue(itn);
      else
	connected->setNodeValue(itn,val);
    }
  } delete itN2;

  //Compute the node partition
  vector< set<node > > result;
  int index=0;
  map<double,int> resultIndex;
  itN2=tmpGraph->getNodes();
  while (itN2->hasNext()) {
    node itn=itN2->next();
    double val=connected->getNodeValue(itn);
    if (resultIndex.find(val)!=resultIndex.end())
      result[resultIndex[val]].insert(itn);
    else {
      set<node> tmp;
      result.push_back(tmp);
      resultIndex[val]=index;
      result[index].insert(itn);
      ++index;
    }
  }delete itN2;

  delete connected;
  graph->delAllSubGraphs(tmpGraph);
  return result;
}
//==============================================================================
void drawGraph(Graph *tmpg) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  string errMsg;
  string layoutName;
  if (tmpg->numberOfNodes() > 300)
    layoutName = "Circular";
  else
    layoutName = "GEM (Frick)";
  string sizesName="Auto_sizing";
  tmpg->computeProperty(layoutName,tmpg->getLocalProperty<LayoutProperty>("viewLayout"),errMsg);
  if (tmpg->numberOfNodes() < 300)
    tmpg->computeProperty(sizesName,tmpg->getLocalProperty<SizeProperty>("viewSize"),errMsg);
}
//==============================================================================
double StrengthClustering::findBestThreshold(int numberOfSteps){
  double maxMQ=-2;
  double threshold = values->getEdgeMin(graph);
  double deltaThreshold = (values->getEdgeMax(graph)-values->getEdgeMin(graph))/double(numberOfSteps);
  for (double i=values->getEdgeMin(graph); i<values->getEdgeMax(graph); i+=deltaThreshold) {
    vector< set<node > > tmp;
    tmp = computeNodePartition(i);
    double mq = computeMQValue(tmp, graph);
    if ( mq > maxMQ) {
      threshold=i;
      maxMQ=mq;
    }
  }
  // cout << __PRETTY_FUNCTION__ << endl;
  // cout << "\t" <<" Nb try : " << numberOfSteps << endl;
  // cout << "\t" <<" Max MQ : " << maxMQ << endl << flush;
  // cout << "\t" <<" Best threshold : " << threshold << endl << flush;
  return threshold;
}
//==============================================================================
Graph* StrengthClustering::buildSubGraphs(const vector< set<node > > &partition){
  if (partition.size()<2) return graph;
  Graph *tmpGraph=tlp::newCloneSubGraph(graph);
  for (unsigned int i=0;i<partition.size();++i) {
    tlp::inducedSubGraph(tmpGraph,partition[i]);
  }
  return tmpGraph;
}
//==============================================================================
void StrengthClustering::recursiveCall(Graph *rootGraph, map<Graph *,Graph *> &mapGraph) {
  Iterator<Graph*> *itS = rootGraph->getSubGraphs();
  while(itS->hasNext()){
    Graph *sg=itS->next();
    double avPath=tlp::averagePathLength(sg);
    double avCluster=tlp::averageCluster(sg);
    /*
      cout << "Average Path Length :" << avPath << endl;
      cout << "Average clustering  :" <<  avCluster << endl; 
      cout << "Number of nodes     :" <<  tmpg->numberOfNodes() << endl; 
      cout << "Number of edges     :" <<  tmpg->numberOfEdges() << endl; 
    */
    Graph *tmpGr=sg;
    if ( avPath>1 && avPath<4 && avCluster>0.25 && sg->numberOfNodes()>10) {
      DataSet tmpData;
      string errMsg;
      tlp::clusterizeGraph(sg,errMsg,&tmpData,"Strength");
      tmpData.get("strengthGraph",tmpGr);
    }
    mapGraph[sg]=tmpGr;
    if (sg==tmpGr) {
      drawGraph(sg);
    }
  } delete itS;
}
//==============================================================================
Graph* StrengthClustering::buildQuotientGraph(Graph *sg) {
  DataSet tmpData;
  string errMsg;
  tlp::clusterizeGraph(sg,errMsg,&tmpData,"QuotientClustering");
  Graph *quotientGraph;
  tmpData.get<Graph *>("quotientGraph",quotientGraph);
  drawGraph(quotientGraph);
  return quotientGraph;
}
//==============================================================================
/**
  Create a new metagraph property in order to have in the "viewMetagraph" the result
  of recursive clustering, and in "strengthMetaGraph" the graphs before without recursive clustering.
*/
void StrengthClustering::adjustMetaGraphProtperty(Graph *quotientGraph, map<Graph *,Graph *> &mapGraph) {
  if (quotientGraph != graph) {
    Graph *rootGraph = graph->getRoot();
    GraphProperty *meta = rootGraph->getLocalProperty<GraphProperty>("viewMetaGraph");
    GraphProperty *meta2 = rootGraph->getLocalProperty<GraphProperty>("strengthMetaGraph");
    Iterator<node> *itN = quotientGraph->getNodes();
    while (itN->hasNext()) {
      node itn=itN->next();
      meta2->setNodeValue(itn,meta->getNodeValue(itn));
      meta->setNodeValue(itn,mapGraph[meta->getNodeValue(itn)]);
    } delete itN;
  }
}
//==============================================================================
namespace {
  const char * paramHelp[] = {
    // nodeSize
    HTML_HELP_OPEN()							\
    HTML_HELP_DEF( "type", "DoubleProperty" )				\
    HTML_HELP_DEF( "values", "An existing metric property" )		\
    HTML_HELP_DEF( "default", "viewSize" )				\
    HTML_HELP_BODY()							\
    "This parameter defines the property used in order to multiply strength values." \
    HTML_HELP_CLOSE(),
    //Complexity
    HTML_HELP_OPEN()							\
    HTML_HELP_DEF( "type", "bool" )					\
    HTML_HELP_DEF( "values", "[true, false] o(nlog(n)) / o(n)" )	\
    HTML_HELP_DEF( "default", "false" )					\
    HTML_HELP_BODY()							\
    "If true the strength value wiil be multipplied by the metric proxy given in parameter." \
    HTML_HELP_CLOSE()
  };
}

//================================================================================
StrengthClustering::StrengthClustering(ClusterContext context):Clustering(context) {
  addParameter<DoubleProperty>("metric", paramHelp[0],"viewMetric");
  addParameter<bool>("multiply",paramHelp[1],"true");
}
//==============================================================================
bool StrengthClustering::run() {
  bool result;
  string errMsg;
  values = new DoubleProperty(graph);
  result = graph->computeProperty("Strength", values, errMsg);
  
  bool multi;
  if ( dataSet==0 || !dataSet->get("multiply",multi)) {
    multi = false;
  }
  
  if (multi) {
    DoubleProperty *metric;
    if (!dataSet->get("metric",metric)) 
      metric = graph->getProperty<DoubleProperty>("viewMetric");
    DoubleProperty mult(graph);
    mult = *metric;
    mult.uniformQuantification(100);
    edge e;
    forEach (e, graph->getEdges()) {
      values->setEdgeValue(e, values->getEdgeValue(e)*(mult.getEdgeValue(e) + 1));
    }
  }
  
  const unsigned int NB_TEST = 100;
  double threshold = findBestThreshold(NB_TEST);
  vector< set<node > > tmp;
  tmp = computeNodePartition(threshold);
  if (tmp.size()==1) {
    drawGraph(graph);
    if (dataSet!=0) {
      dataSet->set("strengthGraph",graph);
    }
    return true;
  }

  map<Graph *,Graph *> mapGraph;
  Graph *tmpGraph, *quotientGraph;

  tmpGraph = buildSubGraphs(tmp);
  recursiveCall(tmpGraph, mapGraph);
  quotientGraph = buildQuotientGraph(tmpGraph);
  adjustMetaGraphProtperty(quotientGraph, mapGraph);

  if (dataSet!=0) {
    dataSet->set("strengthGraph",quotientGraph);
  }
  delete values;
  return true;
}
//================================================================================
bool StrengthClustering::check(string &erreurMsg) {
  erreurMsg="";
  return true;
}
//================================================================================
void StrengthClustering::reset() {
}
//================================================================================




