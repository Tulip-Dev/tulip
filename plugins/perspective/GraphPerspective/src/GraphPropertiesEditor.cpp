#include "GraphPropertiesEditor.h"
#include "ui_GraphPropertiesEditor.h"

#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QMenu>
#include <QtGui/QDialogButtonBox>

#include <tulip/GraphModel.h>
#include <tulip/Graph.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/GraphPropertiesModel.h>
#include <tulip/PropertyInterface.h>
#include <tulip/GraphTableItemDelegate.h>
#include "GraphPerspective.h"

using namespace tlp;

GraphPropertiesEditor::GraphPropertiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphPropertiesEditor), _delegate(new tlp::TulipItemDelegate) {
  _ui->setupUi(this);
}

void GraphPropertiesEditor::setModel(tlp::GraphHierarchiesModel* model) {
  disconnect(this,SLOT(setGraph(tlp::Graph*)));
  connect(model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(setGraph(tlp::Graph*)));
  setGraph(model->currentGraph());
}

void GraphPropertiesEditor::setGraph(tlp::Graph* graph) {
  _graph = graph;
  QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(new GraphPropertiesModel<PropertyInterface>(graph));
  _ui->propertiesList->setModel(proxyModel);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxyModel->setFilterKeyColumn(0);
  proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
}

void GraphPropertiesEditor::showCustomContextMenu(const QPoint& p) {
  _contextProperty = _ui->propertiesList->indexAt(p).data(TulipModel::PropertyRole).value<PropertyInterface*>();
  QMenu menu;
  connect(menu.addAction(trUtf8("Set all nodes")),SIGNAL(triggered()),this,SLOT(setAllNodes()));
  connect(menu.addAction(trUtf8("Set all edges")),SIGNAL(triggered()),this,SLOT(setAllEdges()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("To labels")),SIGNAL(triggered()),this,SLOT(toLabels()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Copy")),SIGNAL(triggered()),this,SLOT(copyProperty()));
  QAction* delAction = menu.addAction(trUtf8("Delete"));
  delAction->setEnabled(!GraphPerspective::isReservedPropertyName(_contextProperty->getName().c_str()));

  connect(delAction,SIGNAL(triggered()),this,SLOT(delProperty()));
  menu.exec(_ui->propertiesList->mapToGlobal(p));
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

void GraphPropertiesEditor::setAllNodes() {
  QVariant defaultValue = GraphModel::nodeDefaultValue(_contextProperty);
  TulipItemEditorCreator* creator = _delegate->creator(defaultValue.userType());
  QWidget* w = creator->createWidget(this);
  creator->setEditorData(w,defaultValue,_graph);

  QDialog* dlg = dynamic_cast<QDialog*>(w);
  if (dlg == NULL) {
    dlg = editDialog(w,this);
    dlg->setWindowTitle(_contextProperty->getName().c_str() + trUtf8(": Set all nodes values"));
  }
  if (dlg->exec() == QDialog::Accepted)
    GraphModel::setAllNodeValue(_contextProperty,creator->editorData(w,_graph));
  delete dlg;
}

void GraphPropertiesEditor::setAllEdges() {
  QVariant defaultValue = GraphModel::edgeDefaultValue(_contextProperty);
  TulipItemEditorCreator* creator = _delegate->creator(defaultValue.userType());
  QWidget* w = creator->createWidget(this);
  creator->setEditorData(w,defaultValue,_graph);

  QDialog* dlg = editDialog(w,this);
  dlg->setWindowTitle(_contextProperty->getName().c_str() + trUtf8(": Set all edges values"));

  if (dlg->exec() == QDialog::Accepted)
    GraphModel::setAllEdgeValue(_contextProperty,creator->editorData(w,_graph));

  delete dlg;
}

void GraphPropertiesEditor::copyProperty() {
  CopyPropertyDialog::copyProperty(_graph,_contextProperty,true,this);
}

void GraphPropertiesEditor::delProperty() {
  _contextProperty->getGraph()->delLocalProperty(_contextProperty->getName());
}

void GraphPropertiesEditor::setPropertyFilter(QString s) {
  QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel*>(_ui->propertiesList->model());
  proxyModel->setFilterFixedString(s);
}

void GraphPropertiesEditor::toLabels() {
  DataSet data;
  data.set("nodes",true);
  data.set("edges",true);
  data.set("input",_contextProperty);
  std::string msg;
  StringProperty* result = _graph->getProperty<StringProperty>("viewLabel");
  _graph->computeProperty<StringProperty>("To labels",result,msg,NULL,&data);
}
