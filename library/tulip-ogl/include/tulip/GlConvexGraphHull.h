/*
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
///@cond DOXYGEN_HIDDEN


#ifndef GLCONVEXGRAPHHULL_H_
#define GLCONVEXGRAPHHULL_H_

#include <tulip/Color.h>
#include <tulip/tulipconf.h>

namespace tlp {

class GlComposite;
class GlSimpleEntity;
class Camera;
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
class TLP_GL_SCOPE GlConvexGraphHull {
public:

  /**
   * @brief ...
   *
   * @param parent ...
   * @param name ...
   * @param fcolor The color used to fill the hull.
   * @param graph The graph whose elements should be inside the hull.
   * @param layout The property used to layout the elements in the graph.
   * @param size The property defining the graph's elements' sizes.
   * @param rotation The property defining the graph's elements' rotation.
   **/
  GlConvexGraphHull(GlComposite* parent, const std::string& name, const tlp::Color &fcolor, Graph *graph, LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation);

  ~GlConvexGraphHull();

  /**
  * Recomputes the whole Hull
  */
  void updateHull();

  void setVisible(bool visible);
  bool isVisible();
private:
  GlComposite* _parent;
  std::string _name;
  Color _fcolor;
  GlComplexPolygon* _polygon;
  Graph *graph;
  LayoutProperty *layout;
  SizeProperty *size;
  DoubleProperty *rotation;
  static int bezierValue;
};
}

#endif /* GLCONVEXGRAPHHULL_H_ */
///@endcond
