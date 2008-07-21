#ifndef GL_HCVXHULL_H
#define GL_HCVXHULL_H

#include <tulip/Graph.h>

#include "tulip/GlLayer.h"

namespace tlp {

  class GlComposite;
  class ConvexHullItem;
  class GlConvexHull;

  /**
   * Build a hierarchy of convex hulls
   */
  class TLP_GL_SCOPE GlHierarchyConvexHulls {
    
  public:
    /**
     * Basic constructor
     */
    GlHierarchyConvexHulls(bool fatherHullsIsDeducedFromChilds = true) {
      deducedFromChilds = fatherHullsIsDeducedFromChilds;
    }
    virtual ~GlHierarchyConvexHulls() {}
    
    /**
     * Build the hierarchy for the given graph in GlLayer
     */
    virtual void compute(tlp::GlLayer*,tlp::Graph *);

    GlComposite *buildComposite(ConvexHullItem *convexHull, GlConvexHull *oldHull);
    void setToOld(ConvexHullItem *convexHull, GlConvexHull *oldHull);
    
    bool deducedFromChilds;
  };
}

#endif
