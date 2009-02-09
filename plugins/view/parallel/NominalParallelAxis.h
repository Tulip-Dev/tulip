//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef NOMINAL_PARALLEL_AXIS_H
#define NOMINAL_PARALLEL_AXIS_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/GlNominativeAxis.h>

#include "ParallelAxis.h"
#include "ParallelCoordinatesGraphProxy.h"


namespace tlp {

// Class which allow to render a nominal axis (labels are string)
class NominalParallelAxis : public ParallelAxis {

 public :

  NominalParallelAxis(const Coord &base_coord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graphProxy, const std::string &graphPropertyName, const Color &axisColor);

  Coord getLabelCoord(const std::string &label);

  Coord getPointCoordOnAxisForData(const unsigned int dataIdx);

  //void translate(const Coord &c);

  void showConfigDialog();

  void redraw();

  void setLabels();

  const std::vector<std::string> &getLabelsOrder() const {return labelsOrder;}
  void setLabelsOrder(const std::vector<std::string> &labelsOrder) {this->labelsOrder = labelsOrder;}

  std::string getTopSliderTextValue() {return "";}
  std::string getBottomSliderTextValue() {return "";}

  std::set<unsigned int> getDataInSlidersRange();
  void updateSlidersWithDataSubset(const std::set<unsigned int> &dataSubset);

private :

  GlNominativeAxis *glNominativeAxis;
  ParallelCoordinatesGraphProxy *graphProxy;
  std::vector<std::string> labelsOrder;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
