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
 
    glHulls=new GlComposite;
    if(oldGlHulls) {
      glHulls->setVisible(oldGlHulls->isVisible());
      glHulls->setStencil(oldGlHulls->getStencil());
    }else{
      glHulls->setVisible(false);
      glHulls->setStencil(0xFFFF);
    }

    layer->addGlEntity(glHulls,"Hulls");
  

    // build convex hulls
    vector<GlConvexHull *> convexHulls=GlConvexHull::buildConvexHullsFromHierarchy(graph,
										   vector<Color>(),
										   vector<Color>(),
										   false,
										   graph,
										   depth);

    // add convex hulls in gl composite
    vector<GlConvexHull *>::const_iterator it = convexHulls.begin();
    unsigned int i = 0;
    unsigned int noname=0;

    for (; it != convexHulls.end(); it++, i++) {
      stringstream sstr;
      if((*it)->getName()!="") {
	sstr << (*it)->getName();
      }else{
	sstr << "noname_" << noname ;
	noname++;
      }

      if(oldGlHulls->findGlEntity(sstr.str())){
	GlSimpleEntity *oldEntity=oldGlHulls->findGlEntity(sstr.str());
	(*it)->setVisible(oldEntity->isVisible());
	(*it)->setStencil(oldEntity->getStencil());
      }else{
	(*it)->setStencil(glHulls->getStencil());
	(*it)->setVisible(glHulls->isVisible());
      }
    
      glHulls->addGlEntity(*it, sstr.str());
    }

    oldGlHulls->reset(true);
  }

}
