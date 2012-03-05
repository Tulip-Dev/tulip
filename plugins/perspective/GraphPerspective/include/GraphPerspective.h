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
#ifndef GRAPHPERSPECTIVE_H
#define GRAPHPERSPECTIVE_H

#include <QtCore/QModelIndex>
#include <tulip/Perspective.h>
#include <tulip/MethodFactory.h>

class QAction;

class GraphHierarchiesEditor;

namespace tlp {
class GraphHierarchiesModel;
class View;
}

namespace Ui {
class GraphPerspectiveMainWindowData;
}

class GraphPerspective : public tlp::Perspective {
  Q_OBJECT
  Ui::GraphPerspectiveMainWindowData *_ui;
  tlp::GraphHierarchiesModel *_graphs;
  bool _maximised;
public:
  PLUGININFORMATIONSWITHGROUP("Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0", "Hierarchy")
  GraphPerspective(const tlp::PluginContext* c);
  virtual void construct(tlp::PluginProgress *);

  tlp::GraphHierarchiesModel* model() const;

public slots:
  void showFullScreen(bool);
  void importGraph();
  void createPanel(tlp::Graph* g = NULL);

  void save();
  void saveAs(const QString& path=QString::null);
  void open(const QString& path=QString::null);

  void centerPanels(tlp::PropertyInterface*);

protected slots:
  void refreshDockExpandControls();
  void panelFocused(tlp::View*);
  void modeSwitch();
};

#endif // GRAPHPERSPECTIVE_H
