/*
	  DoubleStringsListSelectionWidget.cpp

   Created on: 3 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include "tulip/DoubleStringsListSelectionWidget.h"

using namespace std;

namespace tlp {

DoubleStringsListSelectionWidget::DoubleStringsListSelectionWidget(QWidget *parent, const unsigned int maxSelectedStringsListSize) : QWidget(parent) {
	setupUi(this);
	outputList->setMaxListSize(maxSelectedStringsListSize);
	if (maxSelectedStringsListSize != 0) {
		selectButton->setEnabled(false);
	} else {
		selectButton->setEnabled(true);
	}
	qtWidgetsConnection();
}

void DoubleStringsListSelectionWidget::setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList) {
	for (unsigned int i = 0; i < unselectedStringsList.size(); ++i) {
		inputList->addItemList(QString(unselectedStringsList[i].c_str()));
	}
}

void DoubleStringsListSelectionWidget::setSelectedStringsList(const std::vector<std::string> &selectedStringsList) {
	for (unsigned int i = 0; i < selectedStringsList.size(); ++i) {
		outputList->addItemList(QString(selectedStringsList[i].c_str()));
	}
}

void DoubleStringsListSelectionWidget::clearUnselectedStringsList() {
	inputList->clear();
}

void DoubleStringsListSelectionWidget::clearSelectedStringsList() {
	outputList->clear();
}

void DoubleStringsListSelectionWidget::setUnselectedStringsListLabel(const std::string &unselectedStringsListLabel) {
	inputListLabel->setText(QString(unselectedStringsListLabel.c_str()));
}

void DoubleStringsListSelectionWidget::setSelectedStringsListLabel(const std::string &selectedStringsListLabel) {
	outputListLabel->setText(QString(selectedStringsListLabel.c_str()));
}

void DoubleStringsListSelectionWidget::setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize) {
	outputList->setMaxListSize(maxSelectedStringsListSize);
	if (maxSelectedStringsListSize != 0) {
		selectButton->setEnabled(false);
	} else {
		selectButton->setEnabled(true);
	}
}

vector<string> DoubleStringsListSelectionWidget::getSelectedStringsList() const {
	vector<string> outputStringList;
	for (int i = 0; i < outputList->count(); ++i) {
		outputStringList.push_back(outputList->item(i)->text().toStdString());
	}
	return outputStringList;
}

vector<string> DoubleStringsListSelectionWidget::getUnselectedStringsList() const {
	vector<string> inputStringList;
	for (int i = 0; i < inputList->count(); ++i) {
		inputStringList.push_back(inputList->item(i)->text().toStdString());
	}
	return inputStringList;
}

void DoubleStringsListSelectionWidget::selectAllStrings() {
	if (outputList->getMaxListSize() == 0) {
		for (int i = 0 ; i < inputList->count() ; ++i) {
			outputList->addItem(new QListWidgetItem(*(inputList->item(i))));
		}
		clearUnselectedStringsList();
	}
}

void DoubleStringsListSelectionWidget::unselectAllStrings() {
	for (int i = 0 ; i < outputList->count() ; ++i) {
		inputList->addItem(new QListWidgetItem(*(outputList->item(i))));
	}
	clearSelectedStringsList();
}

void DoubleStringsListSelectionWidget::qtWidgetsConnection() {
	connect(addButton, SIGNAL(clicked()),this, SLOT(pressButtonAdd()));
	connect(removeButton, SIGNAL(clicked()),this, SLOT(pressButtonRem()));
	connect(upButton, SIGNAL(clicked()),this, SLOT(pressButtonUp()));
	connect(downButton, SIGNAL(clicked()),this, SLOT(pressButtonDown()));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(pressButtonSelectAll()));
	connect(unselectButton, SIGNAL(clicked()), this, SLOT(pressButtonUnselectAll()));
}

void DoubleStringsListSelectionWidget::pressButtonAdd() {
	if (inputList->currentItem() != NULL) {
		if (outputList->addItemList(inputList->currentItem()->text())) {
			inputList->deleteItemList(inputList->currentItem());
		}
	}
}

void DoubleStringsListSelectionWidget::pressButtonRem() {
	if (outputList->currentItem() != NULL) {
		inputList->addItemList(outputList->currentItem()->text());
		outputList->deleteItemList(outputList->currentItem());
	}
}

void DoubleStringsListSelectionWidget::pressButtonUp() {
	if (outputList->count() > 0) {
		int row = outputList->currentRow();
	    if (row > 0) {
	      QString s = outputList->currentItem()->text();
	      QString s2 = outputList->item(row - 1)->text();
	      outputList->deleteItemList(outputList->item(row - 1));
	      outputList->deleteItemList(outputList->item(row - 1));
	      outputList->insertItem(row - 1, s2);
	      outputList->insertItem(row - 1, s);
	      outputList->setCurrentRow(row - 1);
	    }
	}
}

void DoubleStringsListSelectionWidget::pressButtonDown() {
	if (outputList->count() > 0) {
	    int row = outputList->currentRow();
	    if (row < (outputList->count() - 1)) {
	      QString s = outputList->currentItem()->text();
	      QString s2 = outputList->item(row + 1)->text();
	      outputList->deleteItemList(outputList->item(row));
	      outputList->deleteItemList(outputList->item(row));
	      outputList->insertItem(row, s);
	      outputList->insertItem(row, s2);
	      outputList->setCurrentRow(row + 1);
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
