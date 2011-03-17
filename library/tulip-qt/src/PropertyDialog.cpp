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
#include <typeinfo>
#include <vector>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qlabel.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/PropertyWidget.h>
#include <tulip/SGHierarchyWidget.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ForEach.h>

#include "tulip/PropertyDialog.h"
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
        QStringList lst;
        lst << "Color" << "Integer" << "Layout" << "Metric" << "Selection" << "Size" << "String" << "BooleanVector"
                << "ColorVector" << "CoordVector" << "DoubleVector" << "IntegerVector" << "SizeVector" << "StringVector";
        bool ok = false;
        QString res = QInputDialog::getItem(this, "Property type", "Please select the property type", lst, 3, false, &ok);
        if (ok) {
            QString text = QInputDialog::getText(this, "Property name", "Please enter the property name", QLineEdit::Normal,
                                                 QString::null, &ok);
            if (ok && text == "") {
                ok = false;
                QMessageBox::warning(this, "Fail to create property", "You can't create a property with empty name",
                                     QMessageBox::Ok, QMessageBox::Ok);
            }
            string textString(text.toUtf8().data());
            if (ok && graph->existLocalProperty(textString)) {
                ok = false;
                QMessageBox::warning(this, "Fail to create property", "A property with same name already exist", QMessageBox::Ok,
                                     QMessageBox::Ok);
            }
            if (ok) {
                string erreurMsg;
                graph->push();

                if (res == trUtf8("Selection"))
                    graph->getLocalProperty<BooleanProperty> (textString);
                if (res == trUtf8("Metric"))
                    graph->getLocalProperty<DoubleProperty> (textString);
                if (res == trUtf8("Layout"))
                    graph->getLocalProperty<LayoutProperty> (textString);
                if (res == trUtf8("String"))
                    graph->getLocalProperty<StringProperty> (textString);
                if (res == trUtf8("Integer"))
                    graph->getLocalProperty<IntegerProperty> (textString);
                if (res == trUtf8("Size"))
                    graph->getLocalProperty<SizeProperty> (textString);
                if (res == trUtf8("Color"))
                    graph->getLocalProperty<ColorProperty> (textString);
                if (res == trUtf8("BooleanVector"))
                    graph->getLocalProperty<BooleanVectorProperty> (textString);
                if (res == trUtf8("DoubleVector"))
                    graph->getLocalProperty<DoubleVectorProperty> (textString);
                if (res == trUtf8("CoordVector"))
                    graph->getLocalProperty<CoordVectorProperty> (textString);
                if (res == trUtf8("StringVector"))
                    graph->getLocalProperty<StringVectorProperty> (textString);
                if (res == trUtf8("IntegerVector"))
                    graph->getLocalProperty<IntegerVectorProperty> (textString);
                if (res == trUtf8("SizeVector"))
                    graph->getLocalProperty<SizeVectorProperty> (textString);
                if (res == trUtf8("ColorVector"))
                    graph->getLocalProperty<ColorVectorProperty> (textString);
                setGraph(graph);
                emit newPropertySignal(graph, textString);
            }
        }
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
        // save editedProperty in local variable
        // to avoid any reset due to a setGraph call
        // from a addLocalProperty observer
        PropertyInterface *editedProp = editedProperty;
        CopyPropertyDialog dialog(parentWidget());
        vector<string> localProps;
        vector<string> inheritedProps;
        PropertyInterface* prop;
        Graph *parent = graph->getSuperGraph();
        if (parent == graph)
            parent = 0;
        forEach(prop, graph->getLocalObjectProperties()) {
            if (typeid(*prop) == typeid(*editedProp)) {
                const string &pName = prop->getName();
                if (pName != editedPropertyName)
                    localProps.push_back(pName);
                if (parent && parent->existProperty(pName))
                    inheritedProps.push_back(pName);
            }
        }
        forEach(prop, graph->getInheritedObjectProperties()) {
            const string &pName = prop->getName();
            if ((pName != editedPropertyName) && (typeid(*prop) == typeid(*editedProp)))
                inheritedProps.push_back(pName);
        }
        dialog.setProperties(editedPropertyName, localProps, inheritedProps);
        CopyPropertyDialog::destType type;
        std::string text;
        if (dialog.getDestinationProperty(type, text)) {
            if (text.size() > 0) {
                if (type != CopyPropertyDialog::INHERITED) {
                    if (graph->existProperty(text)) {
                        if (typeid(*graph->getProperty(text)) != typeid(*editedProp)) {
                            QMessageBox::critical(parentWidget(), "Tulip Warning", "Properties are not of the same type.");
                            return;
                        }
                        if (type == CopyPropertyDialog::NEW && graph->existLocalProperty(text)) {
                            if (text == editedPropertyName) {
                                QMessageBox::critical(parentWidget(), "Tulip Warning", "Properties are the same.");
                                return;
                            }
                            else if (QMessageBox::question(parentWidget(), "Copy confirmation", (std::string("Property ")
                                + text + " already exists,\ndo you really want to overwrite it ?").c_str(), QMessageBox::Ok,
                                                           QMessageBox::Cancel) == QDialog::Rejected)
                                return;
                        }
                    }
                    Observable::holdObservers();
                    // allow to undo
                    graph->push();
                    if (typeid((*editedProp)) == typeid(DoubleProperty)) {
                        *graph->getLocalProperty<DoubleProperty> (text) = *((DoubleProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(LayoutProperty)) {
                        *graph->getLocalProperty<LayoutProperty> (text) = *((LayoutProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(StringProperty)) {
                        *graph->getLocalProperty<StringProperty> (text) = *((StringProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(BooleanProperty)) {
                        *graph->getLocalProperty<BooleanProperty> (text) = *((BooleanProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(IntegerProperty)) {
                        *graph->getLocalProperty<IntegerProperty> (text)
                                = *((IntegerProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(ColorProperty)) {
                        *graph->getLocalProperty<ColorProperty> (text)
                                = *((ColorProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(SizeProperty)) {
                        *graph->getLocalProperty<SizeProperty> (text)
                                = *((SizeProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(DoubleVectorProperty)) {
                        *graph->getLocalProperty<DoubleVectorProperty> (text)
                                = *((DoubleVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(CoordVectorProperty)) {
                        *graph->getLocalProperty<CoordVectorProperty> (text)
                                = *((CoordVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(StringVectorProperty)) {
                        *graph->getLocalProperty<StringVectorProperty> (text)
                                = *((StringVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(BooleanVectorProperty)) {
                        *graph->getLocalProperty<BooleanVectorProperty> (text)
                                = *((BooleanVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp)) == typeid(IntegerVectorProperty)) {
                        *graph->getLocalProperty<IntegerVectorProperty> (text)
                                = *((IntegerVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(ColorVectorProperty)) {
                        *graph->getLocalProperty<ColorVectorProperty> (
                                text) = *((ColorVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(SizeVectorProperty)) {
                        *graph->getLocalProperty<SizeVectorProperty> (
                                text)
                                = *((SizeVectorProperty*) editedProp);
                    }
                    //}
                }
                else {
                    Graph *parent = graph->getSuperGraph();
                    Observable::holdObservers();
                    // allow to undo
                    parent->push();
                    if (typeid((*editedProp))
                        == typeid(DoubleProperty)) {
                        *parent->getProperty<DoubleProperty> (
                                text)
                                = *((DoubleProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(LayoutProperty)) {
                        *parent->getProperty<LayoutProperty> (
                                text)
                                = *((LayoutProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(StringProperty)) {
                        *parent->getProperty<
                                StringProperty> (text)
                                = *((StringProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(BooleanProperty)) {
                        *parent->getProperty<
                                BooleanProperty> (text)
                                = *((BooleanProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(IntegerProperty)) {
                        *parent->getProperty<
                                IntegerProperty> (
                                        text)
                                = *((IntegerProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(ColorProperty)) {
                        *parent->getProperty<
                                ColorProperty> (
                                        text)
                                = *((ColorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(SizeProperty)) {
                        *parent->getProperty<
                                SizeProperty> (
                                        text)
                                = *((SizeProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(DoubleVectorProperty)) {
                        *parent->getProperty<
                                DoubleVectorProperty> (
                                        text)
                                = *((DoubleVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(CoordVectorProperty)) {
                        *parent->getProperty<
                                CoordVectorProperty> (
                                        text)
                                = *((CoordVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(StringVectorProperty)) {
                        *parent->getProperty<
                                StringVectorProperty> (
                                        text)
                                = *((StringVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(BooleanVectorProperty)) {
                        *parent->getProperty<
                                BooleanVectorProperty> (
                                        text)
                                = *((BooleanVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(IntegerVectorProperty)) {
                        *parent->getProperty<
                                IntegerVectorProperty> (
                                        text)
                                = *((IntegerVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(ColorVectorProperty)) {
                        *parent->getProperty<
                                ColorVectorProperty> (
                                        text)
                                = *((ColorVectorProperty*) editedProp);
                    }
                    if (typeid((*editedProp))
                        == typeid(SizeVectorProperty)) {
                        *parent->getProperty<
                                SizeVectorProperty> (
                                        text)
                                = *((SizeVectorProperty*) editedProp);
                    }
                }
                setGraph(
                        graph);
                Observable::unholdObservers();
            }
            else {
                QMessageBox::warning(
                        this,
                        tr(
                                "Fail to copy property"),
                        "You can't create a property with empty name",
                        QMessageBox::Ok,
                        QMessageBox::Ok);
            }
        }
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
