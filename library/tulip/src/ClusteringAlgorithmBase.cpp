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

ClusteringAlgorithmBase::ClusteringAlgorithmBase(AlgorithmContext context)
  : Algorithm(context), 
  _quotientGraph(tlp::newGraph()), 
  _intraEdges(_quotientGraph->getProperty<DoubleProperty>("intraEdges")), 
  _extraEdges(_quotientGraph->getProperty<DoubleProperty>("extraEdges")),
  _qualityMeasure(NULL){
    //use default values for the intra and extra edges
    _intraEdges->setAllEdgeValue(0);
    _extraEdges->setAllEdgeValue(1);
    _extraEdges->setAllNodeValue(0);
}

ClusteringAlgorithmBase::~ClusteringAlgorithmBase() {
  delete _qualityMeasure;
}

bool ClusteringAlgorithmBase::run() {
  _qualityMeasure = getQualityMeasure();
  _qualityMeasure->initialize();
  return runClustering();
}

const Graph* ClusteringAlgorithmBase::getOriginalGraph() const {
    return graph;
}

const DataSet& ClusteringAlgorithmBase::getDataSet() const {
    return *dataSet;
}

int ClusteringAlgorithmBase::getPartitionSize() const {
  return this->_partition.size();
}

int ClusteringAlgorithmBase::getPartitionSize(int paritionIndex) const {
  return this->_partition[paritionIndex].size();
}

int ClusteringAlgorithmBase::getPartitionSize(node quotientNode) const {
  return this->_partition[_partitionId.get(quotientNode.id)].size();
}

unsigned int ClusteringAlgorithmBase::getPartitionId(tlp::node n) const {
  return _partitionId.get(n.id);
}

const Graph* ClusteringAlgorithmBase::getQuotientGraph() const {
    return _quotientGraph;
}

double ClusteringAlgorithmBase::getIntraEdges(tlp::node n) const {
  return _intraEdges->getNodeValue(n);
}

double ClusteringAlgorithmBase::getExtraEdges(tlp::edge e) const {
  return _extraEdges->getEdgeValue(e);
}

double ClusteringAlgorithmBase::getExtraEdges(tlp::node n) const {
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
  if(context.graph) {
    _intraEdges->setAllNodeValue(0);
    _extraEdges->setAllEdgeValue(0);
    
    best_ind = -1;
    bestQuality = -2;
    
    MutableContainer<node> nodeMapping;  
    simpleGraphCopy(graph, _quotientGraph, nodeMapping);
    
    unsigned int i = 0;
    Iterator<node> *it = graph->getNodes();
    while (it->hasNext()) {
        set<node> tmp;
        node n = it->next();
        tmp.insert(n);
        _partition.push_back(tmp);
        _partitionId.set(nodeMapping.get(n.id).id, i);
        _partitionNode.set(i, nodeMapping.get(n.id));
        ++i;
    } delete it;
    
    _hierarchy.push_back(_partition);
  }
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
    set<node>::const_iterator itV = _partition[j].begin();
    for (; itV!=_partition[j].end(); ++itV)
        _partition[i].insert(*itV);
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
    }
    _extraEdges->setAllEdgeValue(1);
    _intraEdges->setAllNodeValue(0);
}

void AgglomerativeClusteringBase::buildHierarchy(Graph * graph, vector<std::vector<std::set<node> > >& partitions, int best_ind) {
    assert(((int)partitions.size()) > 0);
    assert(best_ind < (int)partitions.size());
    for (size_t i = 0; i < partitions[best_ind].size(); ++i) {
        set<node> group = partitions[best_ind][i];
        tlp::inducedSubGraph(graph, group);
    }
}

//================================================================================

DivisiveClusteringBase::DivisiveClusteringBase(AlgorithmContext context) 
  :ClusteringAlgorithmBase(context), metric_mode(0) {
  if(context.graph) {
    _sumEdges = context.graph->numberOfEdges();
    set<node> initialClustering;
    node n;
    node quotientOriginal = _quotientGraph->addNode();
    forEach(n, context.graph->getNodes()) {
      initialClustering.insert(n);
      _originalToQuotient[n] = quotientOriginal;
    }
    _partition.push_back(initialClustering);
    
    _intraEdges->setNodeValue(quotientOriginal, graph->numberOfEdges());
    _partitionNode.set(0, quotientOriginal);
    _partitionId.set(quotientOriginal.id, 0);
    
    _workingGraph = tlp::newCloneSubGraph(graph);
    _currentClusters.push_back(tlp::newCloneSubGraph(graph));
  }
}

