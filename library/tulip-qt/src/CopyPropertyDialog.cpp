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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "CopyPropertyDialogData.h"

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qtabwidget.h>

#include "tulip/CopyPropertyDialog.h"
#include "tulip/TlpQtTools.h"
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

void CopyPropertyDialog::init(Graph* graph,PropertyInterface* source){
    _graph = graph;
    _source = source;
    ui->newPropertyRadioButton->setChecked(true);
    if(_graph != NULL){
        PropertyInterface* property;
        Graph *parent = _graph->getSuperGraph();
        if (parent == graph){
            parent = 0;
        }
        forEach(property,_graph->getLocalObjectProperties()){
            //Check if the same type and different name.
            if(typeid(*property) == typeid(*source) && source->getName() != property->getName()){
                ui->localPropertiesComboBox->addItem(tlpStringToQString(property->getName()));
            }
            if (parent && parent->existProperty(property->getName())){
                ui->inheritedPropertiesComboBox->addItem(tlpStringToQString(property->getName()));
            }
        }
        if(ui->localPropertiesComboBox->count()==0){
            ui->localPropertyRadioButton->setEnabled(false);
        }else{
            ui->localPropertyRadioButton->setEnabled(true);
        }

        forEach(property,_graph->getInheritedObjectProperties()){
            //Check if the same type and different name.
            if(typeid(*property) == typeid(*source) && source->getName() != property->getName()){
                ui->inheritedPropertiesComboBox->addItem(tlpStringToQString(property->getName()));
            }
        }
        if(ui->inheritedPropertiesComboBox->count()==0){
            ui->inheritedPropertyRadioButton->setEnabled(false);
        }else{
            ui->inheritedPropertyRadioButton->setEnabled(true);
        }
    }
    checkValidity();
}

