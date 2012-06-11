#include "tulip/SceneLayersConfigWidget.h"

#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/SceneLayersModel.h>
#include "ui_SceneLayersConfigWidget.h"

using namespace tlp;

SceneLayersConfigWidget::SceneLayersConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneLayersConfigWidget), _glMainWidget(NULL) {
  _ui->setupUi(this);
}

SceneLayersConfigWidget::~SceneLayersConfigWidget() {
  delete _ui;
}

void SceneLayersConfigWidget::setGlMainWidget(GlMainWidget *glMainWidget) {
  _glMainWidget = glMainWidget;
  SceneLayersModel* model = new SceneLayersModel(_glMainWidget->getScene(),_ui->treeView);
  _ui->treeView->setModel(model);
  connect(model,SIGNAL(drawNeeded(tlp::GlScene*)),this,SIGNAL(drawNeeded()));
}
