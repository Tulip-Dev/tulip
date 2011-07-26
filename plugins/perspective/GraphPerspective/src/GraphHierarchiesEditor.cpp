#include "GraphHierarchiesEditor.h"

#include "GraphHierarchiesModel.h"
#include "ui_GraphHierarchiesEditor.h"

#include <assert.h>

GraphHierarchiesEditor::GraphHierarchiesEditor(GraphHierarchiesModel *model, QWidget *parent): QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData), _model(model) {
  assert(_model);

  _ui->setupUi(this);
  setWindowFlags(Qt::Popup);
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}
