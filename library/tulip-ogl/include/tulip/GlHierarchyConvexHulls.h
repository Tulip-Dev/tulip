#ifndef GL_HCVXHULL_H
#define GL_HCVXHULL_H

namespace tlp {

  class GlComposite;
  class ConvexHullItem;
  class GlConvexHull;
	class Graph;
	class GlLayer;

  /**
   * Build a hierarchy of convex hulls
   */
  class TLP_GL_SCOPE GlHierarchyConvexHulls {

  public:
    /**
     * Build the hierarchy for the given graph in GlLayer
     */
    static void compute(tlp::GlLayer*,tlp::Graph *);

		private:
		GlHierarchyConvexHulls();
		~GlHierarchyConvexHulls();
		
		/**
     * Create the hierarchy of ConvexHullItem
     */
    static void buildComposite(ConvexHullItem *convexHull, GlConvexHull *oldHull);
    static void setToOld(ConvexHullItem *convexHull, GlConvexHull *oldHull);
  };
}

#endif
