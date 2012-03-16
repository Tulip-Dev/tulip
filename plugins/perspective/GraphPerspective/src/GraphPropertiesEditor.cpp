#include "GraphPropertiesEditor.h"
#include "ui_GraphPropertiesEditor.h"

#include <QtGui/QSortFilterProxyModel>
#include <tulip/Graph.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/GraphPropertiesModel.h>

using namespace tlp;

GraphPropertiesEditor::GraphPropertiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphPropertiesEditor) {
  _ui->setupUi(this);
}

void GraphPropertiesEditor::setModel(tlp::GraphHierarchiesModel* model) {
  disconnect(this,SLOT(setGraph(tlp::Graph*)));
  connect(model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(setGraph(tlp::Graph*)));
  setGraph(model->currentGraph());
}

void GraphPropertiesEditor::setGraph(tlp::Graph* graph) {
  QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(new GraphPropertiesModel<PropertyInterface>(graph));
  _ui->propertiesList->setModel(proxyModel);
  proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
}
