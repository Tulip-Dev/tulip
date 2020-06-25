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
#include <tulip/Perspective.h>

#include <QResizeEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>

using namespace tlp;

TableView::TableView(tlp::PluginContext *)
    : ViewWidget(), _ui(new Ui::TableViewWidget), propertiesEditor(nullptr), _model(nullptr),
      isNewGraph(false), filteringColumns(false), previousGraph(nullptr), minFontSize(-1) {}

TableView::~TableView() {
  delete _ui;
}

#define NODES_DISPLAYED (_ui->eltTypeCombo->currentIndex() == 0)
#define EDGES_DISPLAYED (_ui->eltTypeCombo->currentIndex() == 1)

tlp::BooleanProperty *TableView::getFilteringProperty() const {
  GraphPropertiesModel<BooleanProperty> *model =
      static_cast<GraphPropertiesModel<BooleanProperty> *>(_ui->filteringPropertyCombo->model());
  PropertyInterface *pi = model
                              ->data(model->index(_ui->filteringPropertyCombo->currentIndex(), 0),
                                     TulipModel::PropertyRole)
                              .value<PropertyInterface *>();
  return pi ? static_cast<BooleanProperty *>(pi) : nullptr;
}

bool TableView::hasEffectiveFiltering() {
  GraphSortFilterProxyModel *sortModel =
      static_cast<GraphSortFilterProxyModel *>(_ui->table->model());

  return sortModel->rowCount() != sortModel->sourceModel()->rowCount();
}

tlp::DataSet TableView::state() const {
  DataSet data;
  data.set("show_nodes", NODES_DISPLAYED);
  data.set("show_edges", EDGES_DISPLAYED);

  BooleanProperty *pi = getFilteringProperty();

  if (pi != nullptr)
    data.set("filtering_property", pi->getName());

  return data;
}

void TableView::setState(const tlp::DataSet &data) {
  bool showNodes = true;
  std::string filterPropertyName = "";
  data.get<bool>("show_nodes", showNodes);

  _ui->eltTypeCombo->setCurrentIndex(showNodes ? 0 : 1);

  if (data.exists("filtering_property"))
    data.get<std::string>("filtering_property", filterPropertyName);

  GraphPropertiesModel<BooleanProperty> *model =
      static_cast<GraphPropertiesModel<BooleanProperty> *>(_ui->filteringPropertyCombo->model());
  int r = 0;

  if (!filterPropertyName.empty())
    r = model->rowOf(model->graph()->getProperty<BooleanProperty>(filterPropertyName));

  if (r < 0)
    r = 0;

  _ui->filteringPropertyCombo->setCurrentIndex(r);
}

bool TableView::eventFilter(QObject *obj, QEvent *event) {
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
  } else {
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
}

void TableView::setZoomLevel(int level) {
  int fs = (level * minFontSize) / 100;
  _ui->table->setStyleSheet(
      QString("QTableView { font-size: %1pt; } QHeaderView::section:horizontal { font: bold; "
              "font-size: %2pt; margin-bottom: 5px; margin-left: 12px; margin-right: 12px;}")
          .arg(fs)
          .arg(fs - 1));
}

void TableView::showHideTableSettings() {
  bool expand = _ui->tableSettingsFrame->isVisible();
  if (expand)
    _ui->tableSettingsFrame->hide();
  else
    _ui->tableSettingsFrame->show();
}

