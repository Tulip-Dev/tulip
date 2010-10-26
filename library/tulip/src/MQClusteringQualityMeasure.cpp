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
  double size = double(_clusteringAlgorithm->getPartitionSize());
//   std::cout << "partitionsize: " << size << std::endl; 
  double positive = _positive / size;
  if(size == 1) {
    std::cout << "Quality MQ : " << positive << " => " << size << std::endl;
   return positive;
  }
  double negative = _negative / (double(size*(size-1)) / 2.0);
//   std::cout << positive << "\t" << negative << std::endl;
//   std::cout << "(+: " << positive << ";" << _positive << ")" << "(-:" << negative << ";" << _negative << ")" << "(size:" << size << ")" << std::endl; 
//  std::cout << "Quality MQ : " << "(" << positive << " - "<< negative <<" ) = "<< positive - negative << " => " << size << std::endl;
  return positive - negative;
}

void MQClusteringQualityMeasure::removePositive(tlp::node n1, tlp::node n2, double& positive) {
  unsigned int partId = _clusteringAlgorithm->getPartitionId(n1);
  double partitionSize = double(_clusteringAlgorithm->getPartitionSize(partId));
  if (partitionSize > 1)
    positive -= 2.0 * _clusteringAlgorithm->getIntraEdges(n1) / double(partitionSize * (partitionSize - 1));
  partId = _clusteringAlgorithm->getPartitionId(n2);
  partitionSize = double(_clusteringAlgorithm->getPartitionSize(partId));
  if (partitionSize > 1)
    positive -= 2.0 * _clusteringAlgorithm->getIntraEdges(n2) / double(partitionSize * (partitionSize - 1));
}

void MQClusteringQualityMeasure::removeNegative(node n1, node n2, double& negative) {
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
      if ((_clusteringAlgorithm->getPartitionSize(p1)!=0) && (_clusteringAlgorithm->getPartitionSize(p2)!=0))
        negative -= _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartitionSize(p1) * _clusteringAlgorithm->getPartitionSize(p2));
    } delete itE;
  }
}

void MQClusteringQualityMeasure::beforeMergeNodes(tlp::node n1, tlp::node n2) {
  removePositive(n1, n2, _positive);
  removeNegative(n1, n2, _negative);
}

void MQClusteringQualityMeasure::afterMergeNodes(tlp::node n1) {
  unsigned int partId = _clusteringAlgorithm->getPartitionId(n1);
  double size = double(_clusteringAlgorithm->getPartitionSize(partId));
//   std::cout << "afterMergeNodes: " << size << ";" << _clusteringAlgorithm->getIntraEdges().get(n1.id) << std::endl; 
  if (size > 1)
    _positive += 2.0 * _clusteringAlgorithm->getIntraEdges(n1) / double(size * (size - 1));
  
  edge e;
  forEach(e, _clusteringAlgorithm->getQuotientGraph()->getInOutEdges(n1)) {
    node n_1 = _clusteringAlgorithm->getQuotientGraph()->source(e);
    node n_2 = _clusteringAlgorithm->getQuotientGraph()->target(e);
    unsigned int p1 = _clusteringAlgorithm->getPartitionId(n_1);
    unsigned int p2 = _clusteringAlgorithm->getPartitionId(n_2);
    if ( (_clusteringAlgorithm->getPartitionSize(p1)!=0) && (_clusteringAlgorithm->getPartitionSize(p2)!=0))
      _negative += _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartitionSize(p1) * _clusteringAlgorithm->getPartitionSize(p2));
  }
//   std::cout << "+: " << _positive << " ; -: " << _negative << std::endl; 
}

void MQClusteringQualityMeasure::beforeSplitNode(tlp::node n) {
  // remove positive
//   std::cout << "beforeSplitNode: " << "(+: " << _positive << ")" << "(-:" << _negative << ")" << std::endl; 
  unsigned int partId = _clusteringAlgorithm->getPartitionId(n);
  double partitionSize = double(_clusteringAlgorithm->getPartitionSize(partId));
  if (partitionSize > 1)
    _positive -= 2.0 * _clusteringAlgorithm->getIntraEdges(n) / double(partitionSize * (partitionSize - 1));
  
  //remove negative
  Iterator<edge> *itE = _clusteringAlgorithm->getQuotientGraph()->getInOutEdges(n);
  while(itE->hasNext()) {
    edge e = itE->next();
    node n_1 = _clusteringAlgorithm->getQuotientGraph()->source(e);
    node n_2 = _clusteringAlgorithm->getQuotientGraph()->target(e);
    unsigned int p1 = _clusteringAlgorithm->getPartitionId(n_1);
    unsigned int p2 = _clusteringAlgorithm->getPartitionId(n_2);
    if ((_clusteringAlgorithm->getPartitionSize(p1)!=0) && (_clusteringAlgorithm->getPartitionSize(p2)!=0))
      _negative -= _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartitionSize(p1) * _clusteringAlgorithm->getPartitionSize(p2));
  } delete itE;
//   std::cout << "/beforeSplitNode: " << "(+: " << _positive << ")" << "(-:" << _negative << ")" << std::endl; 
}

