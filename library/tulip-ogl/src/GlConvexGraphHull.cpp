/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <tulip/GlConvexGraphHull.h>

#include <tulip/Graph.h>
#include <tulip/DrawingTools.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlComposite.h>
using namespace std;

namespace tlp {

int GlConvexGraphHull::bezierValue = 1;

GlConvexGraphHull::GlConvexGraphHull(GlComposite* parent, const std::string& name, const tlp::Color &fcolor, Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation)       :
  _parent(parent), _name(name), _fcolor(fcolor), _polygon(NULL), graph(graph), layout(layout), size(size), rotation(rotation) {
  assert(graph);

  if (graph->numberOfNodes() > 0) {
    _polygon = new GlComplexPolygon(computeConvexHull(graph, layout, size, rotation, 0), fcolor, GlConvexGraphHull::bezierValue);
    _parent->addGlEntity(_polygon, _name);
  }
}

GlConvexGraphHull::~GlConvexGraphHull() {
  delete _polygon;
  _polygon = NULL;
}

void GlConvexGraphHull::updateHull() {
  if (!isVisible())
    return;

  if (_polygon) {
    _parent->deleteGlEntity(_polygon);
    delete _polygon;
  }

  _polygon = new GlComplexPolygon(computeConvexHull(graph, layout, size, rotation, 0), _fcolor, GlConvexGraphHull::bezierValue);
  _parent->addGlEntity(_polygon, _name);
}

bool GlConvexGraphHull::isVisible() {
  return _polygon && _polygon->isVisible();
}

void GlConvexGraphHull::setVisible(bool visible) {
  if (_polygon)
    _polygon->setVisible(visible);
}


}
