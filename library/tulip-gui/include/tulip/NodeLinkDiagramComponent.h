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

#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include <tulip/GlMainView.h>
#include <tulip/Camera.h>

namespace Ui {
class GridOptionsWidget;
}

class QDialog;
class QAction;

namespace tlp {
class GlGrid;
class GlCompositeHierarchyManager;
class GlGraphRenderingParameters;
class PropertyInterface;
class StringProperty;

class TLP_QT_SCOPE NodeLinkDiagramComponent : public tlp::GlMainView {
  Q_OBJECT

  GlGrid *_grid;
  QDialog *_gridOptions;
  GlCompositeHierarchyManager *manager;
  bool _hasHulls;

  void registerTriggers();
  void updateGrid();

  Ui::GridOptionsWidget *grid_ui;

public:
  static const std::string viewName;
  PLUGININFORMATION(NodeLinkDiagramComponent::viewName, "Tulip Team", "16/04/2008",
                    "The Node Link Diagram view is the standard representation of relational data, "
                    "where entities are represented as nodes, and their relation as edges.<br>"
                    "This view allows you to change the glyph used to represent nodes (e.g. "
                    "square, round, cross, ...), as well as the shape of the arrows indicating the "
                    "direction of the relationship.",
                    "1.0", "relational")

  NodeLinkDiagramComponent(const tlp::PluginContext *context = nullptr);
  ~NodeLinkDiagramComponent() override;
  std::string icon() const override {
    return ":/tulip/gui/icons/32/node_link_diagram_view.png";
  }
  void setState(const tlp::DataSet &) override;
  tlp::DataSet state() const override;
  // default initialization of scene rendering parameters
  // can be used by other view
  static void initRenderingParameters(tlp::GlGraphRenderingParameters *);

public slots:
  void draw() override;
  void requestChangeGraph(Graph *graph);
  const Camera &goInsideItem(node meta);

protected slots:

  void deleteItem();
  void editColor();
  void editLabel();
  void editShape();
  void editSize();
  void goInsideItem();
  void ungroupItem();
  void setZOrdering(bool);
  void showGridControl();
  void fillContextMenu(QMenu *menu, const QPointF &point) override;

  void addRemoveItemToSelection(bool pushGraph = true, bool toggleSelection = true,
                                bool selectValue = false, bool resetSelection = false);
  void addRemoveInNodesToSelection(bool pushGraph = true, bool toggleSelection = true,
                                   bool selectValue = false, bool resetSelection = false);
  void addRemoveOutNodesToSelection(bool pushGraph = true, bool toggleSelection = true,
                                    bool selectValue = false, bool resetSelection = false);
  void addRemoveInEdgesToSelection(bool pushGraph = true, bool toggleSelection = true,
                                   bool selectValue = false, bool resetSelection = false);
  void addRemoveOutEdgesToSelection(bool pushGraph = true, bool toggleSelection = true,
                                    bool selectValue = false, bool resetSelection = false);
  void addRemoveNodeAndAllNeighbourNodesAndEdges(bool toggleSelection = true,
                                                 bool selectValue = false,
                                                 bool resetSelection = false);
  void addRemoveExtremitiesToSelection(bool pushGraph = true, bool toggleSelection = true,
                                       bool selectValue = false, bool resetSelection = false);
  void addRemoveEdgeAndExtremitiesToSelection(bool toggleSelection = true, bool selectValue = false,
                                              bool resetSelection = false);

  void selectItem();
  void selectInNodes(bool pushGraph = true);
  void selectOutNodes(bool pushGraph = true);
  void selectInEdges(bool pushGraph = true);
  void selectOutEdges(bool pushGraph = true);
  void selectNodeAndAllNeighbourNodesAndEdges();
  void selectExtremities(bool pushGraph = true);
  void selectEdgeAndExtremities();

  void addItemToSelection();
  void addInNodesToSelection(bool pushGraph = true);
  void addOutNodesToSelection(bool pushGraph = true);
  void addInEdgesToSelection(bool pushGraph = true);
  void addOutEdgesToSelection(bool pushGraph = true);
  void addNodeAndAllNeighbourNodesAndEdgesToSelection();
  void addExtremitiesToSelection(bool pushGraph = true);
  void addEdgeAndExtremitiesToSelection();

  void removeItemFromSelection();
  void removeInNodesFromSelection(bool pushGraph = true);
  void removeOutNodesFromSelection(bool pushGraph = true);
  void removeInEdgesFromSelection(bool pushGraph = true);
  void removeOutEdgesFromSelection(bool pushGraph = true);
  void removeNodeAndAllNeighbourNodesAndEdgesFromSelection();
  void removeExtremitiesFromSelection(bool pushGraph = true);
  void removeEdgeAndExtremitiesFromSelection();

protected:
  bool isNode;
  unsigned int itemId;

  void graphChanged(tlp::Graph *) override;

  void createScene(Graph *graph, DataSet dataSet);
  DataSet sceneData() const;
  void loadGraphOnScene(Graph *graph);
  void useHulls(bool hasHulls);
  bool hasHulls() const;
  void editValue(PropertyInterface *pi);
};
} // namespace tlp

#endif /* NODELINKDIAGRAMCOMPONENT_H_ */

///@endcond
