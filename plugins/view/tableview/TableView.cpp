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
#include <tulip/TlpQtTools.h>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QApplication>
#include <QtGui/QMenu>

using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), _model(NULL), isNewGraph(false) {}

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
}

bool TableView::eventFilter(QObject* obj, QEvent* event) {
  if (event->type() == QEvent::Resize) {
    // ensure automatic resize of the viewport
    QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
    graphicsView()->viewport()->setFixedSize(resizeEvent->size());
    // same for PropertiesEditor
    QSize pSize = propertiesEditor->parentWidget()->parentWidget()->size();
    pSize.setHeight(resizeEvent->size().height() - 30);
    propertiesEditor->parentWidget()->parentWidget()->resize(pSize);
    pSize = propertiesEditor->size();
    pSize.setHeight(resizeEvent->size().height() - 40);
    propertiesEditor->resize(pSize);
    return true;
  }
  else {
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
}

void TableView::setupWidget() {
  propertiesEditor = new PropertiesEditor();
  // install this as event filter
  // for automatic resizing of the viewport
  graphicsView()->viewport()->parentWidget()->installEventFilter(this);
  QWidget* centralWidget = new QWidget();
  _ui->setupUi(centralWidget);

  setCentralWidget(centralWidget);
  connect(propertiesEditor,SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface*,bool)),this,SLOT(setPropertyVisible(tlp::PropertyInterface*,bool)));
  connect(propertiesEditor,SIGNAL(mapToGraphSelection()),this,SLOT(mapToGraphSelection()));
  connect(propertiesEditor,SIGNAL(resizeTableRows()), this, SLOT(resizeTableRows()));
  connect(_ui->table,SIGNAL(resizeTableRows()), this, SLOT(resizeTableRows()));

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
  // use a push button instead of a combobox (see matchPropertyCombo)
  // waiting for a fix for combobox in QGraphicsItem
  connect(_ui->matchPropertyButton, SIGNAL(pressed()), this, SLOT(setMatchProperty()));
}

QList<QWidget*> TableView::configurationWidgets() const {
  return QList<QWidget*>() << propertiesEditor;
}


void TableView::graphChanged(tlp::Graph* g) {
  isNewGraph = true;
  QSet<QString> visibleProperties;
  foreach(tlp::PropertyInterface* pi, propertiesEditor->visibleProperties()) {
    visibleProperties.insert(QString::fromUtf8(pi->getName().c_str()));
  }

  _ui->table->setSendSignalOnResize(false);

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

  isNewGraph = false;

  _ui->table->setSendSignalOnResize(true);
}

void TableView::graphDeleted(Graph*) {
  setGraph(NULL);
}

