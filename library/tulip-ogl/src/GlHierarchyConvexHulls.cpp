#include "tulip/GlHierarchyConvexHulls.h"
#include "tulip/GlConvexHull.h"
#include <sstream>

using namespace std;

namespace tlp {

  // the i2dea is to use a GlADComposite to draw the convex hull
  // build from the current graph
  void GlHierarchyConvexHulls::compute(GlLayer *layer,Graph *graph) {
    if(!graph)
      return;
    if(layer->findGlEntity("Hulls")){
      if(!layer->findGlEntity("Hulls")->isVisible()){
	return;
      }
    }else{
      return;
    }

    Graph *newGraph=graph;
    Graph *sGraph = graph->getSuperGraph();
    unsigned int depth = 1;
    while(newGraph != sGraph) {
      Graph *tmp = sGraph;
      sGraph = sGraph->getSuperGraph();
      newGraph = tmp;
      depth++;
    }
  
    GlComposite* glHulls;
    GlComposite* oldGlHulls=NULL;
    if(layer->findGlEntity("Hulls")){
      oldGlHulls=(GlComposite*)(layer->findGlEntity("Hulls"));
      layer->deleteGlEntity(oldGlHulls);
    }
 
    ConvexHullItem* convexHull=GlConvexHull::buildConvexHullsFromHierarchy(graph,
									   vector<Color>(),
									   vector<Color>(),
									   false,
									   graph,
									   depth);
    glHulls=buildComposite(convexHull,(GlConvexHull*)oldGlHulls);
    setToOld(convexHull,(GlConvexHull*)oldGlHulls);

    layer->addGlEntity(glHulls,"Hulls");   

    oldGlHulls->reset(true);
  }

  GlComposite *GlHierarchyConvexHulls::buildComposite(ConvexHullItem *convexHull, GlConvexHull *oldHull) {
    GlComposite *child;
    for(vector<ConvexHullItem *>::iterator it=convexHull->children.begin();it!=convexHull->children.end();++it) {
      GlConvexHull *oldChild=(GlConvexHull*)oldHull->findGlEntity((*it)->name);
      child=buildComposite(*it,oldChild);
      convexHull->hull->addGlEntity(child,(*it)->name);
    }
    return convexHull->hull;
  }

  void GlHierarchyConvexHulls::setToOld(ConvexHullItem *convexHull, GlConvexHull *oldHull) {
    if(oldHull) {
      convexHull->hull->setVisible(oldHull->isVisible());
      convexHull->hull->setStencil(oldHull->getStencil());
    }
    for(vector<ConvexHullItem *>::iterator it=convexHull->children.begin();it!=convexHull->children.end();++it) {
      GlConvexHull *oldChild=(GlConvexHull*)oldHull->findGlEntity((*it)->name);
      setToOld(*it,oldChild);
    }
  }

}
