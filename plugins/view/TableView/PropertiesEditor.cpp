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

Q_DECLARE_METATYPE(Qt::CheckState)

using namespace tlp;

PropertiesEditor::PropertiesEditor(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PropertiesEditor), _contextProperty(NULL), _graph(NULL),
      _delegate(new tlp::TulipItemDelegate), _sourceModel(NULL), filteringProperties(false),
      editorParent(parent) {
  _ui->setupUi(this);
  connect(_ui->newButton, SIGNAL(clicked()), this, SLOT(newProperty()));
}

PropertiesEditor::~PropertiesEditor() {
  delete _ui;
  delete _delegate;
  delete _sourceModel;
}

void PropertiesEditor::setGraph(tlp::Graph *g) {
  _graph = g;
  QSortFilterProxyModel *model = new QSortFilterProxyModel(_ui->tableView);
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
  _ui->tableView->setModel(model);
  connect(_sourceModel, SIGNAL(checkStateChanged(QModelIndex, Qt::CheckState)), this,
          SLOT(checkStateChanged(QModelIndex, Qt::CheckState)));
  _ui->tableView->resizeColumnsToContents();
  _ui->tableView->sortByColumn(0, Qt::AscendingOrder);
  _ui->visualPropertiesCheck->setChecked(true);
}

void PropertiesEditor::setPropertiesFilter(QString filter) {
  filteringProperties = true;
  static_cast<QSortFilterProxyModel *>(_ui->tableView->model())->setFilterRegExp(filter);
  filteringProperties = false;
}

QLineEdit *PropertiesEditor::getPropertiesFilterEdit() {
  return _ui->propertiesFilterEdit;
}

void PropertiesEditor::showCustomContextMenu(const QPoint &p) {
  _contextProperty =
      _ui->tableView->indexAt(p).data(TulipModel::PropertyRole).value<PropertyInterface *>();
  _contextPropertyList.clear();

  foreach (const QModelIndex &sidx, _ui->tableView->selectionModel()->selectedRows()) {
    _contextPropertyList += sidx.data(TulipModel::PropertyRole).value<PropertyInterface *>();
  }

  if (_contextProperty == NULL)
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

    foreach (PropertyInterface *pi, _contextPropertyList) {
      if (Perspective::instance()->isReservedPropertyName(pi->getName().c_str()) &&
          (_graph == _graph->getRoot() || !_graph->existLocalProperty(pi->getName()))) {
        enabled = false;
        break;
      }
    }

    if (enabled) {
      action = menu.addAction(trUtf8("Delete highlighted properties"));
      action->setToolTip("Delete the highlighted properties");
      connect(action, SIGNAL(triggered()), this, SLOT(delProperties()));
      action = menu.addAction(trUtf8("Hide all other properties"));
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
    action = menu.addAction(trUtf8("Hide all other properties"));
    action->setToolTip("Show only the colun corresponding to this property");
    connect(action, SIGNAL(triggered()), this, SLOT(setPropsNotVisibleExcept()));
    menu.addSeparator();

    action = menu.addAction(QIcon(":/tulip/gui/icons/64/list-add.png"),
                            trUtf8("Add property of the same type"));
    action->setToolTip("Display a dialog to create a new property owning to the current graph");
    connect(action, SIGNAL(triggered()), this, SLOT(newProperty()));
    connect(menu.addAction(trUtf8("Copy")), SIGNAL(triggered()), this, SLOT(copyProperty()));

    bool enabled = true;
    const std::string &propName = _contextProperty->getName();

    if (Perspective::instance()->isReservedPropertyName(propName.c_str())) {
      // Enable deletion of reserved properties when on a subgraph and that properties are local
      if (_graph == _graph->getRoot() || !_graph->existLocalProperty(propName))
        enabled = false;
    }

    if (enabled) {
      action = menu.addAction(trUtf8("Delete"));
      action->setToolTip(QString("Delete the property \"") + propName.c_str() + '"');
      connect(action, SIGNAL(triggered()), this, SLOT(delProperty()));
    }

    QAction *rename = NULL;

    if (!Perspective::instance()->isReservedPropertyName(propName.c_str())) {
      rename = menu.addAction("Rename");
      rename->setToolTip(QString("Rename the property \"") + propName.c_str() + '"');
    }

    menu.addSeparator();

    QMenu *subMenu =
        /*menu.addMenu(trUtf8("Set default value for"));
    QAction* nodesSetDefault = subMenu->addAction(trUtf8("nodes"));
    QAction* edgesSetDefault = subMenu->addAction(trUtf8("edges"));

    subMenu =*/menu.addMenu(trUtf8("Set values of"));
    QAction *nodesSetAll =
        subMenu->addAction(trUtf8("All nodes") + OF_PROPERTY + trUtf8(" to a new default value"));
    nodesSetAll->setToolTip(
        QString("Choose a new node default value to reset the values of all nodes") + OF_PROPERTY);
    QAction *edgesSetAll =
        subMenu->addAction(trUtf8("All edges") + OF_PROPERTY + trUtf8(" to a new default value"));
    edgesSetAll->setToolTip(
        QString("Choose a new edge default value to reset the values of all edges ") + OF_PROPERTY);
    QAction *nodesSetAllGraph = subMenu->addAction(trUtf8("All nodes") + OF_GRAPH);
    nodesSetAllGraph->setToolTip(
        QString("Choose a value to be assigned to all the existing nodes") + OF_GRAPH);
    QAction *edgesSetAllGraph = subMenu->addAction(trUtf8("All edges") + OF_GRAPH);
    edgesSetAllGraph->setToolTip(
        QString("Choose a value to be assigned to all the existing edges") + OF_GRAPH);
    QAction *selectedNodesSetAll = subMenu->addAction(trUtf8("Selected nodes") + OF_GRAPH);
    selectedNodesSetAll->setToolTip(QString("Choose a value to be assigned to the selected nodes") +
                                    OF_GRAPH);
    QAction *selectedEdgesSetAll = subMenu->addAction(trUtf8("Selected edges") + OF_GRAPH);
    selectedEdgesSetAll->setToolTip(QString("Choose a value to be assigned to the selected edges") +
                                    OF_GRAPH);

    enabled = (pname != "viewLabel");

    if (enabled) {
      subMenu = menu.addMenu(trUtf8("To labels of"));
      action = subMenu->addAction(trUtf8("All elements") + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of all elements") + OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toLabels()));
      action = subMenu->addAction(trUtf8("All nodes") + OF_GRAPH);
      action->setToolTip(QString("Set the values of the current property as labels of the nodes") +
                         OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toNodesLabels()));
      action = subMenu->addAction(trUtf8("All edges") + OF_GRAPH);
      action->setToolTip(QString("Set the values of the current property as labels of the edges") +
                         OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toEdgesLabels()));
      action = subMenu->addAction(trUtf8("All selected elements") + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of the selected elements") +
          OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toSelectedLabels()));
      action = subMenu->addAction(trUtf8("Selected nodes") + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of the selected nodes") +
          OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toSelectedNodesLabels()));
      action = subMenu->addAction(trUtf8("Selected edges") + OF_GRAPH);
      action->setToolTip(
          QString("Set the values of the current property as labels of the selected edges") +
          OF_GRAPH);
      connect(action, SIGNAL(triggered()), this, SLOT(toSelectedEdgesLabels()));
    }

    QAction *action = menu.exec(QCursor::pos());

    /*if (action == nodesSetDefault || action == edgesSetDefault) {
      setDefaultValue(_contextProperty, action == nodesSetDefault);
    }

    else*/ if (action != NULL) {
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

  _contextProperty = NULL;
}

