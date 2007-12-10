#ifndef GL_CONVEX_HULL_H
#define GL_CONVEX_HULL_H

#include <vector>
#include <tulip/BoundingBox.h>
#include <tulip/Graph.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlComposite.h"


class GlConvexHull: public tlp::GlSimpleEntity {
 public:
  // build a Polygon with the convex hull of points
  GlConvexHull(const std::vector<tlp::Coord> &points, 
	       const std::vector<tlp::Color> &fillColors,
	       const std::vector<tlp::Color> &outlineColors,
	       const bool filled,
	       const bool outlined,
	       bool computeHull=true);

  virtual ~GlConvexHull() {}

  virtual tlp::BoundingBox getBoundingBox();

  virtual void draw(float lod);

  static std::vector<GlConvexHull *> buildConvexHullsFromHierarchy(tlp::Graph *graph,
								   std::vector<tlp::Color> fColors,
								   std::vector<tlp::Color> oColors,
								   bool deduceFromChilds = true,
								   tlp::Graph *root = 0,
								   unsigned int depth = 0);

 protected:
  std::vector<tlp::Coord> _points;
  std::vector<tlp::Color> _fillColors;
  std::vector<tlp::Color> _outlineColors;
  bool _filled;
  bool _outlined;
};
#endif
