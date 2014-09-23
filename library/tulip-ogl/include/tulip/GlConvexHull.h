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
 * Class used to represent ConvexHull. GlHierarchyConvexHull class use this to create all convexHull of the graph
 */
class TLP_GL_SCOPE GlConvexHull: public GlComposite {
public:

  /**
   * Default constructor
   */
  GlConvexHull() {}

  /**
   * Build a Polygon with the convex hull of points
   */
  GlConvexHull(const std::vector<Coord> &points,
               const std::vector<Color> &fillColors,
               const std::vector<Color> &outlineColors,
               const bool filled,
               const bool outlined,
               const std::string &name,
               bool computeHull=true);

  virtual ~GlConvexHull() {}

  /**
   * Function used to visit composite's children
   */
  virtual void acceptVisitor(GlSceneVisitor *visitor) {
    if(boundingBox.isValid()) {
      visitor->visit(this);
    }

    for(std::list<GlSimpleEntity*>::iterator it=_sortedElements.begin(); it!=_sortedElements.end(); ++it) {
      (*it)->acceptVisitor(visitor);
    }
  }


  /**
   * Retrun the name of this convex hull
   */
  std::string getName() {
    return _name;
  }

  /**
   * Draw the convexHull
   */
  virtual void draw(float lod,Camera *camera);

  /**
   * Static function who build a hierarchy of convexHull with the given graph
   */
  static ConvexHullItem *buildConvexHullsFromHierarchy(Graph *graph,
      std::vector<Color> fColors,
      std::vector<Color> oColors,
      bool deduceFromChilds = true,
      Graph *root = NULL,
      unsigned int depth = 0);

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

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
  GlConvexHull* hull;
  Graph *_graph;
  std::string name;
  std::vector<ConvexHullItem *> children;
};

}
#endif
///@endcond
