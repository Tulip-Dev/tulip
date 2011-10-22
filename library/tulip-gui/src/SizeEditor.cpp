/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
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
