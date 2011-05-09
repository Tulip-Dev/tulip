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

#include <iostream>
#include <sstream>
#include <vector>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/ForEach.h>


#include "tulip/PropertyDialog.h"
#include "tulip/PropertyCreationDialog.h"
#include "tulip/CopyPropertyDialog.h"
#include "tulip/CSVImportWizard.h"

using namespace std;

namespace tlp {

    //==================================================================================================
    PropertyDialog::PropertyDialog(QWidget* parent) :
            QWidget(parent) {
        setupUi(this);
        _filterSelection = false;
        graph = 0;
        connect(removeButton, SIGNAL(clicked()), SLOT(removeProperty()));
        connect(newButton, SIGNAL(clicked()), SLOT(newProperty()));
        connect(cloneButton, SIGNAL(clicked()), SLOT(cloneProperty()));
        connect(propertiesWidget, SIGNAL(itemSelectionChanged()), this, SLOT(propertySelectionChanged()));
        connect(radioAll, SIGNAL(clicked()), SLOT(propertyTypeFilterChanged()));
        connect(radioWork, SIGNAL(clicked()), SLOT(propertyTypeFilterChanged()));
        connect(radioView, SIGNAL(clicked()), SLOT(propertyTypeFilterChanged()));
        connect(propertiesNameFilterLineEdit, SIGNAL(textChanged ( const QString &)), this,
                SLOT(nameFilterChanged(const QString &)));
#if QT_MINOR_REL > 7
	propertiesNameFilterLineEdit->setPlaceHolderText("Enter a filter on properties name here...");
#endif
        propertiesWidget->setSortingEnabled(true);
        propertiesWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        propertiesWidget->verticalHeader()->setVisible(false);
        propertiesWidget->horizontalHeader()->setStretchLastSection(true);
        propertiesWidget->resizeColumnsToContents();

    }
    //=================================================
    PropertyDialog::~PropertyDialog() {
    }
    //=================================================
    void PropertyDialog::propertySelectionChanged() {
        vector<string> selectedProperties = propertiesWidget->getSelectedPropertiesNames();
        if (selectedProperties.empty()) {
            editedProperty = NULL;
            return;
        }

        displayProperty(selectedProperties.front());
    }
    //=================================================
    void PropertyDialog::displayProperty(const string& propertyName){
        tableNodes->selectNodeOrEdge(true);
        tableEdges->selectNodeOrEdge(false);
        tableNodes->changeProperty(graph, propertyName);
        tableEdges->changeProperty(graph, propertyName);

        PropertyInterface *tmpProxy = graph->getProperty(propertyName);
        editedProperty = tmpProxy;
        editedPropertyName = propertyName;

        tableEdges->update();
        tableNodes->update();
    }
    //=================================================
    void PropertyDialog::filterSelection(bool b) {
        _filterSelection = b;
        tableEdges->filterSelection(b);
        tableNodes->filterSelection(b);
        update();
    }
    //=================================================
    void PropertyDialog::update() {
        tableEdges->update();
        tableNodes->update();
    }
    //=================================================
    void PropertyDialog::selectNode(node) {
    }
    //=================================================
    void PropertyDialog::selectEdge(edge) {
    }
    //=================================================
    void PropertyDialog::setAllValue() {
        if (tabWidget->currentIndex() == 0)
            tableNodes->setAll();
        else
            tableEdges->setAll();
        setAllButton->setDown(false);
    }
    //=================================================
    void PropertyDialog::setGraph(Graph *graph) {
        this->graph = graph;
        editedProperty = 0;

        //Build the property list
        tableNodes->selectNodeOrEdge(true);
        tableEdges->selectNodeOrEdge(false);
        tableNodes->setGraph(graph);
        tableEdges->setGraph(graph);
        tableEdges->filterSelection(_filterSelection);
        tableNodes->filterSelection(_filterSelection);

        propertiesWidget->setGraph(graph);
    }

