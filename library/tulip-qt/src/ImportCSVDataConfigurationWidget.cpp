/*
 * ImportCSVDataConfigurationWidget.cpp
 *
 *  Created on: 15 févr. 2010
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU Lesser General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */
#include "tulip/ImportCSVDataConfigurationWidget.h"
#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QScrollBar>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QTableWidget>
#include <cassert>
#include <iostream>
using namespace tlp;
using namespace std;
ImportCSVDataConfigurationWidget::ImportCSVDataConfigurationWidget(QWidget *parent) :
  QWidget(parent), propertyNameValidator(new PropertyNameValidator(this)) {
  setupUi(this);
  //Build and add the preview widget.
  QGroupBox *previewGroupBox = new QGroupBox(tr("Preview"), this);
  previewGroupBox->setLayout(new QVBoxLayout(this));
  previewWidget = buildPreviewWidget(previewGroupBox);
  previewWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  previewGroupBox->layout()->addWidget(previewWidget);
  previewGroupBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  layout()->addWidget(previewGroupBox);
}

ImportCSVDataConfigurationWidget::~ImportCSVDataConfigurationWidget() {
}

void ImportCSVDataConfigurationWidget::browseNewCsvFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CSV file"), QString(), tr(
      "CSV files (*.txt *.csv)"));
  csvFileLineEdit->setText(fileName);
  emit csvFileChange(csvFileLineEdit->text());
}

void ImportCSVDataConfigurationWidget::lineEditEditionFinished() {
  if (sender() == csvFileLineEdit) {
    emit csvFileChange(csvFileLineEdit->text());
  }
  else if (sender() == separatorLineEdit) {
    emit separatorChange(separatorLineEdit->text());
  }
}

void ImportCSVDataConfigurationWidget::useFirstRowAsPropertyNameStateChanged(int state) {
  emit useFirstRowAsPropertyNameChange(useFirstRowAsPropertyName());
}

void ImportCSVDataConfigurationWidget::clearPropertiesTypeList() {
  for (vector<PropertyConfigurationWidget*>::iterator it = propertyWidgets.begin(); it != propertyWidgets.end(); ++it) {
    propertyListGroupBox->layout()->removeWidget(*it);
    (*it)->deleteLater();
  }
  propertyWidgets.clear();
  propertiesListScrollArea->setVisible(false);
}
void ImportCSVDataConfigurationWidget::addPropertyToPropertyList(const string& propertyName, bool isEditable,
    PropertyType propertyType) {
  if (!propertiesListScrollArea->isVisible())
    propertiesListScrollArea->setVisible(true);
  PropertyConfigurationWidget *propertyConfigurationWidget = createPropertyConfigurationWidget(propertyWidgets.size(),
      QString::fromStdString(propertyName), isEditable, propertyType, propertiesListScrollAreaWidgetContents);
  propertiesListScrollAreaWidgetContents->layout()->addWidget(propertyConfigurationWidget);
  //Update propertiesListScrollArea height to ensure that properties configuration widgets are fully visible.
  //The height is computed from the minimum property widget configuration height , the scroll bar height and a small spacing (10).
  propertiesListScrollArea->setMinimumHeight(propertyConfigurationWidget->sizeHint().height()
      + propertiesListScrollArea->horizontalScrollBar()->sizeHint().height() + 10);
  propertyWidgets.push_back(propertyConfigurationWidget);
}
PropertyConfigurationWidget *ImportCSVDataConfigurationWidget::createPropertyConfigurationWidget(
    unsigned int propertyNumber, const QString& propertyName, bool isEditable,
    ImportCSVDataConfigurationWidget::PropertyType propertyType, QWidget* parent) {
  PropertyConfigurationWidget *propertyConfigurationWidget = new PropertyConfigurationWidget(propertyNumber,
      propertyName, isEditable, propertyType, parent);
  propertyConfigurationWidget->setPropertyNameValidator(propertyNameValidator);
  propertyConfigurationWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  connect(propertyConfigurationWidget, SIGNAL(propertyNameChange(QString)), this, SLOT(propertyNameChanged(QString)));
  connect(propertyConfigurationWidget, SIGNAL(stateChange(bool)), this, SLOT(propertyStateChanged(bool )));
  return propertyConfigurationWidget;
}

QString ImportCSVDataConfigurationWidget::getPropertyName(unsigned int propertyNumber) const {
  return propertyWidgets[propertyNumber]->getPropertyName();
}

bool ImportCSVDataConfigurationWidget::useProperty(unsigned int propertyNumber) {
  return propertyWidgets[propertyNumber]->getPropertyUsed();
}

