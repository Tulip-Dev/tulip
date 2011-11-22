#include "ControlFrame.h"

#include "ui_ControlFrame.h"

ControlFrame::ControlFrame(QWidget* parent): QFrame(parent), _ui(new Ui::ControlFrame) {
  _ui->setupUi(this);
}

ControlFrame::~ControlFrame() {
  delete _ui;
}
