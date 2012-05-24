#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <tulip/ViewWidget.h>
#include <QtCore/QModelIndex>

namespace Ui {
class TableViewWidget;
}

namespace tlp {
class NodesGraphModel;
class EdgesGraphModel;
}

class TableViewConfiguration;
class PropertiesEditor;

class TableView : public tlp::ViewWidget {
  Q_OBJECT

  Ui::TableViewWidget* _ui;
  TableViewConfiguration* _tableViewConfiguration;

  tlp::NodesGraphModel* _nodesModel;
  tlp::EdgesGraphModel* _edgesModel;

public:
  PLUGININFORMATIONS("Spreadsheet","Tulip Team","04/17/2012","Spreadsheet view for raw data","4.0","")

  TableView(tlp::PluginContext*);
  virtual ~TableView();

  tlp::DataSet state() const;
  void setState(const tlp::DataSet &);
  void setupWidget();

  QList<QWidget*> configurationWidgets() const;

public slots:
  void readSettings();
  void setPropertyVisible(tlp::PropertyInterface*,bool);

protected:
  void graphChanged(tlp::Graph *);
  void graphDeleted();

protected slots:
  void columnsInserted(const QModelIndex&,int,int);
};

#endif // TABLEVIEW_H
