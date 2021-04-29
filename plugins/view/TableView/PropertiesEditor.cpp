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

#include "PropertiesEditor.h"
#include "ui_PropertiesEditor.h"

#include <QSortFilterProxyModel>
#include <QMenu>
#include <QDialogButtonBox>
#include <QDialog>
#include <QCursor>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>

#include <tulip/Perspective.h>
#include <tulip/GraphModel.h>
#include <tulip/TulipItemEditorCreators.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/PropertyCreationDialog.h>
#include <tulip/RenamePropertyDialog.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TlpQtTools.h>

Q_DECLARE_METATYPE(Qt::CheckState)

using namespace tlp;

PropertiesEditor::PropertiesEditor(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PropertiesEditor), _contextProperty(nullptr), _graph(nullptr),
      _delegate(new tlp::TulipItemDelegate), _sourceModel(nullptr), filteringProperties(false),
      editorParent(parent), _caseSensitiveSearch(Qt::CaseSensitive) {
  Perspective::setStyleSheet(this);
  _ui->setupUi(this);
  connect(_ui->newButton, SIGNAL(clicked()), this, SLOT(newProperty()));
  // use a push button instead of a combobox
  // waiting for a fix for combobox in QGraphicsItem
  connect(_ui->propMatchButton, SIGNAL(pressed()), this, SLOT(setMatchProperty()));
}

PropertiesEditor::~PropertiesEditor() {
  delete _ui;
  delete _delegate;
  delete _sourceModel;
}

void PropertiesEditor::setCaseSensitive(Qt::CaseSensitivity cs) {
  _caseSensitiveSearch = cs;
  setPropertiesFilter(_ui->propertiesFilterEdit->text());
}

void PropertiesEditor::setGraph(tlp::Graph *g) {
  _graph = g;
  QSortFilterProxyModel *model = new QSortFilterProxyModel(_ui->propertiesTableView);
  delete _sourceModel;
  _sourceModel = new GraphPropertiesModel<PropertyInterface>(g, true);
  model->setSourceModel(_sourceModel);
  model->setFilterCaseSensitivity(Qt::CaseInsensitive);
  // the 3 signal-to-slot connections below ensure the propagation
  // of the displayed properties filtering
  connect(_ui->propertiesFilterEdit, SIGNAL(textChanged(QString)), this,
          SLOT(setPropertiesFilter(QString)));
  connect(model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)), this,
          SLOT(displayedPropertiesRemoved(const QModelIndex &, int, int)));
  connect(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this,
          SLOT(displayedPropertiesInserted(const QModelIndex &, int, int)));
  _ui->propertiesTableView->setModel(model);
  connect(_sourceModel, SIGNAL(checkStateChanged(QModelIndex, Qt::CheckState)), this,
          SLOT(checkStateChanged(QModelIndex, Qt::CheckState)));
  _ui->propertiesTableView->resizeColumnsToContents();
  _ui->propertiesTableView->sortByColumn(0, Qt::AscendingOrder);
  _ui->visualPropertiesCheck->setChecked(true);
}

void PropertiesEditor::setMatchProperty() {
  QMenu menu;
  QAction *action = menu.addAction("matching");
  if (_ui->propMatchButton->text() == "matching")
    menu.setActiveAction(action);
  action = menu.addAction("like");
  if (_ui->propMatchButton->text() == "like")
    menu.setActiveAction(action);

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
                                    ->subWidgetRect(_ui->propMatchButton)
                                    .bottomLeft())));

  action = menu.exec(popupPos);

  if (action) {
    if (action->text() != _ui->propMatchButton->text()) {
      _ui->propMatchButton->setText(action->text());
      _ui->propertiesFilterEdit->setText("");
      QString tooltip;
      if (_ui->propMatchButton->text() == "like") {
        tooltip = QString(
            "Only show the properties whose name\nis like the given pattern (sql like pattern).");
        _ui->propertiesFilterEdit->setPlaceholderText("a sql like pattern");
      } else {
        tooltip =
            QString("Only show the properties whose name\nmatches the given regular expression.");
        _ui->propertiesFilterEdit->setPlaceholderText("a regular expression");
      }
      _ui->propMatchLabel->setToolTip(tooltip);
      // tooltip += "\nPress 'Return' to validate.";
      _ui->propertiesFilterEdit->setToolTip(tooltip);
    }
  }
}

