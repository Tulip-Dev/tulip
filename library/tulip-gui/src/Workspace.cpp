#include "tulip/Workspace.h"

#include "ui_Workspace.h"

Workspace::Workspace(QWidget *parent)
  : QWidget(parent), _ui(new Ui::Workspace) {
  _ui->setupUi(this);
}
