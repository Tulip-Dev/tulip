#include "tulip/GlHierarchyConvexHulls.h"
#include "tulip/GlConvexHull.h"
#include <sstream>

using namespace tlp;
using namespace std;

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

  GlComposite* glHulls=new GlComposite;
  Graph *newGraph=graph;
  Graph *sGraph = graph->getSuperGraph();
  unsigned int depth = 1;
  while(newGraph != sGraph) {
    Graph *tmp = sGraph;
    sGraph = sGraph->getSuperGraph();
    newGraph = tmp;
    depth++;
  }
  
  // build convex hulls
  vector<GlConvexHull *> convexHulls =
    GlConvexHull::buildConvexHullsFromHierarchy(graph,
						vector<Color>(),
						vector<Color>(),
						deducedFromChilds,
						graph,
						depth);

  // add convex hulls in gl composite
  vector<GlConvexHull *>::const_iterator it = convexHulls.begin();
  stringstream sstr;
  unsigned int i = 0;
  bool isVisible;
  int stencil;
  if(layer->findGlEntity("Hulls")){
    isVisible=layer->findGlEntity("Hulls")->isVisible();
    stencil=layer->findGlEntity("Hulls")->getStencil();
  }else{
    isVisible=false;
    stencil=0xFFFF;
  }

  layer->deleteGlEntity("Hulls");
  glHulls->setVisible(isVisible);
  glHulls->setStencil(stencil);
  layer->addGlEntity(glHulls,"Hulls");
  
  for (; it != convexHulls.end(); it++, i++) {
    sstr.clear();
    sstr << i;
    (*it)->setStencil(stencil);
    glHulls->addGlEntity(*it, sstr.str());
  }
}
