/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef MQCLUSTERINGQUALITYMEASURE_H
#define MQCLUSTERINGQUALITYMEASURE_H

#include "ClusteringQualityMeasure.h"

namespace tlp {
  class ClusteringAlgorithmBase;

  class TLP_SCOPE MQClusteringQualityMeasure : public ClusteringQualityMeasure {
    public:
      MQClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase* const clustering);
      virtual double getQuality();
      virtual double getQualityIfMerged(tlp::node n1, tlp::node n2);
      void removePositive(tlp::node n1, tlp::node n2, double& positive);
      void removeNegative(tlp::node n1, tlp::node n2, double& negative);
      virtual void beforeMergeNodes(tlp::node n1, tlp::node n2);
      virtual void afterMergeNodes(tlp::node n1);
      virtual void beforeSplitNode(tlp::node n);
      //oldNode is the old node, but now represents a new cluster, newnode is a new node and represents a new cluster
      virtual void afterSplitNode(tlp::node oldNode, tlp::node newNode);
      virtual void initialize();
    private:
      double _positive;
      double _negative;
  };
}

#endif //MQCLUSTERINGQUALITYMEASURE_H
