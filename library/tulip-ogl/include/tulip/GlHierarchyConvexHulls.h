#ifndef GL_HCVXHULL_H
#define GL_HCVXHULL_H

#include <tulip/Graph.h>

#include "tulip/GlLayer.h"

class GlHierarchyConvexHulls {

 public:
  GlHierarchyConvexHulls(bool fatherHullsIsDeducedFromChilds = true) {
    deducedFromChilds = fatherHullsIsDeducedFromChilds;
  }
  virtual ~GlHierarchyConvexHulls() {}
  virtual void compute(tlp::GlLayer*,tlp::Graph *);

  bool deducedFromChilds;
};

#endif
