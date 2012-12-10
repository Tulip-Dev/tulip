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
  _contextPropertyList.clear();
  foreach(QModelIndex sidx, _ui->tableView->selectionModel()->selectedRows()) {
    _contextPropertyList += sidx.data(TulipModel::PropertyRole).value<PropertyInterface*>();
  }

  if (_contextProperty == NULL)
    return;

  QString pname = _contextProperty->getName().c_str();

  if (pname.length()>30) {
    pname.truncate(30);
    pname+= "...";
  }

  QMenu menu;
  connect(menu.addAction(trUtf8("Check all")),SIGNAL(triggered()),this,SLOT(checkAll()));
  connect(menu.addAction(trUtf8("Uncheck all")),SIGNAL(triggered()),this,SLOT(unCheckAll()));
  connect(menu.addAction(trUtf8("Uncheck all except \"") + pname + "\""),SIGNAL(triggered()),this,SLOT(unCheckAllExcept()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Set all nodes")),SIGNAL(triggered()),this,SLOT(setAllNodes()));
  connect(menu.addAction(trUtf8("Set all edges")),SIGNAL(triggered()),this,SLOT(setAllEdges()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Map visible items to graph")),SIGNAL(triggered()),this,SIGNAL(mapToGraphSelection()));
  connect(menu.addAction(trUtf8("Set displayed nodes")),SIGNAL(triggered()),this,SIGNAL(setFilteredNodes()));
  connect(menu.addAction(trUtf8("Set displayed edges")),SIGNAL(triggered()),this,SIGNAL(setFilteredEdges()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("To labels")),SIGNAL(triggered()),this,SLOT(toLabels()));
  connect(menu.addAction(trUtf8("To labels (nodes only)")),SIGNAL(triggered()),this,SLOT(toNodesLabels()));
  connect(menu.addAction(trUtf8("To labels (edges only)")),SIGNAL(triggered()),this,SLOT(toEdgesLabels()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Copy")),SIGNAL(triggered()),this,SLOT(copyProperty()));
  connect(menu.addAction(trUtf8("New")),SIGNAL(triggered()),this,SLOT(newProperty()));
  QAction* delAction = menu.addAction(trUtf8("Delete"));
  bool enabled = true;
  foreach(PropertyInterface* pi, _contextPropertyList) {
    if (Perspective::instance()->isReservedPropertyName(pi->getName().c_str())) {
      // Enable deletion of reserved properties when on a subgraph and that properties are local
      if (_graph == _graph->getRoot() || !_graph->existLocalProperty(pi->getName())) {
        enabled = false;
        break;
      }
    }
  }
  delAction->setEnabled(enabled);

  connect(delAction,SIGNAL(triggered()),this,SLOT(delProperty()));
  menu.addSeparator();
  menu.addAction(trUtf8("Hide this menu"));
  menu.exec(QCursor::pos());
}

void PropertiesEditor::checkAll() {
  for(int i=0; i<_sourceModel->rowCount(); ++i)
    _sourceModel->setData(_sourceModel->index(i,0),Qt::Checked,Qt::CheckStateRole);
}

void PropertiesEditor::unCheckAll() {
  for(int i=0; i<_sourceModel->rowCount(); ++i) {
    _sourceModel->setData(_sourceModel->index(i,0),Qt::Unchecked,Qt::CheckStateRole);
  }
}

void PropertiesEditor::unCheckAllExcept() {
  for(int i=0; i<_sourceModel->rowCount(); ++i) {
    setPropertyChecked(i,_sourceModel->index(i,0).data().toString() == _contextProperty->getName().c_str());
  }
}

void PropertiesEditor::showSystemProperties(bool f) {
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
  foreach(PropertyInterface* pi, _contextPropertyList)
  pi->getGraph()->delLocalProperty(pi->getName());
}

void PropertiesEditor::toLabels() {
  toLabels(true,true);
}

void PropertiesEditor::toNodesLabels() {
  toLabels(true,false);
}

void PropertiesEditor::toEdgesLabels() {
  toLabels(false,true);
}

void PropertiesEditor::toLabels(bool nodes, bool edges) {
  DataSet data;
  data.set("nodes",nodes);
  data.set("edges",edges);
  data.set("input",_contextProperty);
  std::string msg;
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
