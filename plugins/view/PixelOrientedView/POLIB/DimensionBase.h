/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef DIMENSION_BASE
#define DIMENSION_BASE

#include <vector>
#include <string>

namespace pocore {
struct DimensionBase {
  virtual ~DimensionBase() {}
  virtual unsigned int numberOfItems() const = 0;
  virtual unsigned int numberOfValues() const = 0;
  virtual std::string getItemLabelAtRank(const unsigned int rank) const =0;
  virtual std::string getItemLabel(const unsigned int itemId) const =0;
  virtual double getItemValue(const unsigned int itemId) const = 0;
  virtual double getItemValueAtRank(const unsigned int rank) const = 0;
  virtual unsigned int getItemIdAtRank(const unsigned int rank) = 0;
  virtual unsigned int getRankForItem(const unsigned int itemId) = 0;
  virtual double minValue() const = 0;
  virtual double maxValue() const = 0;
  virtual std::vector<unsigned int> links(const unsigned int itemId) const = 0;
  virtual std::string getDimensionName() const = 0;
};
}

#endif
