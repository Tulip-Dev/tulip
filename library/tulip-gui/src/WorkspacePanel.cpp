#include "tulip/WorkspacePanel.h"

#include "ui_WorkspacePanel.h"

using namespace tlp;

WorkspacePanel::WorkspacePanel(QWidget *parent)
  : QWidget(parent), _ui(new Ui::WorkspacePanel) {
  _ui->setupUi(this);
}

WorkspacePanel::~WorkspacePanel() {
}
