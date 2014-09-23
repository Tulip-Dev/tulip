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

#include <QString>
#include <QStringList>
#include <QListView>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QTabWidget>

#include "ui_CopyPropertyDialog.h"

#include <tulip/CopyPropertyDialog.h>
#include <tulip/TlpQtTools.h>
#include <tulip/ForEach.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

#define COPY_PROPERTY(TYPE,GRAPH,SOURCE,NAME,SCOPE) if (typeid((*SOURCE)) == typeid(TYPE)) { \
TYPE* newProperty = SCOPE==INHERITED?GRAPH->getSuperGraph()->getProperty<TYPE>(NAME):GRAPH->getLocalProperty<TYPE>(NAME); \
*newProperty = *(static_cast<TYPE*>(SOURCE)); \
property = newProperty; \
}

//=============================================================================
CopyPropertyDialog::CopyPropertyDialog(QWidget* parent)
  : QDialog(parent),ui(new Ui::CopyPropertyDialogData()),_graph(NULL),_source(NULL) {
  ui->setupUi(this);
  connect(ui->buttonOK , SIGNAL(clicked()), this, SLOT(accept()) );
  connect(ui->buttonCancel , SIGNAL(clicked()), this, SLOT(reject()) );
  ui->errorIconLabel->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(16,16));
  connect(ui->newPropertyNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(checkValidity()));
  connect(ui->buttonGroup,SIGNAL(buttonClicked (int)),this,SLOT(checkValidity()));
  checkValidity();
}

CopyPropertyDialog::~CopyPropertyDialog() {
  delete ui;
}

void CopyPropertyDialog::init(Graph* graph,PropertyInterface* source) {
  _graph = graph;
  _source = source;
  ui->newPropertyRadioButton->setChecked(true);

  if(_graph != NULL) {
    PropertyInterface* property;
    Graph *parent = _graph->getSuperGraph();

    if (parent == graph) {
      parent = 0;
    }

    forEach(property,_graph->getObjectProperties()) {
      // type must be the same
      if (property->getTypename() == source->getTypename()) {
        // Check if name is different
        if (source->getName() != property->getName() &&
            _graph->existLocalProperty(property->getName()))
          ui->localPropertiesComboBox->addItem(tlpStringToQString(property->getName()));

        // check if inherited exists
        if (parent && parent->existProperty(property->getName())) {
          ui->inheritedPropertiesComboBox->addItem(tlpStringToQString(property->getName()));
        }
      }
    }

    if(ui->localPropertiesComboBox->count()==0) {
      ui->localPropertyRadioButton->setEnabled(false);
    }
    else {
      ui->localPropertyRadioButton->setEnabled(true);
    }

    if(ui->inheritedPropertiesComboBox->count()==0) {
      ui->inheritedPropertyRadioButton->setEnabled(false);
    }
    else {
      ui->inheritedPropertyRadioButton->setEnabled(true);
    }
  }

  checkValidity();
}