void PropertiesEditor::setPropertiesFilter(QString filter) {
  filteringProperties = true;

  if (_ui->propMatchButton->text() == "like")
    // convert the sql like filter
    convertLikeFilter(filter);

  static_cast<QSortFilterProxyModel *>(_ui->propertiesTableView->model())
      ->setFilterRegExp(QRegExp(filter, _caseSensitiveSearch));
  filteringProperties = false;
}

QLineEdit *PropertiesEditor::getPropertiesFilterEdit() {
  return _ui->propertiesFilterEdit;
}

QPushButton *PropertiesEditor::getPropertiesMatchButton() {
  return _ui->propMatchButton;
}

void PropertiesEditor::showCustomContextMenu(const QPoint &p) {
  _contextProperty = _ui->propertiesTableView->indexAt(p)
                         .data(TulipModel::PropertyRole)
                         .value<PropertyInterface *>();
  _contextPropertyList.clear();

  for (const QModelIndex &sidx : _ui->propertiesTableView->selectionModel()->selectedRows()) {
    _contextPropertyList += sidx.data(TulipModel::PropertyRole).value<PropertyInterface *>();
  }

  if (_contextProperty == nullptr)
    return;

  QString pname = tlpStringToQString(_contextProperty->getName());

  if (pname.length() > 30) {
    pname.truncate(30);
    pname += "...";
  }

  QMenu menu;
  Perspective::redirectStatusTipOfMenu(&menu);
  QAction *action;

  if (_contextPropertyList.size() > 1) {
    bool enabled = true;

    for (auto pi : _contextPropertyList) {
      if (Perspective::instance()->isReservedPropertyName(pi->getName().c_str()) &&
          (_graph == _graph->getRoot() || !_graph->existLocalProperty(pi->getName()))) {
        enabled = false;
        break;
      }
    }

    if (enabled) {
      action = menu.addAction("Delete highlighted properties");
      action->setToolTip("Delete the highlighted properties");
      connect(action, SIGNAL(triggered()), this, SLOT(delProperties()));
      action = menu.addAction("Hide all other properties");
      action->setToolTip("Show only the columns corresponding to the highlighted properties");
      connect(action, SIGNAL(triggered()), this, SLOT(setPropsNotVisibleExcept()));
    }

    menu.exec(QCursor::pos());
  } else {
    // the style sheet below allows to display disabled items
    // as "title" items in the "mainMenu"
    menu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; background-color: "
                       "qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), "
                       "stop:1 rgb(60, 60, 60))}");
    // so it is the "mainMenu"
    menu.setProperty("mainMenu", true);
    menu.addAction(pname)->setEnabled(false);
    menu.addSeparator();
    action = menu.addAction("Hide all other properties");
    action->setToolTip("Show only the column corresponding to this property");
    connect(action, SIGNAL(triggered()), this, SLOT(setPropsNotVisibleExcept()));
    menu.addSeparator();

    action = menu.addAction(QIcon(":/tulip/gui/icons/64/list-add.png"), "Add new property");
    action->setToolTip("Display a dialog box to create a new local property of the current graph");
    connect(action, SIGNAL(triggered()), this, SLOT(newProperty()));

    const std::string &propName = _contextProperty->getName();

    action = menu.addAction("Copy");
    action->setToolTip(QString("Display a dialog box allowing to copy the values of <b>") +
                       propName.c_str() + "</b> into a new or existing property of the same type");
    connect(action, SIGNAL(triggered()), this, SLOT(copyProperty()));

    bool enabled = true;

    if (Perspective::instance()->isReservedPropertyName(propName.c_str())) {
      // Enable deletion of reserved properties when on a subgraph and that properties are local
      if (_graph == _graph->getRoot() || !_graph->existLocalProperty(propName))
        enabled = false;
    }

    if (enabled) {
      action = menu.addAction("Delete");
      action->setToolTip(QString("Delete the property <b>") + propName.c_str() + "</b>");
      connect(action, SIGNAL(triggered()), this, SLOT(delProperty()));
    }

    QAction *rename = nullptr;

    if (!Perspective::instance()->isReservedPropertyName(propName.c_str())) {
      rename = menu.addAction("Rename");
      rename->setToolTip(QString("Rename the property <b>") + propName.c_str() + "</b>");
    }

    menu.addSeparator();

    QMenu *subMenu = menu.addMenu("Set values of");
    QAction *nodesSetAll =
        subMenu->addAction(QString("All nodes") + OF_PROPERTY + " to a new default value");
    nodesSetAll->setToolTip(
        QString("Choose a new node default value to reset the values of all nodes") + OF_PROPERTY);
    QAction *edgesSetAll =
        subMenu->addAction(QString("All edges") + OF_PROPERTY + " to a new default value");
    edgesSetAll->setToolTip(
        QString("Choose a new edge default value to reset the values of all edges ") + OF_PROPERTY);
    QAction *nodesSetAllGraph = subMenu->addAction(QString("All nodes") + OF_GRAPH);
    nodesSetAllGraph->setToolTip(
        QString("Choose a value to be assigned to all the existing nodes") + OF_GRAPH);
    QAction *edgesSetAllGraph = subMenu->addAction("All edges" + OF_GRAPH);
    edgesSetAllGraph->setToolTip(
        QString("Choose a value to be assigned to all the existing edges") + OF_GRAPH);
    QAction *selectedNodesSetAll = subMenu->addAction("Selected nodes" + OF_GRAPH);
    selectedNodesSetAll->setToolTip(QString("Choose a value to be assigned to the selected nodes") +
                                    OF_GRAPH);
    QAction *selectedEdgesSetAll = subMenu->addAction("Selected edges" + OF_GRAPH);
    selectedEdgesSetAll->setToolTip(QString("Choose a value to be assigned to the selected edges") +
                                    OF_GRAPH);

    enabled = (pname != "viewLabel");

    if (enabled) {
      subMenu = menu.addMenu("To labels of");
      action = subMenu->addAction("All elements" + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of all elements") + OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toLabels()));
      action = subMenu->addAction("All nodes" + OF_GRAPH);
      action->setToolTip(QString("Set the values of the current property as labels of the nodes") +
                         OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toNodesLabels()));
      action = subMenu->addAction("All edges" + OF_GRAPH);
      action->setToolTip(QString("Set the values of the current property as labels of the edges") +
                         OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toEdgesLabels()));
      action = subMenu->addAction("All selected elements" + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of the selected elements") +
          OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toSelectedLabels()));
      action = subMenu->addAction("Selected nodes" + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of the selected nodes") +
          OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toSelectedNodesLabels()));
      action = subMenu->addAction("Selected edges" + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of the selected edges") +
          OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toSelectedEdgesLabels()));
    }

    QAction *action = menu.exec(QCursor::pos());

    /*if (action == nodesSetDefault || action == edgesSetDefault) {
      setDefaultValue(_contextProperty, action == nodesSetDefault);
    }

    else*/
    if (action != nullptr) {
      bool result = false;

      _graph->push();

      if (action == nodesSetAll)
        result = setAllValues(_contextProperty, true, false);

      if (action == nodesSetAllGraph)
        result = setAllValues(_contextProperty, true, false, true);

      if (action == edgesSetAll)
        result = setAllValues(_contextProperty, false, false);

      if (action == edgesSetAllGraph)
        result = setAllValues(_contextProperty, false, false, true);

      if (action == selectedNodesSetAll)
        result = setAllValues(_contextProperty, true, true);

      if (action == selectedEdgesSetAll)
        result = setAllValues(_contextProperty, false, true);

      if (action == rename)
        result = renameProperty(_contextProperty);

      if (!result)
        // edition cancelled
        _graph->pop();
    }
  }

  _contextProperty = nullptr;
}

