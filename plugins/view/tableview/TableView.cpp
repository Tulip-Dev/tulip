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
  data.set("show_nodes",_ui->eltTypeCombo->currentIndex() == 0);
  data.set("show_edges",_ui->eltTypeCombo->currentIndex() == 1);

  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  PropertyInterface* pi = model->data(model->index(_ui->filteringPropertyCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();

  if (pi != NULL)
    data.set("filtering_property",pi->getName());

  return data;
}

void TableView::setState(const tlp::DataSet& data) {
  bool showNodes = true;
  std::string filterPropertyName = "";
  data.get<bool>("show_nodes",showNodes);

  _ui->eltTypeCombo->setCurrentIndex(showNodes ? 0 : 1);

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
  connect(_ui->propertiesEditor,SIGNAL(setFilteredNodes()),this,SLOT(setFilteredNodesValue()));
  connect(_ui->propertiesEditor,SIGNAL(setFilteredEdges()),this,SLOT(setFilteredEdgesValue()));
  connect(_ui->propertiesEditor,SIGNAL(mapToGraphSelection()),this,SLOT(mapToGraphSelection()));

  _ui->table->setItemDelegate(new GraphTableItemDelegate(_ui->table));
  _ui->table->horizontalHeader()->setMovable(true);
  connect(_ui->filterEdit,SIGNAL(returnPressed()),this,SLOT(filterChanged()));
  _ui->splitter_2->setSizes(QList<int>() << centralWidget->width()/4 << centralWidget->width()*3/4);

  _ui->eltTypeCombo->addItem("Nodes");
  _ui->eltTypeCombo->addItem("Edges");
  _ui->eltTypeCombo->setCurrentIndex(0);
  connect(_ui->eltTypeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings()));
  connect(_ui->filteringPropertyCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings()));
}

void TableView::setFilteredNodesValue() {
  tlp::BooleanProperty* filterProp = static_cast<GraphSortFilterProxyModel*>(_ui->table->model())->filterProperty();
  QVariant val = TulipItemDelegate::showEditorDialog(NODE,_ui->propertiesEditor->contextProperty(),graph(),static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  Observable::holdObservers();
  graph()->push();

  if (filterProp != NULL) {
    node n;
    forEach(n,filterProp->getNodesEqualTo(true))
    GraphModel::setNodeValue(n.id,_ui->propertiesEditor->contextProperty(),val);
  }
  else {
    GraphModel::setAllNodeValue(_ui->propertiesEditor->contextProperty(),val);
  }

  Observable::unholdObservers();
}

void TableView::setFilteredEdgesValue() {
  tlp::BooleanProperty* filterProp = static_cast<GraphSortFilterProxyModel*>(_ui->table->model())->filterProperty();
  QVariant val = TulipItemDelegate::showEditorDialog(EDGE,_ui->propertiesEditor->contextProperty(),graph(),static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  Observable::holdObservers();
  graph()->push();

  if (filterProp != NULL) {
    edge e;
    forEach(e,filterProp->getEdgesEqualTo(true))
    GraphModel::setEdgeValue(e.id,_ui->propertiesEditor->contextProperty(),val);
  }
  else {
    GraphModel::setAllEdgeValue(_ui->propertiesEditor->contextProperty(),val);
  }

  Observable::unholdObservers();
}

void TableView::mapToGraphSelection() {
  BooleanProperty* out = graph()->getProperty<BooleanProperty>("viewSelection");

  if (_ui->eltTypeCombo->currentIndex() == 0 /* nodes */) {
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
  QSet<QString> visibleProperties;
  foreach(tlp::PropertyInterface* pi, _ui->propertiesEditor->visibleProperties()) {
    visibleProperties.insert(pi->getName().c_str());
  }

  GraphPropertiesModel<BooleanProperty>* model = new GraphPropertiesModel<BooleanProperty>(trUtf8("no selection"),g,false,_ui->filteringPropertyCombo);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(0);

  _ui->propertiesEditor->setGraph(g);

  _ui->frame->hide();

  //Show all the properties whose name starts with 'view'
  if (_model != NULL) {
    for(int i=0; i < _model->columnCount(); ++i) {
      QString propName = _model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
      bool checked = propName.startsWith("view");
      if (!visibleProperties.isEmpty()) {
        checked = visibleProperties.contains(propName);
      }
      _ui->propertiesEditor->setPropertyChecked(i, checked);
    }
  }
}

void TableView::graphDeleted() {
  setGraph(NULL);
}

void TableView::readSettings() {
  if ( ((_ui->eltTypeCombo->currentIndex() == 0) && dynamic_cast<NodesGraphModel*>(_model) == NULL) ||
       ((_ui->eltTypeCombo->currentIndex() == 1) && dynamic_cast<EdgesGraphModel*>(_model) == NULL)) {
    _ui->table->setModel(NULL);

    if (_ui->eltTypeCombo->currentIndex() == 0)
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
