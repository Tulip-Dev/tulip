#include "tulip/SceneLayersConfigWidget.h"

#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/SceneLayersModel.h>
#include "ui_SceneLayersConfigWidget.h"

using namespace tlp;

SceneLayersConfigWidget::SceneLayersConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneLayersConfigWidget), _view(NULL) {
  _ui->setupUi(this);
}

SceneLayersConfigWidget::~SceneLayersConfigWidget() {
  delete _ui;
}

void SceneLayersConfigWidget::setView(GlMainView *view) {
  _view = view;
  SceneLayersModel* model = new SceneLayersModel(_view->getGlMainWidget()->getScene(),_ui->treeView);
  _ui->treeView->setModel(model);
  connect(model,SIGNAL(drawNeeded(tlp::GlScene*)),this,SIGNAL(drawNeeded()));
}
