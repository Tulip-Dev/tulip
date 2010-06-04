//-*-c++-*-
/*
 Author: Ludwig Fiolka
 Email : ludwig.fiolka@inria.fr
 Last modification : Mar 24, 2010
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 */

#ifndef GLCONVEXGRAPHHULL_H_
#define GLCONVEXGRAPHHULL_H_

#include <tulip/GlComplexPolygon.h>
#ifndef NDEBUG
#include <tulip/ObservableProperty.h>
#include <tulip/ObservableGraph.h>
#endif /* NDEBUG */

namespace tlp {
class Graph;

class LayoutProperty;
class SizeProperty;
class DoubleProperty;

/**
 * @brief This class draws a convex hull around a graph.
 *
 * Given a graph, this class computes the various control points of a convex hull that embraces the nodes and edges present into the graph.
 * If this entity is translated using the translate method, every element of the linked graph will be translated as well.
 *
 * @warning The graph or any of the properties linked to a GlConvexGraphHull should never be deleted before the entity. Such a thing should be
 * reported to the user in debug mode, raising an assertion.
 */
#ifndef NDEBUG
class TLP_GL_SCOPE GlConvexGraphHull: public GlComplexPolygon, public GraphObserver, public PropertyObserver {
#else
  class TLP_GL_SCOPE GlConvexGraphHull: public GlComplexPolygon {
#endif /* NDEBUG */
public:
  /**
   * @param fcolor The color used to fill the hull.
   * @param graph The graph whose elements should be inside the hull.
   * @param layout The property used to layout the elements in the graph.
   * @param size The property defining the graph's elements' sizes.
   * @param rotation The property defining the graph's elements' rotation.
   */
  GlConvexGraphHull(const tlp::Color &fcolor, Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation);

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Draw the complex polygon
   */
  virtual void draw(float lod, Camera *camera);

#ifndef NDEBUG
  virtual void destroy(Graph *);
  virtual void destroy(PropertyInterface*);
#endif /* NDEBUG */

private:
  Graph *graph;
  LayoutProperty *layout;
  SizeProperty *size;
  DoubleProperty *rotation;
};
}

#endif /* GLCONVEXGRAPHHULL_H_ */
