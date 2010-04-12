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
#include <cassert>
#include <iostream>
using namespace tlp;
using namespace std;
ImportCSVDataConfigurationWidget::ImportCSVDataConfigurationWidget(QWidget *parent) :
  QWidget(parent), propertyNameValidator(new PropertyNameValidator(this)) {
  setupUi(this);
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
  propertiesListScrollArea->setMinimumHeight(propertyConfigurationWidget->sizeHint().height()+propertiesListScrollArea->horizontalScrollBar()->sizeHint().height() + 10);
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
  PropertyConfigurationWidget *propConfWidget = qobject_cast<PropertyConfigurationWidget*> (sender());
  if (propConfWidget) {
    for (int i = 0; i < propertyWidgets.size(); ++i) {
      emit propertyNameChange(propConfWidget->getPropertyNumber(), newName);
    }
  }
}

void ImportCSVDataConfigurationWidget::propertyStateChanged(bool state) {
  PropertyConfigurationWidget *propConfWidget = qobject_cast<PropertyConfigurationWidget*> (sender());
  if (propConfWidget) {
    emit propertyStateChange(propConfWidget->getPropertyNumber(), state);
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

