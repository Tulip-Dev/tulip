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

#ifndef NOMINAL_PARALLEL_AXIS_H
#define NOMINAL_PARALLEL_AXIS_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include "ParallelAxis.h"

namespace tlp {

class ParallelCoordinatesGraphProxy;
class GlNominativeAxis;

// Class which allow to render a nominal axis (labels are string)
class NominalParallelAxis : public ParallelAxis {

public:
  NominalParallelAxis(const Coord &base_coord, const float height, const float axisAreaWidth,
                      ParallelCoordinatesGraphProxy *graphProxy,
                      const std::string &graphPropertyName, const Color &axisColor,
                      const float rotationAngle = 0,
                      const GlAxis::CaptionLabelPosition captionPosition = GlAxis::BELOW);

  Coord getLabelCoord(const std::string &label);

  Coord getPointCoordOnAxisForData(const unsigned int dataIdx) override;

  void showConfigDialog() override;

  void redraw() override;

  void setLabels();

  const std::vector<std::string> &getLabelsOrder() const {
    return labelsOrder;
  }
  void setLabelsOrder(const std::vector<std::string> &labelsOrder) {
    this->labelsOrder = labelsOrder;
  }

  std::string getTopSliderTextValue() override {
    return " ";
  }
  std::string getBottomSliderTextValue() override {
    return " ";
  }

  const std::set<unsigned int> &getDataInSlidersRange() override;
  void updateSlidersWithDataSubset(const std::set<unsigned int> &dataSubset) override;

private:
  GlNominativeAxis *glNominativeAxis;
  ParallelCoordinatesGraphProxy *graphProxy;
  std::vector<std::string> labelsOrder;
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
