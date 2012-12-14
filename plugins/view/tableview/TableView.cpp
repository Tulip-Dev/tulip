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

#include <tulip/Observable.h>
#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>
#include <QtGui/QMenu>

using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), _model(NULL) {
  propertiesEditor = new PropertiesEditor();
}

TableView::~TableView() {
  delete _ui;
}

#define NODES_DISPLAYED (_ui->eltTypeCombo->currentIndex() == 0)
#define EDGES_DISPLAYED (_ui->eltTypeCombo->currentIndex() == 1)

tlp::BooleanProperty* TableView::getFilteringProperty() const {
  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  PropertyInterface* pi = model->data(model->index(_ui->filteringPropertyCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();
  return pi ? static_cast<BooleanProperty*>(pi) : NULL;
}

bool TableView::hasEffectiveFiltering() {
  GraphSortFilterProxyModel* sortModel = static_cast<GraphSortFilterProxyModel*>(_ui->table->model());

  return sortModel->rowCount() != sortModel->sourceModel()->rowCount();
}

tlp::DataSet TableView::state() const {
  DataSet data;
  data.set("show_nodes", NODES_DISPLAYED);
  data.set("show_edges", EDGES_DISPLAYED);

  BooleanProperty* pi = getFilteringProperty();

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
  connect(propertiesEditor,SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface*,bool)),this,SLOT(setPropertyVisible(tlp::PropertyInterface*,bool)));
  connect(propertiesEditor,SIGNAL(mapToGraphSelection()),this,SLOT(mapToGraphSelection()));

  _ui->table->setItemDelegate(new GraphTableItemDelegate(_ui->table));
  _ui->table->horizontalHeader()->setMovable(true);
  connect(_ui->table, SIGNAL(customContextMenuRequested (const QPoint &)),
          SLOT(showCustomContextMenu(const QPoint&)));
  connect(_ui->filterEdit,SIGNAL(returnPressed()),this,SLOT(filterChanged()));

  _ui->eltTypeCombo->addItem("Nodes");
  _ui->eltTypeCombo->addItem("Edges");
  _ui->eltTypeCombo->setCurrentIndex(0);
  connect(_ui->eltTypeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings()));
  connect(_ui->filteringPropertyCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings()));
}

QList<QWidget*> TableView::configurationWidgets() const {
  return QList<QWidget*>() << propertiesEditor;
}


void TableView::graphChanged(tlp::Graph* g) {
  QSet<QString> visibleProperties;
  foreach(tlp::PropertyInterface* pi, propertiesEditor->visibleProperties()) {
    visibleProperties.insert(pi->getName().c_str());
  }

  GraphPropertiesModel<BooleanProperty>* model = new GraphPropertiesModel<BooleanProperty>(trUtf8("no selection"),g,false,_ui->filteringPropertyCombo);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(0);

  propertiesEditor->setGraph(g);

  _ui->frame->hide();

  //Show all the properties whose name starts with 'view'
  if (_model != NULL) {
    for(int i=0; i < _model->columnCount(); ++i) {
      QString propName = _model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
      bool checked = propName.startsWith("view");

      if (!visibleProperties.isEmpty()) {
        checked = visibleProperties.contains(propName);
      }

      propertiesEditor->setPropertyChecked(i, checked);
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

  sortModel->setFilterProperty(getFilteringProperty());

  QSet<tlp::PropertyInterface*> visibleProperties = propertiesEditor->visibleProperties();

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


void TableView::setAllFilteredValue(PropertyInterface* prop) {
  QVariant val =
    TulipItemDelegate::showEditorDialog(NODES_DISPLAYED ? NODE : EDGE,
                                        prop, graph(),
                                        static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  if (hasEffectiveFiltering()) {
    QAbstractItemModel* model = _ui->table->model();
    int nbRows = model->rowCount();

    for (int row = 0; row < nbRows; ++row) {
      if (NODES_DISPLAYED)
        GraphModel::setNodeValue(model->index(row, 0).data(TulipModel::ElementIdRole).toUInt(),
                                 prop, val);
      else
        GraphModel::setEdgeValue(model->index(row, 0).data(TulipModel::ElementIdRole).toUInt(),
                                 prop, val);
    }
  }
  else {
    if (NODES_DISPLAYED)
      GraphModel::setAllNodeValue(prop,val);
    else
      GraphModel::setAllEdgeValue(prop,val);
  }
}

void TableView::mapToGraphSelection() {
  BooleanProperty* out = graph()->getProperty<BooleanProperty>("viewSelection");

  if (NODES_DISPLAYED) {
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

void TableView::delHighlightedRows() {
  Graph* g = graph();
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED)
      g->delNode(node(itIdx->data(TulipModel::ElementIdRole).toUInt()));
    else
      g->delEdge(edge(itIdx->data(TulipModel::ElementIdRole).toUInt()));
  }
}

void TableView::toggleHighlightedRows() {
  Graph* g = graph();
  BooleanProperty *selection =
    g->getProperty<BooleanProperty>("viewSelection");
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED) {
      node n(itIdx->data(TulipModel::ElementIdRole).toUInt());
      selection->setNodeValue(n, !selection->getNodeValue(n));
    }
    else {
      edge e(itIdx->data(TulipModel::ElementIdRole).toUInt());
      selection->setEdgeValue(e, !selection->getEdgeValue(e));
    }
  }
}

void TableView::selectHighlightedRows() {
  Graph* g = graph();
  BooleanProperty *selection =
    g->getProperty<BooleanProperty>("viewSelection");
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED)
      selection->setNodeValue(node(itIdx->data(TulipModel::ElementIdRole).toUInt()), true);
    else
      selection->setEdgeValue(edge(itIdx->data(TulipModel::ElementIdRole).toUInt()), true);
  }
}

