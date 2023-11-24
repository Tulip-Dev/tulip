/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#ifndef GRAPHHIERARCHIESEDITOR_H
#define GRAPHHIERARCHIESEDITOR_H

#include <tulip/TreeViewComboBox.h>

#include <QWidget>
#include <QModelIndex>
#include <tulip/Observable.h>

class QAbstractButton;

namespace tlp {
class GraphHierarchiesModel;
class Graph;
class BooleanProperty;
} // namespace tlp

namespace Ui {
class GraphHierarchiesEditorData;
}

class CustomTreeView : public QTreeView {

  Q_OBJECT

public:
  CustomTreeView(QWidget *parent = nullptr);

  void setModel(QAbstractItemModel *model) override;
  void setAllHierarchyVisible(const QModelIndex &index, bool visible);

public slots:

  void resizeFirstColumnToContent();

protected:
  int sizeHintForColumn(int col) const override;
  void scrollContentsBy(int dx, int dy) override;
};

class GraphHierarchiesEditor : public QWidget, tlp::Observable {
  Q_OBJECT
  Ui::GraphHierarchiesEditorData *_ui;
  tlp::Graph *_contextGraph;
  QModelIndex _contextIndex;
  QAbstractButton *_linkButton;
  tlp::GraphHierarchiesModel *_model;
  tlp::BooleanProperty *_currentSelection;

public:
  explicit GraphHierarchiesEditor(QWidget *parent = nullptr);
  ~GraphHierarchiesEditor() override;
  tlp::Graph *currentGraph();
  void setModel(tlp::GraphHierarchiesModel *model);
  bool synchronized() const;
  void treatEvents(const std::vector<tlp::Event> &) override;
  void updateSelectionInfos();

signals:
  void changeSynchronization(bool);

public slots:
  void setSynchronizeButtonVisible(bool);

protected slots:
  void contextMenuRequested(const QPoint &);
  void doubleClicked(const QModelIndex &);
  void clicked(const QModelIndex &);
  void currentChanged(const QModelIndex &, const QModelIndex &);
  void currentGraphChanged(tlp::Graph *);

  void addSubGraph();
  void cloneSubGraph();
  void cloneSibling();
  void cloneSiblingWithProperties();
  void addInducedSubGraph();
  void delGraph();
  void delAllGraph();
  void delAllNodes();
  void delAllEdges();
  void delSelection(bool fromRoot = false);
  void delSelectionFromRoot();
  void collapseGraphHierarchy();
  void expandGraphHierarchy();
  void renameGraph();
  void saveGraphHierarchyInTlpFile();
  void toggleSynchronization(bool);
};

#endif // GRAPHHIERARCHIESEDITOR_H
