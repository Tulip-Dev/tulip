#include <math.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <tulip/StableIterator.h>
#include "StrengthClustering.h"


using namespace std;
using namespace tlp;
using namespace stdext;

ALGORITHMPLUGIN(StrengthClustering, "Strength Clustering", "David Auber", "27/01/2003", "Alpha", "1.1");

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
    const pair<node, node>& eEnds = sg->ends(e);
    node n1 = eEnds.first;
    node n2 = eEnds.second;
    
    if (n1.id >= n2.id) {
      n1 = n2;
      n2 = eEnds.first;
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
      const pair<node, node>& eEnds = graph->ends(ite);
      if (graph->deg(eEnds.first)>1 && graph->deg(eEnds.second)>1)
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
    const pair<node, node>& eEnds = graph->ends(ite);
    if (singleton.find(eEnds.first)!=singleton.end() && 
	singleton.find(eEnds.second)!=singleton.end()) {
      tmpGraph->addEdge(ite);
    }
  }

  //Extract connected componnent
  DoubleProperty *connected= new DoubleProperty(tmpGraph); 
  tmpGraph->computeProperty("Connected Component", connected, errMsg);

  //Compute the node partition
  int index=0;
  vector< set<node > > result;
  map<double,int> resultIndex;
  Iterator<node> *itN2=tmpGraph->getNodes();
  while (itN2->hasNext()) {
    node itn=itN2->next();
    const double& val=connected->getNodeValue(itn);
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
  string sizesName="Auto Sizing";
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
    if (pluginProgress && ((++steps % (numberOfSteps / 10)) == 0)) {
      pluginProgress->progress(steps, numberOfSteps);
      if (stopped = (pluginProgress->state() !=TLP_CONTINUE)) {
	return threshold;
      }
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
Graph* StrengthClustering::buildSubGraphs(const vector< set<node > > &partition) {
  if (partition.size()<2) return graph;
  Graph *tmpGraph=tlp::newCloneSubGraph(graph);
  stringstream sstr;
  sstr << "clone of ";
  string graphName = graph->getAttribute<string>("name");
  if (graphName.size() == 0)
    sstr << graph->getId();
  else
    sstr << graphName;
  tmpGraph->setAttribute(string("name"), sstr.str());
  unsigned int step = partition.size() / 10;
  for (unsigned int i=0;i<partition.size();++i) {
    if (pluginProgress && step && ((i % step) == 0)) {
      pluginProgress->progress(i, partition.size());
      if (pluginProgress->state() !=TLP_CONTINUE) {
	graph->delSubGraph(tmpGraph);
	return 0;
      }
    }
    tlp::inducedSubGraph(tmpGraph, partition[i]);
  }
  return tmpGraph;
}
//==============================================================================
bool StrengthClustering::recursiveCall(Graph *rootGraph, map<Graph *,Graph *> &mapGraph) {
  Iterator<Graph*> *itS = rootGraph->getSubGraphs();
  while(itS->hasNext()) {
    Graph *sg=itS->next();
    Graph *tmpGr = sg;
    if (sg->numberOfNodes() > 10) {
      double avPath;
      if (pluginProgress)
	pluginProgress->setComment("Computing average path length on subgraphs");
      if (!tlp::averagePathLength(sg, avPath, pluginProgress))
	return false;
      double avCluster;
      if (pluginProgress)
	pluginProgress->setComment("Computing average cluster on subgraphs");
      if (!tlp::averageCluster(sg, avCluster, pluginProgress))
	return false;
      /*
	cout << "Average Path Length :" << avPath << endl;
	cout << "Average clustering  :" <<  avCluster << endl; 
	cout << "Number of nodes     :" <<  tmpg->numberOfNodes() << endl; 
	cout << "Number of edges     :" <<  tmpg->numberOfEdges() << endl; 
      */
      if ( avPath > 1 && avPath < 4 && avCluster > 0.25) {
	DataSet tmpData; 
	string errMsg;
      
	//pluginProgress->setComment("Computing strength clustering on subgraphs...");
	// propagate values for layout parameters
	tmpData.set("layout subgraphs", subgraphsLayout);
	tmpData.set("layout quotient graph", quotientLayout);
	if (!tlp::applyAlgorithm(sg, errMsg, &tmpData, "Strength Clustering",
				 pluginProgress)) {
	  return false;
	}
	tmpData.get("strengthGraph",tmpGr);
      }
    }
    mapGraph[sg]=tmpGr;
    if (subgraphsLayout && sg==tmpGr) {
      drawGraph(sg);
    }
  } delete itS;
  return true;
}
//==============================================================================
Graph* StrengthClustering::buildQuotientGraph(Graph *sg) {
  DataSet tmpData;
  string errMsg;
  if (!tlp::applyAlgorithm(sg, errMsg, &tmpData, "Quotient Clustering",
			   pluginProgress))
    return 0;
  Graph *quotientGraph;
  tmpData.get<Graph *>("quotientGraph", quotientGraph);
  vector<edge> toRemoved;
  SimpleTest::makeSimple(quotientGraph, toRemoved);
  for(vector<edge>::iterator it= toRemoved.begin(); it!=toRemoved.end(); ++it) 
    quotientGraph->delAllEdge(*it);

  if (quotientLayout)
    drawGraph(quotientGraph);
  return quotientGraph;
}
//==============================================================================
namespace {
  const char * paramHelp[] = {
    // metric
    HTML_HELP_OPEN()							\
    HTML_HELP_DEF( "type", "DoubleProperty" )				\
    HTML_HELP_DEF( "value", "An existing metric property" )		\
    HTML_HELP_BODY()							\
    "This parameter defines the metric used in order to multiply strength metric computed values."\
    "If one is given the complexity will be in o(nlog(n)), o(n) neither." \
    HTML_HELP_CLOSE(),
    // layout subgraphs
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the layout of the newly created subgraphs has to be computed or not." \
    HTML_HELP_CLOSE(),
    // layout quotient graph
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the layout of the quotient graph has to be computed or not." \
    HTML_HELP_CLOSE(),
  };
}

//================================================================================
StrengthClustering::StrengthClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<DoubleProperty>("metric", paramHelp[0], 0, false);
  addParameter<bool>("layout subgraphs", paramHelp[1], "true");
  addParameter<bool>("layout quotient graph", paramHelp[2], "true");
  addDependency<Algorithm>("Quotient Clustering", "1.0");
  addDependency<DoubleAlgorithm>("Connected Component", "1.0");
  addDependency<DoubleAlgorithm>("Strength", "1.0");
  addDependency<LayoutAlgorithm>("Circular", "1.0");
  addDependency<LayoutAlgorithm>("GEM (Frick)", "1.0");
  addDependency<SizeAlgorithm>("Auto Sizing", "1.0");
}

//==============================================================================
bool StrengthClustering::run() {
  string errMsg;
  values = new DoubleProperty(graph);

  if (!graph->computeProperty("Strength", values, errMsg, pluginProgress))
    return false;
  
  DoubleProperty *metric;
  bool multi = false;
  subgraphsLayout = true;
  quotientLayout = true;
  if (dataSet) {
    multi = dataSet->get("metric", metric);
    dataSet->get("layout subgraphs", subgraphsLayout);
    dataSet->get("layout quotient graph", quotientLayout);
  }

  if (multi) {
    DoubleProperty mult(graph);
    if (pluginProgress)
      pluginProgress->setComment("Computing Strength metric X specified metric on edges ...");
    mult = *metric;
    mult.uniformQuantification(100);
    edge e;
    unsigned int steps = 0, maxSteps = graph->numberOfEdges();
    if (maxSteps < 10)
      maxSteps = 10;
    forEach (e, graph->getEdges()) {
      values->setEdgeValue(e, values->getEdgeValue(e)*(mult.getEdgeValue(e) + 1));
      if (pluginProgress && ((++steps % (maxSteps / 10) == 0))) {
	pluginProgress->progress(++steps, maxSteps);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return pluginProgress->state()!= TLP_CANCEL;
      }
    }
  }
  
  bool stopped = false;
  const unsigned int NB_TEST = 100;
  if (pluginProgress) {
    pluginProgress->setComment("Partitioning nodes...");
    pluginProgress->progress(0, NB_TEST + 1);
  }

  double threshold = findBestThreshold(NB_TEST, stopped);
  if (stopped)
    return pluginProgress->state()!= TLP_CANCEL;
  vector< set<node > > tmp;
  tmp = computeNodePartition(threshold);
  if (tmp.size()==1) {
    if (quotientLayout)
      drawGraph(graph);
    if (dataSet!=0) {
      dataSet->set("strengthGraph",graph);
    }
    return true;
  }

  map<Graph *,Graph *> mapGraph;
  Graph *tmpGraph, *quotientGraph;

  if (pluginProgress)
    pluginProgress->setComment("Building subgraphs...");
  tmpGraph = buildSubGraphs(tmp);
  if (!tmpGraph)
    return pluginProgress->state()!= TLP_CANCEL;
  if (!recursiveCall(tmpGraph, mapGraph))
    return pluginProgress->state()!= TLP_CANCEL;
  if (pluginProgress)
    pluginProgress->setComment("Building quotient graph...");
  quotientGraph = buildQuotientGraph(tmpGraph);
  if (!quotientGraph)
    return pluginProgress->state()!= TLP_CANCEL;

  if (dataSet!=0) {
    dataSet->set("strengthGraph", quotientGraph);
  }
  delete values;
  return true;
}
//================================================================================
bool StrengthClustering::check(string &erreurMsg) {
  if (!SimpleTest::isSimple(graph)) {
    erreurMsg ="The graph must be simple";
    return false;
  }
  if (!ConnectedTest::isConnected(graph)) {
    erreurMsg ="The graph must be connected";
    return false;
  }
  
  return true;
}
