#include "tulip/MQClusteringQualityMeasure.h"

#include "tulip/ClusteringAlgorithmBase.h"
#include <tulip/Iterator.h>
#include <tulip/Edge.h>
#include <tulip/ForEach.h>

using namespace tlp;

MQClusteringQualityMeasure::MQClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase*const clustering) 
  : ClusteringQualityMeasure(clustering), _positive(0), _negative(0) {
}

double MQClusteringQualityMeasure::getQuality() {
  double size = double(_clusteringAlgorithm->getPartition().size());
  double positive = _positive / size;
  double negative = _negative / (double(size*(size-1)) / 2.0);
//   std::cout << "(+: " << positive << ";" << _positive << ")" << "(-:" << negative << ";" << _negative << ")" << "(size:" << size << ")" << std::endl; 
  return positive - negative;
}

void MQClusteringQualityMeasure::removePositive(tlp::node n1, tlp::node n2, double positive) {
  unsigned int partId = _clusteringAlgorithm->getPartitionId(n1);
  double size = double(_clusteringAlgorithm->getPartition()[partId].size());
  if (size > 1)
    positive -= 2.0 * _clusteringAlgorithm->getIntraEdges(n1) / double(size * (size - 1));
  partId = _clusteringAlgorithm->getPartitionId(n2);
  if (size > 1)
    positive -= 2.0 * _clusteringAlgorithm->getIntraEdges(n2) / double(size * (size - 1));
}

void MQClusteringQualityMeasure::removeNegative(node n1, node n2, double negative) {
  node tab[2]; //Tab enables iteration and thus avoids code duplication
  tab[0] = n1;
  tab[1] = n2;  
  for (unsigned int i=0;i<2; ++i) {
    Iterator<edge> *itE = _clusteringAlgorithm->getQuotientGraph()->getInOutEdges(tab[i]);
    while(itE->hasNext()) {
      edge e = itE->next();
      node n_1 = _clusteringAlgorithm->getQuotientGraph()->source(e);
      node n_2 = _clusteringAlgorithm->getQuotientGraph()->target(e);
      if (i == 1 && (n_1 == tab[0] || n_2 == tab[0])) continue; //prevents treating n1n2 twice
      unsigned int p1 = _clusteringAlgorithm->getPartitionId(n_1);
      unsigned int p2 = _clusteringAlgorithm->getPartitionId(n_2);
      if ((_clusteringAlgorithm->getPartition()[p1].size()!=0) && (_clusteringAlgorithm->getPartition()[p2].size()!=0))
        negative -= _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartition()[p1].size() * _clusteringAlgorithm->getPartition()[p2].size());
    } delete itE;
  }
}

void MQClusteringQualityMeasure::beforeMergeNodes(tlp::node n1, tlp::node n2) {
  removePositive(n1, n2, _positive);
  removeNegative(n1, n2, _negative);
}

void MQClusteringQualityMeasure::afterMergeNodes(tlp::node n1) {
  unsigned int partId = _clusteringAlgorithm->getPartitionId(n1);
  double size = double(_clusteringAlgorithm->getPartition()[partId].size());
//   std::cout << "afterMergeNodes: " << size << ";" << _clusteringAlgorithm->getIntraEdges().get(n1.id) << std::endl; 
  if (size > 1)
    _positive += 2.0 * _clusteringAlgorithm->getIntraEdges(n1) / double(size * (size - 1));
  
  {
    edge e;
    forEach(e, _clusteringAlgorithm->getQuotientGraph()->getInOutEdges(n1)) {
      node n_1 = _clusteringAlgorithm->getQuotientGraph()->source(e);
      node n_2 = _clusteringAlgorithm->getQuotientGraph()->target(e);
      unsigned int p1 = _clusteringAlgorithm->getPartitionId(n_1);
      unsigned int p2 = _clusteringAlgorithm->getPartitionId(n_2);
      if ( (_clusteringAlgorithm->getPartition()[p1].size()!=0) && (_clusteringAlgorithm->getPartition()[p2].size()!=0))
        _negative += _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartition()[p1].size() * _clusteringAlgorithm->getPartition()[p2].size());
    }
  }
}