    //=================================================
    void PropertyDialog::newProperty() {
        if (!graph)
            return;

        Observable::holdObservers();
        PropertyInterface* property = PropertyCreationDialog::createNewProperty(graph,this);
        if(property){
            setGraph(graph);
            emit newPropertySignal(graph, property->getName());
        }
        Observable::unholdObservers();
    }
    //=================================================
    void PropertyDialog::toStringProperty() {
        if (editedProperty == 0)
            return;
        string name = editedPropertyName;
        if (name == "viewLabel")
            return;
        Observable::holdObservers();
        // allow to undo
        graph->push();
        PropertyInterface *newLabels = graph->getProperty(name);
        StringProperty *labels = graph->getLocalProperty<StringProperty> ("viewLabel");
        if (tabWidget->currentIndex() == 0) {
            Iterator<node> *itN;
            if (_filterSelection)
                itN = graph->getProperty<BooleanProperty> ("viewSelection")-> getNonDefaultValuatedNodes();
            else {
                labels->setAllNodeValue(newLabels->getNodeDefaultStringValue());
                itN = graph->getNodes();
            }
            while (itN->hasNext()) {
                node itn = itN->next();
                labels->setNodeValue(itn, newLabels->getNodeStringValue(itn));
            }
            delete itN;
        }
        else {
            Iterator<edge> *itE;
            if (_filterSelection)
                itE = graph->getProperty<BooleanProperty> ("viewSelection")-> getNonDefaultValuatedEdges();
            else {
                labels->setAllEdgeValue(newLabels->getEdgeDefaultStringValue());
                itE = graph->getEdges();
            }
            while (itE->hasNext()) {
                edge ite = itE->next();
                labels->setEdgeValue(ite, newLabels->getEdgeStringValue(ite));
            }
            delete itE;
        }
        Observable::unholdObservers();
    }
    //=================================================
    void PropertyDialog::removeProperty() {
        if (editedProperty == 0)
            return;
        if (graph->existLocalProperty(editedPropertyName)) {
            // allow to undo
            graph->push();
            graph->delLocalProperty(editedPropertyName);
            //setGlMainWidget(glWidget);
            setGraph(graph);
            editedProperty = 0;
            emit removePropertySignal(graph, editedPropertyName);
        }
        else
            QMessageBox::critical(0, "Tulip Property Editor Remove Failed", QString(
                    "You cannot remove an inherited property,\n"));
    }
    //=================================================
    void PropertyDialog::cloneProperty() {
        if (!graph || !editedProperty)
            return;
            Observable::holdObservers();
        CopyPropertyDialog::copyProperty(graph,editedProperty,true,this);
        Observable::unholdObservers();
    }
    //=================================================
    void PropertyDialog::importCSVData() {
        if (graph
            != NULL) {
            CSVImportWizard *wizard = new CSVImportWizard(this);
            wizard->setGraph(graph);
            graph->push();
            Observable::holdObservers();            
            int result = wizard->exec();
            //If user cancel cancel push.
            if(result == QDialog::Rejected){
                graph->pop(false);
            }
            Observable::unholdObservers();
            wizard->deleteLater();
        }
    }
    //=================================================
    void PropertyDialog::propertyTypeFilterChanged() {
        //Get the sender button.
        QRadioButton
                *senderButton =
                qobject_cast<
                QRadioButton*> (
                        sender());
        assert(senderButton!= NULL);
        if (senderButton
            == radioAll) {
            propertiesWidget->setPropertyTypeFilter(
                    GraphPropertiesTableWidget::All);
        }

        else if (senderButton
                 == radioView) {
            propertiesWidget->setPropertyTypeFilter(
                    GraphPropertiesTableWidget::View);
        }
        else if (senderButton
                 == radioWork) {
            propertiesWidget->setPropertyTypeFilter(
                    GraphPropertiesTableWidget::User);
        }
    }
    //=================================================
    void PropertyDialog::nameFilterChanged(
            const QString& text) {
        propertiesWidget->setPropertyNameFilter(
                QRegExp(
                        text));
    }
    //=================================================
    void PropertyDialog::keyReleaseEvent  ( QKeyEvent * event ){
        //If there is only one property and user press enter or return set this property as selected property.
        if(event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter){
            vector<string> selectedProperties = propertiesWidget->getDisplayedPropertiesNames();
            if(selectedProperties.size()==1){
                displayProperty(selectedProperties.front());
                //Select the property in the widget
                propertiesWidget->setSelectedPropertiesNames(selectedProperties);
            }
        }
    }

}
