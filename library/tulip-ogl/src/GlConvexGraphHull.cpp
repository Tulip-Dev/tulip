/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

void GlConvexGraphHullsComposite::setHullsTextureActivation(bool flag) {
  if (flag != textured) {
    textured = flag;
    for (auto entity : getGlEntities()) {
      GlComplexPolygon *pl = dynamic_cast<GlComplexPolygon *>(entity.second);
      if (pl)
        pl->setTextureActivation(flag);
      else {
        GlConvexGraphHullsComposite *composite =
            dynamic_cast<GlConvexGraphHullsComposite *>(entity.second);
        if (composite)
          composite->setHullsTextureActivation(flag);
      }
    }
  }
}

int GlConvexGraphHull::bezierValue = 1;

GlConvexGraphHull::GlConvexGraphHull(GlConvexGraphHullsComposite *parent, const std::string &name,
                                     const tlp::Color &fcolor, const std::string &tex, Graph *graph,
                                     LayoutProperty *layout, SizeProperty *size,
                                     DoubleProperty *rotation)
    : _parent(parent), _name(name), _fcolor(fcolor), _tex(tex), _polygon(nullptr), graph(graph),
      _layout(layout), _size(size), _rotation(rotation) {
  assert(graph);

  updateHull();
}

GlConvexGraphHull::~GlConvexGraphHull() {
  delete _polygon;
}

void GlConvexGraphHull::updateHull(LayoutProperty *layout, SizeProperty *size,
                                   DoubleProperty *rotation) {
  bool visible = !_polygon || _polygon->isVisible();

  if (_polygon) {
    _parent->deleteGlEntity(_polygon);
    delete _polygon;
    _polygon = nullptr;
  }

  if (layout)
    _layout = layout;

  if (size)
    _size = size;

  if (rotation)
    _rotation = rotation;

  if (graph->isEmpty() == false) {
    _polygon = new GlComplexPolygon(computeConvexHull(graph, _layout, _size, _rotation, nullptr),
                                    _fcolor, GlConvexGraphHull::bezierValue, _tex, false);
    _polygon->setVisible(visible);
    _parent->addGlEntity(_polygon, _name);
  }
}

bool GlConvexGraphHull::isVisible() {
  return _polygon && _polygon->isVisible();
}

void GlConvexGraphHull::setVisible(bool visible) {
  if (_polygon)
    _polygon->setVisible(visible);
}

void GlConvexGraphHull::setTextureZoom(float zoom) {
  _polygon->setTextureZoom(zoom);
}

} // namespace tlp