PropertyInterface* CopyPropertyDialog::copyProperty(QString& errorMsg){
    PropertyInterface* property = NULL;    
    QString propertyName;
    bool valid = true;

    //Check if parameters are valid.
    if(_graph==NULL){
        valid = false;
        errorMsg = tr("Invalid graph");
    }else if(_source == NULL){
        valid = false;
        errorMsg = tr("Invalid source property");
    } else if(ui->newPropertyRadioButton->isChecked()){
        propertyName = ui->newPropertyNameLineEdit->text();
        if(propertyName.isEmpty()){
            valid = false;
            errorMsg = tr("Cannot create a property with an empty name");
        }else if(_graph->existProperty(QStringToTlpString(propertyName))){
            PropertyInterface* existingProperty = _graph->getProperty(QStringToTlpString(propertyName));
            if(typeid(*existingProperty) != typeid(*_source)){
                valid = false;
                errorMsg = tr("A property with the same name but a different type already exists");
            }
        }
    }else if(ui->localPropertyRadioButton->isChecked()){
        propertyName = ui->localPropertiesComboBox->currentText();
        if(propertyName.isEmpty()){
            valid = false;
            errorMsg = tr("No properties available");
        }
    }else{
        propertyName = ui->inheritedPropertiesComboBox->currentText();
        if(propertyName.isEmpty()){
            valid = false;
            errorMsg = tr("No properties available");
        }
    }
    if(valid){
        string tulipPropertyName = QStringToTlpString(propertyName);
        CopyPropertyDialog::PropertyScope destinationScope = destinationPropertyScope();
        Graph *parentGraph = _graph->getSuperGraph();
        _graph->push();
        if (typeid((*_source)) == typeid(DoubleProperty)) {
            DoubleProperty* doubleProperty = destinationScope==NEW?_graph->getLocalProperty<DoubleProperty>(tulipPropertyName):parentGraph->getProperty<DoubleProperty>(tulipPropertyName);
            *doubleProperty = *((DoubleProperty*) _source);
            property = doubleProperty;
        }
        if (typeid((*_source)) == typeid(LayoutProperty)) {
            LayoutProperty* layoutProperty = destinationScope==NEW?_graph->getLocalProperty<LayoutProperty>(tulipPropertyName):parentGraph->getProperty<LayoutProperty>(tulipPropertyName);
            *layoutProperty = *((LayoutProperty*) _source);
            property = layoutProperty;
        }
        if (typeid((*_source)) == typeid(StringProperty)) {
            StringProperty* stringProperty = destinationScope==NEW?_graph->getLocalProperty<StringProperty>(tulipPropertyName):parentGraph->getProperty<StringProperty>(tulipPropertyName);
            *stringProperty = *((StringProperty*) _source);
            property = stringProperty;
        }
        if (typeid((*_source)) == typeid(BooleanProperty)) {
            BooleanProperty* booleanProperty = destinationScope==NEW?_graph->getLocalProperty<BooleanProperty>(tulipPropertyName):parentGraph->getProperty<BooleanProperty>(tulipPropertyName);
            * booleanProperty= *((BooleanProperty*) _source);
            property= booleanProperty;
        }
        if (typeid((*_source)) == typeid(IntegerProperty)) {
            IntegerProperty* integerProperty = destinationScope==NEW?_graph->getLocalProperty<IntegerProperty>(tulipPropertyName):parentGraph->getProperty<IntegerProperty>(tulipPropertyName);
            *integerProperty = *((IntegerProperty*) _source);
            property = integerProperty;
        }
        if (typeid((*_source)) == typeid(ColorProperty)) {
            ColorProperty* colorProperty = destinationScope==NEW?_graph->getLocalProperty<ColorProperty>(tulipPropertyName):parentGraph->getProperty<ColorProperty>(tulipPropertyName);
            *colorProperty = *((ColorProperty*) _source);
            property = colorProperty;
        }
        if (typeid((*_source)) == typeid(SizeProperty)) {
            SizeProperty* sizeProperty = destinationScope==NEW?_graph->getLocalProperty<SizeProperty>(tulipPropertyName):parentGraph->getProperty<SizeProperty>(tulipPropertyName);
            *sizeProperty = *((SizeProperty*) _source);
            property = sizeProperty;
        }
        if (typeid((*_source)) == typeid(DoubleVectorProperty)) {
            DoubleVectorProperty* doubleVectorProperty = destinationScope==NEW?_graph->getLocalProperty<DoubleVectorProperty>(tulipPropertyName):parentGraph->getProperty<DoubleVectorProperty>(tulipPropertyName);
            *doubleVectorProperty= *((DoubleVectorProperty*) _source);
            property = doubleVectorProperty;
        }
        if (typeid((*_source)) == typeid(CoordVectorProperty)) {
            CoordVectorProperty* coordVectorProperty = destinationScope==NEW?_graph->getLocalProperty<CoordVectorProperty>(tulipPropertyName):parentGraph->getProperty<CoordVectorProperty>(tulipPropertyName);
            *coordVectorProperty = *((CoordVectorProperty*) _source);
            property = coordVectorProperty;
        }
        if (typeid((*_source)) == typeid(StringVectorProperty)) {
            StringVectorProperty* stringVectorProperty = destinationScope==NEW?_graph->getLocalProperty<StringVectorProperty>(tulipPropertyName):parentGraph->getProperty<StringVectorProperty>(tulipPropertyName);
            *stringVectorProperty = *((StringVectorProperty*) _source);
            property = stringVectorProperty;
        }
        if (typeid((*_source)) == typeid(BooleanVectorProperty)) {
            BooleanVectorProperty* booleanVectorProperty = destinationScope==NEW?_graph->getLocalProperty<BooleanVectorProperty>(tulipPropertyName):parentGraph->getProperty<BooleanVectorProperty>(tulipPropertyName);
            *booleanVectorProperty = *((BooleanVectorProperty*) _source);
            property = booleanVectorProperty;
        }
        if (typeid((*_source)) == typeid(IntegerVectorProperty)) {
            IntegerVectorProperty* integerVectorProperty = destinationScope==NEW?_graph->getLocalProperty<IntegerVectorProperty>(tulipPropertyName):parentGraph->getProperty<IntegerVectorProperty>(tulipPropertyName);
            *integerVectorProperty   = *((IntegerVectorProperty*) _source);
            property = integerVectorProperty;
        }
        if (typeid((*_source))
            == typeid(ColorVectorProperty)) {
            ColorVectorProperty* colorVectorProperty = destinationScope==NEW?_graph->getLocalProperty<ColorVectorProperty>(tulipPropertyName):parentGraph->getProperty<ColorVectorProperty>(tulipPropertyName);
            *colorVectorProperty= *((ColorVectorProperty*) _source);
            property = colorVectorProperty;
        }
        if (typeid((*_source))
            == typeid(SizeVectorProperty)) {
            SizeVectorProperty* sizeVectorProperty = destinationScope==NEW?_graph->getLocalProperty<SizeVectorProperty>(tulipPropertyName):parentGraph->getProperty<SizeVectorProperty>(tulipPropertyName);
            *sizeVectorProperty = *((SizeVectorProperty*) _source);
            property = sizeVectorProperty;
        }
    }
    return property;
}

