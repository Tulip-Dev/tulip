/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <QPushButton>

#include "AddressSelectionDialog.h"
#include "ui_AddressSelectionDialog.h"
#include <tulip/TlpQtTools.h>

namespace tlp {

AddressSelectionDialog::AddressSelectionDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::AddressSelectionDialogData) {
  _ui->setupUi(this);
  // fix display of QCheckBox and QRadioButton children
  tlpFixCBRBs(this);

  _ui->buttonBox->button(QDialogButtonBox::Abort)->setToolTip("abort the Geolocation process");
}

AddressSelectionDialog::~AddressSelectionDialog() {
  delete _ui;
}

void AddressSelectionDialog::setBaseAddress(const QString &address) {
  _ui->addressLabel->setText(QString("<b>%1</b>").arg(address));
}

void AddressSelectionDialog::addResultToList(const QString &result) {
  _ui->resultsList->addItem(result);
}

void AddressSelectionDialog::clearList() {
  _ui->resultsList->clear();
  _ui->rememberChoiceCB->setChecked(true);
}

bool AddressSelectionDialog::rememberAddressChoice() const {
  return _ui->rememberChoiceCB->isChecked();
}

int AddressSelectionDialog::getPickedResultIdx() {
  return _ui->resultsList->currentRow();
}

void AddressSelectionDialog::showEvent(QShowEvent *showEvt) {
  if (_ui->resultsList->count() > 0) {
    _ui->resultsList->setCurrentRow(0);
  }
  QDialog::showEvent(showEvt);
}

} // namespace tlp
