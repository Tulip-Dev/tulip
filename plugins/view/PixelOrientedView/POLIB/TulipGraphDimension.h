/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef TULIPGRAPHDIMENSION_H_
#define TULIPGRAPHDIMENSION_H_

#include <tulip/Graph.h>

#include <string>
#include <map>

#include "DimensionBase.h"
#include "TulipNodeMetricSorter.h"

namespace pocore {

class TulipGraphDimension : public DimensionBase {

public:
  TulipGraphDimension(tlp::Graph *graph, const std::string &dimensionName);
  ~TulipGraphDimension() override;

  unsigned int numberOfItems() const override;
  unsigned int numberOfValues() const override;
  /* not used
  std::string getItemLabelAtRank(const unsigned int rank) const override;
  std::string getItemLabel(const unsigned int itemId) const override;
  */
  double getItemValue(const unsigned int itemId) const override;
  double getItemValueAtRank(const unsigned int rank) const override;
  unsigned int getItemIdAtRank(const unsigned int rank) override;
  unsigned int getRankForItem(const unsigned int itemId) override;
  double minValue() const override;
  double maxValue() const override;
  std::vector<unsigned int> links(const unsigned int itemId) const override;
  std::string getDimensionName() const override {
    return dimName;
  }
  tlp::Graph *getTulipGraph() const {
    return graph;
  }
  void updateNodesRank();

private:
  template <typename PROPERTY>
  double getNodeValue(const tlp::node n) const;

  tlp::Graph *graph;
  std::string dimName;
  std::string propertyType;
  std::vector<tlp::node> dataOrder;

  TulipNodeMetricSorter *nodeSorter;
  static std::map<tlp::Graph *, unsigned int> graphDimensionsMap;
};
} // namespace pocore

#endif /* TULIPGRAPHDIMENSION_H_ */