void PropertiesEditor::setPropsVisibility(int state) {
  if (state == Qt::PartiallyChecked)
    return;

  _ui->propsVisibilityCheck->setTristate(false);

  if (state == Qt::Checked) {
    // reset property name filter
    _ui->propertiesFilterEdit->setText(QString());
    // no filter
    static_cast<QSortFilterProxyModel *>(_ui->tableView->model())->setFilterFixedString("");
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

  foreach (PropertyInterface *pi, _contextPropertyList)
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

  static_cast<QSortFilterProxyModel *>(_ui->tableView->model())->setFilterFixedString("");

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
      node n;
      forEach(n, selection->getNonDefaultValuatedNodes(_graph)) {
        GraphModel::setNodeValue(n.id, prop, val);
      }
    } else {
      edge e;
      forEach(e, selection->getNonDefaultValuatedEdges(_graph)) {
        GraphModel::setEdgeValue(e.id, prop, val);
      }
    }
  } else {
    Observable::holdObservers();

    if (nodes)
      GraphModel::setAllNodeValue(prop, val, graphOnly ? _graph : NULL);
    else
      GraphModel::setAllEdgeValue(prop, val, graphOnly ? _graph : NULL);

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
                                       Perspective::instance()->mainWindow()) == NULL)
    // copy has been cancelled
    _graph->pop();
}

void PropertiesEditor::newProperty() {
  _graph->push();

  if (PropertyCreationDialog::createNewProperty(_graph, Perspective::instance()->mainWindow(),
                                                _contextProperty ? _contextProperty->getTypename()
                                                                 : std::string()) == NULL)
    // creation has been cancelled
    _graph->pop();
}

void PropertiesEditor::delProperty() {
  _graph->push();
  _contextProperty->getGraph()->delLocalProperty(_contextProperty->getName());
}

void PropertiesEditor::delProperties() {
  _graph->push();

  foreach (PropertyInterface *pi, _contextPropertyList)
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
  _graph->applyPropertyAlgorithm("To labels", result, msg, NULL, &data);
}

void PropertiesEditor::checkStateChanged(QModelIndex index, Qt::CheckState state) {
  PropertyInterface *pi =
      _sourceModel->data(index, TulipModel::PropertyRole).value<PropertyInterface *>();
  emit propertyVisibilityChanged(pi, state == Qt::Checked);
}

QSet<PropertyInterface *> PropertiesEditor::visibleProperties() const {
  if (_sourceModel != NULL)
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
