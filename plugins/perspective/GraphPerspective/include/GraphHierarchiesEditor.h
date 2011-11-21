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
#ifndef GRAPHHIERARCHIESEDITOR_H
#define GRAPHHIERARCHIESEDITOR_H

#include <QtGui/QWidget>
#include <QtCore/QModelIndex>

class GraphHierarchiesModel;

namespace tlp {
class Graph;
}

namespace Ui {
class GraphHierarchiesEditorData;
}

class GraphHierarchiesEditor : public QWidget {
  Q_OBJECT
  Ui::GraphHierarchiesEditorData *_ui;
  tlp::Graph* _contextGraph;
  QModelIndex _contextIndex;

public:
  explicit GraphHierarchiesEditor(QWidget *parent = 0);
  virtual ~GraphHierarchiesEditor();
  void setModel(GraphHierarchiesModel *model);

protected slots:
  void activeGraphSelected(const QModelIndex&);
  void contextMenuRequested(const QPoint&);

  void addSubGraph();
  void cloneSubGraph();
  void delGraph();
  void delAllGraph();
  void createPanel();
};

#endif // GRAPHHIERARCHIESEDITOR_H
