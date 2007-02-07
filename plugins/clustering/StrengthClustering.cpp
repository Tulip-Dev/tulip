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
#include <tulip/SimpleTest.h>
#include "StrengthClustering.h"


using namespace std;
using namespace tlp;
using namespace stdext;

ALGORITHMPLUGIN(StrengthClustering,"Strength","David Auber","27/01/2003","Alpha","0","1");

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
  int index=0;
  vector< set<node > > result;
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
double StrengthClustering::findBestThreshold(int numberOfSteps, bool& stopped){
  double maxMQ=-2;
  double threshold = values->getEdgeMin(graph);
  double deltaThreshold = (values->getEdgeMax(graph)-values->getEdgeMin(graph))/double(numberOfSteps);
  int steps = 0;
  for (double i=values->getEdgeMin(graph); i<values->getEdgeMax(graph); i+=deltaThreshold) {
    vector< set<node > > tmp;
    tmp = computeNodePartition(i);
    pluginProgress->progress(++steps, numberOfSteps);
    if (stopped = (pluginProgress->state() !=TLP_CONTINUE)) {
      return threshold;
    }
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
    pluginProgress->progress(i, partition.size());
    if (pluginProgress->state() !=TLP_CONTINUE) {
      graph->delSubGraph(tmpGraph);
      return 0;
    }
    tlp::inducedSubGraph(tmpGraph, partition[i]);
  }
  return tmpGraph;
}
//==============================================================================
bool StrengthClustering::recursiveCall(Graph *rootGraph, map<Graph *,Graph *> &mapGraph) {
  Iterator<Graph*> *itS = rootGraph->getSubGraphs();
  int i = 0;
  while(itS->hasNext()) {
    Graph *sg=itS->next();
    pluginProgress->setComment("Computing average path length on subgraphs");
    pluginProgress->progress(i, 100);
    double avPath = tlp::averagePathLength(sg);
    pluginProgress->setComment("Computing average cluster on subgraphs");
    pluginProgress->progress(i, 100);
    if (pluginProgress->state() !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;
    double avCluster = tlp::averageCluster(sg);
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
      
      pluginProgress->setComment("Computing strength clustering on subgraphs...");
      pluginProgress->progress(i, 100);
      if (pluginProgress->state() !=TLP_CONTINUE)
	return pluginProgress->state()!= TLP_CANCEL;
      if (!tlp::applyAlgorithm(sg,errMsg,&tmpData,"Strength")) {
	return false;
      }
      tmpData.get("strengthGraph",tmpGr);
    }
    mapGraph[sg]=tmpGr;
    if (sg==tmpGr) {
      drawGraph(sg);
    }
    i++;
  } delete itS;
  return true;
}
//==============================================================================
Graph* StrengthClustering::buildQuotientGraph(Graph *sg) {
  DataSet tmpData;
  string errMsg;
  if (!tlp::applyAlgorithm(sg,errMsg,&tmpData,"QuotientClustering"))
    return 0;
  Graph *quotientGraph;
  tmpData.get<Graph *>("quotientGraph",quotientGraph);
  vector<edge> toRemoved;
  SimpleTest::makeSimple(quotientGraph, toRemoved);
  for(vector<edge>::iterator it= toRemoved.begin(); it!=toRemoved.end(); ++it) 
    quotientGraph->delAllEdge(*it);

  drawGraph(quotientGraph);
  return quotientGraph;
}
//==============================================================================
/**
  Create a new metagraph property in order to have in the "viewMetagraph" the result
  of recursive clustering, and in "strengthMetaGraph" the graphs before without recursive clustering.
*/
void StrengthClustering::adjustMetaGraphProperty(Graph *quotientGraph, map<Graph *,Graph *> &mapGraph) {
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
    HTML_HELP_DEF( "value", "An existing metric property" )		\
    HTML_HELP_BODY()							\
    "This parameter defines the metric used in order to multiply strength metric computed values."\
    "If one is given the complexity will be in o(nlog(n)), o(n) neither." \
    HTML_HELP_CLOSE(),
  };
}

//================================================================================
StrengthClustering::StrengthClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<DoubleProperty>("metric", paramHelp[0], 0, false);
  addDependency<DoubleAlgorithm>("Connected Component");
  addDependency<DoubleAlgorithm>("Strength");
  addDependency<Algorithm>("QuotientClustering");
}

//==============================================================================
bool StrengthClustering::run() {
  bool result;
  string errMsg;
  values = new DoubleProperty(graph);
  pluginProgress->setComment("Computing Strength metric");
  pluginProgress->progress(0, 100);
  result = graph->computeProperty("Strength", values, errMsg);
  
  DoubleProperty *metric;
  bool multi = true;
  if ( dataSet==0 || !dataSet->get("metric", metric)) {
    multi = false;
  }

  if (multi) {
    DoubleProperty mult(graph);
    pluginProgress->setComment("Computing Strength metric X specified metric on edges ...");
    mult = *metric;
    mult.uniformQuantification(100);
    edge e;
    int steps = 0, maxSteps = graph->numberOfEdges();
    forEach (e, graph->getEdges()) {
      values->setEdgeValue(e, values->getEdgeValue(e)*(mult.getEdgeValue(e) + 1));
      pluginProgress->progress(++steps, maxSteps);
      if (pluginProgress->state() !=TLP_CONTINUE)
	return pluginProgress->state()!= TLP_CANCEL;
    }
  }
  
  bool stopped = false;
  const unsigned int NB_TEST = 100;
  pluginProgress->setComment("Partitioning nodes...");
  pluginProgress->progress(0, NB_TEST + 1);

  double threshold = findBestThreshold(NB_TEST, stopped);
  if (stopped)
    return pluginProgress->state()!= TLP_CANCEL;
  vector< set<node > > tmp;
  tmp = computeNodePartition(threshold);
  pluginProgress->progress(NB_TEST + 1, NB_TEST + 1);
  if (pluginProgress->state() !=TLP_CONTINUE)
    return pluginProgress->state()!= TLP_CANCEL;
  if (tmp.size()==1) {
    drawGraph(graph);
    if (dataSet!=0) {
      dataSet->set("strengthGraph",graph);
    }
    return true;
  }

  map<Graph *,Graph *> mapGraph;
  Graph *tmpGraph, *quotientGraph;

  pluginProgress->setComment("Building subgraphs...");
  tmpGraph = buildSubGraphs(tmp);
  if (!tmpGraph)
    return pluginProgress->state()!= TLP_CANCEL;
  if (!recursiveCall(tmpGraph, mapGraph))
    return pluginProgress->state()!= TLP_CANCEL;
  pluginProgress->setComment("Building quotient graph...");
  quotientGraph = buildQuotientGraph(tmpGraph);
  if (!quotientGraph)
    return pluginProgress->state()!= TLP_CANCEL;
  pluginProgress->setComment("Adjusting metagraph property...");
  adjustMetaGraphProperty(quotientGraph, mapGraph);

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
