#include "TableView.h"

#include "ui_TableView.h"
#include "PropertiesEditor.h"
#include "TableViewConfiguration.h"

#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>


using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), _tableViewConfiguration(NULL), _model(NULL) {
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
  _ui->table->setItemDelegate(new GraphTableItemDelegate(_ui->table));
  _ui->table->horizontalHeader()->setMovable(true);
  connect(_ui->filterEdit,SIGNAL(returnPressed()),this,SLOT(filterChanged()));
  _ui->splitter_2->setSizes(QList<int>() << centralWidget->width()/4 << centralWidget->width()*3/4);
}

void TableView::graphChanged(tlp::Graph* g) {
  _tableViewConfiguration->setGraph(g);
  _ui->propertiesEditor->setGraph(g);

  _ui->frame->hide();

  // Hide all data in the table view
  if (_model != NULL) {
    for(int i=0; i < _model->columnCount(); ++i) {
      _ui->table->horizontalHeader()->setSectionHidden(i,true);
      _ui->table->horizontalHeader()->resizeSection(i,_ui->table->horizontalHeader()->defaultSectionSize());
    }
  }
}

void TableView::graphDeleted() {
  setGraph(NULL);
}

QList<QWidget*> TableView::configurationWidgets() const {
  return QList<QWidget*>() << _tableViewConfiguration;
}

void TableView::readSettings() {
  if ( (_tableViewConfiguration->isShowNodes() && dynamic_cast<NodesGraphModel*>(_model) == NULL) ||
       (_tableViewConfiguration->isShowEdges() && dynamic_cast<EdgesGraphModel*>(_model) == NULL)) {
    _ui->table->setModel(NULL);

    if (_tableViewConfiguration->isShowNodes())
      _model = new NodesGraphModel(_ui->table);
    else
      _model = new EdgesGraphModel(_ui->table);

    _model->setGraph(graph());
    GraphSortFilterProxyModel* sortModel = new GraphSortFilterProxyModel(_ui->table);
    sortModel->setSourceModel(_model);
    _ui->table->setModel(sortModel);
    connect(_model,SIGNAL(columnsInserted(QModelIndex,int,int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
    filterChanged();
  }

  QSet<tlp::PropertyInterface*> visibleProperties = _ui->propertiesEditor->visibleProperties();

  for (int i=0; i < _model->columnCount(); ++i) {
    _ui->table->setColumnHidden(i,!visibleProperties.contains(_model->headerData(i, Qt::Horizontal, TulipModel::PropertyRole).value<tlp::PropertyInterface*>()));
  }
}

void TableView::columnsInserted(const QModelIndex&, int start, int end) {
  QAbstractItemModel* model = static_cast<QAbstractItemModel*>(sender());

  for (int c = start; c <= end; c++) {
    PropertyInterface* pi = model->headerData(c,Qt::Horizontal,TulipModel::PropertyRole).value<PropertyInterface*>();
    setPropertyVisible(pi,false);
  }
}

void TableView::setPropertyVisible(PropertyInterface* pi, bool v) {
  if (_model == NULL)
    return;

  QString propName = pi->getName().c_str();

  for(int i=0; i < _model->columnCount(); ++i) {
    if (_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString() == propName) {
      _ui->table->horizontalHeader()->setSectionHidden(i,!v);
      break;
    }
  }

  // Hide table if no more column is displayed
  bool visible = false;

  for(int i=0; i < _model->columnCount(); ++i) {
    if (!_ui->table->isColumnHidden(i)) {
      visible = true;
      break;
    }
  }

  _ui->frame->setVisible(visible);
}

void TableView::filterChanged() {
  QString filter = _ui->filterEdit->text();
  GraphSortFilterProxyModel* sortModel = static_cast<GraphSortFilterProxyModel*>(_ui->table->model());
  QVector<PropertyInterface*> props;

  for (int i=0; i < _model->columnCount(); ++i) {
    if (!_ui->table->horizontalHeader()->isSectionHidden(i))
      props += _model->headerData(i,Qt::Horizontal,TulipModel::PropertyRole).value<PropertyInterface*>();
  }

  sortModel->setProperties(props);
  sortModel->setFilterFixedString(filter);
}

PLUGIN(TableView)
