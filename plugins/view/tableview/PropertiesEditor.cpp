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

#include "ui_PropertiesEditor.h"

using namespace tlp;

PropertiesEditor::PropertiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::PropertiesEditor), _graph(NULL), _delegate(new tlp::TulipItemDelegate), _sourceModel(NULL) {
  _ui->setupUi(this);
}

PropertiesEditor::~PropertiesEditor() {
  delete _ui;
}

void PropertiesEditor::setGraph(tlp::Graph *g) {
  _graph=g;
  QSortFilterProxyModel* model = new QSortFilterProxyModel();
  _sourceModel = new GraphPropertiesModel<PropertyInterface>(g,true);
  model->setSourceModel(_sourceModel);
  model->setFilterCaseSensitivity(Qt::CaseInsensitive);
  connect(_ui->lineEdit,SIGNAL(textChanged(QString)),model,SLOT(setFilterFixedString(QString)));
  _ui->tableView->setModel(model);
  connect(_sourceModel,SIGNAL(checkStateChanged(QModelIndex,Qt::CheckState)),this,SLOT(checkStateChanged(QModelIndex,Qt::CheckState)));
}

void PropertiesEditor::showCustomContextMenu(const QPoint& p) {
  _contextProperty = _ui->tableView->indexAt(p).data(TulipModel::PropertyRole).value<PropertyInterface*>();
  QMenu menu;
  connect(menu.addAction(trUtf8("Check all")),SIGNAL(triggered()),this,SLOT(checkAll()));
  connect(menu.addAction(trUtf8("Uncheck all")),SIGNAL(triggered()),this,SLOT(unCheckAll()));
  connect(menu.addAction(trUtf8("Uncheck all except \"") + _contextProperty->getName().c_str() + "\""),SIGNAL(triggered()),this,SLOT(unCheckAllExcept()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Set all nodes")),SIGNAL(triggered()),this,SLOT(setAllNodes()));
  connect(menu.addAction(trUtf8("Set all edges")),SIGNAL(triggered()),this,SLOT(setAllEdges()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("To labels")),SIGNAL(triggered()),this,SLOT(toLabels()));
  menu.addSeparator();
  connect(menu.addAction(trUtf8("Copy")),SIGNAL(triggered()),this,SLOT(copyProperty()));
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
    dlg->setWindowTitle(_contextProperty->getName().c_str() + trUtf8(": Set all nodes values"));
  }

  if (dlg->exec() == QDialog::Accepted)
    GraphModel::setAllNodeValue(_contextProperty,creator->editorData(w,_graph));

  delete dlg;
}

void PropertiesEditor::setAllEdges() {
  QVariant defaultValue = GraphModel::edgeDefaultValue(_contextProperty);
  TulipItemEditorCreator* creator = _delegate->creator(defaultValue.userType());
  QWidget* w = creator->createWidget(Perspective::instance()->mainWindow());
  creator->setEditorData(w,defaultValue,_graph);

  QDialog* dlg = editDialog(w,Perspective::instance()->mainWindow());
  dlg->setWindowTitle(_contextProperty->getName().c_str() + trUtf8(": Set all edges values"));

  if (dlg->exec() == QDialog::Accepted)
    GraphModel::setAllEdgeValue(_contextProperty,creator->editorData(w,_graph));

  delete dlg;
}

void PropertiesEditor::copyProperty() {
  CopyPropertyDialog::copyProperty(_graph,_contextProperty,true,Perspective::instance()->mainWindow());
}

void PropertiesEditor::delProperty() {
  _contextProperty->getGraph()->delLocalProperty(_contextProperty->getName());
}

void PropertiesEditor::toLabels() {
  DataSet data;
  data.set("nodes",true);
  data.set("edges",true);
  data.set("input",_contextProperty);
  std::string msg;
  StringProperty* result = _graph->getProperty<StringProperty>("viewLabel");
  _graph->computeProperty<StringProperty>("To labels",result,msg,NULL,&data);
}

void PropertiesEditor::checkStateChanged(QModelIndex index,Qt::CheckState state) {
  PropertyInterface* pi = _sourceModel->data(index,TulipModel::PropertyRole).value<PropertyInterface*>();
  emit propertyVisibilityChanged(pi,state == Qt::Checked);
}