void PropertiesEditor::setPropsVisibility(int state) {
  if (state == Qt::PartiallyChecked)
    return;

  _ui->propsVisibilityCheck->setTristate(false);

  if (state == Qt::Checked) {
    // reset property name filter
    _ui->propertiesFilterEdit->setText(QString());
    // no filter
    static_cast<QSortFilterProxyModel *>(_ui->propertiesTableView->model())
        ->setFilterFixedString("");
  }

  bool showVisualP = _ui->visualPropertiesCheck->isChecked();

  for (int i = 0; i < _sourceModel->rowCount(); ++i) {
    if (_sourceModel->index(i, 0).data().toString().indexOf("view") == 0)
      setPropertyChecked(i, showVisualP);
    else
      _sourceModel->setData(_sourceModel->index(i, 0), state, Qt::CheckStateRole);
  }
}

void PropertiesEditor::setPropsNotVisibleExcept() {
  std::set<std::string> ctxPropNames;

  for (auto pi : _contextPropertyList)
    ctxPropNames.insert(pi->getName());

  for (int i = 0; i < _sourceModel->rowCount(); ++i) {
    setPropertyChecked(i, ctxPropNames.count(QStringToTlpString(
                              _sourceModel->index(i, 0).data().toString())) == 1);
  }

  _ui->propsVisibilityCheck->setTristate(true);
  _ui->propsVisibilityCheck->setCheckState(Qt::PartiallyChecked);
}

