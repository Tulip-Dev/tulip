#include "TableView.h"

#include "ui_TableView.h"
#include "PropertiesEditor.h"
#include "TableViewConfiguration.h"

#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>


using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), _tableViewConfiguration(NULL),
  _nodesModel(new NodesGraphModel(this)), _edgesModel(new EdgesGraphModel(this)) {
  connect(_nodesModel,SIGNAL(columnsInserted(QModelIndex,int,int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
  connect(_edgesModel,SIGNAL(columnsInserted(QModelIndex,int,int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
}

TableView::~TableView() {
  delete _ui;
}

tlp::DataSet TableView::state() const {
  DataSet data;
  data.set("show_nodes",_tableViewConfiguration->isShowNodes());
  data.set("show_edges",_tableViewConfiguration->isShowEdges());
  PropertyInterface* pi = _tableViewConfiguration->filteringProperty();

  if (pi != NULL)
    data.set("filtering_property",pi->getName());

  return data;
}

void TableView::setState(const tlp::DataSet& data) {
  bool showNodes = true, showEdges = false;
  std::string filterPropertyName = "";
  data.get<bool>("show_nodes",showNodes);
  data.get<bool>("show_edges",showEdges);

  if (data.exist("filtering_property"))
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
  connect(_ui->propertiesEditor,SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface*,bool)),this,SLOT(setPropertyVisible(tlp::PropertyInterface*,bool)));

  _tableViewConfiguration = new TableViewConfiguration;
  connect(_tableViewConfiguration,SIGNAL(settingsChanged()),this,SLOT(readSettings()));

  QSortFilterProxyModel* nodesModel = new GraphSortFilterProxyModel(_ui->nodesTable);
  nodesModel->setSourceModel(_nodesModel);
  nodesModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  nodesModel->setFilterKeyColumn(-1);

  QSortFilterProxyModel* edgesModel = new GraphSortFilterProxyModel(_ui->edgesTable);
  edgesModel->setSourceModel(_edgesModel);
  edgesModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  edgesModel->setFilterKeyColumn(-1);

  _ui->nodesTable->setModel(nodesModel);
  _ui->edgesTable->setModel(edgesModel);
  _ui->nodesTable->setItemDelegate(new GraphTableItemDelegate);
  _ui->edgesTable->setItemDelegate(new GraphTableItemDelegate);
  _ui->nodesTable->horizontalHeader()->setMovable(true);
  _ui->edgesTable->horizontalHeader()->setMovable(true);

  connect(_ui->nodesTable, SIGNAL(destroyed()), _ui->nodesTable->itemDelegate(), SLOT(deleteLater()));
  connect(_ui->edgesTable, SIGNAL(destroyed()), _ui->edgesTable->itemDelegate(), SLOT(deleteLater()));

  _ui->splitter_2->setSizes(QList<int>() << centralWidget->width()/4 << centralWidget->width()*3/4);
}

void TableView::graphChanged(tlp::Graph* g) {
  _nodesModel->setGraph(g);
  _edgesModel->setGraph(g);
  _ui->nodesTable->resizeColumnsToContents();
  _ui->edgesTable->resizeColumnsToContents();
  _tableViewConfiguration->setGraph(g);
  _ui->propertiesEditor->setGraph(g);

  // Hide all data in the table view
  foreach(QTableView* view, QList<QTableView*>() << _ui->nodesTable << _ui->edgesTable) {
    QAbstractItemModel* model = view->model();

    for(int i=0; i < model->columnCount(); ++i) {
      view->horizontalHeader()->setSectionHidden(i,true);
    }
  }
  _ui->frame->hide();
}

void TableView::graphDeleted() {
  graphChanged(NULL);
}

void TableView::columnsInserted(const QModelIndex&, int start, int end) {
  QAbstractItemModel* model = static_cast<QAbstractItemModel*>(sender());

  for (int c = start; c <= end; c++) {
    PropertyInterface* pi = model->headerData(c,Qt::Horizontal,TulipModel::PropertyRole).value<PropertyInterface*>();
    setPropertyVisible(pi,false);
  }
}

QList<QWidget*> TableView::configurationWidgets() const {
  return QList<QWidget*>() << _tableViewConfiguration;
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

  QTableView* view = _ui->nodesTable->isVisible() ? _ui->nodesTable : _ui->edgesTable;
  QAbstractItemModel* model = view->model();
  bool visible = false;

  for(int i=0; i < model->columnCount(); ++i) {
    if (!view->isColumnHidden(i)) {
      visible = true;
      break;
    }
  }

  _ui->frame->setVisible(visible);
}

PLUGIN(TableView)
