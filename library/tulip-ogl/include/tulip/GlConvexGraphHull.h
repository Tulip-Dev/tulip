/*
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
///@cond DOXYGEN_HIDDEN

#ifndef GLCONVEXGRAPHHULL_H_
#define GLCONVEXGRAPHHULL_H_

#include <tulip/Color.h>
#include <tulip/tulipconf.h>
#include <tulip/GlComposite.h>

namespace tlp {

class Graph;
class LayoutProperty;
class SizeProperty;
class DoubleProperty;
class GlComplexPolygon;

/**
 * @brief This class is a specific container for GlConvexGraphHull
 * it allows to indicates if the textures of the hulls have to be applied
 * or not
 */
class TLP_GL_SCOPE GlConvexGraphHullsComposite : public GlComposite {
  bool textured;

public:
  GlConvexGraphHullsComposite() : textured(false) {}

  bool hullsTextureActivation() {
    return textured;
  }
  void setHullsTextureActivation(bool);
};

/**
 * @brief This class draws a convex hull around a graph.
 *
 * Given a graph, this class computes the various control points of a convex hull that embraces the
 * nodes and edges present into the graph.
 * If this entity is translated using the translate method, every element of the linked graph will
 * be translated as well.
 *
 * @warning The graph or any of the properties linked to a GlConvexGraphHull should never be deleted
 * before the entity. Such a thing should be
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
  GlConvexGraphHull(GlConvexGraphHullsComposite *parent, const std::string &name,
                    const tlp::Color &fcolor, const std::string &texName, Graph *graph,
                    LayoutProperty *layout, SizeProperty *size, DoubleProperty *rotation);

  ~GlConvexGraphHull();

  /**
   * Recomputes the whole Hull
   */
  void updateHull(LayoutProperty *layout = nullptr, SizeProperty *size = nullptr,
                  DoubleProperty *rotation = nullptr);

  void setVisible(bool visible);
  bool isVisible();
  void setTextureZoom(float zoom);

private:
  GlConvexGraphHullsComposite *_parent;
  std::string _name;
  Color _fcolor;
  std::string _tex;
  GlComplexPolygon *_polygon;
  Graph *graph;
  LayoutProperty *_layout;
  SizeProperty *_size;
  DoubleProperty *_rotation;
  static int bezierValue;
};
} // namespace tlp

#endif /* GLCONVEXGRAPHHULL_H_ */
///@endcond
