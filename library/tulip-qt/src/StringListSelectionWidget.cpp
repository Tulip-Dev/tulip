/*
 * StringListSelectionWidget.cpp
 *
 *  Created on: 3 févr. 2009
 *      Author: antoine
 */

#include "tulip/StringListSelectionWidget.h"

using namespace std;

namespace tlp {

StringListSelectionWidget::StringListSelectionWidget(QWidget *parent,
    const unsigned int maxOutputListSize) :
  QWidget(parent) {
  setupUi(this);
  outputList->setMaxListSize(maxOutputListSize);
  qtWidgetsConnection();
}

StringListSelectionWidget::StringListSelectionWidget(
    const vector<string> &inputStringList, QWidget *parent,
    const unsigned int maxOutputListSize) :
  QWidget(parent) {
  setupUi(this);
  outputList->setMaxListSize(maxOutputListSize);
  qtWidgetsConnection();
  setInputStringList(inputStringList);
}

void StringListSelectionWidget::qtWidgetsConnection() {
  connect(addButton, SIGNAL(clicked()),this, SLOT(pressButtonAdd()));
  connect(removeButton, SIGNAL(clicked()),this, SLOT(pressButtonRem()));
  connect(upButton, SIGNAL(clicked()),this, SLOT(pressButtonUp()));
  connect(downButton, SIGNAL(clicked()),this, SLOT(pressButtonDown()));
}

void StringListSelectionWidget::setInputStringList(
    const vector<string> &inputStringList) {
  for (unsigned int i = 0; i < inputStringList.size(); ++i) {
    inputList->addItemList(QString(inputStringList[i].c_str()));
  }
}

void StringListSelectionWidget::setOutputStringList(
    const vector<string> &outputStringList) {
  for (unsigned int i = 0; i < outputStringList.size(); ++i) {
    outputList->addItemList(QString(outputStringList[i].c_str()));
  }
}

void StringListSelectionWidget::setInputStringListLabel(
    const string &inputStringListLabel) {
  inputListLabel->setText(QString(inputStringListLabel.c_str()));
}

void StringListSelectionWidget::setOutputStringListLabel(
    const string &outputStringListLabel) {
  outputListLabel->setText(QString(outputStringListLabel.c_str()));
}

void StringListSelectionWidget::setMaxOutputListSize(
    const unsigned int maxOutputListSize) {
  outputList->setMaxListSize(maxOutputListSize);
}
vector<string> StringListSelectionWidget::getInputStringList() const {
  vector<string> inputStringList;
  for (int i = 0; i < inputList->count(); ++i) {
    inputStringList.push_back(inputList->item(i)->text().toStdString());
  }
  return inputStringList;
}

vector<string> StringListSelectionWidget::getOutputStringList() const {
  vector<string> outputStringList;
  for (int i = 0; i < outputList->count(); ++i) {
    outputStringList.push_back(outputList->item(i)->text().toStdString());
  }
  return outputStringList;
}

void StringListSelectionWidget::clearInputStringList() {
  inputList->clear();
}

void StringListSelectionWidget::clearOutputStringList() {
  outputList->clear();
}

void StringListSelectionWidget::pressButtonAdd() {
  if (inputList->currentItem() != NULL) {
    if (outputList->addItemList(inputList->currentItem()->text())) {
      inputList->deleteItemList(inputList->currentItem());
    }
  }
}

void StringListSelectionWidget::pressButtonRem() {
  if (outputList->currentItem() != NULL) {
    inputList->addItemList(outputList->currentItem()->text());
    outputList->deleteItemList(outputList->currentItem());
  }
}

void StringListSelectionWidget::pressButtonUp() {
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

void StringListSelectionWidget::pressButtonDown() {
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

}
