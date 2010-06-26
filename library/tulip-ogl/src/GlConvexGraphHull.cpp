/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/DrawingTools.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlComposite.h>
using namespace std;

#ifndef NDEBUG
#include <iostream>
#endif /* NDEBUG */

namespace tlp {

int GlConvexGraphHull::bezierValue = 1;
	
GlConvexGraphHull::GlConvexGraphHull(GlComposite* parent, const std::string& name, const Color &fcolor, Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation) :
  _parent(parent), _name(name), _fcolor(fcolor), _polygon(NULL), graph(graph), layout(layout), size(size), rotation(rotation) {
	if(graph->numberOfNodes() > 0) {
		_polygon = new GlComplexPolygon(computeConvexHull(graph, layout, size, rotation, 0), fcolor, GlConvexGraphHull::bezierValue);
		_parent->addGlEntity(_polygon, _name);
	}
	graph->addGraphObserver(this);
	layout->addPropertyObserver(this);
#ifndef NDEBUG
  assert(graph);
  assert(layout);
  assert(size);
  assert(rotation);
  size->addPropertyObserver(this);
  rotation->addPropertyObserver(this);
#endif /* NDEBUG */
}

void GlConvexGraphHull::setVisible(bool visible) {
	if(_polygon)
		_polygon->setVisible(visible);
}

bool GlConvexGraphHull::isVisible() {
	return _polygon && _polygon->isVisible();
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
	if(_polygon) {
		_polygon->draw(lod, camera);
	}
}

void GlConvexGraphHull::updateHull() {
	if(!isVisible())
		return;
	
	if(_polygon) {
		_parent->deleteGlEntity(_polygon);
		delete _polygon;
	}
	
  _polygon = new GlComplexPolygon(computeConvexHull(graph, layout, size, rotation, 0), _fcolor, GlConvexGraphHull::bezierValue);
	_parent->addGlEntity(_polygon, _name);
}

void GlConvexGraphHull::addNode(tlp::Graph* graph, tlp::node n) {
	updateHull();
}

void GlConvexGraphHull::afterSetNodeValue(PropertyInterface*, const node) {
	updateHull();
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
