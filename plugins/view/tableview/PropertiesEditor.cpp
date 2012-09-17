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
#include <tulip/TulipItemDelegate.h>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QMenu>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDialog>
#include <QtGui/QCursor>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>

#include "ui_PropertiesEditor.h"
#include "ui_AddPropertyDialog.h"

Q_DECLARE_METATYPE(Qt::CheckState)

using namespace tlp;

PropertiesEditor::PropertiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::PropertiesEditor), _graph(NULL), _delegate(new tlp::TulipItemDelegate), _sourceModel(NULL) {
  _ui->setupUi(this);
  connect(_ui->edgesButton, SIGNAL(toggled(bool)), this, SIGNAL(showElementTypeChanged()));
  connect(_ui->nodesButton, SIGNAL(toggled(bool)), this, SIGNAL(showElementTypeChanged()));
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
  connect(_ui->lineEdit,SIGNAL(textChanged(QString)),model,SLOT(setFilterFixedString(QString)));
  _ui->tableView->setModel(model);
  connect(_sourceModel,SIGNAL(checkStateChanged(QModelIndex,Qt::CheckState)),this,SLOT(checkStateChanged(QModelIndex,Qt::CheckState)));
  _ui->tableView->resizeColumnsToContents();
  _ui->tableView->sortByColumn(0,Qt::AscendingOrder);
}

