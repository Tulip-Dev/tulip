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

#include "TableView.h"
#include "ui_TableView.h"
#include "PropertiesEditor.h"

#include <tulip/Observable.h>
#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>
#include <tulip/TlpQtTools.h>
#include <tulip/StringProperty.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/Perspective.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/PropertyCreationDialog.h>

#include <QResizeEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>

using namespace tlp;

TableView::TableView(tlp::PluginContext *): ViewWidget(), _ui(new Ui::TableViewWidget), propertiesEditor(NULL), _model(NULL), isNewGraph(false), filteringColumns(false), previousGraph(NULL) {}

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
  // install this as event filter
  // for automatic resizing of the viewport
  graphicsView()->viewport()->parentWidget()->installEventFilter(this);
  QWidget* centralWidget = new QWidget();
  _ui->setupUi(centralWidget);

  setCentralWidget(centralWidget);

  propertiesEditor = new PropertiesEditor(static_cast<QGraphicsProxyWidget *>(centralItem())->widget());

  connect(propertiesEditor,SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface*,bool)),this,SLOT(setPropertyVisible(tlp::PropertyInterface*,bool)));
  connect(propertiesEditor,SIGNAL(mapToGraphSelection()),this,SLOT(mapToGraphSelection()));

  _ui->table->setItemDelegate(new GraphTableItemDelegate(_ui->table));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  _ui->table->horizontalHeader()->setSectionsMovable(true);
#else
  _ui->table->horizontalHeader()->setMovable(true);
#endif
  _ui->table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_ui->table->horizontalHeader(),
          SIGNAL(customContextMenuRequested (const QPoint &)),
          this,
          SLOT(showHorizontalHeaderCustomContextMenu(const QPoint&)));
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
  // columns/properties filtering
  filteringColumns = false;
  connect(_ui->columnsFilterEdit,SIGNAL(textChanged(QString)),this,SLOT(setColumnsFilter(QString)));
  connect(propertiesEditor->getPropertiesFilterEdit(),SIGNAL(textChanged(QString)),this,SLOT(setPropertiesFilter(QString)));
}

QList<QWidget*> TableView::configurationWidgets() const {
  return QList<QWidget*>() << propertiesEditor;
}


void TableView::graphChanged(tlp::Graph* g) {
  isNewGraph = true;
  QSet<QString> visibleProperties;

  if (g && propertiesEditor->getGraph() &&
      (g->getRoot() == propertiesEditor->getGraph()->getRoot())) {
    foreach(tlp::PropertyInterface* pi, propertiesEditor->visibleProperties()) {
      visibleProperties.insert(tlpStringToQString(pi->getName()));
    }
  }

  GraphPropertiesModel<BooleanProperty>* model = new GraphPropertiesModel<BooleanProperty>(trUtf8("no selection"),g,false,_ui->filteringPropertyCombo);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(0);

  propertiesEditor->setGraph(g);

  _ui->table->horizontalHeader()->show();
  _ui->table->verticalHeader()->show();

  //Show all the properties
  if (_model != NULL) {
    for(int i=0; i < _model->columnCount(); ++i) {
      QString propName = _model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
      // a property is visible only if it was previously visible
      bool checked = !visibleProperties.isEmpty() ?
                     visibleProperties.contains(propName) : true;

      // unless the property did not exist in the previous graph
      if (previousGraph &&
          !previousGraph->existProperty(QStringToTlpString(propName)))
        checked = true;

      propertiesEditor->setPropertyChecked(propName, checked);
    }
  }

  previousGraph = g;
  isNewGraph = false;
}