void PropertiesEditor::showVisualProperties(bool f) {
  // reset property name filter
  _ui->propertiesFilterEdit->setText(QString());

  static_cast<QSortFilterProxyModel *>(_ui->propertiesTableView->model())->setFilterFixedString("");

  // ensure all visual properties are shown/hidden
  for (int i = 0; i < _sourceModel->rowCount(); ++i) {
    if (_sourceModel->index(i, 0).data().toString().indexOf("view") == 0)
      setPropertyChecked(i, f);
  }
}

// properties inserted when filtering
// are visible according to their CheckState
void PropertiesEditor::displayedPropertiesInserted(const QModelIndex &parent, int start, int end) {
  QSortFilterProxyModel *model = static_cast<QSortFilterProxyModel *>(sender());

  for (; start <= end; ++start) {
    QModelIndex sIndex = model->mapToSource(model->index(start, 0, parent));
    PropertyInterface *pi =
        _sourceModel->data(sIndex, TulipModel::PropertyRole).value<PropertyInterface *>();

    if (filteringProperties == false)
      _sourceModel->setData(sIndex, Qt::Checked, Qt::CheckStateRole);

    emit propertyVisibilityChanged(pi, _sourceModel->data(sIndex, Qt::CheckStateRole).toInt() !=
                                           Qt::Unchecked);
  }
}

// properties removed when filtering
// are no longer visible
void PropertiesEditor::displayedPropertiesRemoved(const QModelIndex &parent, int start, int end) {
  QSortFilterProxyModel *model = static_cast<QSortFilterProxyModel *>(sender());

  for (; start <= end; ++start) {
    PropertyInterface *pi =
        _sourceModel
            ->data(model->mapToSource(model->index(start, 0, parent)), TulipModel::PropertyRole)
            .value<PropertyInterface *>();
    emit propertyVisibilityChanged(pi, false);
  }
}

bool PropertiesEditor::setAllValues(PropertyInterface *prop, bool nodes, bool selectedOnly,
                                    bool graphOnly) {
  QVariant val = TulipItemDelegate::showEditorDialog(
      nodes ? NODE : EDGE, prop, _graph, static_cast<TulipItemDelegate *>(_delegate), editorParent);

  // Check if edition has been cancelled
  if (!val.isValid())
    return false;

  if (selectedOnly) {
    BooleanProperty *selection = _graph->getProperty<BooleanProperty>("viewSelection");

    if (nodes) {
      for (auto n : selection->getNonDefaultValuatedNodes(_graph)) {
        GraphModel::setNodeValue(n.id, prop, val);
      }
    } else {
      for (auto e : selection->getNonDefaultValuatedEdges(_graph)) {
        GraphModel::setEdgeValue(e.id, prop, val);
      }
    }
  } else {
    Observable::holdObservers();

    if (nodes)
      GraphModel::setAllNodeValue(prop, val, graphOnly ? _graph : nullptr);
    else
      GraphModel::setAllEdgeValue(prop, val, graphOnly ? _graph : nullptr);

    Observable::unholdObservers();
  }

  return true;
}