void TableView::setAllHighlightedRows(PropertyInterface* prop) {
  Graph* g = graph();
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  QVariant val =
    TulipItemDelegate::showEditorDialog(NODES_DISPLAYED ? NODE : EDGE,
                                        prop, g,
                                        static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED)
      GraphModel::setNodeValue(itIdx->data(TulipModel::ElementIdRole).toUInt(),
                               prop, val);
    else
      GraphModel::setEdgeValue(itIdx->data(TulipModel::ElementIdRole).toUInt(),
                               prop, val);
  }
}

void TableView::showCustomContextMenu(const QPoint & pos) {
  QModelIndex idx = _ui->table->indexAt(pos);
  unsigned int eltId = idx.data(TulipModel::ElementIdRole).toUInt();

  bool hasFiltering = hasEffectiveFiltering();
  QString eltsName(NODES_DISPLAYED ? tr("nodes") : tr("edges"));
  std::string propName = _model->headerData(idx.column(), Qt::Horizontal, Qt::DisplayRole).toString().toUtf8().data();
  QModelIndexList highlightedRows =
    _ui->table->selectionModel()->selectedRows();

  QMenu contextMenu;
  QAction* action = contextMenu.addAction(propName.c_str());
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction* setAll;
  QAction* filteredSetAll = NULL;
  QAction* highlightedSetAll = NULL;
  QAction* toLabels;
  QAction* filteredToLabels = NULL;
  QAction* highlightedToLabels = NULL;

  if (!hasFiltering && highlightedRows.size() == 1) {
    setAll = contextMenu.addAction(tr("Set all ")  + eltsName + ' ' + tr("values"));
    toLabels = contextMenu.addAction(tr("Set all values as labels"));
  }
  else {
    QMenu* subMenu = contextMenu.addMenu(tr("Set values of "));
    setAll = subMenu->addAction(tr("All") + ' ' + eltsName);

    if (hasFiltering)
      filteredSetAll = subMenu->addAction(tr("Filtered") + ' ' + eltsName);

    if (highlightedRows.size() > 1)
      highlightedSetAll = subMenu->addAction(tr("Rows highlighted") + ' '
                                             + eltsName);

    subMenu = contextMenu.addMenu(tr("Set values as labels of "));
    toLabels = subMenu->addAction(tr("All") + ' ' + eltsName);

    if (hasFiltering)
      filteredToLabels = subMenu->addAction(tr("Filtered") + ' ' + eltsName);

    if (highlightedRows.size() > 1)
      highlightedToLabels = subMenu->addAction(tr("Rows highlighted") + ' '
                            + eltsName);
  }

  contextMenu.addSeparator();
  action = contextMenu.addAction(highlightedRows.size() > 1 ?
                                 (tr("Rows highlighted") + ' ' + eltsName) :
                                 QString(NODES_DISPLAYED ? "Node #%1" : "Edge #%1").arg(eltId));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction* toggleAction = contextMenu.addAction(tr("Toggle selection"));
  QAction* selectAction = contextMenu.addAction(tr("Select"));
  QAction* deleteAction = contextMenu.addAction(tr("Delete"));

  contextMenu.setStyleSheet("QMenu::item:disabled {color: white; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");

  // display the menu with the mouse inside to allow
  // keyboard navigation
  action = contextMenu.exec(QCursor::pos() - QPoint(5,5));

  if (!action)
    return;

  // hold/unhold observers
  tlp::ObserverLocker ol;

  // allow to undo
  graph()->push();

  if (action == deleteAction) {
    // delete elts corresponding to highlighted rows
    delHighlightedRows();
    // no more highlighted rows
    _ui->table->clearSelection();
    return;
  }

  if (action == toggleAction) {
    // select/deselect elts corresponding to highlighted rows
    toggleHighlightedRows();
    return;
  }

  if (action == selectAction) {
    // select elts corresponding to highlighted rows
    selectHighlightedRows();
    return;
  }

  if (action == setAll || action == filteredSetAll) {
    // set values for all rows elts
    setAllFilteredValue(graph()->getProperty(propName));
    return;
  }

  if (action == highlightedSetAll) {
    // set values for elts corresponding to highlighted rows
    setAllHighlightedRows(graph()->getProperty(propName));
    return;
  }

  if (action == toLabels || action == filteredToLabels) {
    // set values as labels
    setAllFilteredValue(graph()->getProperty("viewLabel"));
    return;
  }

  if (action == highlightedToLabels) {
    // set values as labels for elts corresponding to highlighted rows
    setAllHighlightedRows(graph()->getProperty("viewLabel"));
    return;
  }
}

PLUGIN(TableView)