void TableView::graphDeleted(Graph* ancestor) {
  // if the current graph is deleted
  // just inform the WorkspacePanel
  // that we can display its ancestor instead
  assert(ancestor == NULL || graph()->getSuperGraph() == ancestor);

  if (ancestor)
    emit graphSet(ancestor);
  else {
    setGraph(NULL);
    readSettings();
  }
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

  // reset columns filtering
  _ui->columnsFilterEdit->setText("");
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

  _ui->table->horizontalHeader()->setVisible(visible);
  _ui->table->verticalHeader()->setVisible(visible);
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

  foreach(const QString& prop, props) {
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
  QGraphicsView* pGraphicsView = static_cast<QGraphicsView*>(pView);
  QGraphicsItem* pGraphicsItem =
    pGraphicsView->items(pViewport->mapFromGlobal(QCursor::pos())).first();
  QPoint popupPos = pGraphicsView->mapToGlobal(pGraphicsView->mapFromScene(pGraphicsItem->mapToScene(static_cast<QGraphicsProxyWidget *>(pGraphicsItem)->subWidgetRect(_ui->matchPropertyButton).bottomLeft())));

  action = menu.exec(popupPos);

  if (action) {
    if (action->text() == "-- Any --")
      _ui->matchPropertyButton->setText("Any");
    else
      _ui->matchPropertyButton->setText(action->text());
  }
}

void TableView::setColumnsFilter(QString text) {
  if (filteringColumns)
    return;

  filteringColumns = true;
  propertiesEditor->getPropertiesFilterEdit()->setText(text);
  filteringColumns = false;
}

void TableView::setPropertiesFilter(QString text) {
  if (filteringColumns)
    return;

  filteringColumns = true;
  _ui->columnsFilterEdit->setText(text);
  filteringColumns = false;
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
  sortModel->setFilterRegExp(filter);
}

void TableView::mapToGraphSelection() {
  BooleanProperty* out = graph()->getProperty<BooleanProperty>("viewSelection");

  if (NODES_DISPLAYED) {
    out->setAllNodeValue(false);
    QItemSelectionModel *selectionModel = _ui->table->selectionModel();

    foreach(const QModelIndex& idx,selectionModel->selectedRows()) {
      node n(idx.data(TulipModel::ElementIdRole).toUInt());
      out->setNodeValue(n,true);
    }
  }
  else {
    out->setAllEdgeValue(false);
    QItemSelectionModel *selectionModel = _ui->table->selectionModel();

    foreach(const QModelIndex& idx,selectionModel->selectedRows()) {
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

  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED)
      selection->setNodeValue(node(itIdx->data(TulipModel::ElementIdRole).toUInt()), true);
    else
      selection->setEdgeValue(edge(itIdx->data(TulipModel::ElementIdRole).toUInt()), true);
  }
}

bool TableView::setAllHighlightedRows(PropertyInterface* prop) {
  Graph* g = graph();
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  QVariant val =
    TulipItemDelegate::showEditorDialog(NODES_DISPLAYED ? NODE : EDGE,
                                        prop, g,
                                        static_cast<TulipItemDelegate*>(_ui->table->itemDelegate()));

  // Check if edition has been cancelled
  if (!val.isValid())
    return false;

  for (QList<QModelIndex>::const_iterator itIdx = rows.begin();
       itIdx != rows.end(); ++itIdx) {
    if (NODES_DISPLAYED)
      GraphModel::setNodeValue(itIdx->data(TulipModel::ElementIdRole).toUInt(),
                               prop, val);
    else
      GraphModel::setEdgeValue(itIdx->data(TulipModel::ElementIdRole).toUInt(),
                               prop, val);
  }

  return true;
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
  QString eltName(NODES_DISPLAYED ? trUtf8("node") : trUtf8("edge"));
  std::string propName = QStringToTlpString(_model->headerData(idx.column(), Qt::Horizontal, Qt::DisplayRole).toString());

  if (propName.empty())
    return;

  PropertyInterface* prop = graph()->getProperty(propName);

  QModelIndexList highlightedRows =
    _ui->table->selectionModel()->selectedRows();

  QMenu contextMenu;
  Perspective::redirectStatusTipOfMenu(&contextMenu);
  // the style sheet below allows to display disabled items
  // as "title" items in the "mainMenu"
  contextMenu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
  // so it is the "mainMenu"
  contextMenu.setProperty("mainMenu", true);

  QAction* action = contextMenu.addAction(tlpStringToQString(propName));
  action->setEnabled(false);
  contextMenu.addSeparator();

  //QAction* setDefault = contextMenu.addAction(trUtf8("Set default") + ' ' + eltName + " value");

  QMenu* subMenu = contextMenu.addMenu(trUtf8("Set values of "));
  QAction* setAll = subMenu->addAction(trUtf8("All") + ' ' + eltsName + OF_PROPERTY  + trUtf8(" to a new default value"));
  setAll->setToolTip(QString("Choose a new ") + eltsName + " default value to reset the values of all " + eltsName + OF_PROPERTY);
  QAction* setAllGraph = subMenu->addAction(trUtf8("All") + ' ' + eltsName + OF_GRAPH);
  setAllGraph->setToolTip(QString("Choose a value to be assigned to all the existing ") + eltsName + OF_GRAPH);

  QAction* selectedSetAll = subMenu->addAction(trUtf8("Selected") + ' ' + eltsName + OF_GRAPH);
  selectedSetAll->setToolTip(QString("Choose a value to be assigned to the selected ") + eltsName + OF_GRAPH);

  QAction* highlightedSetAll = subMenu->addAction((trUtf8("Rows highlighted") + ' ' + eltsName) + (highlightedRows.size() > 1 ? "" : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)").arg(highlightedRows[0].data(TulipModel::ElementIdRole).toUInt())));
  highlightedSetAll->setToolTip(QString("Choose a value to be assigned to the ") + eltsName + " displayed in the currently highlighted row(s)");

  subMenu = contextMenu.addMenu(trUtf8("To labels of "));
  QAction* toLabels = subMenu->addAction(trUtf8("All ") + eltsName + OF_GRAPH);
  toLabels->setToolTip(QString("Set the values of the current property as labels of the ") + eltsName + OF_GRAPH);
  QAction* selectedToLabels = subMenu->addAction(trUtf8("Selected") + ' ' + eltsName + OF_GRAPH);
  selectedToLabels->setToolTip(QString("Set the values of the current property as labels of the selected ") + eltsName + OF_GRAPH);

  QAction* highlightedToLabels = subMenu->addAction((trUtf8("Rows highlighted") + ' ' + eltsName) + (highlightedRows.size() > 1 ? "" : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)").arg(highlightedRows[0].data(TulipModel::ElementIdRole).toUInt())));
  highlightedToLabels->setToolTip(QString("Set the values of the current property as labels of the ") + eltsName + " displayed in the currently highlighted row(s)");

  contextMenu.addSeparator();
  action = contextMenu.addAction(highlightedRows.size() > 1 ?
                                 (trUtf8("Rows highlighted") + ' ' + eltsName) :
                                 QString(NODES_DISPLAYED ? "Node #%1" : "Edge #%1").arg(eltId));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction* toggleAction = contextMenu.addAction(trUtf8("Toggle selection"));
  toggleAction->setToolTip(QString("Invert the selection of the ") + action->text() + ": deselect if selected or select if not selected");
  QAction* selectAction = contextMenu.addAction(trUtf8("Select"));
  selectAction->setToolTip(QString("Set the selection with the ") + action->text());
  QAction* deleteAction = contextMenu.addAction(trUtf8("Delete"));
  deleteAction->setToolTip(QString("Delete the ") + action->text());

  // display the menu with the mouse inside to allow
  // keyboard navigation
  action = contextMenu.exec(QCursor::pos() - QPoint(5,5));

  if (!action)
    return;

  /*if (action == setDefault) {
    propertiesEditor->setDefaultValue(prop, NODES_DISPLAYED);
    return;
    }*/

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
    if (!propertiesEditor->setAllValues(prop, NODES_DISPLAYED, false))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == setAllGraph) {
    if (!propertiesEditor->setAllValues(prop, NODES_DISPLAYED, false, graph()))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == selectedSetAll) {
    // set values for all rows elts
    if (!propertiesEditor->setAllValues(prop, NODES_DISPLAYED, true))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == highlightedSetAll) {
    // set values for elts corresponding to highlighted rows
    setAllHighlightedRows(prop);
    return;
  }

  if (action == toLabels) {
    bool nodes = NODES_DISPLAYED;
    propertiesEditor->toLabels(prop, nodes, !nodes);
    return;
  }

  if (action == selectedToLabels) {
    // set values as labels
    bool nodes = NODES_DISPLAYED;
    propertiesEditor->toLabels(prop, nodes, !nodes,
                               true);
    return;
  }

  if (action == highlightedToLabels) {
    // set values as labels for elts corresponding to highlighted rows
    setLabelsOfHighlightedRows(prop);
    return;
  }
}

void TableView::showHorizontalHeaderCustomContextMenu(const QPoint & pos) {
  if (_ui->table->model()->columnCount() == 0)
    return;

  QModelIndex idx = _ui->table->indexAt(pos);

  QString eltsName(NODES_DISPLAYED ? trUtf8("nodes") : trUtf8("edges"));
  std::string propName = QStringToTlpString(_model->headerData(idx.column(), Qt::Horizontal, Qt::DisplayRole).toString());

  if (propName.empty())
    return;

  PropertyInterface* prop = graph()->getProperty(propName);

  QModelIndexList highlightedRows =
    _ui->table->selectionModel()->selectedRows();

  QMenu contextMenu;
  Perspective::redirectStatusTipOfMenu(&contextMenu);
  // the style sheet below allows to display disabled items
  // as "title" items in the "mainMenu"
  contextMenu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
  // so it is the "mainMenu"
  contextMenu.setProperty("mainMenu", true);

  QAction* action = contextMenu.addAction(tlpStringToQString(propName));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction* addProp = contextMenu.addAction(QIcon(":/tulip/gui/icons/64/list-add.png"),"Add property of the same type");
  addProp->setToolTip("Display a dialog to create a new property owning to the current graph");
  QAction* copyProp = contextMenu.addAction("Copy");
  copyProp->setToolTip(QString("Copy the values of \"") + action->text() + "\" in a property of the same type");
  QAction* deleteProp = NULL;

  if (!Perspective::instance()->isReservedPropertyName(propName.c_str()) ||
      // Enable deletion of reserved properties when on a subgraph and that properties are local
      (graph() != graph()->getRoot() && graph()->existLocalProperty(propName))) {
    deleteProp = contextMenu.addAction("Delete");
    deleteProp->setToolTip(QString("Delete the property \"") + action->text() + '"');
  }

  QAction* renameProp = NULL;

  if (!Perspective::instance()->isReservedPropertyName(propName.c_str())) {
    renameProp = contextMenu.addAction("Rename");
    renameProp->setToolTip(QString("Rename the property \"") + action->text() + '"');
  }

  /*QMenu* subMenu = contextMenu.addMenu(trUtf8("Set default value for"));
  QAction* nodesSetDefault = subMenu->addAction(trUtf8("nodes"));
  QAction* edgesSetDefault = subMenu->addAction(trUtf8("edges"));*/

  QMenu* subMenu = contextMenu.addMenu(trUtf8("Set values of "));
  QAction* nodesSetAll = subMenu->addAction(trUtf8("All nodes") + OF_PROPERTY + trUtf8(" to a new default value"));
  nodesSetAll->setToolTip(QString("Choose a new node default value to reset the values of all nodes") + OF_PROPERTY);
  QAction* edgesSetAll = subMenu->addAction(trUtf8("All edges") + OF_PROPERTY + trUtf8(" to a new default value"));
  edgesSetAll->setToolTip(QString("Choose a new edge default value to reset the values of all edges ") + OF_PROPERTY);
  QAction* nodesSetAllGraph = subMenu->addAction(trUtf8("All nodes") + OF_GRAPH);
  nodesSetAllGraph->setToolTip(QString("Choose a value to be assigned to all the existing nodes") + OF_GRAPH);
  QAction* edgesSetAllGraph = subMenu->addAction(trUtf8("All edges") + OF_GRAPH);
  edgesSetAllGraph->setToolTip(QString("Choose a value to be assigned to all the existing edges") + OF_GRAPH);
  QAction* nodesSelectedSetAll = subMenu->addAction(trUtf8("Selected nodes") + OF_GRAPH);
  nodesSelectedSetAll->setToolTip(QString("Choose a value to be assigned to the selected nodes") + OF_GRAPH);
  QAction* edgesSelectedSetAll = subMenu->addAction(trUtf8("Selected edges") + OF_GRAPH);
  edgesSelectedSetAll->setToolTip(QString("Choose a value to be assigned to the selected edges") + OF_GRAPH);
  QAction* highlightedSetAll = NULL;

  if (highlightedRows.size() != 0) {
    highlightedSetAll = subMenu->addAction((trUtf8("Rows highlighted") + ' ' + eltsName) + (highlightedRows.size() > 1 ? "" : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)").arg(highlightedRows[0].data(TulipModel::ElementIdRole).toUInt())));
    highlightedSetAll->setToolTip(QString("Choose a value to be assigned to the ") + eltsName + " displayed in the currently highlighted row(s)");
  }

  QAction* toLabels = NULL;
  QAction* nodesToLabels = NULL;
  QAction* edgesToLabels = NULL;
  QAction* selectedToLabels = NULL;
  QAction* nodesSelectedToLabels = NULL;
  QAction* edgesSelectedToLabels = NULL;
  QAction* highlightedToLabels = NULL;

  if (propName != "viewLabel") {
    subMenu = contextMenu.addMenu(trUtf8("To labels of "));
    toLabels = subMenu->addAction(trUtf8("All elements") + OF_GRAPH);
    toLabels->setToolTip(QString("Set the values of the current property as labels of all elements") + OF_GRAPH);
    nodesToLabels = subMenu->addAction(trUtf8("All nodes") + OF_GRAPH);
    nodesToLabels->setToolTip(QString("Set the values of the current property as labels of the nodes") + OF_GRAPH);
    edgesToLabels = subMenu->addAction(trUtf8("All edges") + OF_GRAPH);
    edgesToLabels->setToolTip(QString("Set the values of the current property as labels of the edges") + OF_GRAPH);
    selectedToLabels = subMenu->addAction(trUtf8("Selected elements") + OF_GRAPH);
    selectedToLabels->setToolTip(QString("Set the values of the current property as labels of the selected elements") + OF_GRAPH);
    nodesSelectedToLabels = subMenu->addAction(trUtf8("Selected nodes") + OF_GRAPH);
    nodesSelectedToLabels->setToolTip(QString("Set the values of the current property as labels of the selected nodes") + OF_GRAPH);
    edgesSelectedToLabels = subMenu->addAction(trUtf8("Selected edges") + OF_GRAPH);
    edgesSelectedToLabels->setToolTip(QString("Set the values of the current property as labels of the selected edges") + OF_GRAPH);

    if (highlightedRows.size() != 0) {
      highlightedToLabels = subMenu->addAction((trUtf8("Rows highlighted") + ' ' + eltsName) + (highlightedRows.size() > 1 ? "" : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)").arg(highlightedRows[0].data(TulipModel::ElementIdRole).toUInt())));
      highlightedToLabels->setToolTip(QString("Set the values of the current property as labels of the ") + eltsName + " displayed in the currently highlighted row(s)");
    }
  }

  contextMenu.addSeparator();
  QAction* sortById = contextMenu.addAction("Sort by id");
  sortById->setToolTip(QString("Display the rows in ordering of the id of the ") + eltsName);
  contextMenu.addSeparator();

  // display the menu with the mouse inside to allow
  // keyboard navigation
  action = contextMenu.exec(QCursor::pos() - QPoint(5,5));

  if (!action)
    return;

  if (action == sortById) {
    if (_ui->table->horizontalHeader()->sortIndicatorSection() != -1) {
      _ui->table->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
      GraphSortFilterProxyModel* sortModel = static_cast<GraphSortFilterProxyModel*>(_ui->table->model());
      QAbstractItemModel* model = sortModel->sourceModel();
      sortModel->setSourceModel(NULL);
      sortModel->setSourceModel(model);
      sortModel->setFilterProperty(getFilteringProperty());

      QSet<tlp::PropertyInterface*> visibleProperties = propertiesEditor->visibleProperties();

      for (int i=0; i <model->columnCount(); ++i) {
        PropertyInterface* pi =
          _model->headerData(i, Qt::Horizontal, TulipModel::PropertyRole).value<tlp::PropertyInterface*>();

        if (!visibleProperties.contains(pi))
          _ui->table->setColumnHidden(i, true);
      }
    }

    return;
  }

  /*if (action == nodesSetDefault || action == edgesSetDefault) {
    propertiesEditor->setDefaultValue(prop, action == nodesSetDefault);
    return;
    }*/

  // hold/unhold observers
  tlp::ObserverHolder oh;

  // allow to undo
  graph()->push();

  if (action == copyProp) {
    if (CopyPropertyDialog::copyProperty(graph(), prop, true,
                                         Perspective::instance()->mainWindow())
        == NULL)
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == deleteProp) {
    prop->getGraph()->delLocalProperty(propName);
    return;
  }

  if (action == renameProp) {
    if (!propertiesEditor->renameProperty(prop))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == addProp) {
    if (PropertyCreationDialog::createNewProperty(graph(), Perspective::instance()->mainWindow(), prop->getTypename())
        == NULL)
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == nodesSetAll) {
    if (!propertiesEditor->setAllValues(prop, true, false))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == nodesSetAllGraph) {
    if (!propertiesEditor->setAllValues(prop, true, false, true))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == edgesSetAll) {
    if (!propertiesEditor->setAllValues(prop, false, false))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == edgesSetAllGraph) {
    if (!propertiesEditor->setAllValues(prop, false, false, true))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == nodesSelectedSetAll) {
    // set values for all rows elts
    if (!propertiesEditor->setAllValues(prop, true, true))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == edgesSelectedSetAll) {
    // set values for all rows elts
    if (!propertiesEditor->setAllValues(prop, false, true))
      // cancelled so undo
      graph()->pop();

    return;
  }

  if (action == highlightedSetAll) {
    // set values for elts corresponding to highlighted rows
    setAllHighlightedRows(prop);
    return;
  }

  if (action == toLabels) {
    propertiesEditor->toLabels(prop, true, true);
    return;
  }

  if (action == nodesToLabels) {
    propertiesEditor->toLabels(prop, true, false);
    return;
  }

  if (action == edgesToLabels) {
    propertiesEditor->toLabels(prop, false, true);
    return;
  }

  if (action == selectedToLabels) {
    // set values as labels
    propertiesEditor->toLabels(prop, true, true, true);
    return;
  }

  if (action == nodesSelectedToLabels) {
    // set values as labels
    propertiesEditor->toLabels(prop, true, false, true);
    return;
  }

  if (action == edgesSelectedToLabels) {
    // set values as labels
    propertiesEditor->toLabels(prop, false, true, true);
    return;
  }

  if (action == highlightedToLabels) {
    // set values as labels for elts corresponding to highlighted rows
    setLabelsOfHighlightedRows(prop);
    return;
  }
}

PLUGIN(TableView)
