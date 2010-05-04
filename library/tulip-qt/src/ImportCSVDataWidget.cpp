/*
 * ImportCSVDataWidget.cpp
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
#include "tulip/ImportCSVDataWidget.h"
#include "tulip/ImportCSVDataConfigurationWidget.h"
#include "tulip/CSVParser.h"
#include <tulip/MutableContainer.h>
#include <tulip/PropertyInterface.h>
#include <tulip/GraphProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Graph.h>
#include <tulip/QtProgress.h>
#include <QtCore/QFile>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTableWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QSplitter>

#include <cassert>
#include <iostream>
#include <sstream>

using namespace tlp;
using namespace std;

const char minusChar = '-';
const string decimalChars = ";,.";

ImportCSVDataWidget::ImportCSVDataWidget(QWidget *parent) :
  QWidget(parent), graph(NULL), rootWidget(NULL), propertiesWidget(NULL) {
  setLayout(new QVBoxLayout());
}

ImportCSVDataWidget::~ImportCSVDataWidget() {
  delete parser;
}

void ImportCSVDataWidget::setGraph(Graph* graph) {
  if (rootWidget == NULL) {
    buildWidget(this);
  }
  this->graph = graph;
  graphChanged(this->graph);
}

QWidget* ImportCSVDataWidget::buildWidget(QWidget *parent) {
  rootWidget = new QWidget(parent);
  layout()->addWidget(rootWidget);
  rootWidget->setLayout(new QVBoxLayout());
  rootWidget->layout()->setContentsMargins(0, 0, 0, 0);
  propertiesWidget = buildConfigurationWidget(rootWidget);
  rootWidget->layout()->addWidget(propertiesWidget);
  return rootWidget;
}

ImportCSVDataConfigurationWidget* ImportCSVDataWidget::buildConfigurationWidget(QWidget* parent) {
  ImportCSVDataConfigurationWidget *confWidget = new ImportCSVDataConfigurationWidget(parent);
  connect(confWidget, SIGNAL(csvFileChange(QString)), this, SLOT(CSVFileChanged(QString)));
  connect(confWidget, SIGNAL(separatorChange(QString)), this, SLOT(separatorChanged(QString)));
  connect(confWidget, SIGNAL(useFirstRowAsPropertyNameChange(bool )), this,
      SLOT(useFirstRowAsPropertyNameChanged(bool )));
  connect(confWidget, SIGNAL(propertiesOrientationChange(ImportCSVDataConfigurationWidget::PropertyOrientation)), this,
      SLOT(propertyOrientationChanged(ImportCSVDataConfigurationWidget::PropertyOrientation)));
  return confWidget;
}

void ImportCSVDataWidget::graphChanged(Graph* newGraph) {
  parser = buildCSVParser();
  updateWidgetState();
  updatePreviews();
}

CSVParser *ImportCSVDataWidget::buildCSVParser() {
  return new CSVParser();
}

void ImportCSVDataWidget::CSVFileChanged(QString file) {
  updateWidgetState();
  updatePreviews();
}
void ImportCSVDataWidget::separatorChanged(QString newSeparator) {
  updateWidgetState();
  updatePreviews();
}
void ImportCSVDataWidget::updateWidgetState() {
  emit validStateChange(checkIfDataAreValid());
}
bool ImportCSVDataWidget::checkIfDataAreValid() {
  return (!propertiesWidget->getSeparator().isEmpty() && QFile::exists(propertiesWidget->getCsvFile()));
}

void ImportCSVDataWidget::updatePreviews() {
  loadCSVData();
}
void ImportCSVDataWidget::loadCSVData() {
  QtProgress progress(this, "Generate preview");
  progress.setComment("Generating preview");
  progress.showPreview(false);
  progress.show();
  parser->parse(propertiesWidget->getCsvFile().toUtf8().data(), propertiesWidget->getSeparator().toUtf8().data(),
      propertiesWidget, &progress);
}

void ImportCSVDataWidget::useFirstRowAsPropertyNameChanged(bool useFirstRow) {
  updatePreviews();
}

void ImportCSVDataWidget::propertyOrientationChanged(ImportCSVDataConfigurationWidget::PropertyOrientation oritentation) {
  updatePreviews();
}

bool ImportCSVDataWidget::loadDataIntoGraph() {
  QtProgress progress(this, "Adding properties to graph");
  progress.setComment("Importing properties");
  pluginProgress = &progress;
  progress.showPreview(false);
  progress.show();
  parser->parse(propertiesWidget->getCsvFile().toUtf8().data(), propertiesWidget->getSeparator().toUtf8().data(), this,
      &progress);
  if (progress.state() == TLP_CANCEL) {
    return false;
  }
  else {
    return true;
  }
}

bool ImportCSVDataWidget::existingPropertyIsCompatibleWithType(Graph* graph, const string& propertyName,
    ImportCSVDataConfigurationWidget::PropertyType propertyType) const {
  PropertyInterface* property = graph->getProperty(propertyName);

  string propertyTypeString = "";
  if (propertyType == ImportCSVDataConfigurationWidget::Double) {
    propertyTypeString = "double";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Layout) {
    propertyTypeString = "layout";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::String) {
    propertyTypeString = "string";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Int) {
    propertyTypeString = "int";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Color) {
    propertyTypeString = "color";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Size) {
    propertyTypeString = "size";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Bool) {
    propertyTypeString = "bool";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::DoubleVector) {
    propertyTypeString = "vector<double>";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::StringVector) {
    propertyTypeString = "vector<string>";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::IntVector) {
    propertyTypeString = "vector<int>";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::CoordVector) {
    propertyTypeString = "vector<coord>";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::ColorVector) {
    propertyTypeString = "vector<color>";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::BoolVector) {
    propertyTypeString = "vector<bool>";
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::SizeVector) {
    propertyTypeString = "vector<size>";
  }
  return property != NULL && property->getTypename().compare(propertyTypeString) == 0;
}

PropertyInterface *ImportCSVDataWidget::getProperty(Graph* graph, const string& propertyName,
    ImportCSVDataConfigurationWidget::PropertyType propertyType) {

  if (propertyType == ImportCSVDataConfigurationWidget::AutoDetect) {
    return NULL;
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Double) {
    return graph->getProperty<DoubleProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Layout) {
    return graph->getProperty<LayoutProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::String) {
    return graph->getProperty<StringProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Int) {
    return graph->getProperty<IntegerProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Color) {
    return graph->getProperty<ColorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Size) {
    return graph->getProperty<SizeProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::Bool) {
    return graph->getProperty<BooleanProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::DoubleVector) {
    return graph->getProperty<DoubleVectorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::StringVector) {
    return graph->getProperty<StringVectorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::IntVector) {
    return graph->getProperty<IntegerVectorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::CoordVector) {
    return graph->getProperty<CoordVectorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::ColorVector) {
    return graph->getProperty<ColorVectorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::BoolVector) {
    return graph->getProperty<BooleanVectorProperty> (propertyName);
  }
  else if (propertyType == ImportCSVDataConfigurationWidget::SizeVector) {
    return graph->getProperty<SizeVectorProperty> (propertyName);
  }
  else {
    return NULL;
  }
}

ImportCSVDataConfigurationWidget::PropertyType ImportCSVDataWidget::guessVectorDataType(const vector<string> &dataRow) {
  ImportCSVDataConfigurationWidget::PropertyType dataType;
  for (vector<string>::const_iterator it = dataRow.begin(); it != dataRow.end(); ++it) {
    //Store the first type.
    if (it == dataRow.begin()) {
      dataType = guessDataType(*it, decimalChars);
    }
    else {
      if ((*it).empty()) {
        continue;
      }
      //Test if the type found is compatible with previous types.
      ImportCSVDataConfigurationWidget::PropertyType currentDataType = guessDataType(*it, decimalChars);
      if (currentDataType != dataType) {
        if ((currentDataType == ImportCSVDataConfigurationWidget::Double && dataType
            == ImportCSVDataConfigurationWidget::Int) || (currentDataType == ImportCSVDataConfigurationWidget::Int
            && dataType == ImportCSVDataConfigurationWidget::Double)) {
          dataType = ImportCSVDataConfigurationWidget::Double;
        }
        else {
          dataType = ImportCSVDataConfigurationWidget::String;
        }
      }
    }
  }
  return dataType;
}

ImportCSVDataConfigurationWidget::PropertyType ImportCSVDataWidget::guessDataType(const string& data,
    const string& decimalSeparator) {
  bool stringValue = false;
  bool intValue = false;
  bool doubleValue = false;
  for (unsigned int j = 0; j < data.length(); ++j) {
    if (isalpha(data[j])) {
      stringValue = true;
    }
    else if (isdigit(data[j]) && !stringValue) {
      if (!doubleValue) {
        intValue = true;
      }
      else {
        doubleValue = true;
      }
    }
    else if (decimalSeparator.find_first_of(data[j]) != string::npos && intValue) {
      doubleValue = true;
      intValue = false;
    }
    else if (j == 0 && data[j] == minusChar) {
      intValue = true;
    }
    else {
      stringValue = true;
      intValue = false;
      doubleValue = false;
    }
  }
  if (stringValue) {
    return ImportCSVDataConfigurationWidget::String;
  }
  else if (intValue) {
    return ImportCSVDataConfigurationWidget::Int;
  }
  else if (doubleValue) {
    return ImportCSVDataConfigurationWidget::Double;
  }
  else {
    return ImportCSVDataConfigurationWidget::AutoDetect;
  }
}

void ImportCSVDataWidget::begin() {
  nodesIterator = getGraph()->getNodes();
  overwritePropertiesButton = QMessageBox::Yes;
  propertiesBuffer.clear();
  nodesBuffer.clear();
  currentNode = node();
}

void ImportCSVDataWidget::token(unsigned int row, unsigned int column, const std::string& token) {
  //Ignore the first row if it is used as property name.
  if (row == 0 && propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column
      && propertiesWidget->useFirstRowAsPropertyName()) {
    return;
  }
  //Ignore the first column if it is used as property name.
  if (column == 0 && propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Row
      && propertiesWidget->useFirstRowAsPropertyName()) {
    return;
  }

  currentNode = getNode(row, column);
  if (currentNode.isValid()) {
    PropertyInterface *property = getPropertyInterface(row, column, token);
    if (property != NULL) {
      property->setNodeStringValue(currentNode, token);
    }
  }
  else {
    QMessageBox::warning(
        this,
        tr("Not enough nodes in graph."),
        tr(
            "The given file has more records than the actual number of nodes in the graph. Excess of data will be ignored."),
        QMessageBox::Ok);
    stopDataLoading();
  }

}

node ImportCSVDataWidget::getNode(unsigned int row, unsigned int column) {
  unsigned nodeNumber = 0;
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Row) {
    nodeNumber = column;
  }
  else {
    nodeNumber = row;
  }
  if (nodesBuffer.find(nodeNumber) == nodesBuffer.end()) {
    nodesBuffer[nodeNumber] = getNextNode();
  }
  return nodesBuffer[nodeNumber];
}

PropertyInterface *ImportCSVDataWidget::getPropertyInterface(unsigned int row, unsigned int column, const string& token) {

  unsigned int i = propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Row ? row : column;

  if (propertiesBuffer.find(i) != propertiesBuffer.end()) {
    return propertiesBuffer[i];
  }

  ImportCSVDataConfigurationWidget::PropertyType propertyType = propertiesWidget->getPropertyType(i);
  if (propertyType == ImportCSVDataConfigurationWidget::AutoDetect) {
    //Determine type
    propertyType = guessDataType(token, decimalChars);
    //If auto detection fail set to default type : string.
    if (propertyType == ImportCSVDataConfigurationWidget::AutoDetect) {
      propertyType = ImportCSVDataConfigurationWidget::String;
    }
  }

  string propertyName = propertiesWidget->getPropertyName(i).toStdString();
  if (getGraph()->existProperty(propertyName)) {
    if (existingPropertyIsCompatibleWithType(getGraph(), propertyName, propertyType)) {
      if (overwritePropertiesButton != QMessageBox::YesToAll && overwritePropertiesButton != QMessageBox::NoToAll) {
        overwritePropertiesButton = QMessageBox::question(this, tr("Property exist."),
            tr("A property with the name \"") + QString::fromStdString(propertyName) + tr(
                "\" already exist. Overwrite?"), QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No
                | QMessageBox::NoToAll, QMessageBox::Yes);
      }
      if (overwritePropertiesButton == QMessageBox::NoToAll || overwritePropertiesButton == QMessageBox::No) {
        propertiesBuffer[i] = NULL;
      }
    }
    else {
      QMessageBox::critical(this, tr("Property exist."), tr("A property with the name \"") + QString::fromStdString(
          propertyName) + tr("\" already exist with a different type. This property will be ignored."));
      propertiesBuffer[i] = NULL;
    }
  }
  else {
    propertiesBuffer[i] = getProperty(getGraph(), propertyName, propertyType);
  }
  return propertiesBuffer[i];
}

node ImportCSVDataWidget::getNextNode() {
  if (nodesIterator->hasNext()) {
    return nodesIterator->next();
  }
  else {
    return node();
  }
}

void ImportCSVDataWidget::stopDataLoading() {
  pluginProgress->stop();
}
void ImportCSVDataWidget::end(unsigned int rowNumber, unsigned int columnNumber) {
  delete nodesIterator;
  pluginProgress = NULL;
}