double MQClusteringQualityMeasure::getQualityIfMerged(node n1, node n2) {
  _clusteringAlgorithm->orderByPartitionId(n1, n2);
  double tmpneg = _negative;
  double tmppos = _positive;
  removePositive(n1, n2, tmpneg);
  removeNegative(n1, n2, tmppos);
  //=====================================================
  unsigned int newPartSize;
  newPartSize = _clusteringAlgorithm->getPartition()[_clusteringAlgorithm->getPartitionId(n1)].size() 
    + _clusteringAlgorithm->getPartition()[_clusteringAlgorithm->getPartitionId(n2)].size();
  //=====================================================
  // compute new negative
  //Tab for enabling iteration and thus removing code duplication
  node tab[2]; tab[0] = n1; tab[1] = n2; 
  edge n1n2;
  for (unsigned int k=0; k<2; ++k) {
    edge e;
    forEach(e, _clusteringAlgorithm->getQuotientGraph()->getInOutEdges(tab[k])) {
      node n = _clusteringAlgorithm->getQuotientGraph()->opposite(e, tab[k]);
      if (tab[(k+1)%2] != n) { //not the edge between n1 and n2
        unsigned partIdn = _clusteringAlgorithm->getPartitionId(n);
        tmpneg += _clusteringAlgorithm->getExtraEdges(e) / double(newPartSize * _clusteringAlgorithm->getPartition()[partIdn].size());
      } 
      else {
        n1n2 = e;
      }
    }
  }
  //=========================================================
  // compute new positive
  double newIntra = _clusteringAlgorithm->getIntraEdges(n1) + _clusteringAlgorithm->getIntraEdges(n2) + _clusteringAlgorithm->getExtraEdges(n1n2);
  tmppos += 2.0 * newIntra / double(newPartSize * (newPartSize - 1));
  double partSize = _clusteringAlgorithm->getPartition().size() - 1;
  tmppos  /= partSize;
  tmpneg  /= partSize*(partSize-1) / 2.0;
  //  cerr  << __FUNCTION__ << "pos : " << tmppos << " neg: " << tmpneg << endl;
  return tmppos - tmpneg;
}

void MQClusteringQualityMeasure::initialize() {
 double positive = 0;
  Iterator<node> *itN = _clusteringAlgorithm->getQuotientGraph()->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    unsigned int partId = _clusteringAlgorithm->getPartitionId(n);
    if (_clusteringAlgorithm->getPartition()[partId].size() > 1) {
//       std::cout << _clusteringAlgorithm->getIntraEdges().get(n.id) << "/(" << _clusteringAlgorithm->getPartition()[partId].size() << "*" << (_clusteringAlgorithm->getPartition()[partId].size() - 1) << std::endl;
      positive += 2.0 * _clusteringAlgorithm->getIntraEdges(n) / double(_clusteringAlgorithm->getPartition()[partId].size() * (_clusteringAlgorithm->getPartition()[partId].size() - 1));
    }
  } delete itN;
  _positive = positive;
  double partitionSize = _clusteringAlgorithm->getPartition().size();
  positive /= partitionSize;

  //compute negative part of MQ
  double negative = 0;  
  Iterator<edge> *itE = _clusteringAlgorithm->getQuotientGraph()->getEdges();
  while(itE->hasNext()) {
    edge e = itE->next();
    node n1 = _clusteringAlgorithm->getQuotientGraph()->source(e);
    node n2 = _clusteringAlgorithm->getQuotientGraph()->target(e);
    unsigned int p1 = _clusteringAlgorithm->getPartitionId(n1);
    unsigned int p2 = _clusteringAlgorithm->getPartitionId(n2);
    if ( (_clusteringAlgorithm->getPartition()[p1].size()!=0) && (_clusteringAlgorithm->getPartition()[p2].size()!=0)) {
//       std::cout << _clusteringAlgorithm->getExtraEdges().get(e.id) << "/(" << _clusteringAlgorithm->getPartition()[p1].size() << "*" << _clusteringAlgorithm->getPartition()[p2].size() << std::endl;
      negative += _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartition()[p1].size() * _clusteringAlgorithm->getPartition()[p2].size());
    }
  } delete itE;
  _negative = negative;
  if (partitionSize > 1)
    negative /= double(partitionSize*(partitionSize-1)) / 2.0;
//   std::cerr << __FUNCTION__ << " : pos : " << positive << " neg:" << negative << std::endl;
//   return positive - negative;
}