PropertyInterface* CopyPropertyDialog::copyProperty(QString& errorMsg) {
  PropertyInterface* property = NULL;
  QString propertyName;
  bool valid = true;

  //Check if parameters are valid.
  if(_graph==NULL) {
    valid = false;
    errorMsg = tr("Invalid graph");
  }
  else if(_source == NULL) {
    valid = false;
    errorMsg = tr("Invalid source property");
  }
  else if(ui->newPropertyRadioButton->isChecked()) {
    propertyName = ui->newPropertyNameLineEdit->text();

    if(propertyName.isEmpty()) {
      valid = false;
      errorMsg = tr("Cannot create a property with an empty name");
    }
    else if(_graph->existProperty(QStringToTlpString(propertyName))) {
      PropertyInterface* existingProperty = _graph->getProperty(QStringToTlpString(propertyName));

      if(typeid(*existingProperty) != typeid(*_source)) {
        valid = false;
        errorMsg = tr("A property with the same name but a different type already exists");
      }
    }
  }
  else if(ui->localPropertyRadioButton->isChecked()) {
    propertyName = ui->localPropertiesComboBox->currentText();

    if(propertyName.isEmpty()) {
      valid = false;
      errorMsg = tr("No properties available");
    }
  }
  else {
    propertyName = ui->inheritedPropertiesComboBox->currentText();

    if(propertyName.isEmpty()) {
      valid = false;
      errorMsg = tr("No properties available");
    }
  }

  if(valid) {
    string tulipPropertyName = QStringToTlpString(propertyName);
    CopyPropertyDialog::PropertyScope destinationScope = destinationPropertyScope();
    _graph->push();

    COPY_PROPERTY(DoubleProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(LayoutProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(StringProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(BooleanProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(IntegerProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(ColorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(SizeProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(DoubleVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(CoordVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(StringVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(BooleanVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(IntegerVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(ColorVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
    COPY_PROPERTY(SizeVectorProperty,_graph,_source,tulipPropertyName,destinationScope);
  }

  return property;
}

PropertyInterface* CopyPropertyDialog::copyProperty(Graph* graph,PropertyInterface* toCopy,bool askBeforePropertyOverwriting,QWidget* parent) {
  PropertyInterface* property = NULL;
  CopyPropertyDialog dialog(parent);
  dialog.setWindowTitle(tr("Copy property ")+tlpStringToQString(toCopy->getName()));
  dialog.init(graph,toCopy);

  if(dialog.exec() == QDialog::Accepted) {
    QString errorMsg;
    bool copy = true;

    if(askBeforePropertyOverwriting && dialog.destinationPropertyScope()== CopyPropertyDialog::NEW) {
      QString selectedPropertyName = dialog.destinationPropertyName();

      if(graph->existProperty(QStringToTlpString(selectedPropertyName))) {
        if (QMessageBox::question(parent, "Copy confirmation", QString("Property ")
                                  + selectedPropertyName + " already exists,\ndo you really want to overwrite it ?", QMessageBox::Ok,
                                  QMessageBox::Cancel) == QDialog::Rejected) {
          copy = false;
        }
      }
    }

    if(copy) {
      PropertyInterface* createdProperty = dialog.copyProperty(errorMsg);

      if(createdProperty == NULL) {
        QMessageBox::critical(parent,tr("Error during the copy"),errorMsg);
      }

      property = createdProperty;
    }
  }

  return property;
}

void CopyPropertyDialog::checkValidity() {
  bool valid = true;
  QString errorMsg;

  if(_graph==NULL) {
    valid = false;
    errorMsg = tr("Invalid graph");
  }
  else if(_source == NULL) {
    valid = false;
    errorMsg = tr("Invalid source property");
  }
  else if(ui->newPropertyRadioButton->isChecked()) {
    QString propertyName = ui->newPropertyNameLineEdit->text();

    if(propertyName.isEmpty()) {
      valid = false;
      errorMsg = tr("Cannot create a property with an empty name");
    }
    else if(_graph->existProperty(QStringToTlpString(propertyName))) {
      PropertyInterface* existingProperty = _graph->getProperty(QStringToTlpString(propertyName));

      if(typeid(*existingProperty) != typeid(*_source)) {
        valid = false;
        errorMsg = tr("A property with the same name but a different type already exists");
      }
    }
  }
  else if(ui->localPropertyRadioButton->isChecked()) {
    if(ui->localPropertiesComboBox->currentText().isEmpty()) {
      valid = false;
      errorMsg = tr("No properties available");
    }
  }
  else {
    if(ui->inheritedPropertiesComboBox->currentText().isEmpty()) {
      valid = false;
      errorMsg = tr("No properties available");
    }
  }

  ui->errorNotificationWidget->setVisible(!errorMsg.isEmpty());
  ui->errorLabel->setText(errorMsg);
  ui->buttonOK->setEnabled(valid);
}

QString CopyPropertyDialog::destinationPropertyName()const {
  if(_graph==NULL || _source == NULL) {
    return QString();
  }

  QString propertyName;

  if(ui->newPropertyRadioButton->isChecked()) {
    propertyName = ui->newPropertyNameLineEdit->text();
  }
  else if(ui->localPropertyRadioButton->isChecked()) {
    propertyName = ui->localPropertiesComboBox->currentText();
  }
  else {
    propertyName = ui->inheritedPropertiesComboBox->currentText();
  }

  return propertyName;
}

CopyPropertyDialog::PropertyScope CopyPropertyDialog::destinationPropertyScope()const {
  if(ui->newPropertyRadioButton->isChecked()) {
    return NEW;
  }
  else if(ui->localPropertyRadioButton->isChecked()) {
    return LOCAL;
  }
  else {
    return INHERITED;
  }
}
