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

#ifndef GLCONVEXGRAPHHULL_H_
#define GLCONVEXGRAPHHULL_H_

#include <tulip/ObservableGraph.h>
#include <tulip/Color.h>
#include <tulip/ObservableProperty.h>

namespace tlp {

class GlComposite;
class GlSimpleEntity;
class Camera;
class Coord;
class Graph;
class LayoutProperty;
class SizeProperty;
class DoubleProperty;
class GlComplexPolygon;

/**
 * @brief This class draws a convex hull around a graph.
 *
 * Given a graph, this class computes the various control points of a convex hull that embraces the nodes and edges present into the graph.
 * If this entity is translated using the translate method, every element of the linked graph will be translated as well.
 *
 * @warning The graph or any of the properties linked to a GlConvexGraphHull should never be deleted before the entity. Such a thing should be
 * reported to the user in debug mode, raising an assertion.
 */
class TLP_GL_SCOPE GlConvexGraphHull: public GraphObserver, public PropertyObserver {
public:
  /**
   * @param fcolor The color used to fill the hull.
   * @param graph The graph whose elements should be inside the hull.
   * @param layout The property used to layout the elements in the graph.
   * @param size The property defining the graph's elements' sizes.
   * @param rotation The property defining the graph's elements' rotation.
   */
  GlConvexGraphHull(GlComposite* parent, const std::string& name, const tlp::Color &fcolor, Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation);
  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

	/**
	 * Recomputes the whole Hull
	 */
	void updateHull();
	
	void setVisible(bool visible);
	bool isVisible();
	
  /**
   * Draw the complex polygon
   */
  virtual void draw(float lod, Camera *camera);

	virtual void addNode(tlp::Graph* graph, tlp::node n);
	
	virtual void afterSetNodeValue(PropertyInterface* property, const node n);
	
#ifndef NDEBUG
  virtual void destroy(Graph *);
  virtual void destroy(PropertyInterface*);
#endif /* NDEBUG */

private:
	GlComposite* _parent;
	std::string _name;
	GlComplexPolygon* _polygon;
	Color _fcolor;
  Graph *graph;
  LayoutProperty *layout;
  SizeProperty *size;
  DoubleProperty *rotation;
	static int bezierValue;
};
}

#endif /* GLCONVEXGRAPHHULL_H_ */