void TableView::readSettings() {
  if ( isNewGraph || ((_ui->eltTypeCombo->currentIndex() == 0) && dynamic_cast<NodesGraphModel*>(_model) == NULL) ||
       ((_ui->eltTypeCombo->currentIndex() == 1) && dynamic_cast<EdgesGraphModel*>(_model) == NULL)) {
    _ui->table->setModel(NULL);

    delete _model;

    if (_ui->eltTypeCombo->currentIndex() == 0)
      _model = new NodesGraphModel(_ui->table);
    else
      _model = new EdgesGraphModel(_ui->table);



    _model->setGraph(graph());
    GraphSortFilterProxyModel* sortModel = new GraphSortFilterProxyModel(_ui->table);
    sortModel->setSourceModel(_model);
    _ui->table->setModel(sortModel);
    connect(_model,SIGNAL(columnsInserted(QModelIndex,int,int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
    connect(_model,SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
    filterChanged();
  }

  GraphSortFilterProxyModel* sortModel = static_cast<GraphSortFilterProxyModel*>(_ui->table->model());

  sortModel->setFilterProperty(getFilteringProperty());

  QSet<tlp::PropertyInterface*> visibleProperties = propertiesEditor->visibleProperties();

  for (int i=0; i < _model->columnCount(); ++i) {
    PropertyInterface* pi =
      _model->headerData(i, Qt::Horizontal, TulipModel::PropertyRole).value<tlp::PropertyInterface*>();

    if (!visibleProperties.contains(pi))
      _ui->table->setColumnHidden(i, true);
  }

  if (!isNewGraph) {
    resizeTableRows();
  }

}

void TableView::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight) {
  QAbstractItemModel* model = static_cast<QAbstractItemModel*>(sender());

  for (int i = topLeft.row() ; i <= bottomRight.row() ; ++i) {
    PropertyInterface* pi = model->headerData(topLeft.column(),Qt::Horizontal,TulipModel::PropertyRole).value<PropertyInterface*>();

    if (pi->getTypename() == "string" && pi->getName() != "viewTexture" && pi->getName() != "viewFont")
      _ui->table->resizeRowToContents(i);
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

  QString propName = tlpStringToQString(pi->getName());

  for(int i=0; i < _model->columnCount(); ++i) {
    if (_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString() == propName) {
      _ui->table->horizontalHeader()->setSectionHidden(i,!v);
    }
  }

  if (_ui->matchPropertyButton->text() == propName)
    // set to Any
    _ui->matchPropertyButton->setText("Any");

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

void TableView::setMatchProperty() {
  QVector<QString> props;
  foreach(PropertyInterface* pi, propertiesEditor->visibleProperties()) {
    QString propName = tlpStringToQString(pi->getName());
    int i = 0;

    for (; i < props.size(); ++i) {
      if (props[i] > propName)
        break;
    }

    props.insert(i, propName);
  }

  QMenu menu;
  QAction* action = menu.addAction("-- Any --");
  menu.setActiveAction(action);
  foreach(QString prop, props) {
    if (_ui->matchPropertyButton->text() == prop) {
      action = menu.addAction(prop);
      menu.setActiveAction(action);
    }
    else
      menu.addAction(prop);
  }

  QPalette palette = QComboBox().palette();

  // set a combo like stylesheet
  menu.setStyleSheet(QString("QMenu::item {border-image: none; border-width: 4; padding: 0px 6px; font-size: 12px; color: %1; background-color: %2;} QMenu::item:selected {color: %3; background-color: %4}").arg(palette.color(QPalette::Active, QPalette::Text).name()).arg(palette.color(QPalette::Active, QPalette::Base).name()).arg(palette.color(QPalette::Active, QPalette::HighlightedText).name()).arg(palette.color(QPalette::Active, QPalette::Highlight).name()));

  // compute a combo like position
  // to popup the menu
  QWidget* pViewport = QApplication::widgetAt(QCursor::pos());
  QWidget* pView = pViewport->parentWidget();
  QGraphicsView* pGraphicsView = qobject_cast<QGraphicsView*>(pView);
  QGraphicsItem* pGraphicsItem =
    pGraphicsView->items(pViewport->mapFromGlobal(QCursor::pos())).first();
  QPoint popupPos = pGraphicsView->mapToGlobal(pGraphicsView->mapFromScene(pGraphicsItem->mapToScene(((QGraphicsProxyWidget *) pGraphicsItem)->subWidgetRect(_ui->matchPropertyButton).bottomLeft())));

  action = menu.exec(popupPos);

  if (action) {
    if (action->text() == "-- Any --")
      _ui->matchPropertyButton->setText("Any");
    else
      _ui->matchPropertyButton->setText(action->text());
  }
}

void TableView::filterChanged() {
  QString filter = _ui->filterEdit->text();
  GraphSortFilterProxyModel* sortModel = static_cast<GraphSortFilterProxyModel*>(_ui->table->model());
  QVector<PropertyInterface*> props;

  Graph* g = graph();

  if (_ui->matchPropertyButton->text() == "Any") {
    for (int i=0; i < _model->columnCount(); ++i) {
      if (!_ui->table->horizontalHeader()->isSectionHidden(i))
        props += _model->headerData(i,Qt::Horizontal,TulipModel::PropertyRole).value<PropertyInterface*>();
    }
  }
  else
    // a visible column
    props +=
      g->getProperty(QStringToTlpString(_ui->matchPropertyButton->text()));

  sortModel->setProperties(props);
  sortModel->setFilterFixedString(filter);
}

void TableView::setAllValues(PropertyInterface* prop, bool selectedOnly) {
  QVariant val =
    TulipItemDelegate::showEditorDialog(NODES_DISPLAYED ? NODE : EDGE,
                                        prop, graph(),
                                        static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()), (QWidget *) this->centralItem()->parentWidget());

  if (selectedOnly) {
    BooleanProperty* selection = graph()->getProperty<BooleanProperty>("viewSelection");

    if (NODES_DISPLAYED) {
      node n;
      forEach(n, selection->getNonDefaultValuatedNodes(graph())) {
        GraphModel::setNodeValue(n.id, prop, val);
      }
    }
    else {
      edge e;
      forEach(e, selection->getNonDefaultValuatedEdges(graph())) {
        GraphModel::setEdgeValue(e.id, prop, val);
      }
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

void TableView::setLabelsOfHighlightedRows(PropertyInterface *prop) {
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  StringProperty* label = graph()->getProperty<StringProperty>("viewLabel");

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED) {
      node n(itIdx->data(TulipModel::ElementIdRole).toUInt());
      label->setNodeStringValue(n, prop->getNodeStringValue(n));
    }
    else {
      edge e(itIdx->data(TulipModel::ElementIdRole).toUInt());
      label->setEdgeStringValue(e, prop->getEdgeStringValue(e));
    }
  }
}

void TableView::showCustomContextMenu(const QPoint & pos) {
  if (_ui->table->model()->rowCount() == 0)
    return;

  QModelIndex idx = _ui->table->indexAt(pos);
  unsigned int eltId = idx.data(TulipModel::ElementIdRole).toUInt();

  QString eltsName(NODES_DISPLAYED ? trUtf8("nodes") : trUtf8("edges"));
  std::string propName = _model->headerData(idx.column(), Qt::Horizontal, Qt::DisplayRole).toString().toUtf8().data();

  if (propName.empty())
    return;

  QModelIndexList highlightedRows =
    _ui->table->selectionModel()->selectedRows();

  QMenu contextMenu;
  contextMenu.setProperty("mainMenu", true);
  // the style sheet below allows to display disabled items
  // as "title" items
  contextMenu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");

  QAction* action = contextMenu.addAction(QString::fromUtf8(propName.c_str()));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction* setAll;
  QAction* selectedSetAll = NULL;
  QAction* highlightedSetAll = NULL;
  QAction* toLabels;
  QAction* selectedToLabels = NULL;
  QAction* highlightedToLabels = NULL;

  QMenu* subMenu = contextMenu.addMenu(trUtf8("Set values of "));
  setAll = subMenu->addAction(trUtf8("All") + ' ' + eltsName);

  selectedSetAll = subMenu->addAction(trUtf8("Selected") + ' ' + eltsName);

  highlightedSetAll = subMenu->addAction((trUtf8("Rows highlighted") + ' ' + eltsName) + (highlightedRows.size() > 1 ? "" : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)").arg(eltId)));

  subMenu = contextMenu.addMenu(trUtf8("To labels of "));
  toLabels = subMenu->addAction(trUtf8("All") + ' ' + eltsName);

  selectedToLabels = subMenu->addAction(trUtf8("Selected") + ' ' + eltsName);

  highlightedToLabels = subMenu->addAction((trUtf8("Rows highlighted") + ' ' + eltsName) + (highlightedRows.size() > 1 ? "" : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)").arg(eltId)));

  contextMenu.addSeparator();
  action = contextMenu.addAction(highlightedRows.size() > 1 ?
                                 (trUtf8("Rows highlighted") + ' ' + eltsName) :
                                 QString(NODES_DISPLAYED ? "Node #%1" : "Edge #%1").arg(eltId));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction* toggleAction = contextMenu.addAction(trUtf8("Toggle selection"));
  QAction* selectAction = contextMenu.addAction(trUtf8("Select"));
  QAction* deleteAction = contextMenu.addAction(trUtf8("Delete"));

  // display the menu with the mouse inside to allow
  // keyboard navigation
  action = contextMenu.exec(QCursor::pos() - QPoint(5,5));

  if (!action)
    return;

  // hold/unhold observers
  tlp::ObserverHolder oh;

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

  if (action == setAll) {
    setAllValues(graph()->getProperty(propName), false);
    return;
  }

  if (action == selectedSetAll) {
    // set values for all rows elts
    setAllValues(graph()->getProperty(propName), true);
    return;
  }

  if (action == highlightedSetAll) {
    // set values for elts corresponding to highlighted rows
    setAllHighlightedRows(graph()->getProperty(propName));
    return;
  }

  if (action == toLabels) {
    bool nodes = NODES_DISPLAYED;
    propertiesEditor->toLabels(graph()->getProperty(propName), nodes, !nodes);
    return;
  }

  if (action == selectedToLabels) {
    // set values as labels
    bool nodes = NODES_DISPLAYED;
    propertiesEditor->toLabels(graph()->getProperty(propName), nodes, !nodes,
                               true);
    return;
  }

  if (action == highlightedToLabels) {
    // set values as labels for elts corresponding to highlighted rows
    setLabelsOfHighlightedRows(graph()->getProperty(propName));
    return;
  }
}

void TableView::resizeTableRows() {
  if (!_ui->table->model())
    return;

  int top = qMax(0, _ui->table->verticalHeader()->visualIndexAt(0));
  int bottom = _ui->table->verticalHeader()->visualIndexAt(_ui->table->viewport()->height());

  if (bottom == -1 || (bottom+10) >= _ui->table->model()->rowCount())
    bottom = _ui->table->model()->rowCount() - 1;
  else
    bottom += 10;

  for (int i = top ; i <= bottom ; ++i)
    _ui->table->resizeRowToContents(i);

  _ui->table->resizeColumnsToContents();
}


PLUGIN(TableView)
