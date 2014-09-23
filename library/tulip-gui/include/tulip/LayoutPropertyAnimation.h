/*
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
///@cond DOXYGEN_HIDDEN

#ifndef LAYOUTPROPERTYANIMATION_H_
#define LAYOUTPROPERTYANIMATION_H_

#include <tulip/Coord.h>
#include <tulip/CachedPropertyAnimation.h>

#include <vector>

namespace tlp {
class LayoutProperty;
class Graph;
class BooleanProperty;

class TLP_QT_SCOPE LayoutPropertyAnimation: public CachedPropertyAnimation<tlp::LayoutProperty, tlp::Coord, std::vector<tlp::Coord> > {
public:
  LayoutPropertyAnimation(tlp::Graph *g, tlp::LayoutProperty *start, tlp::LayoutProperty *end, tlp::LayoutProperty *out,
                          tlp::BooleanProperty *selection = NULL, int frameCount = 1, bool computeNodes = true, bool computeEdges = true, QObject *parent=NULL);

  virtual ~LayoutPropertyAnimation() {}

protected:
  virtual tlp::Coord getNodeFrameValue(const tlp::Coord &startValue, const tlp::Coord &endValue, int frame);
  virtual std::vector<tlp::Coord> getEdgeFrameValue(const std::vector<tlp::Coord> &startValue, const std::vector<tlp::Coord> &endValue, int frame);
  bool equalEdges(const std::vector<tlp::Coord> &v1, const std::vector<tlp::Coord> &v2);

private:
  std::map<std::pair<tlp::Coord,tlp::Coord>, tlp::Vector<double, 3> > steps;
};

}

#endif /* LAYOUTPROPERTYANIMATION_H_ */
///@endcond