ImportCSVDataConfigurationWidget::PropertyType ImportCSVDataConfigurationWidget::getPropertyType(
    unsigned int propertyNumber) const {
  assert(propertyWidgets.size() > propertyNumber);
  return (ImportCSVDataConfigurationWidget::PropertyType)(propertyWidgets[propertyNumber]->getPropertyType());
}

void ImportCSVDataConfigurationWidget::propertyNameChanged(QString newName) {
  //get property configuration widget
  PropertyConfigurationWidget *propConfWidget = qobject_cast<PropertyConfigurationWidget*> (sender());
  if (propConfWidget) {
    propertyNameChanged(propConfWidget->getPropertyNumber(), newName);
  }
}

void ImportCSVDataConfigurationWidget::propertyNameChanged(unsigned int propertyNumber, QString newPropertyName) {
  //Update header label
  if (getPropertyOrientation() == Column) {
    previewWidget->horizontalHeaderItem(propertyNumber)->setText(newPropertyName);
  }
  else {
    previewWidget->verticalHeaderItem(propertyNumber)->setText(newPropertyName);
  }
}

void ImportCSVDataConfigurationWidget::propertyStateChanged(bool state) {
  //get property configuration widget
  PropertyConfigurationWidget *propConfWidget = qobject_cast<PropertyConfigurationWidget*> (sender());
  if (propConfWidget) {
    propertyStateChanged(propConfWidget->getPropertyNumber(), state);
  }
}

void ImportCSVDataConfigurationWidget::propertyStateChanged(unsigned int propertyNumber, bool state) {
  //update the right column or row in the previewWidget.
  if (getPropertyOrientation() == Column) {
    for (int i = 0; i < previewWidget->rowCount(); ++i) {
      previewWidget ->item(i, propertyNumber)->setBackgroundColor(state ? QColor(255, 255, 255, 255) : QColor(200, 200,
          200, 255));
    }
  }
  else {
    for (int i = 0; i < previewWidget->columnCount(); ++i) {
      previewWidget ->item(propertyNumber, i)->setBackgroundColor(state ? QColor(255, 255, 255, 255) : QColor(200, 200,
          200, 255));
    }
  }
}

bool ImportCSVDataConfigurationWidget::checkIfMoreThanOnePropertyHaveTheName(const QString& name) {
  bool foundOnce = false;
  for (vector<PropertyConfigurationWidget*>::iterator it = propertyWidgets.begin(); it != propertyWidgets.end(); ++it) {
    if ((*it)->getPropertyName().compare(name) == 0) {
      if (!foundOnce) {
        foundOnce = true;
      }
      else {
        return true;
      }
    }
  }
  return false;
}

void ImportCSVDataConfigurationWidget::propertyOrientationButtonPressed(QAbstractButton * button) {
  if (button == colAsPropertiesRadioButton) {
    emit propertiesOrientationChange(Column);
  }
  else {
    emit propertiesOrientationChange(Row);
  }
}

void ImportCSVDataConfigurationWidget::begin() {
  previewWidget->clear();
  clearPropertiesTypeList();
  previewWidget->setRowCount(0);
  previewWidget->setColumnCount(0);
}

void ImportCSVDataConfigurationWidget::token(unsigned int row, unsigned int column, const string& token) {

  unsigned int previewNumber = numberOfPreviews();
  if (useFirstRowAsPropertyName()) {
    ++previewNumber;
  }
  unsigned int currentNumber = getPropertyOrientation() == Row ? column : row;
  if (currentNumber >= previewNumber) {
    return;
  }

  //Update preview table size.
  if (previewWidget->rowCount() <= row) {
    previewWidget->insertRow(row);
  }
  if (previewWidget->columnCount() <= column) {
    previewWidget->insertColumn(column);
  }

  //Create the property widget and update the headers.
  if (row == 0 && getPropertyOrientation() == Column) {
    QString propertyName = useFirstRowAsPropertyName() ? QString::fromStdString(token) : QString("Property")
        + QString::number(column);
    addPropertyToPropertyList(propertyName.toStdString(), true);
    previewWidget->setHorizontalHeaderItem(column, new QTableWidgetItem(propertyName));
    //The first row is empty hide them.
    if (useFirstRowAsPropertyName()) {
      previewWidget->hideRow(0);
      return;
    }
  }
  if (column == 0 && getPropertyOrientation() == Row) {
    QString propertyName = useFirstRowAsPropertyName() ? QString::fromStdString(token) : QString("Property")
        + QString::number(row);
    addPropertyToPropertyList(propertyName.toStdString(), useFirstRowAsPropertyName());
    previewWidget->setVerticalHeaderItem(row, new QTableWidgetItem(propertyName));
    //The first column is empty hide them.
    if (useFirstRowAsPropertyName()) {
      previewWidget->hideColumn(0);
      return;
    }
  }
  previewWidget->setItem(row, column, new QTableWidgetItem(QString::fromStdString(token)));
}
void ImportCSVDataConfigurationWidget::end(unsigned int rowNumber, unsigned int columnNumber) {
  //delete empty row and columns created.
  if (useFirstRowAsPropertyName() && getPropertyOrientation() == Row) {
    previewWidget->removeColumn(0);
  }
  if (useFirstRowAsPropertyName() && getPropertyOrientation() == Column) {
    previewWidget->removeRow(0);
  }
}

