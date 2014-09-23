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

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <tulip/ViewWidget.h>
#include <tulip/BooleanProperty.h>
#include <QModelIndex>

namespace Ui {
class TableViewWidget;
}

namespace tlp {
class GraphModel;
}

class TableViewConfiguration;
class PropertiesEditor;

class TableView : public tlp::ViewWidget {
  Q_OBJECT

  Ui::TableViewWidget* _ui;
  PropertiesEditor* propertiesEditor;
  tlp::GraphModel* _model;
  bool isNewGraph;
  bool filteringColumns;

public:
  PLUGININFORMATION("Spreadsheet view","Tulip Team","04/17/2012","Spreadsheet view for raw data","4.0","")

  TableView(tlp::PluginContext*);
  virtual ~TableView();
  std::string icon() const {
    return ":/spreadsheet_view.png";
  }
  tlp::DataSet state() const;
  void setState(const tlp::DataSet &);
  void setupWidget();
  QList<QWidget*> configurationWidgets() const;

public slots:
  void readSettings();
  void setPropertyVisible(tlp::PropertyInterface*,bool);
  void filterChanged();
  tlp::BooleanProperty* getFilteringProperty() const;
  bool hasEffectiveFiltering();

protected:
  void graphChanged(tlp::Graph *);
  void graphDeleted(tlp::Graph *);
  bool eventFilter(QObject* obj, QEvent* event);

protected slots:
  void delHighlightedRows();
  void toggleHighlightedRows();
  void selectHighlightedRows();
  bool setAllHighlightedRows(tlp::PropertyInterface*);
  void setLabelsOfHighlightedRows(tlp::PropertyInterface*);
  void setMatchProperty();
  void setColumnsFilter(QString);
  void setPropertiesFilter(QString);
  void mapToGraphSelection();

  void columnsInserted(const QModelIndex&,int,int);
  void showCustomContextMenu(const QPoint & pos);
  void showHorizontalHeaderCustomContextMenu(const QPoint & pos);
  void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
};

#endif // TABLEVIEW_H
