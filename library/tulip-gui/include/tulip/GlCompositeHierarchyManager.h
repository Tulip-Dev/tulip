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

#ifndef GL_HCVXHULL_H
#define GL_HCVXHULL_H

#include <string>
#include <map>

#include <tulip/DataSet.h>
#include <tulip/Observable.h>
#include <tulip/GlComposite.h>

namespace tlp {

class GlConvexGraphHull;

struct node;
class Color;
class LayoutProperty;
class DoubleProperty;
class SizeProperty;
struct ConvexHullItem;
class GlConvexHull;
class Graph;
class GlLayer;

/**
 * Create a GlComposite item for each and every graph and subgraph.
 * This class observes the graph to update said hierarchy when a subgraph is added or deleted.
 */
class TLP_QT_SCOPE GlCompositeHierarchyManager : private Observable {

public:
  GlCompositeHierarchyManager(Graph* graph, GlLayer* layer,
                              const std::string& layerName,
                              LayoutProperty* layout, SizeProperty* size,
                              DoubleProperty* rotation, bool visible = false,
                              const std::string& namingProperty = "name",
                              const std::string& subCompositeSuffix = " sub-hulls");

  void setGraph(tlp::Graph* graph);
  DataSet getData();
  void setData(const DataSet &dataSet);

  void createComposite();

  void setVisible(bool visible);
  bool isVisible() const;

protected :
  virtual void treatEvents(const  std::vector<Event> &events );
  virtual void treatEvent(const Event&);

private:
  bool _shouldRecreate;
  const tlp::Color getColor();
  int _currentColor;

  /**
   * Create the hierarchy of ConvexHullItem
   */
  void buildComposite(tlp::Graph* current, tlp::GlComposite* composite);

  Graph* _graph;
  GlLayer* _layer;
  GlComposite* _composite;
  LayoutProperty* _layout;
  SizeProperty* _size;
  DoubleProperty* _rotation;
  std::vector<Color> _fillColors;
  std::string _layerName;
  bool _isVisible;
  const std::string _subCompositesSuffix;
  const std::string _nameAttribute;

  const static std::string temporaryPropertyValue;
  /**
   * This map contains the composite that contains the associated graph's hull.
   */
  std::map<tlp::Graph*, std::pair<tlp::GlComposite*, GlConvexGraphHull*> > _graphsComposites;
};

class GlHierarchyMainComposite : public GlComposite {
public:
  GlHierarchyMainComposite(GlCompositeHierarchyManager* manager);
  virtual void setVisible(bool visible);
private:
  GlCompositeHierarchyManager* _manager;
};
}

#endif
///@endcond
