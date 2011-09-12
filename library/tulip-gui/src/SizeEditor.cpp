#include "tulip/SizeEditor.h"

#include "ui_SizeEditor.h"

using namespace tlp;

SizeEditor::SizeEditor(QWidget *parent): QWidget(parent), _ui(new Ui::SizeEditorData) {
  _ui->setupUi(this);
}

SizeEditor::~SizeEditor() {
  delete _ui;
}

void SizeEditor::setSize(const tlp::Size &s) {
  _ui->width->setValue(s[0]);
  _ui->height->setValue(s[1]);
  _ui->depth->setValue(s[2]);
}

tlp::Size SizeEditor::size() const {
  return tlp::Size(_ui->width->value(),_ui->height->value(),_ui->depth->value());
}
