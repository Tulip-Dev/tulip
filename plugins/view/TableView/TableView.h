/**
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

  Ui::TableViewWidget *_ui;
  PropertiesEditor *propertiesEditor;
  tlp::GraphModel *_model;
  bool isNewGraph;
  bool filteringColumns;
  tlp::Graph *previousGraph;
  int minFontSize;

public:
  PLUGININFORMATION("Spreadsheet view", "Tulip Team", "04/17/2012", "Spreadsheet view for raw data",
                    "4.0", "")

  TableView(tlp::PluginContext *);
  ~TableView() override;
  std::string icon() const override {
    return ":/spreadsheet_view.png";
  }
  tlp::DataSet state() const override;
  void setState(const tlp::DataSet &) override;
  void setupWidget() override;
  QList<QWidget *> configurationWidgets() const override;
  bool getNodeOrEdgeAtViewportPos(int x, int y, tlp::node &n, tlp::edge &e) const override;

  // Qt bug workaround
  // see void WorkspacePanel::showEvent(QShowEvent *event);
  bool rebuildSceneOnShowEvent() override {
    return true;
  }

public slots:
  void readSettings();
  void setPropertyVisible(tlp::PropertyInterface *, bool);
  void filterChanged();
  tlp::BooleanProperty *getFilteringProperty() const;
  bool hasEffectiveFiltering();

protected:
  void graphChanged(tlp::Graph *) override;
  void graphDeleted(tlp::Graph *) override;
  bool eventFilter(QObject *obj, QEvent *event) override;

protected slots:
  void delHighlightedRows();
  void toggleHighlightedRows();
  void selectHighlightedRows();
  bool setAllHighlightedRows(tlp::PropertyInterface *);
  bool setCurrentValue(tlp::PropertyInterface *, unsigned int);
  void setLabelsOfHighlightedRows(tlp::PropertyInterface *);
  void clearValueMatchFilter();
  void clearColumnMatchFilter();
  void setMatchProperty();
  void setColumnsFilter();
  void setColumnsFilterCase();
  void setPropertiesFilter(const QString &);
  void mapToGraphSelection();

  void columnsInserted(const QModelIndex &, int, int);
  void showCustomContextMenu(const QPoint &pos);
  void showHorizontalHeaderCustomContextMenu(const QPoint &pos);
  void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
  void setZoomLevel(int);
  void showHideTableSettings();
};

#endif // TABLEVIEW_H