QTableWidget* ImportCSVDataConfigurationWidget::buildPreviewWidget(QWidget* parent) {
  QTableWidget *tableWidget = new QTableWidget(parent);
  tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
  return tableWidget;
}

PropertyConfigurationWidget::PropertyConfigurationWidget(unsigned int propertyNumber, const QString& propertyName,
    bool propertyNameIsEditable, int PropertyType, QWidget* parent) :
  QWidget(parent), propertyNameLineEdit(new QLineEdit(this)), propertyTypeComboBox(new QComboBox(this)), usedCheckBox(
      new QCheckBox("", this)), nameEditable(propertyNameIsEditable), propertyNumber(propertyNumber) {
  setLayout(new QVBoxLayout());
  layout()->setContentsMargins(0, 0, 0, 0);
  layout()->setSpacing(0);
  layout()->addWidget(usedCheckBox);
  layout()->setAlignment(usedCheckBox, Qt::AlignHCenter);
  connect(usedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useStateChanged(int)));
  usedCheckBox->setCheckState(Qt::Checked);
  propertyNameLineEdit->setText(propertyName);
  propertyNameLineEdit->setEnabled(propertyNameIsEditable);
  propertyNameLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  layout()->addWidget(propertyNameLineEdit);
  layout()->setAlignment(propertyNameLineEdit, Qt::AlignHCenter);
  connect(propertyNameLineEdit, SIGNAL(editingFinished()), this, SLOT(nameEditFinished()));
  fillPropertyTypeComboBox();
  propertyTypeComboBox->setCurrentIndex(PropertyType);
  propertyTypeComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  layout()->addWidget(propertyTypeComboBox);
  layout()->setAlignment(propertyTypeComboBox, Qt::AlignHCenter);

}

void PropertyConfigurationWidget::fillPropertyTypeComboBox() {
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::AutoDetect, "Auto detect");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::Double, "double");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::Layout, "layout");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::String, "string");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::Int, "int");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::Color, "color");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::Size, "size");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::Bool, "bool");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::DoubleVector, "list of double");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::SizeVector, "list of string");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::IntVector, "list of int");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::CoordVector, "list of coord");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::ColorVector, "list of color");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::SizeVector, "list of size");
  propertyTypeComboBox->insertItem(ImportCSVDataConfigurationWidget::BoolVector, "list of bool");
}

int PropertyConfigurationWidget::getPropertyType() const {
  return propertyTypeComboBox->currentIndex();
}
QString PropertyConfigurationWidget::getPropertyName() const {
  return propertyNameLineEdit->text();
}
bool PropertyConfigurationWidget::getPropertyUsed() const {
  return usedCheckBox->checkState() == Qt::Checked;
}
unsigned int PropertyConfigurationWidget::getPropertyNumber() const {
  return propertyNumber;
}
void PropertyConfigurationWidget::nameEditFinished() {
  emit propertyNameChange(propertyNameLineEdit->text());
}

void PropertyConfigurationWidget::useStateChanged(int state) {
  if (state == Qt::Checked) {
    propertyNameLineEdit->setEnabled(nameEditable);
    propertyTypeComboBox->setEnabled(true);
    emit stateChange(true);
  }
  else {
    propertyNameLineEdit->setEnabled(false);
    propertyTypeComboBox->setEnabled(false);
    emit stateChange(false);
  }
}

void PropertyConfigurationWidget::setPropertyNameValidator(QValidator* validator) {
  propertyNameLineEdit->setValidator(validator);
}
QValidator::State PropertyNameValidator::validate(QString & input, int & pos) const {
  if (input.isEmpty() || parent->checkIfMoreThanOnePropertyHaveTheName(input)) {
    return QValidator::Invalid;
  }
  else {
    return QValidator::Acceptable;
  }
}

