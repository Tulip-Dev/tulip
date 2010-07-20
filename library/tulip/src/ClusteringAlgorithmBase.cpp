#include "tulip/ClusteringAlgorithmBase.h"
#include "tulip/ClusteringQualityMeasure.h"

#include <tulip/DoubleProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/ForEach.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/SimpleTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/QClusteringQualityMeasure.h>

using namespace tlp;
using namespace std;
ALGORITHMPLUGIN(DivisiveQClustering, "Divisive Newman Clustering (new version)", "Romain Bourqui, Charles Huet","12/07/2010","OK","1.2");

ClusteringAlgorithmBase::ClusteringAlgorithmBase(AlgorithmContext context)
  : Algorithm(context), 
  _quotientGraph(tlp::newGraph()), 
  _intraEdges(_quotientGraph->getProperty<DoubleProperty>("intraEdges")), 
  _extraEdges(_quotientGraph->getProperty<DoubleProperty>("extraEdges")),
  _qualityMeasure(NULL){
}

ClusteringAlgorithmBase::~ClusteringAlgorithmBase() {
  delete _qualityMeasure;
}

bool ClusteringAlgorithmBase::run() {
  _qualityMeasure = getQualityMeasure();
  _qualityMeasure->initialize();;
  runClustering();
}

//================================================================================
const Graph* const ClusteringAlgorithmBase::getOriginalGraph() const {
    return graph;
}

const DataSet& ClusteringAlgorithmBase::getDataSet() const {
    return *dataSet;
}

const std::vector<std::vector<node> >& ClusteringAlgorithmBase::getPartition() const {
    return this->_partition;
}

const unsigned int ClusteringAlgorithmBase::getPartitionId(tlp::node n) const {
  return _partitionId.get(n.id);
}

const Graph* const ClusteringAlgorithmBase::getQuotientGraph() const {
    return _quotientGraph;
}

const double ClusteringAlgorithmBase::getIntraEdges(tlp::node n) const {
  return _intraEdges->getNodeValue(n);
}

const double ClusteringAlgorithmBase::getExtraEdges(tlp::edge e) const {
  return _extraEdges->getEdgeValue(e);
}

const double ClusteringAlgorithmBase::getExtraEdges(tlp::node n) const {
  return _extraEdges->getNodeValue(n);
}

void ClusteringAlgorithmBase::orderByPartitionId(node &n1, node &n2) const {
    unsigned int i = _partitionId.get(n1.id);
    unsigned int j = _partitionId.get(n2.id);
    if (i>j) {
        node tmp = n1;
        n1 = n2;
        n2 = tmp;
    }
}

//================================================================================

AgglomerativeClusteringBase::AgglomerativeClusteringBase(AlgorithmContext context)
  :ClusteringAlgorithmBase(context) {   
}

node AgglomerativeClusteringBase::mergeNodes(node n1, node n2) {
   assert(SimpleTest::isSimple(_quotientGraph));
    assert(n1 != n2);
#ifndef NDEBUG
    cerr << n1.id << "/" << n2.id << "==>";
#endif
    orderByPartitionId(n1, n2);
#ifndef NDEBUG
    cerr << n1.id << "/" << n2.id << endl;;
#endif
    assert(n1 != n2);
    //====================================================
    _qualityMeasure->beforeMergeNodes(n1, n2);
    //====================================================
    //contains all the neighbours nodes of n1
    TLP_HASH_SET<node> n1Set;
    edge n1n2;
    //fast access to an edge between n1 and one of its neighbours from the neighbour of n1
    MutableContainer<edge> nodeToEdge;
    edge e;
    forEach(e, _quotientGraph->getInOutEdges(n1)) {
        node n = _quotientGraph->opposite(e, n1);
        nodeToEdge.set(n.id, e);
        n1Set.insert(n);
    }

    double val_n1n2=0;;
    stableForEach(e, _quotientGraph->getInOutEdges(n2)) {
        node n = _quotientGraph->opposite(e, n2);
        if (n1 == n) { //find the egde between n1 and n2
            n1n2 = e;
            val_n1n2 += _extraEdges->getEdgeValue(n1n2);
        }
        else {
            if (n1Set.find(n) == n1Set.end()) {
                edge newEdge = _quotientGraph->addEdge(n1, n);
                _extraEdges->setEdgeValue(newEdge, _extraEdges->getEdgeValue(e));
            }
            else {
                edge n1Edge = nodeToEdge.get(n.id);
                _extraEdges->setEdgeValue(n1Edge, _extraEdges->getEdgeValue(e) + _extraEdges->getEdgeValue(n1Edge));
            }
        }
    }

//   std::cout << "setting intraEdges(" << n1.id << ") to: " << _intraEdges.get(n1.id) << "+" << _intraEdges.get(n2.id) << "+" << val_n1n2 << std::endl;
    _intraEdges->setNodeValue(n1, _intraEdges->getNodeValue(n1) + _intraEdges->getNodeValue(n2) + val_n1n2);
    _extraEdges->setNodeValue(n1, _extraEdges->getNodeValue(n1) + _extraEdges->getNodeValue(n2) - 2*val_n1n2);

    unsigned int i = _partitionId.get(n1.id);
    unsigned int j = _partitionId.get(n2.id);
    vector<node>::const_iterator itV = _partition[j].begin();
    for (; itV!=_partition[j].end(); ++itV)
        _partition[i].push_back(*itV);
    _partition[j] = _partition.back();
    node tmp = _partitionNode.get(_partition.size()-1);
    _partitionId.set(tmp.id, j);
    _partitionNode.set(j, tmp);
    _partition.pop_back();

    _quotientGraph->delNode(n2);

    _qualityMeasure->afterMergeNodes(n1);
    return n1;
}