void MQClusteringQualityMeasure::afterSplitNode(tlp::node oldNode, tlp::node newNode) {
  //add positive
//   std::cout << "afterSplitNode: " << "(+: " << _positive << ")" << "(-:" << _negative << ")" << std::endl; 
  node tab[2]; //Tab enables iteration and thus avoids code duplication
  tab[0] = oldNode;
  tab[1] = newNode;
  for (unsigned int i=0; i<2; ++i) {
    double size = double(_clusteringAlgorithm->getPartitionSize(_clusteringAlgorithm->getPartitionId(tab[i])));
//     std::cout << i << ": 2*" << _clusteringAlgorithm->getIntraEdges(tab[i]) << "/"  << double(size * (size - 1)) << "[" << size << "]" << std::endl; 
    if(size == 1)
      _positive++;
    else
      _positive += 2.0 * _clusteringAlgorithm->getIntraEdges(tab[i]) / double(size * (size - 1));
  }
  
  //add negative
  for (unsigned int i=0; i<2; ++i) {
    edge e;
    forEach(e, _clusteringAlgorithm->getQuotientGraph()->getInOutEdges(tab[i])) {
      node n_1 = _clusteringAlgorithm->getQuotientGraph()->source(e);
      node n_2 = _clusteringAlgorithm->getQuotientGraph()->target(e);
      unsigned int p1 = _clusteringAlgorithm->getPartitionId(n_1);
      unsigned int p2 = _clusteringAlgorithm->getPartitionId(n_2);
      if ( (_clusteringAlgorithm->getPartitionSize(p1)!=0) && (_clusteringAlgorithm->getPartitionSize(p2)!=0)) {
//         std::cout << "negative += " << _clusteringAlgorithm->getExtraEdges(e) << "/" << _clusteringAlgorithm->getPartitionSize(p1) << "*" << _clusteringAlgorithm->getPartitionSize(p2) << std::endl; 
        _negative += _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartitionSize(p1) * _clusteringAlgorithm->getPartitionSize(p2));
      }
    }
  }
  edge between = _clusteringAlgorithm->getQuotientGraph()->existEdge(oldNode, newNode, false);
  _negative -= _clusteringAlgorithm->getExtraEdges(between) / double(_clusteringAlgorithm->getPartitionSize(oldNode) * _clusteringAlgorithm->getPartitionSize(newNode));
  
  //because we sum on all the couples of clusters, (i.e. sum on i, j and not sum on j > i)
//   std::cout << "/afterSplitNode: " << "(+: " << _positive << ")" << "(-:" << _negative << ")" << std::endl; 
}

double MQClusteringQualityMeasure::getQualityIfMerged(node n1, node n2) {
  _clusteringAlgorithm->orderByPartitionId(n1, n2);
  double tmpneg = _negative;
  double tmppos = _positive;
 /*  removePositive(n1, n2, tmpneg);
     removeNegative(n1, n2, tmppos);*/
  removePositive(n1, n2, tmppos);
  removeNegative(n1, n2, tmpneg);
  //=====================================================
  unsigned int newPartSize;
  newPartSize = _clusteringAlgorithm->getPartitionSize(_clusteringAlgorithm->getPartitionId(n1)) 
    + _clusteringAlgorithm->getPartitionSize(_clusteringAlgorithm->getPartitionId(n2));
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
        tmpneg += _clusteringAlgorithm->getExtraEdges(e) / double(newPartSize * _clusteringAlgorithm->getPartitionSize(partIdn));
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
  double partSize = _clusteringAlgorithm->getPartitionSize() - 1;
  tmppos  /= partSize;
  if(partSize <=1)
    return tmppos;
  tmpneg  /= partSize*(partSize-1) / 2.0;
  //std::cerr  << __FUNCTION__ << " pos : " << tmppos << " neg: " << tmpneg << std::endl;
  return tmppos - tmpneg;
}

void MQClusteringQualityMeasure::initialize() {
  double positive = 0;
  Iterator<node> *itN = _clusteringAlgorithm->getQuotientGraph()->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    unsigned int partId = _clusteringAlgorithm->getPartitionId(n);
    double nbNodes = double(_clusteringAlgorithm->getPartitionSize(partId));
    if (nbNodes > 1) {
  //       std::cout << _clusteringAlgorithm->getIntraEdges().get(n.id) << "/(" << _clusteringAlgorithm->getPartition()[partId].size() << "*" << (_clusteringAlgorithm->getPartition()[partId].size() - 1) << std::endl;
      positive += 2.0 * _clusteringAlgorithm->getIntraEdges(n) / double(nbNodes * (nbNodes - 1));
    }
  } delete itN;
  _positive = positive;
  double partitionSize = _clusteringAlgorithm->getPartitionSize();
  //std::cout << "pos : " << positive << "/" << partitionSize << std::endl;
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
    if ( (_clusteringAlgorithm->getPartitionSize(p1)!=0) && (_clusteringAlgorithm->getPartitionSize(p2)!=0)) {
      //std::cout << _clusteringAlgorithm->getExtraEdges(e) << "/(" << _clusteringAlgorithm->getPartitionSize(p1) << "*" << _clusteringAlgorithm->getPartitionSize(p2) << std::endl;
      negative += _clusteringAlgorithm->getExtraEdges(e) / double(_clusteringAlgorithm->getPartitionSize(p1) * _clusteringAlgorithm->getPartitionSize(p2));
    }
  } delete itE;
  _negative = negative;
  if (partitionSize > 1)
    negative /= double(partitionSize*(partitionSize-1)) / 2.0;
  //  std::cout << "neg : " << negative << "/" << double(partitionSize*(partitionSize-1)) / 2.0 << std::endl;
//   std::cout << "pos : " << _positive << " neg:" << _negative << std::endl;
//   double nbNodes = double(_clusteringAlgorithm->getOriginalGraph()->numberOfNodes());
//   double nbEdges = double(_clusteringAlgorithm->getOriginalGraph()->numberOfEdges());
//   std::cout << "pos : " << (2*nbEdges)/(nbNodes*(nbNodes-1)) << " (" << nbNodes << ";" << nbEdges << ")";
//   std::cout << " neg:" << 0 << std::endl;
  return; // positive - negative;
}
