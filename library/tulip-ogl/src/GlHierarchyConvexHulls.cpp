#include <tulip/Graph.h>
#include "tulip/GlLayer.h"
#include "tulip/GlHierarchyConvexHulls.h"
#include "tulip/GlConvexHull.h"
#include <sstream>

using namespace std;

namespace tlp {

	// the idea is to use a GlADComposite to draw the convex hull
  // build from the current graph
  void GlHierarchyConvexHulls::compute(GlLayer *layer,Graph *graph) {
    if(!graph)
      return;
		GlSimpleEntity* hullsEntity = layer->findGlEntity("Hulls");
    
		if(!(hullsEntity && hullsEntity->isVisible())){
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

    GlComposite* oldGlHulls=NULL;
    GlComposite* oldRootGraphHull=NULL;
    
		oldGlHulls=(GlComposite*)hullsEntity;

		string rootGraphName;
		graph->getAttributes().get("name",rootGraphName);
		
		if(oldGlHulls->findGlEntity(rootGraphName)) {
			oldRootGraphHull=(GlComposite*)(oldGlHulls->findGlEntity(rootGraphName));
			oldGlHulls->deleteGlEntity(oldRootGraphHull);
		}
		layer->deleteGlEntity(oldGlHulls);

    ConvexHullItem* convexHull=GlConvexHull::buildConvexHullsFromHierarchy(graph,
                                                                           vector<Color>(),
                                                                           vector<Color>(),
                                                                           false,
                                                                           graph,
                                                                           depth);
    GlHierarchyConvexHulls::buildComposite(convexHull,(GlConvexHull*)oldGlHulls);
    
    GlConvexHull *hullsComposite=new GlConvexHull;
    hullsComposite->addGlEntity(convexHull->hull,rootGraphName);
    GlHierarchyConvexHulls::setToOld(convexHull,(GlConvexHull*)oldRootGraphHull);

    layer->addGlEntity(hullsComposite,"Hulls");  

    oldGlHulls->reset(true);
  }

  void GlHierarchyConvexHulls::buildComposite(ConvexHullItem *convexHull, GlConvexHull *oldHull) {
    for(vector<ConvexHullItem *>::iterator it=convexHull->children.begin();it!=convexHull->children.end();++it) {
      GlConvexHull *oldChild;
      if(oldHull)
        oldChild=(GlConvexHull*)oldHull->findGlEntity((*it)->name);
      else
        oldChild=NULL;

      buildComposite(*it,oldChild);
      convexHull->hull->addGlEntity((*it)->hull,(*it)->name);
    }
  }

  void GlHierarchyConvexHulls::setToOld(ConvexHullItem *convexHull, GlConvexHull *oldHull) {
		if(!oldHull)
			return;
		
		convexHull->hull->setVisible(oldHull->isVisible());
		convexHull->hull->setStencil(oldHull->getStencil());
    for(vector<ConvexHullItem *>::iterator it=convexHull->children.begin();it!=convexHull->children.end();++it)
      setToOld(*it,(GlConvexHull*)oldHull->findGlEntity((*it)->name));
  }

}