void TableView::setupWidget() {
  // install this as event filter
  // for automatic resizing of the viewport
  graphicsView()->viewport()->parentWidget()->installEventFilter(this);
  QWidget *centralWidget = new QWidget();
  Perspective::setStyleSheet(centralWidget);
  _ui->setupUi(centralWidget);
  activateTooltipAndUrlManager(_ui->table->viewport());
  // no need to display standard View context menu
  setShowContextMenu(false);
  setCentralWidget(centralWidget);

  propertiesEditor =
      new PropertiesEditor(static_cast<QGraphicsProxyWidget *>(centralItem())->widget());

  connect(propertiesEditor, SIGNAL(propertyVisibilityChanged(tlp::PropertyInterface *, bool)), this,
          SLOT(setPropertyVisible(tlp::PropertyInterface *, bool)));
  connect(propertiesEditor, SIGNAL(mapToGraphSelection()), this, SLOT(mapToGraphSelection()));

  _ui->table->setItemDelegate(new GraphTableItemDelegate(_ui->table));
  _ui->table->horizontalHeader()->setSectionsMovable(true);
  _ui->table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_ui->table->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this,
          SLOT(showHorizontalHeaderCustomContextMenu(const QPoint &)));
  connect(_ui->table, SIGNAL(customContextMenuRequested(const QPoint &)),
          SLOT(showCustomContextMenu(const QPoint &)));
  connect(_ui->zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setZoomLevel(int)));
  minFontSize = _ui->table->font().pointSize();
  connect(_ui->filterEdit, SIGNAL(returnPressed()), this, SLOT(filterChanged()));
  connect(_ui->filtercase, SIGNAL(stateChanged(int)), this, SLOT(filterChanged()));

  _ui->eltTypeCombo->addItem("Nodes");
  _ui->eltTypeCombo->addItem("Edges");
  _ui->eltTypeCombo->setCurrentIndex(0);
  connect(_ui->eltTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(readSettings()));
  connect(_ui->filteringPropertyCombo, SIGNAL(currentIndexChanged(int)), this,
          SLOT(readSettings()));
  _ui->valueMatchCombo->addItem("matching");
  _ui->valueMatchCombo->addItem("like");
  _ui->valueMatchCombo->setCurrentIndex(0);
  connect(_ui->valueMatchCombo, SIGNAL(currentIndexChanged(int)), this,
          SLOT(clearValueMatchFilter()));
  _ui->columnMatchCombo->addItem("matching");
  _ui->columnMatchCombo->addItem("like");
  _ui->columnMatchCombo->setCurrentIndex(0);
  connect(_ui->columnMatchCombo, SIGNAL(currentIndexChanged(int)), this,
          SLOT(clearColumnMatchFilter()));
  // use a push button instead of a combobox (see matchPropertyCombo)
  // waiting for a fix for combobox in QGraphicsItem
  connect(_ui->matchPropertyButton, SIGNAL(pressed()), this, SLOT(setMatchProperty()));
  // columns/properties filtering
  filteringColumns = false;
  connect(_ui->columnsFilterEdit, SIGNAL(returnPressed()), this, SLOT(setColumnsFilter()));
  connect(_ui->columnsfiltercase, SIGNAL(stateChanged(int)), this, SLOT(setColumnsFilterCase()));
  connect(propertiesEditor->getPropertiesFilterEdit(), SIGNAL(textChanged(QString)), this,
          SLOT(setPropertiesFilter(QString)));
  connect(_ui->tableSettingsButton, SIGNAL(clicked()), this, SLOT(showHideTableSettings()));
}

QList<QWidget *> TableView::configurationWidgets() const {
  return QList<QWidget *>() << propertiesEditor;
}

void TableView::graphChanged(tlp::Graph *g) {
  isNewGraph = true;
  QSet<QString> visibleProperties;

  if (g && propertiesEditor->getGraph() &&
      (g->getRoot() == propertiesEditor->getGraph()->getRoot())) {
    for (auto pi : propertiesEditor->visibleProperties()) {
      visibleProperties.insert(tlpStringToQString(pi->getName()));
    }
  }

  GraphPropertiesModel<BooleanProperty> *model = new GraphPropertiesModel<BooleanProperty>(
      "no selection", g, false, _ui->filteringPropertyCombo);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(0);

  propertiesEditor->setGraph(g);

  _ui->table->horizontalHeader()->show();
  _ui->table->verticalHeader()->show();

  // Show all the properties
  if (_model != nullptr) {
    for (int i = 0; i < _model->columnCount(); ++i) {
      QString propName = _model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
      // a property is visible only if it was previously visible
      bool checked = !visibleProperties.isEmpty() ? visibleProperties.contains(propName) : true;

      // unless the property did not exist in the previous graph
      if (previousGraph && !previousGraph->existProperty(QStringToTlpString(propName)))
        checked = true;

      propertiesEditor->setPropertyChecked(propName, checked);
    }
  }

  previousGraph = g;
  isNewGraph = false;
}

void TableView::graphDeleted(Graph *ancestor) {
  // if the current graph is deleted
  // just inform the WorkspacePanel
  // that we can display its ancestor instead
  assert(ancestor == nullptr || graph()->getSuperGraph() == ancestor);

  if (ancestor)
    emit graphSet(ancestor);
  else {
    setGraph(nullptr);
    readSettings();
  }
}

