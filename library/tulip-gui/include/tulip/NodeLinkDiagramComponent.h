/*
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
///@cond DOXYGEN_HIDDEN

#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include <tulip/GlMainView.h>
#include <tulip/Camera.h>

class QDialog;

namespace tlp {
class GlGrid;
class GlCompositeHierarchyManager;


class TLP_QT_SCOPE NodeLinkDiagramComponent: public tlp::GlMainView {
  Q_OBJECT

  tlp::GlGrid* _grid;
  QDialog* _gridOptions;
  GlCompositeHierarchyManager* manager;
  bool _hasHulls;

  bool isNode;
  unsigned int itemId;

  void registerTriggers();
  void updateGrid();
public:
  static const std::string viewName;
  PLUGININFORMATIONS(NodeLinkDiagramComponent::viewName, "Tulip Team", "16/04/2008",
                     "The Node Link Diagram view is the standard representation of relational data, where entities are represented as nodes, and their relation as edges."
                     "This view allows you to change the glyph used to represent nodes (e.g. square, round, donut), as well as the shape of the arrows indicating the direction of the relationship.",
                     "1.0", "relational")

  NodeLinkDiagramComponent(const tlp::PluginContext* context = NULL);

  void setState(const tlp::DataSet &);
  tlp::DataSet state() const;

public slots:
  void draw();
  void requestChangeGraph(Graph *graph);
  const Camera& goInsideItem(node meta);

protected slots:
  void addRemoveItemToSelection();
  void selectItem();
  void deleteItem();
  void goInsideItem();
  void ungroupItem();
  void setZOrdering(bool);
  void showGridControl();
  void fillContextMenu(QMenu *menu,const QPointF &point);

protected:
  void graphChanged(tlp::Graph *);

  void createScene(Graph *graph,DataSet dataSet);
  DataSet sceneData() const;
  void loadGraphOnScene(Graph *graph);
  void useHulls(bool hasHulls);
  bool hasHulls() const;

};
}

#endif /* NODELINKDIAGRAMCOMPONENT_H_ */






///@endcond
