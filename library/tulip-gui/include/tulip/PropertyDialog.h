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
#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H
#include <string>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <tulip/Observable.h>

#include "ui_PropertyDialog.h"

class QStringList;
class QScrollBar;

namespace tlp {
class Graph;
class GlMainWidget;
class PropertyInterface;

class TLP_QT_SCOPE PropertyDialog: public QWidget, public Ui::PropertyDialogData {
Q_OBJECT
  ;

public:
  PropertyDialog(QWidget *parent = 0);
  ~PropertyDialog();
  void setGraph(Graph *);

protected:
  void keyReleaseEvent  ( QKeyEvent * event );
private:
  Graph *graph;
  PropertyInterface *editedProperty;
  std::string editedPropertyName;
  bool _filterSelection;
  void updateNbElements();
  /**
   * @brief Display the property with the given name in the node and edge table.
   */
  void displayProperty(const std::string& propertyName);

public slots:
  ///Manages observers change notification.
  void update();
  void selectNode(node);
  void selectEdge(edge);
  void newProperty();
  void removeProperty();
  void cloneProperty();
  void toStringProperty();
  void filterSelection(bool);
  void setAllValue();
  void importCSVData();
private slots:
  void propertySelectionChanged();
  void propertyTypeFilterChanged();
  void nameFilterChanged(const QString& text);
signals:
  void tulipAllNodePropertyChanged(Graph *, const QString &property, const QString &value);
  void tulipAllEdgePropertyChanged(Graph *, const QString &property, const QString &value);
  void newPropertySignal(Graph *, const std::string &);
  void removePropertySignal(Graph *, const std::string &);
};

}
#endif // PROPERTYDIALOG_H
