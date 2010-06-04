#include <tulip/GlConvexGraphHull.h>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/DrawingTools.h>

using namespace std;

#ifndef NDEBUG
#include <iostream>
#endif /* NDEBUG */

namespace tlp {

GlConvexGraphHull::GlConvexGraphHull(const Color &fcolor, Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation) :
  GlComplexPolygon(computeConvexHull(graph, layout, size, rotation, 0), fcolor), graph(graph), layout(layout), size(size), rotation(rotation) {
#ifndef NDEBUG
  assert(graph);
  assert(layout);
  assert(size);
  assert(rotation);
  graph->addGraphObserver(this);
  layout->addPropertyObserver(this);
  size->addPropertyObserver(this);
  rotation->addPropertyObserver(this);
#endif /* NDEBUG */
}

void GlConvexGraphHull::translate(const Coord& mouvement) {
#ifndef NDEBUG
  assert(graph);
#endif /* NDEBUG */
  layout->translate(mouvement, graph);
}

// TODO: optimize this to update control points at redraw only if one the properties has been modified
void GlConvexGraphHull::draw(float lod, Camera *camera) {
#ifndef NDEBUG
  assert(graph);
#endif /* NDEBUG */
  points.clear();
  createPolygon(computeConvexHull(graph, layout, size, rotation, 0), 1);
  GlComplexPolygon::draw(lod, camera);
}

#ifndef NDEBUG
void GlConvexGraphHull::destroy(Graph *) {
  graph = 0;
  cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " The graph linked to this entity has been destroyed. This situation should not happen." << endl;
}
void GlConvexGraphHull::destroy(PropertyInterface *prop) {
  graph = 0;
  cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " The property " << prop->getName()
      << " linked to this entity has been destroyed. This situation should not happen." << endl;
}
#endif /* NDEBUG */

}
