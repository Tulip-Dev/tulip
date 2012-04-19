#include "TableView.h"

#include "ui_TableView.h"
#include "PropertiesEditor.h"
#include "TableViewConfiguration.h"

#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>


using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), _tableViewConfiguration(NULL), _propertiesEditor(NULL),
  _nodesModel(new NodesGraphModel), _edgesModel(new EdgesGraphModel) {
}

TableView::~TableView() {
  delete _ui;
}

tlp::DataSet TableView::state() const {
  DataSet data;
  data.set("show_nodes",_tableViewConfiguration->isShowNodes());
  data.set("show_edges",_tableViewConfiguration->isShowEdges());
  data.set("filtering_property",_tableViewConfiguration->filteringProperty()->getName());
  return data;
}

void TableView::setState(const tlp::DataSet& data) {
  bool showNodes = true, showEdges = false;
  std::string filterPropertyName = "";
  data.get<bool>("show_nodes",showNodes);
  data.get<bool>("show_edges",showEdges);
  data.get<std::string>("filtering_property",filterPropertyName);
  _tableViewConfiguration->setShowEdges(showEdges);
  _tableViewConfiguration->setShowNodes(showNodes);
  _tableViewConfiguration->setFilteringProperty(filterPropertyName.c_str());
  readSettings();
}

void TableView::setupWidget() {
  QWidget* centralWidget = new QWidget();
  _ui->setupUi(centralWidget);
  setCentralWidget(centralWidget);

  _tableViewConfiguration = new TableViewConfiguration;
  _propertiesEditor = new PropertiesEditor;
  connect(_tableViewConfiguration,SIGNAL(settingsChanged()),this,SLOT(readSettings()));

  QSortFilterProxyModel* nodesModel = new GraphSortFilterProxyModel;
  nodesModel->setSourceModel(_nodesModel);
  nodesModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  nodesModel->setFilterKeyColumn(-1);
  QSortFilterProxyModel* edgesModel = new GraphSortFilterProxyModel;
  edgesModel->setSourceModel(_edgesModel);
  edgesModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  edgesModel->setFilterKeyColumn(-1);
  _ui->nodesTable->setModel(nodesModel);
  _ui->edgesTable->setModel(edgesModel);
  _ui->nodesTable->setItemDelegate(new GraphTableItemDelegate);
  _ui->edgesTable->setItemDelegate(new GraphTableItemDelegate);
  connect(_ui->nodesTable, SIGNAL(destroyed()), _ui->nodesTable->itemDelegate(), SLOT(deleteLater()));
  connect(_ui->edgesTable, SIGNAL(destroyed()), _ui->edgesTable->itemDelegate(), SLOT(deleteLater()));

  connect(_propertiesEditor,SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface*,bool)),this,SLOT(setPropertyVisible(tlp::PropertyInterface*,bool)));
}

void TableView::graphChanged(tlp::Graph* g) {
  _nodesModel->setGraph(g);
  _edgesModel->setGraph(g);
  _tableViewConfiguration->setGraph(g);
  _propertiesEditor->setGraph(g);
}

void TableView::graphDeleted() {
  graphChanged(NULL);
}

QList<QWidget*> TableView::configurationWidgets() const {
  return QList<QWidget*>() << _tableViewConfiguration << _propertiesEditor;
}

void TableView::readSettings() {
  _ui->nodesTable->setVisible(_tableViewConfiguration->isShowNodes());
  _ui->edgesTable->setVisible(_tableViewConfiguration->isShowEdges());
  static_cast<GraphSortFilterProxyModel*>(_ui->nodesTable->model())->setFilterProperty(_tableViewConfiguration->filteringProperty());
  static_cast<GraphSortFilterProxyModel*>(_ui->edgesTable->model())->setFilterProperty(_tableViewConfiguration->filteringProperty());
}

void TableView::setPropertyVisible(PropertyInterface* pi, bool v) {
  QString propName = pi->getName().c_str();
  foreach(QTableView* view, QList<QTableView*>() << _ui->nodesTable << _ui->edgesTable) {
    QAbstractItemModel* model = view->model();

    for(int i=0; i < model->columnCount(); ++i) {
      if (model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString() == propName) {
        view->horizontalHeader()->setSectionHidden(i,!v);
        break;
      }
    }
  }
}

PLUGIN(TableView)
