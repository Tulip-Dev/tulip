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
#include "PropertiesEditor.h"

#include <tulip/Perspective.h>
#include <tulip/GraphModel.h>
#include <tulip/TulipItemEditorCreators.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/PropertyCreationDialog.h>
#include <tulip/TulipItemDelegate.h>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QMenu>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDialog>
#include <QtGui/QCursor>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>

#include "ui_PropertiesEditor.h"

Q_DECLARE_METATYPE(Qt::CheckState)

using namespace tlp;

PropertiesEditor::PropertiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::PropertiesEditor), _graph(NULL), _delegate(new tlp::TulipItemDelegate), _sourceModel(NULL) {
  _ui->setupUi(this);
}

PropertiesEditor::~PropertiesEditor() {
  delete _ui;
}

void PropertiesEditor::setGraph(tlp::Graph *g) {
  _graph=g;
  QSortFilterProxyModel* model = new QSortFilterProxyModel(_ui->tableView);
  _sourceModel = new GraphPropertiesModel<PropertyInterface>(g,true);
  model->setSourceModel(_sourceModel);
  model->setFilterCaseSensitivity(Qt::CaseInsensitive);
  // the 3 signal-to-slot connections below ensure the propagation
  // of the displayed properties filtering
  connect(_ui->lineEdit,SIGNAL(textChanged(QString)),model,SLOT(setFilterFixedString(QString)));
  connect(model,SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)),
          this,SLOT(displayedPropertiesRemoved(const QModelIndex&, int, int)));
  connect(model,SIGNAL(rowsInserted(const QModelIndex&, int, int)),
          this,SLOT(displayedPropertiesInserted(const QModelIndex&, int, int)));
  _ui->tableView->setModel(model);
  connect(_sourceModel,SIGNAL(checkStateChanged(QModelIndex,Qt::CheckState)),this,SLOT(checkStateChanged(QModelIndex,Qt::CheckState)));
  _ui->tableView->resizeColumnsToContents();
  _ui->tableView->sortByColumn(0,Qt::AscendingOrder);
}

void PropertiesEditor::showCustomContextMenu(const QPoint& p) {
  _contextProperty = _ui->tableView->indexAt(p).data(TulipModel::PropertyRole).value<PropertyInterface*>();

  if (_contextProperty == NULL)
    return;

  QString pname = _contextProperty->getName().c_str();

  if (pname.length()>30) {
    pname.truncate(30);
    pname+= "...";
  }

  QMenu menu;
  menu.setProperty("mainMenu", true);
  menu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
  menu.addAction(pname)->setEnabled(false);
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Set other properties not visible")),SIGNAL(triggered()),this,SLOT(setPropsNotVisibleExcept()));
  menu.addSeparator();
  bool enabled = (pname != "viewLabel");
  QMenu* subMenu = menu.addMenu(trUtf8("To labels of"));
  QAction* action = subMenu->addAction(trUtf8("All"));

  if (enabled)
    connect(action,SIGNAL(triggered()),this,SLOT(toLabels()));
  else
    action->setEnabled(false);

  action = subMenu->addAction(trUtf8("All nodes"));

  if (enabled)
    connect(action,SIGNAL(triggered()),this,SLOT(toNodesLabels()));
  else
    action->setEnabled(false);

  action = subMenu->addAction(trUtf8("All edges"));

  if (enabled)
    connect(action,SIGNAL(triggered()),this,SLOT(toEdgesLabels()));
  else
    action->setEnabled(false);

  action = subMenu->addAction(trUtf8("All selected"));

  if (enabled)
    connect(action,SIGNAL(triggered()),this,SLOT(toSelectedLabels()));
  else
    action->setEnabled(false);

  action = subMenu->addAction(trUtf8("Selected nodes"));

  if (enabled)
    connect(action,SIGNAL(triggered()),this,SLOT(toSelectedNodesLabels()));
  else
    action->setEnabled(false);

  action = subMenu->addAction(trUtf8("Selected edges"));

  if (enabled)
    connect(action,SIGNAL(triggered()),this,SLOT(toSelectedEdgesLabels()));
  else
    action->setEnabled(false);

  menu.addSeparator();
  connect(menu.addAction(trUtf8("Copy")),SIGNAL(triggered()),this,SLOT(copyProperty()));
  connect(menu.addAction(trUtf8("New")),SIGNAL(triggered()),this,SLOT(newProperty()));
  enabled = true;

  if (Perspective::instance()->isReservedPropertyName(_contextProperty->getName().c_str())) {
    // Enable deletion of reserved properties when on a subgraph and that properties are local
    if (_graph == _graph->getRoot() || !_graph->existLocalProperty(_contextProperty->getName()))
      enabled = false;
  }

  if (enabled) {
    connect(menu.addAction(trUtf8("Delete")),SIGNAL(triggered()),this,SLOT(delProperty()));
  }

  menu.exec(QCursor::pos());
}

