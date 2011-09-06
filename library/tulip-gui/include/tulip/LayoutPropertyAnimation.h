#ifndef LAYOUTPROPERTYANIMATION_H_
#define LAYOUTPROPERTYANIMATION_H_

#include <tulip/Coord.h>
#include "tulip/CachedPropertyAnimation.h"
#include <vector>

namespace tlp {
class LayoutProperty;
class Graph;
class BooleanProperty;

class LayoutPropertyAnimation: public CachedPropertyAnimation<tlp::LayoutProperty, tlp::Coord, std::vector<tlp::Coord> > {
public:
  LayoutPropertyAnimation(tlp::Graph *g, tlp::LayoutProperty *start, tlp::LayoutProperty *end, tlp::LayoutProperty *out,
                          tlp::BooleanProperty *selection = 0, int frameCount = 1, bool computeNodes = true, bool computeEdges = true);

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