PropertyInterface* CopyPropertyDialog::copyProperty(Graph* graph,PropertyInterface* toCopy,bool askBeforePropertyOverwriting,QWidget* parent){
    PropertyInterface* property = NULL;
    CopyPropertyDialog dialog(parent);
    dialog.setWindowTitle(tr("Copy property ")+tlpStringToQString(toCopy->getName()));
    dialog.init(graph,toCopy);
    if(dialog.exec() == QDialog::Accepted){
        QString errorMsg;
        bool copy = true;
        if(askBeforePropertyOverwriting && dialog.destinationPropertyScope()== CopyPropertyDialog::NEW){
            QString selectedPropertyName = dialog.destinationPropertyName();
            if(graph->existProperty(QStringToTlpString(selectedPropertyName))){
                if (QMessageBox::question(parent, "Copy confirmation", QString("Property ")
                    + selectedPropertyName + " already exists,\ndo you really want to overwrite it ?", QMessageBox::Ok,
                    QMessageBox::Cancel) == QDialog::Rejected){
                    copy = false;
                }
            }
        }
        if(copy){
            PropertyInterface* createdProperty = dialog.copyProperty(errorMsg);
            if(createdProperty == NULL){
                QMessageBox::critical(parent,tr("Error during the copy"),errorMsg);
            }
            property = createdProperty;
        }
    }
    return property;
}

void CopyPropertyDialog::checkValidity(){
    bool valid = true;
    QString errorMsg;
    if(_graph==NULL){
        valid = false;
        errorMsg = tr("Invalid graph");
    }else if(_source == NULL){
        valid = false;
        errorMsg = tr("Invalid source property");
    }else if(ui->newPropertyRadioButton->isChecked()){
        QString propertyName = ui->newPropertyNameLineEdit->text();
        if(propertyName.isEmpty()){
            valid = false;
            errorMsg = tr("Cannot create a property with an empty name");
        }else if(_graph->existProperty(QStringToTlpString(propertyName))){
            PropertyInterface* existingProperty = _graph->getProperty(QStringToTlpString(propertyName));
            if(typeid(*existingProperty) != typeid(*_source)){
                valid = false;
                errorMsg = tr("A property with the same name but a different type already exists");
            }
        }
    }else if(ui->localPropertyRadioButton->isChecked()){
        if(ui->localPropertiesComboBox->currentText().isEmpty()){
            valid = false;
            errorMsg = tr("No properties available");
        }
    }else {
        if(ui->inheritedPropertiesComboBox->currentText().isEmpty()){
            valid = false;
            errorMsg = tr("No properties available");
        }
    }
    ui->errorNotificationWidget->setVisible(!errorMsg.isEmpty());
    ui->errorLabel->setText(errorMsg);
    ui->buttonOK->setEnabled(valid);
}

QString CopyPropertyDialog::destinationPropertyName()const{
    if(_graph==NULL || _source == NULL){
        return QString();
    }
    QString propertyName;
    if(ui->newPropertyRadioButton->isChecked()){
        propertyName = ui->newPropertyNameLineEdit->text();
    }else if(ui->localPropertyRadioButton->isChecked()){
        propertyName = ui->localPropertiesComboBox->currentText();
    }else{
        propertyName = ui->inheritedPropertiesComboBox->currentText();
    }
    return propertyName;
}

CopyPropertyDialog::PropertyScope CopyPropertyDialog::destinationPropertyScope()const{
    if(ui->newPropertyRadioButton->isChecked()){
        return NEW;
    }else if(ui->localPropertyRadioButton->isChecked()){
        return LOCAL;
    }else{
        return INHERITED;
    }
}
