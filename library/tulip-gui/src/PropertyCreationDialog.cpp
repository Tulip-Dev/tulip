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
#include "tulip/PropertyCreationDialog.h"
#include "ui_PropertyCreationDialog.h"

#include <QMessageBox>
#include <QPushButton>

#include <tulip/Graph.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;

PropertyCreationDialog::PropertyCreationDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PropertyCreationDialog),_createPropertyButton(NULL),_graph(NULL),_createdProperty(NULL) {
  initGui();
}

PropertyCreationDialog::PropertyCreationDialog(Graph* graph, QWidget *parent,
    const std::string& selectedType):
  QDialog(parent),
  ui(new Ui::PropertyCreationDialog),_graph(graph),_createdProperty(NULL) {
  initGui();

  if (!selectedType.empty()) {
    int selectedIndex = ui->propertyTypeComboBox->findText(propertyTypeToPropertyTypeLabel(selectedType));

    if (selectedIndex != -1)
      ui->propertyTypeComboBox->setCurrentIndex(selectedIndex);
  }
}

void PropertyCreationDialog::initGui() {
  ui->setupUi(this);
  QStringList labels;
  //Init properties list
  labels<<propertyTypeToPropertyTypeLabel("color");
  labels<<propertyTypeToPropertyTypeLabel("int");
  labels<<propertyTypeToPropertyTypeLabel("layout");
  labels<<propertyTypeToPropertyTypeLabel("double");
  labels<<propertyTypeToPropertyTypeLabel("bool");
  labels<<propertyTypeToPropertyTypeLabel("size");
  labels<<propertyTypeToPropertyTypeLabel("string");
  labels<<propertyTypeToPropertyTypeLabel("vector<bool>");
  labels<<propertyTypeToPropertyTypeLabel("vector<color>");
  labels<<propertyTypeToPropertyTypeLabel("vector<coord>");
  labels<<propertyTypeToPropertyTypeLabel("vector<double>");
  labels<<propertyTypeToPropertyTypeLabel("vector<int>");
  labels<<propertyTypeToPropertyTypeLabel("vector<size>");
  labels<<propertyTypeToPropertyTypeLabel("vector<string>");
  ui->propertyTypeComboBox->addItems(labels);
  _createPropertyButton = ui->buttonBox->addButton(tr("Create"),QDialogButtonBox::AcceptRole);
  ui->errorIconLabel->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(16,16));
  connect(ui->propertyNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(checkValidity()));
  checkValidity();
}

PropertyCreationDialog::~PropertyCreationDialog() {
  delete ui;
}

void PropertyCreationDialog::setGraph(Graph* graph) {
  _graph = graph;
  checkValidity();
}

void PropertyCreationDialog::accept() {
  bool error = false;

  if(_graph == NULL) {
    QMessageBox::warning(this, "Fail to create property", "The parent graph is invalid",
                         QMessageBox::Ok, QMessageBox::Ok);
    error = true;
  }

  QString propertyName = ui->propertyNameLineEdit->text();

  if(propertyName.isEmpty()) {
    QMessageBox::warning(this, "Fail to create property", "You can't create a property with an empty name",
                         QMessageBox::Ok, QMessageBox::Ok);
    error = true;
  }

  if (_graph->existLocalProperty(QStringToTlpString(propertyName))) {
    QMessageBox::warning(this, "Fail to create property", "A property with the same name already exist", QMessageBox::Ok,
                         QMessageBox::Ok);
    error = true;
  }

  if(!error) {
    _graph->push();
    _createdProperty = _graph->getLocalProperty(QStringToTlpString(propertyName),propertyTypeLabelToPropertyType(ui->propertyTypeComboBox->currentText()));
  }

  QDialog::accept();
}

PropertyInterface* PropertyCreationDialog::createNewProperty(tlp::Graph* graph,QWidget* parent, const std::string& selectedType) {
  PropertyCreationDialog *dialog = new PropertyCreationDialog(graph,parent,selectedType);
  PropertyInterface* result = NULL;

  if(dialog->exec() == QDialog::Accepted) {
    result = dialog->createdProperty();
  }

  dialog->deleteLater();
  return result;
}

void PropertyCreationDialog::checkValidity() {
  QString propertyName = ui->propertyNameLineEdit->text();

  if(_graph == NULL) {
    ui->errorLabel->setText(tr("You need to specify a parent graph"));
    ui->errorNotificationWidget->setVisible(true);
    _createPropertyButton->setEnabled(false);
    return;
  }
  else if(propertyName.isEmpty()) {
    ui->errorLabel->setText(tr("You can't create a property with an empty name"));
    _createPropertyButton->setEnabled(false);
    ui->errorNotificationWidget->setVisible(true);
    return;
  }
  else if (_graph->existLocalProperty(QStringToTlpString(propertyName))) {
    ui->errorLabel->setText(tr("A property with the same name already exist"));
    _createPropertyButton->setEnabled(false);
    ui->errorNotificationWidget->setVisible(true);
    return;
  }

  ui->errorNotificationWidget->setVisible(false);
  _createPropertyButton->setEnabled(true);
}