void TableView::readSettings() {
  if (isNewGraph ||
      ((_ui->eltTypeCombo->currentIndex() == 0) &&
       dynamic_cast<NodesGraphModel *>(_model) == nullptr) ||
      ((_ui->eltTypeCombo->currentIndex() == 1) &&
       dynamic_cast<EdgesGraphModel *>(_model) == nullptr)) {
    _ui->table->setModel(nullptr);

    delete _model;

    if (_ui->eltTypeCombo->currentIndex() == 0) {
      _ui->label->setText("Nodes");
      _model = new NodesGraphModel(_ui->table);
    } else {
      _ui->label->setText("Edges");
      _model = new EdgesGraphModel(_ui->table);
    }

    _model->setGraph(graph());
    GraphSortFilterProxyModel *sortModel = new GraphSortFilterProxyModel(_ui->table);
    sortModel->setSourceModel(_model);
    _ui->table->setModel(sortModel);
    connect(_model, SIGNAL(columnsInserted(QModelIndex, int, int)), this,
            SLOT(columnsInserted(QModelIndex, int, int)));
    connect(_model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this,
            SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
    filterChanged();
  }

  GraphSortFilterProxyModel *sortModel =
      static_cast<GraphSortFilterProxyModel *>(_ui->table->model());

  sortModel->setFilterProperty(getFilteringProperty());

  QSet<tlp::PropertyInterface *> visibleProperties = propertiesEditor->visibleProperties();

  for (int i = 0; i < _model->columnCount(); ++i) {
    PropertyInterface *pi = _model->headerData(i, Qt::Horizontal, TulipModel::PropertyRole)
                                .value<tlp::PropertyInterface *>();

    if (!visibleProperties.contains(pi))
      _ui->table->setColumnHidden(i, true);
  }

  // reset columns filtering
  _ui->columnsFilterEdit->setText("");
}

void TableView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
  QAbstractItemModel *model = static_cast<QAbstractItemModel *>(sender());

  for (int i = topLeft.row(); i <= bottomRight.row(); ++i) {
    PropertyInterface *pi =
        model->headerData(topLeft.column(), Qt::Horizontal, TulipModel::PropertyRole)
            .value<PropertyInterface *>();

    if (pi->getTypename() == "string" && pi->getName() != "viewTexture" &&
        pi->getName() != "viewFont")
      _ui->table->resizeRowToContents(i);
  }
}

void TableView::columnsInserted(const QModelIndex &, int start, int end) {
  QAbstractItemModel *model = static_cast<QAbstractItemModel *>(sender());

  for (int c = start; c <= end; c++) {
    PropertyInterface *pi =
        model->headerData(c, Qt::Horizontal, TulipModel::PropertyRole).value<PropertyInterface *>();
    setPropertyVisible(pi, false);
  }
}