void AgglomerativeClusteringBase::simpleGraphCopy(const Graph* source, Graph* target, MutableContainer<node>& nodeMapping) {
    _metric = target->getProperty<DoubleProperty>("viewMetric");
    node n;
    forEach(n, source->getNodes()) {
        node newNode = target->addNode();
        _extraEdges->setNodeValue(newNode, source->deg(n));
        nodeMapping.set(n.id, newNode);
    }
    edge e;
    forEach(e, source->getEdges()) {
        edge e2 = target->addEdge(nodeMapping.get(source->source(e).id), nodeMapping.get(source->target(e).id));
        _metric->setEdgeValue(e2, 1);
        _extraEdges->setEdgeValue(e2, 1);
    }
}

void AgglomerativeClusteringBase::buildHierarchy(Graph * graph, vector<std::vector<std::vector<node> > >& partitions, int best_ind) {
    assert(best_ind < partitions.size());
    for (unsigned int i = 0; i < partitions[best_ind].size(); ++i) {
        vector<node> group = partitions[best_ind][i];
        set<node> toGroup;
        for (unsigned int j = 0; j < group.size(); ++j)
            toGroup.insert(group[j]);
        tlp::inducedSubGraph(graph, toGroup);
    }
}
//================================================================================

DivisiveClusteringBase::DivisiveClusteringBase(AlgorithmContext context) 
  :ClusteringAlgorithmBase(context) {
  if(context.graph)
    _sumEdges = context.graph->numberOfEdges();
}

edge DivisiveClusteringBase::findEdgeToRemove() {
  edge edgeToDel;
  edge e;
  
  string errMsg;
  DoubleProperty values(_workingGraph);
  _workingGraph->computeProperty(_metricAlgorithm, &values, errMsg, NULL);

  double max = -1;
  double min = 1000000;
  
  switch (metric_mode) {
    case 0:
      forEach(e, _workingGraph->getEdges()) {
        if (values.getEdgeValue(e) > max) {
          max = values.getEdgeValue(e) ;
          edgeToDel = e;
        }
      }
      break;
    case 1:
    case 2:
      forEach(e, _workingGraph->getEdges()) {
        if (values.getEdgeValue(e)  < min) {
            min = values.getEdgeValue(e);
            edgeToDel = e;
        }
      }
      break;
  }
  return edgeToDel;
}


const char * paramHelp2[] = {
  // Q/MQ
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Boolean" ) \
  HTML_HELP_BODY() \
  "If true uses MQ to compute the hierarchy, otherwise uses Q" \
  HTML_HELP_CLOSE(), \
  // Metric
  HTML_HELP_OPEN()       \
  HTML_HELP_DEF( "type", "int" )   \
  HTML_HELP_DEF( "values", "{0,1,2}" )   \
  HTML_HELP_DEF( "default", "0" )  \
  HTML_HELP_BODY()              \
  "This parameter defines the metric used for the algorithm. Following values are corrects :" \
  "<ul><li>0: betweenness centrality;</li>"     \
  "<li>1: strength metric;</li>"      \
  "<li>2: Jaccard's index.</li>"    \
   HTML_HELP_CLOSE(),
};


DivisiveQClustering::DivisiveQClustering(AlgorithmContext context)
  : DivisiveClusteringBase(context) {
    addParameter<bool>("MQ / Q", paramHelp2[0], "true");
    addParameter<int>("Metric type", paramHelp2[1], "0");
}

