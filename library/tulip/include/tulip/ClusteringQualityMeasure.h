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
#ifndef CLUSTERINGQUALITYMEASURE_H
#define CLUSTERINGQUALITYMEASURE_H
#include "tulip/Node.h"

namespace tlp {
  class Graph;
  class DoubleProperty;
  class ClusteringAlgorithmBase;

  class TLP_SCOPE ClusteringQualityMeasure {
    public:
      ClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase* const clustering);
      virtual double getQuality() = 0;
      virtual double getQualityIfMerged(tlp::node n1, tlp::node n2) = 0;
      virtual void beforeMergeNodes(tlp::node, tlp::node) {}
      virtual void afterMergeNodes(tlp::node) {}
      virtual void beforeSplitNode(tlp::node) {}
      virtual void afterSplitNode(tlp::node, tlp::node) {}
      virtual void initialize()  {};
    protected:
      const tlp::ClusteringAlgorithmBase* const _clusteringAlgorithm;
  };
}
#endif //CLUSTERINGQUALITYMEASURE_H