void PropertiesEditor::setPropsVisibility(int state) {
  if (state == Qt::PartiallyChecked)
    return;

  _ui->propsVisibilityCheck->setTristate(false);

  for(int i=0; i<_sourceModel->rowCount(); ++i)
    _sourceModel->setData(_sourceModel->index(i,0), state,
                          Qt::CheckStateRole);
}

void PropertiesEditor::setPropsNotVisibleExcept() {
  for(int i=0; i<_sourceModel->rowCount(); ++i) {
    setPropertyChecked(i,_sourceModel->index(i,0).data().toString() == _contextProperty->getName().c_str());
  }

  _ui->propsVisibilityCheck->setTristate(true);
  _ui->propsVisibilityCheck->setCheckState(Qt::PartiallyChecked);
}

void PropertiesEditor::showVisualProperties(bool f) {
  if (f)
    static_cast<QSortFilterProxyModel*>(_ui->tableView->model())->setFilterFixedString("");
  else
    static_cast<QSortFilterProxyModel*>(_ui->tableView->model())->setFilterRegExp("^(?!view).*");
}

// properties inserted when filtering
// are visible according to their CheckState
void PropertiesEditor::displayedPropertiesInserted(const QModelIndex &parent,
    int start, int end) {
  QSortFilterProxyModel* model = static_cast<QSortFilterProxyModel*>(sender());

  for(; start <= end; ++start) {
    QModelIndex sIndex = model->mapToSource(model->index(start, 0, parent));
    PropertyInterface* pi = _sourceModel->data(sIndex, TulipModel::PropertyRole).value<PropertyInterface*>();
    emit propertyVisibilityChanged(pi, _sourceModel->data(sIndex, Qt::CheckStateRole).toInt() != Qt::Unchecked);
  }
}

// properties removed when filtering
// are no longer visible
void PropertiesEditor::displayedPropertiesRemoved(const QModelIndex &parent,
    int start, int end) {
  QSortFilterProxyModel* model = static_cast<QSortFilterProxyModel*>(sender());

  for(; start <= end; ++start) {
    PropertyInterface* pi = _sourceModel->data(model->mapToSource(model->index(start, 0, parent)), TulipModel::PropertyRole).value<PropertyInterface*>();
    emit propertyVisibilityChanged(pi, false);
  }
}

void PropertiesEditor::setAllNodes() {
  QVariant val = TulipItemDelegate::showEditorDialog(NODE,_contextProperty,_graph,static_cast<TulipItemDelegate*>(_delegate));
  Observable::holdObservers();
  _graph->push();
  GraphModel::setAllNodeValue(_contextProperty,val);
  Observable::unholdObservers();
}

void PropertiesEditor::setAllEdges() {
  QVariant val = TulipItemDelegate::showEditorDialog(EDGE,_contextProperty,_graph,static_cast<TulipItemDelegate*>(_delegate));
  Observable::holdObservers();
  _graph->push();
  GraphModel::setAllEdgeValue(_contextProperty,val);
  Observable::unholdObservers();
}

void PropertiesEditor::copyProperty() {
  CopyPropertyDialog::copyProperty(_graph,_contextProperty,true,Perspective::instance()->mainWindow());
}

void PropertiesEditor::newProperty() {
  PropertyCreationDialog::createNewProperty(_graph, Perspective::instance()->mainWindow());
}

void PropertiesEditor::delProperty() {
  _graph->push();
  _contextProperty->getGraph()->delLocalProperty(_contextProperty->getName());
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
  toLabels(_contextProperty, false,true);
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

void PropertiesEditor::toLabels(PropertyInterface* prop, bool nodes, bool edges, bool selectedOnly) {
  DataSet data;
  data.set("nodes",nodes);
  data.set("edges",edges);
  data.set("input", prop);

  if (selectedOnly)
    data.set("selection", _graph->getProperty<BooleanProperty>("viewSelection"));

  std::string msg;
  // _graph->push() must be done outside of this method
  // to allow call from TabelView.cpp
  StringProperty* result = _graph->getProperty<StringProperty>("viewLabel");
  _graph->applyPropertyAlgorithm("To labels",result,msg,NULL,&data);
}

void PropertiesEditor::checkStateChanged(QModelIndex index,Qt::CheckState state) {
  PropertyInterface* pi = _sourceModel->data(index,TulipModel::PropertyRole).value<PropertyInterface*>();
  emit propertyVisibilityChanged(pi,state == Qt::Checked);
}

QSet<PropertyInterface *> PropertiesEditor::visibleProperties() const {
  if (_sourceModel != NULL)
    return _sourceModel->checkedProperties();

  return QSet<tlp::PropertyInterface*>();
}

void PropertiesEditor::setPropertyChecked(int index, bool state) {
  _sourceModel->setData(_sourceModel->index(index,0),state ? Qt::Checked : Qt::Unchecked,Qt::CheckStateRole);
}

PropertyInterface *PropertiesEditor::contextProperty() const {
  return _contextProperty;
}
