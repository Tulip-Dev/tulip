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

#ifndef PROPERTIESEDITOR_H
#define PROPERTIESEDITOR_H

#include <QWidget>
#include <QModelIndex>
#include <QLineEdit>

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
  bool filteringProperties;
  QWidget* editorParent;

public:
  explicit PropertiesEditor(QWidget *parent = NULL);
  virtual ~PropertiesEditor();

  void setGraph(tlp::Graph* g);
  tlp::Graph* getGraph() {
    return _graph;
  }
  QSet<tlp::PropertyInterface*> visibleProperties() const;

  void setPropertyChecked(int index, bool state);
  void setPropertyChecked(const QString& pName, bool state);

  QLineEdit* getPropertiesFilterEdit();
  void toLabels(tlp::PropertyInterface* prop,
                bool nodes, bool edges, bool selectedOnly = false);
  bool setAllValues(tlp::PropertyInterface* prop, bool nodes, bool selectedOnly);
  bool renameProperty(tlp::PropertyInterface* prop);

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
  void delProperties();
  void toLabels();
  void toNodesLabels();
  void toEdgesLabels();
  void toSelectedLabels();
  void toSelectedNodesLabels();
  void toSelectedEdgesLabels();
  void setPropsVisibility(int);
  void setPropsNotVisibleExcept();
  void showVisualProperties(bool);
  void displayedPropertiesInserted(const QModelIndex &parent, int start,
                                   int end);
  void displayedPropertiesRemoved(const QModelIndex &parent, int start,
                                  int end);
  void setPropertiesFilter(QString filter);
};

#endif // PROPERTIESEDITOR_H
