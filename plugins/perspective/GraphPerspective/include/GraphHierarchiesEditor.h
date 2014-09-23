/**
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
#ifndef GRAPHHIERARCHIESEDITOR_H
#define GRAPHHIERARCHIESEDITOR_H

#include <tulip/TreeViewComboBox.h>

#include <QWidget>
#include <QModelIndex>

class QAbstractButton;

namespace tlp {
class GraphHierarchiesModel;
class Graph;
}

namespace Ui {
class GraphHierarchiesEditorData;
}

class CustomTreeView : public DeferredUpdateTreeView {

  Q_OBJECT

public :

  CustomTreeView(QWidget *parent=0);

  void setModel(QAbstractItemModel * model);

public slots:

  void resizeFirstColumnToContent();

protected:

  int sizeHintForColumn(int col) const;
  void scrollContentsBy(int dx, int dy);

};

class GraphHierarchiesEditor : public QWidget {
  Q_OBJECT
  Ui::GraphHierarchiesEditorData *_ui;
  tlp::Graph* _contextGraph;
  QModelIndex _contextIndex;
  QAbstractButton* _linkButton;
  tlp::GraphHierarchiesModel* _model;

public:
  explicit GraphHierarchiesEditor(QWidget *parent = NULL);
  virtual ~GraphHierarchiesEditor();
  void setModel(tlp::GraphHierarchiesModel *model);
  bool synchronized() const;

signals:
  void changeSynchronization(bool);

public slots:
  void setSynchronizeButtonVisible(bool);
  void setAddPanelButtonVisible(bool);

protected slots:
  void contextMenuRequested(const QPoint&);
  void doubleClicked(const QModelIndex&);
  void clicked(const QModelIndex&);
  void currentChanged(const QModelIndex&, const QModelIndex&);

  void addSubGraph();
  void cloneSubGraph();
  void addInducedSubGraph();
  void delGraph();
  void delAllGraph();
  void createPanel();
  void exportGraph();
  void renameGraph();
  void saveGraphHierarchyInTlpFile();
  void toggleSynchronization(bool);
};

#endif // GRAPHHIERARCHIESEDITOR_H
