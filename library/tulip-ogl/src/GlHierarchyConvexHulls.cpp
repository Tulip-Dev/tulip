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
  
    ConvexHullItem* glHulls;
    GlComposite* oldGlHulls=NULL;
    GlComposite* oldRootGraphHull=NULL;
    if(layer->findGlEntity("Hulls")){
      oldGlHulls=(GlComposite*)(layer->findGlEntity("Hulls"));

      string rootGraphName;
      graph->getAttributes().get("name",rootGraphName);
      
      if(oldGlHulls->findGlEntity(rootGraphName)) {
	oldRootGraphHull=(GlComposite*)(oldGlHulls->findGlEntity(rootGraphName));
	oldGlHulls->deleteGlEntity(oldRootGraphHull);
      }
      layer->deleteGlEntity(oldGlHulls);
    }
 
    ConvexHullItem* convexHull=GlConvexHull::buildConvexHullsFromHierarchy(graph,
									   vector<Color>(),
									   vector<Color>(),
									   false,
									   graph,
									   depth);
    glHulls=buildComposite(convexHull,(GlConvexHull*)oldGlHulls);
    
    string rootGraphName;
    graph->getAttributes().get("name",rootGraphName);
    GlComposite *hullsComposite=new GlComposite;
    hullsComposite->addGlEntity(glHulls->hull,rootGraphName);
    setToOld(glHulls,(GlConvexHull*)oldRootGraphHull);

    layer->addGlEntity(hullsComposite,"Hulls");  

    oldGlHulls->reset(true);
  }

  ConvexHullItem *GlHierarchyConvexHulls::buildComposite(ConvexHullItem *convexHull, GlConvexHull *oldHull) {
    GlComposite *child;
    for(vector<ConvexHullItem *>::iterator it=convexHull->children.begin();it!=convexHull->children.end();++it) {
      GlConvexHull *oldChild;
      if(oldHull)
	oldChild=(GlConvexHull*)oldHull->findGlEntity((*it)->name);
      else
	oldChild=NULL;

      child=buildComposite(*it,oldChild)->hull;
      convexHull->hull->addGlEntity(child,(*it)->name);
    }
    return convexHull;
  }

  void GlHierarchyConvexHulls::setToOld(ConvexHullItem *convexHull, GlConvexHull *oldHull) {
    if(oldHull) {
      convexHull->hull->setVisible(oldHull->isVisible());
      convexHull->hull->setStencil(oldHull->getStencil());
    }
    for(vector<ConvexHullItem *>::iterator it=convexHull->children.begin();it!=convexHull->children.end();++it) {
      GlConvexHull *oldChild;
      if(oldHull) 
	oldChild=(GlConvexHull*)oldHull->findGlEntity((*it)->name);
      else
	oldChild=NULL;
      
      setToOld(*it,oldChild);
    }
  }

}
