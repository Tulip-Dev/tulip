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

#ifndef GL_CONVEX_HULL_H
#define GL_CONVEX_HULL_H

#include <vector>

#include <tulip/Color.h>
#include <tulip/GlComposite.h>

namespace tlp {

struct ConvexHullItem;
class Graph;

/** \brief Class used to represent ConvexHull
 *
 * Class used to represent ConvexHull. GlHierarchyConvexHull class use this to create all convexHull
 * of the graph
 */
class TLP_GL_SCOPE GlConvexHull : public GlComposite {
  /**
   * Build a Polygon with the convex hull of points
   */
  GlConvexHull(std::vector<Coord> &points, std::vector<Color> &fillColors,
               std::vector<Color> &outlineColors, const bool filled, const bool outlined,
               const std::string &name, bool computeHull = true);

public:
  /**
   * Default constructor
   */
  GlConvexHull() {}

  ~GlConvexHull() override {}

  /**
   * Function used to visit composite's children
   */
  void acceptVisitor(GlSceneVisitor *visitor) override {
    if (boundingBox.isValid()) {
      visitor->visit(this);
    }

    for (std::list<GlSimpleEntity *>::iterator it = _sortedElements.begin();
         it != _sortedElements.end(); ++it) {
      (*it)->acceptVisitor(visitor);
    }
  }

  /**
   * Return the name of this convex hull
   */
  std::string getName() {
    return _name;
  }

  /**
   * Draw the convexHull
   */
  void draw(float lod, Camera *camera) override;

  /**
   * Static function who build a hierarchy of convexHull with the given graph
   */
  static ConvexHullItem *buildConvexHullsFromHierarchy(Graph *graph, std::vector<Color> fColors,
                                                       std::vector<Color> oColors,
                                                       bool deduceFromChilds = true,
                                                       Graph *root = nullptr,
                                                       unsigned int depth = 0);

  /**
   * Translate entity
   */
  void translate(const Coord &mouvement) override;

  /**
   * Function to export data in outString (in XML format)
   */
  void getXML(std::string &outString) override;

  /**
   * Function to set data with inString (in XML format)
   */
  void setWithXML(const std::string &inString, unsigned int &currentPosition) override;

  tlp::Graph *_graph;

protected:
  std::vector<Coord> _points;
  std::vector<Color> _fillColors;
  std::vector<Color> _outlineColors;
  bool _filled;
  bool _outlined;
  std::string _name;
};

struct ConvexHullItem {
  GlConvexHull *hull;
  Graph *_graph;
  std::string name;
  std::vector<ConvexHullItem *> children;
};
} // namespace tlp
#endif
///@endcond
