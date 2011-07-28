#include "GraphHierarchiesEditor.h"

#include "GraphHierarchiesModel.h"
#include "ui_GraphHierarchiesEditor.h"

#include <assert.h>

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData) {
  _ui->setupUi(this);
}

void GraphHierarchiesEditor::setModel(GraphHierarchiesModel *model) {
  _ui->hierarchiesTree->setModel(model);
  _ui->hierarchiesTree->header()->resizeSections(QHeaderView::ResizeToContents);
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}