void PropertiesEditor::showCustomContextMenu(const QPoint& p) {
  _contextProperty = _ui->tableView->indexAt(p).data(TulipModel::PropertyRole).value<PropertyInterface*>();

  if (_contextProperty == NULL)
    return;

  QMenu menu;
  connect(menu.addAction(trUtf8("Check all")),SIGNAL(triggered()),this,SLOT(checkAll()));
  connect(menu.addAction(trUtf8("Uncheck all")),SIGNAL(triggered()),this,SLOT(unCheckAll()));
  connect(menu.addAction(trUtf8("Uncheck all except \"") + _contextProperty->getName().c_str() + "\""),SIGNAL(triggered()),this,SLOT(unCheckAllExcept()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Set all nodes")),SIGNAL(triggered()),this,SLOT(setAllNodes()));
  connect(menu.addAction(trUtf8("Set all edges")),SIGNAL(triggered()),this,SLOT(setAllEdges()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("To labels")),SIGNAL(triggered()),this,SLOT(toLabels()));
  connect(menu.addAction(trUtf8("To labels (nodes only)")),SIGNAL(triggered()),this,SLOT(toNodesLabels()));
  connect(menu.addAction(trUtf8("To labels (edges only)")),SIGNAL(triggered()),this,SLOT(toEdgesLabels()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Copy")),SIGNAL(triggered()),this,SLOT(copyProperty()));
  connect(menu.addAction(trUtf8("New")),SIGNAL(triggered()),this,SLOT(newProperty()));
  QAction* delAction = menu.addAction(trUtf8("Delete"));
  delAction->setEnabled(!Perspective::instance()->isReservedPropertyName(_contextProperty->getName().c_str()));
  connect(delAction,SIGNAL(triggered()),this,SLOT(delProperty()));
  menu.addSeparator();
  menu.addAction(trUtf8("Cancel"));
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
    QModelIndex index = _sourceModel->index(i,0);
    Qt::CheckState state = ( _sourceModel->data(index).toString() == QString(_contextProperty->getName().c_str()) ? Qt::Checked : Qt::Unchecked);
    _sourceModel->setData(index,state,Qt::CheckStateRole);
  }
}

QDialog* editDialog(QWidget* w, QWidget* parent) {
  QDialog* dlg = new QDialog(parent);
  QVBoxLayout* layout = new QVBoxLayout;
  dlg->setLayout(layout);
  layout->addWidget(w);
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok,Qt::Horizontal);
  layout->addWidget(buttonBox);
  QObject::connect(buttonBox,SIGNAL(accepted()),dlg,SLOT(accept()));
  QObject::connect(buttonBox,SIGNAL(rejected()),dlg,SLOT(reject()));
  return dlg;
}

void PropertiesEditor::setAllNodes() {
  QVariant defaultValue = GraphModel::nodeDefaultValue(_contextProperty);
  TulipItemEditorCreator* creator = _delegate->creator(defaultValue.userType());
  QWidget* w = creator->createWidget(Perspective::instance()->mainWindow());
  creator->setEditorData(w,defaultValue,_graph);

  QDialog* dlg = dynamic_cast<QDialog*>(w);
  if (dlg == NULL) {
    dlg = editDialog(w,Perspective::instance()->mainWindow());
    dlg->setWindowTitle(QString::fromUtf8(_contextProperty->getName().c_str()) + trUtf8(": Set all nodes values"));
  }

  if (dlg->exec() == QDialog::Accepted) {
    _graph->push();
    GraphModel::setAllNodeValue(_contextProperty,creator->editorData(w,_graph));
  }

  delete dlg;
}

void PropertiesEditor::setAllEdges() {
  QVariant defaultValue = GraphModel::edgeDefaultValue(_contextProperty);
  TulipItemEditorCreator* creator = _delegate->creator(defaultValue.userType());
  QWidget* w = creator->createWidget(Perspective::instance()->mainWindow());
  creator->setEditorData(w,defaultValue,_graph);

  QDialog* dlg = dynamic_cast<QDialog*>(w);
  if (dlg == NULL) {
    dlg = editDialog(w,Perspective::instance()->mainWindow());
    dlg->setWindowTitle(QString::fromUtf8(_contextProperty->getName().c_str()) + trUtf8(": Set all edges values"));
  }

  if (dlg->exec() == QDialog::Accepted) {
    _graph->push();
    GraphModel::setAllEdgeValue(_contextProperty,creator->editorData(w,_graph));
  }

  delete dlg;
}

void PropertiesEditor::copyProperty() {
  CopyPropertyDialog::copyProperty(_graph,_contextProperty,true,Perspective::instance()->mainWindow());
}

void PropertiesEditor::newProperty() {
  Ui::AddPropertyDialog ui;
  QDialog dlg;
  ui.setupUi(&dlg);

  if (dlg.exec() == QDialog::Accepted && ui.comboBox->currentIndex() > 0 && !ui.lineEdit->text().isEmpty()) {
    std::string name = ui.lineEdit->text().toStdString();

    if (_graph->existProperty(name))
      QMessageBox::critical(NULL,trUtf8("Property already exists"),trUtf8("Graph already contains a property named ") + ui.lineEdit->text());

    switch (ui.comboBox->currentIndex()) {
    case 1:
      _graph->getProperty<IntegerProperty>(name);
      break;

    case 2:
      _graph->getProperty<IntegerVectorProperty>(name);
      break;

    case 3:
      _graph->getProperty<BooleanProperty>(name);
      break;

    case 4:
      _graph->getProperty<BooleanVectorProperty>(name);
      break;

    case 5:
      _graph->getProperty<LayoutProperty>(name);
      break;

    case 6:
      _graph->getProperty<CoordVectorProperty>(name);
      break;

    case 7:
      _graph->getProperty<StringProperty>(name);
      break;

    case 8:
      _graph->getProperty<StringVectorProperty>(name);
      break;

    case 9:
      _graph->getProperty<SizeProperty>(name);
      break;

    case 10:
      _graph->getProperty<SizeVectorProperty>(name);
      break;

    case 11:
      _graph->getProperty<ColorProperty>(name);
      break;

    case 12:
      _graph->getProperty<ColorVectorProperty>(name);
      break;
    }

  }
}

void PropertiesEditor::delProperty() {
  _contextProperty->getGraph()->delLocalProperty(_contextProperty->getName());
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
  _graph->computeProperty<StringProperty>("To labels",result,msg,NULL,&data);
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

bool PropertiesEditor::isShowNodes() {
  return _ui->nodesButton->isChecked();
}

bool PropertiesEditor::isShowEdges() {
  return _ui->edgesButton->isChecked();
}

void PropertiesEditor::showNodes(bool value) {
  _ui->nodesButton->setChecked(value);
}

void PropertiesEditor::showEdges(bool value) {
  _ui->edgesButton->setChecked(value);
}
