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

class QAbstractButton;

namespace tlp {
class GraphHierarchiesModel;
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
  QAbstractButton* _linkButton;

public:
  explicit GraphHierarchiesEditor(QWidget *parent = NULL);
  virtual ~GraphHierarchiesEditor();
  void setModel(tlp::GraphHierarchiesModel *model);
  bool synchronized() const;
  void repackHeaders();

public slots:
  void setSynchronized(bool);
  void setSynchronizeButtonVisible(bool);
  void setAddPanelButtonVisible(bool);

protected slots:
  void activeGraphSelected(const QModelIndex&);
  void contextMenuRequested(const QPoint&);

  void addSubGraph();
  void cloneSubGraph();
  void delGraph();
  void delAllGraph();
  void createPanel();
  void exportGraph();
};

#endif // GRAPHHIERARCHIESEDITOR_H
