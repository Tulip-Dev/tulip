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

#include <tulip/DoubleStringsListSelectionWidget.h>
#include <tulip/TlpQtTools.h>

#include "ui_DoubleStringsListSelectionWidget.h"

using namespace std;

namespace tlp {

DoubleStringsListSelectionWidget::DoubleStringsListSelectionWidget(QWidget *parent, const unsigned int maxSelectedStringsListSize) : QWidget(parent), _ui(new Ui::DoubleStringsListSelectionData()) {
  _ui->setupUi(this);

  _ui->outputList->setMaxListSize(maxSelectedStringsListSize);
  _ui->selectButton->setEnabled(maxSelectedStringsListSize == 0);

  qtWidgetsConnection();
}

DoubleStringsListSelectionWidget::~DoubleStringsListSelectionWidget() {
  delete _ui;
}

void DoubleStringsListSelectionWidget::setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList) {
  for (unsigned int i = 0; i < unselectedStringsList.size(); ++i) {
    _ui->inputList->addItemList(QString::fromUtf8(unselectedStringsList[i].c_str()));
  }
}

void DoubleStringsListSelectionWidget::setSelectedStringsList(const std::vector<std::string> &selectedStringsList) {
  for (unsigned int i = 0; i < selectedStringsList.size(); ++i) {
    _ui->outputList->addItemList(QString::fromUtf8(selectedStringsList[i].c_str()));
  }
}

void DoubleStringsListSelectionWidget::clearUnselectedStringsList() {
  _ui->inputList->clear();
}

void DoubleStringsListSelectionWidget::clearSelectedStringsList() {
  _ui->outputList->clear();
}

void DoubleStringsListSelectionWidget::setUnselectedStringsListLabel(const std::string &unselectedStringsListLabel) {
  _ui->inputListLabel->setText(QString::fromUtf8(unselectedStringsListLabel.c_str()));
}

void DoubleStringsListSelectionWidget::setSelectedStringsListLabel(const std::string &selectedStringsListLabel) {
  _ui->outputListLabel->setText(QString::fromUtf8(selectedStringsListLabel.c_str()));
}

void DoubleStringsListSelectionWidget::setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize) {
  _ui->outputList->setMaxListSize(maxSelectedStringsListSize);

  if (maxSelectedStringsListSize != 0) {
    _ui->selectButton->setEnabled(false);
  }
  else {
    _ui->selectButton->setEnabled(true);
  }
}

vector<string> DoubleStringsListSelectionWidget::getSelectedStringsList() const {
  vector<string> outputStringList;

  for (int i = 0; i < _ui->outputList->count(); ++i) {
    outputStringList.push_back(tlp::QStringToTlpString(_ui->outputList->item(i)->text()));
  }

  return outputStringList;
}

vector<string> DoubleStringsListSelectionWidget::getUnselectedStringsList() const {
  vector<string> inputStringList;

  for (int i = 0; i < _ui->inputList->count(); ++i) {
    inputStringList.push_back(tlp::QStringToTlpString(_ui->inputList->item(i)->text()));
  }

  return inputStringList;
}

void DoubleStringsListSelectionWidget::selectAllStrings() {
  if (_ui->outputList->getMaxListSize() == 0) {
    for (int i = 0 ; i < _ui->inputList->count() ; ++i) {
      _ui->outputList->addItem(new QListWidgetItem(*(_ui->inputList->item(i))));
    }

    clearUnselectedStringsList();
  }
}

void DoubleStringsListSelectionWidget::unselectAllStrings() {
  for (int i = 0 ; i < _ui->outputList->count() ; ++i) {
    _ui->inputList->addItem(new QListWidgetItem(*(_ui->outputList->item(i))));
  }

  clearSelectedStringsList();
}

void DoubleStringsListSelectionWidget::qtWidgetsConnection() {
  connect(_ui->addButton, SIGNAL(clicked()),this, SLOT(pressButtonAdd()));
  connect(_ui->removeButton, SIGNAL(clicked()),this, SLOT(pressButtonRem()));
  connect(_ui->upButton, SIGNAL(clicked()),this, SLOT(pressButtonUp()));
  connect(_ui->downButton, SIGNAL(clicked()),this, SLOT(pressButtonDown()));
  connect(_ui->selectButton, SIGNAL(clicked()), this, SLOT(pressButtonSelectAll()));
  connect(_ui->unselectButton, SIGNAL(clicked()), this, SLOT(pressButtonUnselectAll()));
}

void DoubleStringsListSelectionWidget::pressButtonAdd() {
  if (_ui->inputList->currentItem() != NULL) {
    if (_ui->outputList->addItemList(_ui->inputList->currentItem()->text())) {
      _ui->inputList->deleteItemList(_ui->inputList->currentItem());
    }
  }
}

void DoubleStringsListSelectionWidget::pressButtonRem() {
  if (_ui->outputList->currentItem() != NULL) {
    _ui->inputList->addItemList(_ui->outputList->currentItem()->text());
    _ui->outputList->deleteItemList(_ui->outputList->currentItem());
  }
}

void DoubleStringsListSelectionWidget::pressButtonUp() {
  if (_ui->outputList->count() > 0) {
    int row = _ui->outputList->currentRow();

    if (row > 0) {
      QString s = _ui->outputList->currentItem()->text();
      QString s2 = _ui->outputList->item(row - 1)->text();
      _ui->outputList->deleteItemList(_ui->outputList->item(row - 1));
      _ui->outputList->deleteItemList(_ui->outputList->item(row - 1));
      _ui->outputList->insertItem(row - 1, s2);
      _ui->outputList->insertItem(row - 1, s);
      _ui->outputList->setCurrentRow(row - 1);
    }
  }
}

void DoubleStringsListSelectionWidget::pressButtonDown() {
  if (_ui->outputList->count() > 0) {
    int row = _ui->outputList->currentRow();

    if (row != -1 && row < (_ui->outputList->count() - 1)) {
      QString s = _ui->outputList->currentItem()->text();
      QString s2 = _ui->outputList->item(row + 1)->text();
      _ui->outputList->deleteItemList(_ui->outputList->item(row));
      _ui->outputList->deleteItemList(_ui->outputList->item(row));
      _ui->outputList->insertItem(row, s);
      _ui->outputList->insertItem(row, s2);
      _ui->outputList->setCurrentRow(row + 1);
    }
  }
}

void DoubleStringsListSelectionWidget::pressButtonSelectAll() {
  selectAllStrings();
}

void DoubleStringsListSelectionWidget::pressButtonUnselectAll() {
  unselectAllStrings();
}

}
