#ifndef CONCAVEHULLBUILDER_H
#define CONCAVEHULLBUILDER_H

#include <tulip/Coord.h>

#include <vector>

namespace tlp {

class Graph;

class TLP_GLES_SCOPE ConcaveHullBuilder {

public:

  ConcaveHullBuilder();

  void setScalingFactor(const float scalingFactor);

  void setHullWithHoles(const bool hullWithHoles);

  void setHullZValue(const float z);

  void addPolygon(const std::vector<tlp::Coord> &polygon);

  void computeHulls();

  unsigned int nbComputedHulls() const;

  const std::vector<std::vector<tlp::Coord> > &getHullWithHoles(unsigned int hullId) const;

  const std::vector<tlp::Coord> &getHullOuterContour(unsigned int hullId) const;

  void clear();

  static std::vector<std::vector<tlp::Coord> > computeGraphHullVertices(tlp::Graph *graph, const bool withHoles = true, const float spacing = 0.1f, const float z=0);

private:

  float _scalingFactor;

  bool _hullWithHoles;

  float _z;

  std::vector<std::vector<tlp::Coord> > _polygons;

  std::vector<std::vector<std::vector<tlp::Coord> > > _computedHulls;

};

}


#endif // CONCAVEHULLBUILDER_H