void PropertiesEditor::setDefaultValue(tlp::PropertyInterface *prop, bool nodes) {
  QVariant val = TulipItemDelegate::showEditorDialog(
      nodes ? NODE : EDGE, prop, _graph, static_cast<TulipItemDelegate *>(_delegate), editorParent);

  // Check if edition has been cancelled
  if (!val.isValid())
    return;

  if (nodes) {
    GraphModel::setNodeDefaultValue(prop, val);
  } else {
    GraphModel::setEdgeDefaultValue(prop, val);
  }
}

void PropertiesEditor::copyProperty() {
  _graph->push();

  if (CopyPropertyDialog::copyProperty(_graph, _contextProperty, true,
                                       Perspective::instance()->mainWindow()) == nullptr)
    // copy has been cancelled
    _graph->pop();
}

void PropertiesEditor::newProperty() {
  _graph->push();

  if (PropertyCreationDialog::createNewProperty(_graph, Perspective::instance()->mainWindow(),
                                                _contextProperty ? _contextProperty->getTypename()
                                                                 : std::string()) == nullptr)
    // creation has been cancelled
    _graph->pop();
}

void PropertiesEditor::delProperty() {
  _graph->push();
  _contextProperty->getGraph()->delLocalProperty(_contextProperty->getName());
}

void PropertiesEditor::delProperties() {
  _graph->push();

  for (auto pi : _contextPropertyList)
    pi->getGraph()->delLocalProperty(pi->getName());
}

bool PropertiesEditor::renameProperty(PropertyInterface *prop) {
  return RenamePropertyDialog::renameProperty(prop, Perspective::instance()->mainWindow());
}

void PropertiesEditor::toLabels() {
  _graph->push();
  toLabels(_contextProperty, true, true);
}

void PropertiesEditor::toNodesLabels() {
  _graph->push();
  toLabels(_contextProperty, true, false);
}

void PropertiesEditor::toEdgesLabels() {
  _graph->push();
  toLabels(_contextProperty, false, true);
}

void PropertiesEditor::toSelectedLabels() {
  _graph->push();
  toLabels(_contextProperty, true, true, true);
}

void PropertiesEditor::toSelectedNodesLabels() {
  _graph->push();
  toLabels(_contextProperty, true, false, true);
}

void PropertiesEditor::toSelectedEdgesLabels() {
  _graph->push();
  toLabels(_contextProperty, false, true, true);
}

void PropertiesEditor::toLabels(PropertyInterface *prop, bool nodes, bool edges,
                                bool selectedOnly) {
  DataSet data;
  data.set("nodes", nodes);
  data.set("edges", edges);
  data.set("input", prop);

  if (selectedOnly)
    data.set("selection", _graph->getProperty<BooleanProperty>("viewSelection"));

  std::string msg;
  // _graph->push() must be done outside of this method
  // to allow call from TabelView.cpp
  StringProperty *result = _graph->getProperty<StringProperty>("viewLabel");
  _graph->applyPropertyAlgorithm("To labels", result, msg, &data);
}

void PropertiesEditor::checkStateChanged(QModelIndex index, Qt::CheckState state) {
  PropertyInterface *pi =
      _sourceModel->data(index, TulipModel::PropertyRole).value<PropertyInterface *>();
  emit propertyVisibilityChanged(pi, state == Qt::Checked);
}

QSet<PropertyInterface *> PropertiesEditor::visibleProperties() const {
  if (_sourceModel != nullptr)
    return _sourceModel->checkedProperties();

  return QSet<tlp::PropertyInterface *>();
}

void PropertiesEditor::setPropertyChecked(int index, bool state) {
  _sourceModel->setData(_sourceModel->index(index, 0), state ? Qt::Checked : Qt::Unchecked,
                        Qt::CheckStateRole);
}

void PropertiesEditor::setPropertyChecked(const QString &pName, bool state) {
  int index = _sourceModel->rowOf(pName);

  if (index != -1)
    _sourceModel->setData(_sourceModel->index(index, 0), state ? Qt::Checked : Qt::Unchecked,
                          Qt::CheckStateRole);
}

PropertyInterface *PropertiesEditor::contextProperty() const {
  return _contextProperty;
}
