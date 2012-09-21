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

#include "TableView.h"

#include "ui_TableView.h"
#include "PropertiesEditor.h"

#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>
#include <QtGui/QMenu>

using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), _model(NULL) {
}

TableView::~TableView() {
  delete _ui;
}

tlp::DataSet TableView::state() const {
  DataSet data;
  data.set("show_nodes",_ui->propertiesEditor->isShowNodes());
  data.set("show_edges",_ui->propertiesEditor->isShowNodes());

  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  PropertyInterface* pi = model->data(model->index(_ui->filteringPropertyCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();

  if (pi != NULL)
    data.set("filtering_property",pi->getName());

  return data;
}

void TableView::setState(const tlp::DataSet& data) {
  bool showNodes = true, showEdges = false;
  std::string filterPropertyName = "";
  data.get<bool>("show_nodes",showNodes);
  data.get<bool>("show_edges",showEdges);

  _ui->propertiesEditor->showNodes(showNodes);

  if (data.exist("filtering_property"))
    data.get<std::string>("filtering_property",filterPropertyName);

  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  int r = 0;

  if (!filterPropertyName.empty())
    r = model->rowOf(model->graph()->getProperty<BooleanProperty>(filterPropertyName));

  if (r < 0)
    r=0;

  _ui->filteringPropertyCombo->setCurrentIndex(r);

  readSettings();
}

void TableView::setupWidget() {
  QWidget* centralWidget = new QWidget();
  _ui->setupUi(centralWidget);

  setCentralWidget(centralWidget);
  connect(_ui->propertiesEditor,SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface*,bool)),this,SLOT(setPropertyVisible(tlp::PropertyInterface*,bool)));

  _ui->table->setItemDelegate(new GraphTableItemDelegate(_ui->table));
  _ui->table->horizontalHeader()->setMovable(true);
  connect(_ui->filterEdit,SIGNAL(returnPressed()),this,SLOT(filterChanged()));
  _ui->splitter_2->setSizes(QList<int>() << centralWidget->width()/4 << centralWidget->width()*3/4);

  connect(_ui->propertiesEditor, SIGNAL(showElementTypeChanged()), this, SLOT(readSettings()));
  connect(_ui->filteringPropertyCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings()));
  connect(_ui->table,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showCustomContextMenu(QPoint)));
}

void TableView::showCustomContextMenu(const QPoint& p) {
  if (_ui->table->indexAt(p).isValid())
    _contextProperty = _ui->table->indexAt(p).data(TulipModel::PropertyRole).value<tlp::PropertyInterface*>();
  else
    _contextProperty = NULL;
  if (_contextProperty == NULL)
    return;

  QMenu menu;
  QFont f;
  f.setBold(true);

  QAction* title1 = menu.addAction(trUtf8("Selection"));
  title1->setFont(f);
  menu.addSeparator();
  menu.addAction(trUtf8("Map to graph selection"),this,SLOT(mapToGraphSelection()));

  menu.addSeparator();
  QAction* title2 = menu.addAction(_contextProperty->getName().c_str());
  title2->setFont(f);
  menu.addSeparator();
  if (_ui->propertiesEditor->isShowNodes()) {
    menu.addAction(trUtf8("Set displayed nodes value"),this,SLOT(setFilteredNodesValue()));
  }
  else {
    menu.addAction(trUtf8("Set displayed edges value"),this,SLOT(setFilteredEdgesValue()));
  }

  menu.exec(QCursor::pos());
}

void TableView::setFilteredNodesValue() {
  tlp::BooleanProperty* filterProp = static_cast<GraphSortFilterProxyModel*>(_ui->table->model())->filterProperty();
  QVariant val = TulipItemDelegate::showEditorDialog(NODE,_contextProperty,graph(),static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  Observable::holdObservers();
  graph()->push();
  if (filterProp != NULL) {
    node n;
    forEach(n,filterProp->getNodesEqualTo(true))
      GraphModel::setNodeValue(n.id,_contextProperty,val);
  }
  else {
    GraphModel::setAllNodeValue(_contextProperty,val);
  }
  Observable::unholdObservers();
}

void TableView::setFilteredEdgesValue() {
  tlp::BooleanProperty* filterProp = static_cast<GraphSortFilterProxyModel*>(_ui->table->model())->filterProperty();
  QVariant val = TulipItemDelegate::showEditorDialog(NODE,_contextProperty,graph(),static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  Observable::holdObservers();
  graph()->push();
  if (filterProp != NULL) {
    edge e;
    forEach(e,filterProp->getEdgesEqualTo(true))
      GraphModel::setEdgeValue(e.id,_contextProperty,val);
  }
  else {
    GraphModel::setAllEdgeValue(_contextProperty,val);
  }
  Observable::unholdObservers();
}

void TableView::mapToGraphSelection() {
  BooleanProperty* out = graph()->getProperty<BooleanProperty>("viewSelection");
  if (_ui->propertiesEditor->isShowNodes()) {
    out->setAllNodeValue(false);
    QItemSelectionModel *selectionModel = _ui->table->selectionModel();
    foreach(QModelIndex idx,selectionModel->selectedRows()) {
      node n(idx.data(TulipModel::ElementIdRole).toUInt());
      out->setNodeValue(n,true);
    }
  }
  else {
    out->setAllEdgeValue(false);
    QItemSelectionModel *selectionModel = _ui->table->selectionModel();
    foreach(QModelIndex idx,selectionModel->selectedRows()) {
      edge e(idx.data(TulipModel::ElementIdRole).toUInt());
      out->setEdgeValue(e,true);
    }
  }
}

void TableView::graphChanged(tlp::Graph* g) {
  GraphPropertiesModel<BooleanProperty>* model = new GraphPropertiesModel<BooleanProperty>(trUtf8("No filtering"),g,false,_ui->filteringPropertyCombo);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(0);

  _ui->propertiesEditor->setGraph(g);

  _ui->frame->hide();

  //Show all the properties whose name starts with 'view'
  if (_model != NULL) {
    for(int i=0; i < _model->columnCount(); ++i) {
      if(_model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().startsWith("view")) {
        _ui->propertiesEditor->setPropertyChecked(i, true);
      }
    }
  }
}

void TableView::graphDeleted() {
  setGraph(NULL);
}

void TableView::readSettings() {
  if ( (_ui->propertiesEditor->isShowNodes() && dynamic_cast<NodesGraphModel*>(_model) == NULL) ||
       (_ui->propertiesEditor->isShowEdges() && dynamic_cast<EdgesGraphModel*>(_model) == NULL)) {
    _ui->table->setModel(NULL);

    if (_ui->propertiesEditor->isShowNodes())
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

  GraphSortFilterProxyModel* sortModel = static_cast<GraphSortFilterProxyModel*>(_ui->table->model());

  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  PropertyInterface* pi = model->data(model->index(_ui->filteringPropertyCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();
  sortModel->setFilterProperty(dynamic_cast<BooleanProperty*>(pi));

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