bool DivisiveQClustering::runClustering() {
  mqUse = true;
  if(dataSet != 0){
    dataSet->get("Metric type", metric_mode);
    dataSet->get("MQ / Q", mqUse);
  }
  else{
    metric_mode = 0;
  }
  
  _metricAlgorithm = "Node - Edge Betweenness Centrality";
  if(metric_mode == 1) 
    _metricAlgorithm = "Strength";
  else
    _metricAlgorithm = "Jaccard Index";
  
  cerr << "[BEGIN] NewMan Clustering: " << graph->numberOfNodes() << "  " << graph->numberOfEdges() << endl;
  vector<set<node> > bestLevel(1);
  double bestQ = 0;
  vector<Graph *> curClusters(1);
  _workingGraph = tlp::newCloneSubGraph(graph);
  node n = _quotientGraph->addNode();
  _intraEdges->setNodeValue(n, graph->numberOfEdges());
  _partitionNode.set(0, n);
  _partitionId.set(n.id, 0);
  
  curClusters[0] = tlp::newCloneSubGraph(graph);;
  
  forEach(n,graph->getNodes()){
    bestLevel[0].insert(n);
  }

  edge e;
  int nbEdges = graph->numberOfEdges();
  while(nbEdges > 0){
    pluginProgress->progress(graph->numberOfEdges() - nbEdges, graph->numberOfEdges());
    cerr << ".";
    
    edge edgeToDel = findEdgeToRemove();
    
    Graph * deconnectedG;
    bool allConnected = true;
    int ind =  -1;
    for(unsigned int i = 0 ; i < curClusters.size() ; ++i){
      if(curClusters[i]->isElement(edgeToDel)){
        ind = i;
        deconnectedG = curClusters[i];
        break;
      }
    }
    
    --nbEdges;
    _workingGraph->delEdge(edgeToDel);
    if(ind != -1)
      deconnectedG->delEdge(edgeToDel);

    if(!ConnectedTest::isConnected(deconnectedG)){
      DoubleProperty connectedComponnent(deconnectedG);
      string err;
      deconnectedG->computeProperty(string("Connected Component"), &connectedComponnent, err);
      DataSet tmp;
      tmp.set("Property", &connectedComponnent);
      tlp::applyAlgorithm(deconnectedG, err, &tmp, "Equal Value");
      bool first = true;

      Graph *g;
      forEach(g, deconnectedG->getSubGraphs()){
        if(first) {
          curClusters[ind] = g;
          first= false;
        }
        else 
          curClusters.push_back(g);
      }
      graph->delSubGraph(deconnectedG);
      
      //update the quotient graph
      node newCluster = _quotientGraph->addNode();
      node oldCluster = _partitionNode.get(ind);
      
      double oldClusterIntraEdges = 0;
      double oldClusterDegree = 0;
      forEach(n, curClusters[ind]->getNodes()) {
        oldClusterDegree += graph->deg(n);
        forEach(e, graph->getInOutEdges(n)) {
          node opposite = graph->opposite(e, n);
          if(curClusters[ind]->isElement(opposite))
            ++oldClusterIntraEdges;
        }
      }
      oldClusterDegree -= oldClusterIntraEdges;
      oldClusterIntraEdges /= 2;
      
      double newClusterIntraEdges = 0;
      double newClusterDegree = 0;
      forEach(n, curClusters[curClusters.size()-1]->getNodes()) {
        newClusterDegree += graph->deg(n);
        forEach(e, graph->getInOutEdges(n)) {
          node opposite = graph->opposite(e, n);
          if(curClusters[curClusters.size()-1]->isElement(opposite))
            ++newClusterIntraEdges;
        }
      }
      newClusterDegree -= newClusterIntraEdges;
      newClusterIntraEdges /= 2;
      
      _intraEdges->setNodeValue(newCluster, newClusterIntraEdges);
      _intraEdges->setNodeValue(oldCluster, oldClusterIntraEdges);
//       _intraEdges->setNodeValue(oldCluster, curClusters[ind]->numberOfEdges());
      _partitionNode.set(curClusters.size()-1, newCluster);
//       _partitionId.set(newCluster, curClusters.size()-1);

      _extraEdges->setNodeValue(oldCluster, oldClusterDegree);
      _extraEdges->setNodeValue(newCluster, newClusterDegree);
      
      edge clustersEdge = _quotientGraph->addEdge(oldCluster, newCluster);

      double q = _qualityMeasure->getQuality();

      stringstream comment;
      comment << endl << "New Q = " << q << " with " << curClusters.size() << " clusters"<< endl;
      cerr << comment.str();
      pluginProgress->setComment(comment.str());
      
      if(q > bestQ){
        bestQ = q;
        bestLevel.clear();
        for(unsigned int i = 0; i < curClusters.size(); ++i){
          node u;
          set<node> clust;
          forEach(u,curClusters[i]->getNodes())
            clust.insert(u);
          bestLevel.push_back(clust);
        }
      }
    }
  }
  graph->delSubGraph(_workingGraph);
  for(unsigned int i = 0 ; i < curClusters.size(); ++i){
    graph->delSubGraph(curClusters[i]);
  }
  
  for(unsigned int i = 0 ; i < bestLevel.size(); ++i){
    tlp::inducedSubGraph(graph, bestLevel[i]);
  }
  cerr << "END :  q = " << bestQ << " with " << bestLevel.size() << " clusters" << endl;
  cerr << "[END]" << endl;
}

ClusteringQualityMeasure* DivisiveQClustering::getQualityMeasure() {
  //TODO mqUse should be used to determine the quality measure to use
  return new QClusteringQualityMeasure(this);
}
