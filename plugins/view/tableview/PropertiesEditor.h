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

#ifndef PROPERTIESEDITOR_H
#define PROPERTIESEDITOR_H

#include <QtGui/QWidget>
#include <QtCore/QModelIndex>

#include <tulip/GraphPropertiesModel.h>
#include <tulip/PropertyInterface.h>

namespace Ui {
class PropertiesEditor;
}

namespace tlp {
class Graph;
class TulipItemDelegate;
}

class PropertiesEditor : public QWidget {
  Q_OBJECT

  Ui::PropertiesEditor* _ui;
  tlp::PropertyInterface* _contextProperty;
  QList<tlp::PropertyInterface*> _contextPropertyList;
  tlp::Graph* _graph;
  tlp::TulipItemDelegate* _delegate;
  tlp::GraphPropertiesModel<tlp::PropertyInterface>* _sourceModel;

public:
  explicit PropertiesEditor(QWidget *parent = NULL);
  virtual ~PropertiesEditor();

  void setGraph(tlp::Graph* g);
  QSet<tlp::PropertyInterface*> visibleProperties() const;

  void setPropertyChecked(int index, bool state);

  tlp::PropertyInterface* contextProperty() const;
signals:
  void propertyVisibilityChanged(tlp::PropertyInterface*,bool);
  void setFilteredNodes();
  void setFilteredEdges();
  void mapToGraphSelection();

protected slots:
  void checkStateChanged(QModelIndex,Qt::CheckState);
  void showCustomContextMenu(const QPoint&);
  void copyProperty();
  void newProperty();
  void delProperty();
  void toLabels();
  void toNodesLabels();
  void toEdgesLabels();
  void toLabels(bool nodes, bool edges);
  void checkAll();
  void unCheckAll();
  void unCheckAllExcept();
  void showSystemProperties(bool);
  void setAllNodes();
  void setAllEdges();
  void displayedPropertiesInserted(const QModelIndex &parent, int start,
                                   int end);
  void displayedPropertiesRemoved(const QModelIndex &parent, int start,
                                  int end);
};

#endif // PROPERTIESEDITOR_H