void TableView::setPropertyVisible(PropertyInterface *pi, bool v) {
  if (_model == nullptr)
    return;

  QString propName = tlpStringToQString(pi->getName());

  for (int i = 0; i < _model->columnCount(); ++i) {
    if (_model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() == propName) {
      _ui->table->horizontalHeader()->setSectionHidden(i, !v);
    }
  }

  if (_ui->matchPropertyButton->text() == propName)
    // set to Any
    _ui->matchPropertyButton->setText("Any");

  // Hide table if no more column is displayed
  bool visible = false;

  for (int i = 0; i < _model->columnCount(); ++i) {
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

  for (auto pi : propertiesEditor->visibleProperties()) {
    QString propName = tlpStringToQString(pi->getName());
    int i = 0;

    for (; i < props.size(); ++i) {
      if (props[i] > propName)
        break;
    }

    props.insert(i, propName);
  }

  QMenu menu;
  QAction *action = menu.addAction("-- Any --");
  menu.setActiveAction(action);

  for (const QString &prop : props) {
    if (_ui->matchPropertyButton->text() == prop) {
      action = menu.addAction(prop);
      menu.setActiveAction(action);
    } else
      menu.addAction(prop);
  }

  QPalette palette = QComboBox().palette();

  // set a combo like stylesheet
  menu.setStyleSheet(QString("QMenu::item {border-image: none; border-width: 4; padding: 0px 6px; "
                             "font-size: 12px; color: %1; background-color: %2;} "
                             "QMenu::item:selected {color: %3; background-color: %4}")
                         .arg(palette.color(QPalette::Active, QPalette::Text).name())
                         .arg(palette.color(QPalette::Active, QPalette::Base).name())
                         .arg(palette.color(QPalette::Active, QPalette::HighlightedText).name())
                         .arg(palette.color(QPalette::Active, QPalette::Highlight).name()));

  // compute a combo like position
  // to popup the menu
  QWidget *pViewport = QApplication::widgetAt(QCursor::pos());
  QWidget *pView = pViewport->parentWidget();
  QGraphicsView *pGraphicsView = static_cast<QGraphicsView *>(pView);
  QGraphicsItem *pGraphicsItem =
      pGraphicsView->items(pViewport->mapFromGlobal(QCursor::pos())).first();
  QPoint popupPos = pGraphicsView->mapToGlobal(pGraphicsView->mapFromScene(
      pGraphicsItem->mapToScene(static_cast<QGraphicsProxyWidget *>(pGraphicsItem)
                                    ->subWidgetRect(_ui->matchPropertyButton)
                                    .bottomLeft())));

  action = menu.exec(popupPos);

  if (action) {
    if (action->text() == "-- Any --")
      _ui->matchPropertyButton->setText("Any");
    else
      _ui->matchPropertyButton->setText(action->text());
  }
}

void TableView::setColumnsFilterCase() {
  if (filteringColumns)
    return;

  filteringColumns = true;
  propertiesEditor->setCaseSensitive(_ui->columnsfiltercase->isChecked() ? Qt::CaseSensitive
                                                                         : Qt::CaseInsensitive);
  filteringColumns = false;
}

void TableView::setColumnsFilter() {
  if (filteringColumns)
    return;

  filteringColumns = true;
  propertiesEditor->getPropertiesMatchButton()->setText(
      _ui->columnMatchCombo->currentIndex() ? "like" : "matching");
  propertiesEditor->getPropertiesFilterEdit()->setText(_ui->columnsFilterEdit->text());
  filteringColumns = false;
}

void TableView::setPropertiesFilter(const QString &text) {
  if (filteringColumns)
    return;

  filteringColumns = true;
  _ui->columnMatchCombo->setCurrentIndex(
      propertiesEditor->getPropertiesMatchButton()->text() == "matching" ? 0 : 1);
  _ui->columnsFilterEdit->setText(text);
  filteringColumns = false;
}

void TableView::clearColumnMatchFilter() {
  _ui->columnsFilterEdit->setText("");
  QString tooltip;
  if (_ui->columnMatchCombo->currentIndex()) {
    tooltip = QString("Only show the columns (properties) whose name\nis like the given pattern "
                      "(sql like pattern).");
    _ui->columnsFilterEdit->setPlaceholderText("a sql like pattern");
  } else {
    tooltip = QString(
        "Only show the columns (properties) whose name\nmatches the given regular expression.");
    _ui->columnsFilterEdit->setPlaceholderText("a regular expression");
  }
  _ui->labelColumnMatch->setToolTip(tooltip);
  tooltip += "\nPress 'Return' to validate.";
  _ui->columnsFilterEdit->setToolTip(tooltip);
}

void TableView::clearValueMatchFilter() {
  _ui->filterEdit->setText("");
  QString tooltip;
  if (_ui->valueMatchCombo->currentIndex()) {
    tooltip = QString("Only show the rows (nodes or edges) whose\nthe chosen column value is like "
                      "the given pattern (sql like pattern).");
    _ui->filterEdit->setPlaceholderText("a sql like pattern");
  } else {
    tooltip = QString("Only show the rows (nodes or edges) whose\nthe chosen column value matches "
                      "the given regular expression.");
    _ui->filterEdit->setPlaceholderText("a regular expression");
  }
  _ui->matchPropertyButton->setToolTip(tooltip);
  _ui->valueMatchCombo->setToolTip(tooltip);
  tooltip += "\nPress 'Return' to validate.";
  _ui->filterEdit->setToolTip(tooltip);
}

void TableView::filterChanged() {
  QString filter = _ui->filterEdit->text();
  if (_ui->valueMatchCombo->currentIndex() == 1)
    // convert the sql like filter
    convertLikeFilter(filter);

  GraphSortFilterProxyModel *sortModel =
      static_cast<GraphSortFilterProxyModel *>(_ui->table->model());
  QVector<PropertyInterface *> props;

  Graph *g = graph();
  auto matchProperty = QStringToTlpString(_ui->matchPropertyButton->text());

  if (matchProperty != "Any") {
    // a visible column
    // check if it exists
    if (g->existProperty(matchProperty))
      props += g->getProperty(matchProperty);
    else {
      // property does not exist
      // reset filtering
      _ui->matchPropertyButton->setText("Any");
      _ui->filterEdit->setText("");
      filter.clear();
    }
  }
  if (props.empty()) {
    for (int i = 0; i < _model->columnCount(); ++i) {
      if (!_ui->table->horizontalHeader()->isSectionHidden(i))
        props += _model->headerData(i, Qt::Horizontal, TulipModel::PropertyRole)
                     .value<PropertyInterface *>();
    }
  }

  sortModel->setProperties(props);
  sortModel->setFilterRegExp(
      QRegExp(filter, _ui->filtercase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));
}

void TableView::mapToGraphSelection() {
  BooleanProperty *out = graph()->getProperty<BooleanProperty>("viewSelection");

  if (NODES_DISPLAYED) {
    out->setAllNodeValue(false);
    QItemSelectionModel *selectionModel = _ui->table->selectionModel();

    for (const QModelIndex &idx : selectionModel->selectedRows()) {
      node n(idx.data(TulipModel::ElementIdRole).toUInt());
      out->setNodeValue(n, true);
    }
  } else {
    out->setAllEdgeValue(false);
    QItemSelectionModel *selectionModel = _ui->table->selectionModel();

    for (const QModelIndex &idx : selectionModel->selectedRows()) {
      edge e(idx.data(TulipModel::ElementIdRole).toUInt());
      out->setEdgeValue(e, true);
    }
  }
}

void TableView::delHighlightedRows() {
  Graph *g = graph();
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  for (const auto &idx : rows) {
    if (NODES_DISPLAYED)
      g->delNode(node(idx.data(TulipModel::ElementIdRole).toUInt()));
    else
      g->delEdge(edge(idx.data(TulipModel::ElementIdRole).toUInt()));
  }
}

void TableView::toggleHighlightedRows() {
  Graph *g = graph();
  BooleanProperty *selection = g->getProperty<BooleanProperty>("viewSelection");
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  GraphSortFilterProxyModel *sortModel =
      static_cast<GraphSortFilterProxyModel *>(_ui->table->model());

  if (sortModel->filterProperty() == selection)
    selection->removeListener(sortModel);

  for (const auto &idx : rows) {
    if (NODES_DISPLAYED) {
      node n(idx.data(TulipModel::ElementIdRole).toUInt());
      selection->setNodeValue(n, !selection->getNodeValue(n));
    } else {
      edge e(idx.data(TulipModel::ElementIdRole).toUInt());
      selection->setEdgeValue(e, !selection->getEdgeValue(e));
    }
  }

  if (sortModel->filterProperty() == selection)
    selection->addListener(sortModel);
}

void TableView::selectHighlightedRows() {
  Graph *g = graph();
  BooleanProperty *selection = g->getProperty<BooleanProperty>("viewSelection");
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  GraphSortFilterProxyModel *sortModel =
      static_cast<GraphSortFilterProxyModel *>(_ui->table->model());

  if (sortModel->filterProperty() == selection)
    selection->removeListener(sortModel);

  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  for (const auto &idx : rows) {
    if (NODES_DISPLAYED)
      selection->setNodeValue(node(idx.data(TulipModel::ElementIdRole).toUInt()), true);
    else
      selection->setEdgeValue(edge(idx.data(TulipModel::ElementIdRole).toUInt()), true);
  }

  if (sortModel->filterProperty() == selection)
    selection->addListener(sortModel);
}

bool TableView::setAllHighlightedRows(PropertyInterface *prop) {
  Graph *g = graph();
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();
  uint eltId = UINT_MAX;
  if (rows.size() == 1)
    eltId = rows[0].data(TulipModel::ElementIdRole).toUInt();

  QVariant val = TulipItemDelegate::showEditorDialog(
      NODES_DISPLAYED ? NODE : EDGE, prop, g,
      static_cast<TulipItemDelegate *>(_ui->table->itemDelegate()),
      graphicsView()->viewport()->parentWidget(), eltId);

  // Check if edition has been cancelled
  if (!val.isValid())
    return false;

  for (const auto &idx : rows) {
    if (NODES_DISPLAYED)
      GraphModel::setNodeValue(idx.data(TulipModel::ElementIdRole).toUInt(), prop, val);
    else
      GraphModel::setEdgeValue(idx.data(TulipModel::ElementIdRole).toUInt(), prop, val);
  }

  return true;
}

bool TableView::setCurrentValue(PropertyInterface *prop, unsigned int eltId) {
  QVariant val = TulipItemDelegate::showEditorDialog(
      NODES_DISPLAYED ? NODE : EDGE, prop, graph(),
      static_cast<TulipItemDelegate *>(_ui->table->itemDelegate()),
      graphicsView()->viewport()->parentWidget(), eltId);

  // Check if edition has been cancelled
  if (!val.isValid())
    return false;

  if (NODES_DISPLAYED)
    GraphModel::setNodeValue(eltId, prop, val);
  else
    GraphModel::setEdgeValue(eltId, prop, val);

  return true;
}

void TableView::setLabelsOfHighlightedRows(PropertyInterface *prop) {
  QModelIndexList rows = _ui->table->selectionModel()->selectedRows();

  StringProperty *label = graph()->getProperty<StringProperty>("viewLabel");

  for (const auto &idx : rows) {
    if (NODES_DISPLAYED) {
      node n(idx.data(TulipModel::ElementIdRole).toUInt());
      label->setNodeStringValue(n, prop->getNodeStringValue(n));
    } else {
      edge e(idx.data(TulipModel::ElementIdRole).toUInt());
      label->setEdgeStringValue(e, prop->getEdgeStringValue(e));
    }
  }
}

bool TableView::getNodeOrEdgeAtViewportPos(int x, int y, node &n, edge &e) const {
  QPoint pos = graphicsView()->viewport()->mapToGlobal(QPoint(x, y));
  if (pos.x() < propertiesEditor->mapToGlobal(QPoint(0, 0)).x()) {
    pos = graphicsView()->viewport()->mapToGlobal(
              QPoint(0, y - _ui->table->horizontalHeader()->height())) -
          _ui->table->mapToGlobal(QPoint(0, 0));
    if (_ui->table->rowAt(pos.y()) >= 0) {
      QModelIndex idx = _ui->table->indexAt(pos);
      unsigned int eltId = idx.data(TulipModel::ElementIdRole).toUInt();
      if (NODES_DISPLAYED) {
        n = node(eltId);
        return n.isValid();
      } else {
        e = edge(eltId);
        return e.isValid();
      }
    }
  }
  return false;
}

void TableView::showCustomContextMenu(const QPoint &pos) {
  if (_ui->table->model()->rowCount() == 0)
    return;

  QModelIndex idx = _ui->table->indexAt(pos);
  unsigned int eltId = idx.data(TulipModel::ElementIdRole).toUInt();

  QString eltsName(NODES_DISPLAYED ? "nodes" : "edges");
  QString eltName(NODES_DISPLAYED ? "node" : "edge");
  std::string propName = QStringToTlpString(
      _model->headerData(idx.column(), Qt::Horizontal, Qt::DisplayRole).toString());

  if (propName.empty())
    return;

  PropertyInterface *prop = graph()->getProperty(propName);
  bool propIsInherited = prop->getGraph() != graph();

  QModelIndexList highlightedRows = _ui->table->selectionModel()->selectedRows();

  QMenu contextMenu;
  Perspective::redirectStatusTipOfMenu(&contextMenu);
  // the style sheet below allows to display disabled items
  // as "title" items in the "mainMenu"
  contextMenu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; "
                            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, "
                            "stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
  // so it is the "mainMenu"
  contextMenu.setProperty("mainMenu", true);

  QAction *action = contextMenu.addAction(tlpStringToQString(propName));
  action->setEnabled(false);
  contextMenu.addSeparator();

  QMenu *subMenu = contextMenu.addMenu("Set value(s) of ");
  QAction *setAll = nullptr;
  if (propIsInherited) {
    setAll =
        subMenu->addAction(QString("All ") + eltsName + OF_PROPERTY + " to a new default value");
    setAll->setToolTip(QString("Choose a new ") + eltsName +
                       " default value to reset the values of all " + eltsName + OF_PROPERTY);
  }
  QAction *setAllGraph = subMenu->addAction(QString("All ") + eltsName + OF_GRAPH);
  setAllGraph->setToolTip(QString("Choose a value to be assigned to all the existing ") + eltsName +
                          OF_GRAPH);

  QAction *selectedSetAll = subMenu->addAction(QString("Selected ") + eltsName + OF_GRAPH);
  selectedSetAll->setToolTip(QString("Choose a value to be assigned to the selected ") + eltsName +
                             OF_GRAPH);

  QAction *highlightedSetAll;
  if (highlightedRows.size() > 1) {
    highlightedSetAll = subMenu->addAction(QString("Rows highlighted ") + eltsName);
    highlightedSetAll->setToolTip(QString("Choose a value to be assigned to the ") + eltsName +
                                  " displayed in the currently highlighted row(s)");
  } else {
    highlightedSetAll = subMenu->addAction(QString("%1 #%2").arg(eltName).arg(eltId));
    highlightedSetAll->setToolTip(
        QString("Choose a value for to be assigned to the current property of %1 #%2")
            .arg(eltName)
            .arg(eltId));
  }

  QAction *toLabels, *selectedToLabels, *highlightedToLabels;
  toLabels = selectedToLabels = highlightedToLabels = nullptr;
  if (propName != "viewLabel") {
    subMenu = contextMenu.addMenu("To label(s) of ");
    toLabels = subMenu->addAction("All " + eltsName + OF_GRAPH);
    toLabels->setToolTip(QString("Set the values of the current property as labels of the ") +
                         eltsName + OF_GRAPH);
    selectedToLabels = subMenu->addAction(QString("Selected ") + eltsName + OF_GRAPH);
    selectedToLabels->setToolTip(
        QString("Set the values of the current property as labels of the selected ") + eltsName +
        OF_GRAPH);

    if (highlightedRows.size() > 1) {
      highlightedToLabels = subMenu->addAction(QString("Rows highlighted ") + eltsName);
      highlightedToLabels->setToolTip(
          QString("Set the values of the current property as labels of the ") + eltsName +
          " displayed in the currently highlighted row(s)");
    } else {
      highlightedToLabels = subMenu->addAction(QString("%1 #%2").arg(eltName).arg(eltId));
      highlightedToLabels->setToolTip(
          QString("Set the value of the current property as label of %1 #%2")
              .arg(eltName)
              .arg(eltId));
    }
  }

  contextMenu.addSeparator();
  action =
      contextMenu.addAction(highlightedRows.size() > 1 ? (QString("Rows highlighted ") + eltsName)
                                                       : QString("%1 #%2").arg(eltName).arg(eltId));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction *toggleAction = contextMenu.addAction("Toggle selection");
  toggleAction->setToolTip(QString("Invert the selection of the ") + action->text() +
                           ": deselect if selected or select if not selected");
  QAction *selectAction = contextMenu.addAction("Select");
  selectAction->setToolTip(QString("Set the selection with the ") + action->text());
  QAction *deleteAction = contextMenu.addAction("Delete");
  deleteAction->setToolTip(QString("Delete the ") + action->text());
  QAction *setValueAction =
      contextMenu.addAction(QString((highlightedRows.size() > 1) ? "Set values" : "Set value"));
  setValueAction->setToolTip(highlightedSetAll->toolTip());

  contextMenu.addSeparator();
  View::fillContextMenu(&contextMenu, QPointF());

  // display the menu with the mouse inside to allow
  // keyboard navigation
  action = contextMenu.exec(QCursor::pos() - QPoint(5, 5));

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

  if ((action == highlightedSetAll) || (action == setValueAction)) {
    // set values for elts corresponding to highlighted rows
    if (!((highlightedRows.size() > 1) ? setAllHighlightedRows(prop)
                                       : setCurrentValue(prop, eltId)))
      // cancelled so undo
      graph()->pop();

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
    propertiesEditor->toLabels(prop, nodes, !nodes, true);
    return;
  }

  if (action == highlightedToLabels) {
    // set values as labels for elts corresponding to highlighted rows
    setLabelsOfHighlightedRows(prop);
    return;
  }
}

void TableView::showHorizontalHeaderCustomContextMenu(const QPoint &pos) {
  if (_ui->table->model()->columnCount() == 0)
    return;

  QModelIndex idx = _ui->table->indexAt(pos);

  QString eltsName(NODES_DISPLAYED ? "nodes" : "edges");
  std::string propName = QStringToTlpString(
      _model->headerData(idx.column(), Qt::Horizontal, Qt::DisplayRole).toString());

  if (propName.empty())
    return;

  PropertyInterface *prop = graph()->getProperty(propName);
  bool propIsInherited = prop->getGraph() != graph();

  QModelIndexList highlightedRows = _ui->table->selectionModel()->selectedRows();

  QMenu contextMenu;
  Perspective::redirectStatusTipOfMenu(&contextMenu);
  // the style sheet below allows to display disabled items
  // as "title" items in the "mainMenu"
  contextMenu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; "
                            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, "
                            "stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
  // so it is the "mainMenu"
  contextMenu.setProperty("mainMenu", true);

  QAction *action = contextMenu.addAction(tlpStringToQString(propName));
  action->setEnabled(false);
  contextMenu.addSeparator();
  QAction *addProp =
      contextMenu.addAction(QIcon(":/tulip/gui/icons/64/list-add.png"), "Add new property");
  addProp->setToolTip("Display a dialog box to create a new local property of the current graph");
  QAction *copyProp = contextMenu.addAction("Copy");
  copyProp->setToolTip(QString("Display a dialog box to copy the values of <b>") + action->text() +
                       "</b> into a new or existing property of the same type");
  QAction *deleteProp = nullptr;

  if (!Perspective::instance()->isReservedPropertyName(propName.c_str()) ||
      // Enable deletion of reserved properties when on a subgraph and that properties are local
      (graph() != graph()->getRoot() && graph()->existLocalProperty(propName))) {
    deleteProp = contextMenu.addAction("Delete");
    deleteProp->setToolTip(QString("Delete the property <b>") + action->text() + "</b>");
  }

  QAction *renameProp = nullptr;

  if (!Perspective::instance()->isReservedPropertyName(propName.c_str())) {
    renameProp = contextMenu.addAction("Rename");
    renameProp->setToolTip(QString("Rename the property <b>") + action->text() + "</b>");
  }

  contextMenu.addSeparator();

  QMenu *subMenu = contextMenu.addMenu("Set values of ");
  QAction *nodesSetAll = nullptr;
  QAction *edgesSetAll = nullptr;
  if (propIsInherited) {
    nodesSetAll =
        subMenu->addAction(QString("All nodes") + OF_PROPERTY + " to a new default value");
    nodesSetAll->setToolTip(
        QString("Choose a new node default value to reset the values of all nodes") + OF_PROPERTY);
    edgesSetAll =
        subMenu->addAction(QString("All edges") + OF_PROPERTY + " to a new default value");
    edgesSetAll->setToolTip(
        QString("Choose a new edge default value to reset the values of all edges ") + OF_PROPERTY);
  }
  QAction *nodesSetAllGraph = subMenu->addAction(QString("All nodes") + OF_GRAPH);
  nodesSetAllGraph->setToolTip(QString("Choose a value to be assigned to all the existing nodes") +
                               OF_GRAPH);
  QAction *edgesSetAllGraph = subMenu->addAction(QString("All edges") + OF_GRAPH);
  edgesSetAllGraph->setToolTip(QString("Choose a value to be assigned to all the existing edges") +
                               OF_GRAPH);
  QAction *nodesSelectedSetAll = subMenu->addAction(QString("Selected nodes") + OF_GRAPH);
  nodesSelectedSetAll->setToolTip(QString("Choose a value to be assigned to the selected nodes") +
                                  OF_GRAPH);
  QAction *edgesSelectedSetAll = subMenu->addAction(QString("Selected edges") + OF_GRAPH);
  edgesSelectedSetAll->setToolTip(QString("Choose a value to be assigned to the selected edges") +
                                  OF_GRAPH);
  QAction *highlightedSetAll = nullptr;

  if (!highlightedRows.isEmpty()) {
    highlightedSetAll = subMenu->addAction(
        (QString("Rows highlighted ") + eltsName) +
        (highlightedRows.size() > 1
             ? ""
             : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)")
                   .arg(highlightedRows[0].data(TulipModel::ElementIdRole).toUInt())));
    highlightedSetAll->setToolTip(QString("Choose a value to be assigned to the ") + eltsName +
                                  " displayed in the currently highlighted row(s)");
  }

  QAction *toLabels = nullptr;
  QAction *nodesToLabels = nullptr;
  QAction *edgesToLabels = nullptr;
  QAction *selectedToLabels = nullptr;
  QAction *nodesSelectedToLabels = nullptr;
  QAction *edgesSelectedToLabels = nullptr;
  QAction *highlightedToLabels = nullptr;

  if (propName != "viewLabel") {
    subMenu = contextMenu.addMenu("To labels of ");
    toLabels = subMenu->addAction(QString("All elements") + OF_GRAPH);
    toLabels->setToolTip(
        QString("Set the values of the current property as labels of all elements") + OF_GRAPH);
    nodesToLabels = subMenu->addAction(QString("All nodes") + OF_GRAPH);
    nodesToLabels->setToolTip(
        QString("Set the values of the current property as labels of the nodes") + OF_GRAPH);
    edgesToLabels = subMenu->addAction(QString("All edges") + OF_GRAPH);
    edgesToLabels->setToolTip(
        QString("Set the values of the current property as labels of the edges") + OF_GRAPH);
    selectedToLabels = subMenu->addAction(QString("Selected elements") + OF_GRAPH);
    selectedToLabels->setToolTip(
        QString("Set the values of the current property as labels of the selected elements") +
        OF_GRAPH);
    nodesSelectedToLabels = subMenu->addAction("Selected nodes" + OF_GRAPH);
    nodesSelectedToLabels->setToolTip(
        QString("Set the values of the current property as labels of the selected nodes") +
        OF_GRAPH);
    edgesSelectedToLabels = subMenu->addAction("Selected edges" + OF_GRAPH);
    edgesSelectedToLabels->setToolTip(
        QString("Set the values of the current property as labels of the selected edges") +
        OF_GRAPH);

    if (!highlightedRows.isEmpty()) {
      highlightedToLabels = subMenu->addAction(
          (QString("Rows highlighted ") + eltsName) +
          (highlightedRows.size() > 1
               ? ""
               : QString(NODES_DISPLAYED ? " (Node #%1)" : " (Edge #%1)")
                     .arg(highlightedRows[0].data(TulipModel::ElementIdRole).toUInt())));
      highlightedToLabels->setToolTip(
          QString("Set the values of the current property as labels of the ") + eltsName +
          " displayed in the currently highlighted row(s)");
    }
  }

  contextMenu.addSeparator();
  QAction *sortById = contextMenu.addAction("Sort the rows by id");
  sortById->setToolTip(QString("Display the rows in ordering of the id of the ") + eltsName);

  // display the menu with the mouse inside to give it the focus
  // and thus allow keyboard navigation
  action = contextMenu.exec(QCursor::pos() - QPoint(5, 5));

  if (!action)
    return;

  if (action == sortById) {
    if (_ui->table->horizontalHeader()->sortIndicatorSection() != -1) {
      _ui->table->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
      GraphSortFilterProxyModel *sortModel =
          static_cast<GraphSortFilterProxyModel *>(_ui->table->model());
      QAbstractItemModel *model = sortModel->sourceModel();
      sortModel->setSourceModel(nullptr);
      sortModel->setSourceModel(model);
      sortModel->setFilterProperty(getFilteringProperty());

      QSet<tlp::PropertyInterface *> visibleProperties = propertiesEditor->visibleProperties();

      for (int i = 0; i < model->columnCount(); ++i) {
        PropertyInterface *pi = _model->headerData(i, Qt::Horizontal, TulipModel::PropertyRole)
                                    .value<tlp::PropertyInterface *>();

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
                                         Perspective::instance()->mainWindow()) == nullptr)
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
    if (PropertyCreationDialog::createNewProperty(graph(), Perspective::instance()->mainWindow(),
                                                  prop->getTypename()) == nullptr)
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