edge DivisiveClusteringBase::findEdgeToRemove() {
  assert(_workingGraph->numberOfEdges() > 0);
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

bool DivisiveClusteringBase::splitGraphIfDisconnected(int clusterIndex, Graph*const cluster, Graph** subCluster1, Graph** subCluster2) {
  bool isConnected = ConnectedTest::isConnected(cluster);
  if(!isConnected){
    _qualityMeasure->beforeSplitNode(_partitionNode.get(clusterIndex));
    
    DoubleProperty connectedComponnent(cluster);
    string err;
    cluster->computeProperty(string("Connected Component"), &connectedComponnent, err);
    DataSet tmp;
    tmp.set("Property", &connectedComponnent);
    tlp::applyAlgorithm(cluster, err, &tmp, "Equal Value");
    
    Iterator<Graph*>* it = cluster->getSubGraphs();
    assert(it->hasNext());
    *subCluster1 = it->next();
    assert(it->hasNext());
    *subCluster2 = it->next();
    
    //update the quotient graph
    node newCluster = _quotientGraph->addNode();
    node oldCluster = _partitionNode.get(clusterIndex);
    
    edge e;
    forEach(e, _quotientGraph->getInOutEdges(oldCluster)) {
      _extraEdges->setEdgeValue(e, 0);
    }
    
    double oldClusterIntraEdges = 0;
    double oldClusterDegree = 0;
    double newClusterIntraEdges = 0;
    double newClusterDegree = 0;
    node n;
    
    //arrays and loop to avoid code duplication
    Graph* subClusters[2];
    subClusters[0] = *subCluster1;
    subClusters[1] = *subCluster2;
    node quotientNodes[2];
    quotientNodes[0] = oldCluster;
    quotientNodes[1] = newCluster;
    
    _partition[clusterIndex].clear();
    forEach(n, (*subCluster1)->getNodes()) {
      _partition[clusterIndex].insert(n);
    }
    
    set<node> newPartition;
    forEach(n, (*subCluster2)->getNodes()) {
      newPartition.insert(n);
      _originalToQuotient[n] = newCluster;
    }
    _partition.push_back(newPartition);
    
    _partitionId.set(newCluster.id, _partition.size()-1);
    _partitionNode.set(_partition.size()-1, newCluster);
    
    for(int i = 0; i < 2; ++i) {
      node quotientN = quotientNodes[i];
      forEach(n, subClusters[i]->getNodes()) {
        forEach(e, graph->getInOutEdges(n)) {
          node opposite = graph->opposite(e, n);
          if(subClusters[i]->isElement(opposite)) {
            if(i==0)
              oldClusterIntraEdges++;
            else
              newClusterIntraEdges++;
          }
          else {
            node quotientOpposite = _originalToQuotient[opposite];
            
            edge quotientEdge = _quotientGraph->existEdge(quotientN, quotientOpposite, false);
            if(!quotientEdge.isValid()) {
              quotientEdge = _quotientGraph->addEdge(quotientN, quotientOpposite);
            }
            else {
              _extraEdges->setEdgeValue(quotientEdge, _extraEdges->getEdgeValue(quotientEdge)+1);
            }
            if(i==0)
              oldClusterDegree++;
            else
              newClusterDegree++;
          }
        }
      }
    }
    newClusterIntraEdges /= 2;
    oldClusterIntraEdges /= 2;
    
    edge newEdge = _quotientGraph->existEdge(newCluster, oldCluster, false);
    _extraEdges->setEdgeValue(newEdge, _extraEdges->getEdgeValue(newEdge)/2);
    
    _intraEdges->setNodeValue(newCluster, newClusterIntraEdges);
    _intraEdges->setNodeValue(oldCluster, oldClusterIntraEdges);

    _extraEdges->setNodeValue(newCluster, newClusterDegree);
    _extraEdges->setNodeValue(oldCluster, oldClusterDegree);

//     std::cout << "NEW (intra;extra): " << newClusterIntraEdges << ";" << newClusterDegree<< std::endl; 
//     std::cout << "OLD (intra;extra): " << oldClusterIntraEdges << ";" << oldClusterDegree<< std::endl;
    _qualityMeasure->afterSplitNode(oldCluster, newCluster);
  }
  
  return !isConnected;
}
