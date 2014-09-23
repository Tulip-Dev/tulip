/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include "AddressSelectionDialog.h"
#include "ui_AddressSelectionDialog.h"

namespace tlp {

AddressSelectionDialog::AddressSelectionDialog(QWidget *parent) : QDialog(parent),_ui(new Ui::AddressSelectionDialogData) {
  _ui->setupUi(this);
  connect(_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

AddressSelectionDialog::~AddressSelectionDialog() {
  delete _ui;
}

void AddressSelectionDialog::setBaseAddress(const QString &address) {
  _ui->addressLabel->setText(address);
}

void AddressSelectionDialog::addResultToList(const QString &result) {
  _ui->resultsList->addItem(result);
}

void AddressSelectionDialog::clearList() {
  _ui->resultsList->clear();
  _ui->rememberChoiceCB->setChecked(false);
}

bool AddressSelectionDialog::rememberAddressChoice() const {
  return _ui->rememberChoiceCB->isChecked();
}

int AddressSelectionDialog::getPickedResultIdx() {
  return _ui->resultsList->currentRow();
}
}
