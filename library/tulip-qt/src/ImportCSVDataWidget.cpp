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
  QWidget(parent), graph(NULL), rootWidget(NULL), propertiesWidget(NULL), previewWidget(NULL) {
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

  QGroupBox *previewGroupBox = new QGroupBox(tr("Preview"), this);
  previewGroupBox->setLayout(new QVBoxLayout());
  previewWidget = buildPreviewWidget(previewGroupBox);
  previewWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  previewGroupBox->layout()->addWidget(previewWidget);
  previewGroupBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  rootWidget->layout()->addWidget(previewGroupBox);
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
  connect(confWidget, SIGNAL(propertyNameChange(unsigned int,QString)), this,
      SLOT(propertyNameChanged(unsigned int,QString)));
  connect(confWidget, SIGNAL(propertyStateChange(unsigned int,bool)), this,
      SLOT(propertyStateChanged(unsigned int,bool)));
  return confWidget;
}
QTableWidget* ImportCSVDataWidget::buildPreviewWidget(QWidget* parent) {
  QTableWidget *tableWidget = new QTableWidget(parent);
  tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
  return tableWidget;
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
  updatePropertiesWidget();
  updatePreviewWidget();
}
void ImportCSVDataWidget::loadCSVData() {
  unsigned int rowCount = UINT_MAX;
  unsigned int colCount = UINT_MAX;
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
    rowCount = propertiesWidget->numberOfPreviews() + 1;
  }
  else {
    colCount = propertiesWidget->numberOfPreviews() + 1;
  }
  //take one more line in just in case of the first row is used as header.
  parser->parse(propertiesWidget->getCsvFile().toStdString(), propertiesWidget->getSeparator().toStdString(), rowCount,
      colCount);
}
void ImportCSVDataWidget::updatePropertiesWidget() {
  propertiesWidget->clearPropertiesTypeList();
  if (!parser->resultIsEmpty()) {
    for (int i = 0; i < numberOfProperties(); ++i) {
      propertiesWidget->addPropertyToPropertyList(getPropertyNameFromData(i),
          !propertiesWidget->useFirstRowAsPropertyName());
    }
  }
}
unsigned int ImportCSVDataWidget::numberOfProperties() const {
  if (parser->resultIsEmpty())
    return 0;
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
    return parser->colNumber();
  }
  else {
    return parser->rowNumber();
  }
}

string ImportCSVDataWidget::getPropertyNameFromData(unsigned int propertyNumber) {
  if (!propertiesWidget->useFirstRowAsPropertyName()) {
    ostringstream oss;
    oss << "Property" << propertyNumber;
    return oss.str();
  }
  else {
    if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
      return parser->getResult()[0][propertyNumber];
    }
    else {
      return parser->getResult()[propertyNumber][0];
    }
  }
}

void ImportCSVDataWidget::updatePreviewWidget() {
  previewWidget->clear();
  const vector<vector<string> >& tokens = parser->getResult();
  if (!tokens.empty()) {
    previewWidget->setVisible(true);

    int rowFirst = rowStartIndex();
    int rowNumber = numberOfRow();
    int colFirst = colStartIndex();
    int colNumber = numberOfCol();

    previewWidget->setRowCount(rowNumber);
    previewWidget->setColumnCount(colNumber);
    QStringList headersLabelList;
    for (int i = 0; i < numberOfProperties(); ++i) {
      headersLabelList << propertiesWidget->getPropertyName(i);
    }
    if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
      previewWidget->setHorizontalHeaderLabels(headersLabelList);
    }
    else {
      previewWidget->setVerticalHeaderLabels(headersLabelList);
    }
    fillTableWidget(tokens, rowFirst, rowNumber, colFirst, colNumber);
    previewWidget->resizeColumnsToContents();
    previewWidget->resizeRowsToContents();
  }
  else {
    previewWidget->setVisible(false);
  }
}
unsigned int ImportCSVDataWidget::numberOfRow() const {
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
    return parser->rowNumber() - 1;
  }
  else
    return parser->rowNumber();
}
unsigned int ImportCSVDataWidget::numberOfCol() const {
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Row) {
    return parser->colNumber() - 1;
  }
  else {
    return parser->colNumber();
  }
}
unsigned int ImportCSVDataWidget::rowStartIndex() const {
  if (propertiesWidget->useFirstRowAsPropertyName() && propertiesWidget->getPropertyOrientation()
      == ImportCSVDataConfigurationWidget::Column) {
    return 1;
  }
  else {
    return 0;
  }
}
unsigned int ImportCSVDataWidget::colStartIndex() const {
  if (propertiesWidget->useFirstRowAsPropertyName() && propertiesWidget->getPropertyOrientation()
      == ImportCSVDataConfigurationWidget::Row) {
    return 1;
  }
  else {
    return 0;
  }
}
void ImportCSVDataWidget::fillTableWidget(const vector<vector<string> >& tokens, unsigned int rowBegin,
    unsigned int rowSize, unsigned int colBegin, unsigned int colSize) {
  assert(tokens.size() >= rowBegin + rowSize);
  assert(tokens[0].size() >= colBegin + colSize);
  for (int i = 0; i < rowSize; ++i) {
    for (int j = 0; j < tokens[rowBegin + i].size() && j < colSize; ++j) {
      previewWidget->setItem(i, j, new QTableWidgetItem(QString::fromStdString(tokens[rowBegin + i][colBegin + j])));
      previewWidget ->item(i, j)->setSelected(false);
    }
  }
}
void ImportCSVDataWidget::useFirstRowAsPropertyNameChanged(bool useFirstRow) {
  updatePropertiesWidget();
  updatePreviewWidget();
}

void ImportCSVDataWidget::propertyOrientationChanged(ImportCSVDataConfigurationWidget::PropertyOrientation oritentation) {
  loadCSVData();
  updatePropertiesWidget();
  updatePreviewWidget();
}
void ImportCSVDataWidget::propertyNameChanged(unsigned int propertyNumber, QString newPropertyName) {
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
    previewWidget->horizontalHeaderItem(propertyNumber)->setText(newPropertyName);
  }
  else {
    previewWidget->verticalHeaderItem(propertyNumber)->setText(newPropertyName);
  }
}

void ImportCSVDataWidget::propertyStateChanged(unsigned int propertyNumber, bool state) {
  if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
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

bool ImportCSVDataWidget::loadDataIntoGraph() {
  parser->parse(propertiesWidget->getCsvFile().toStdString(), propertiesWidget->getSeparator().toStdString());
  if (!parser->resultIsEmpty()) {
    MutableContainer<node> nodes;
    nodes.setAll(node());
    unsigned int nodeBeginIndex = 0;

    if (propertiesWidget->useFirstRowAsPropertyName()) {
      nodeBeginIndex = 1;
    }
    Observable::holdObservers();

    QMessageBox::StandardButton overwritePropertiesButton = QMessageBox::Yes;

    for (int i = 0; i < numberOfProperties(); ++i) {
      if (propertiesWidget->useProperty(i)) {
        ImportCSVDataConfigurationWidget::PropertyType propertyType = propertiesWidget->getPropertyType(i);
        vector<string> data;
        if (propertiesWidget->getPropertyOrientation() == ImportCSVDataConfigurationWidget::Column) {
          data = parser->extractColumn(i, nodeBeginIndex, parser->rowNumber() - nodeBeginIndex);
          assert(data.size() == parser->rowNumber() - nodeBeginIndex);
        }
        else {
          data = parser->extractRow(i, nodeBeginIndex, parser->colNumber() - nodeBeginIndex);
          assert(data.size() == parser->colNumber() - nodeBeginIndex);
        }
        if (i == 0 && data.size() > getGraph()->numberOfNodes()) {
          if (QMessageBox::warning(
              this,
              tr("Not enough nodes in graph."),
              tr(
                  "The given file has more records than the actual number of nodes in the graph. Excess of data will be ignored. Continue?"),
              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel) {
            return false;
          }
        }

        if (propertyType == ImportCSVDataConfigurationWidget::AutoDetect) {
          //Determiner le type
          propertyType = guessVectorDataType(data);
          if (propertyType == ImportCSVDataConfigurationWidget::AutoDetect) {
            propertyType = ImportCSVDataConfigurationWidget::String;
          }
        }
        string propertyName = propertiesWidget->getPropertyName(i).toStdString();
        if (getGraph()->existProperty(propertyName)) {
          if (existingPropertyIsCompatibleWithType(getGraph(), propertyName, propertyType)) {
            if (overwritePropertiesButton != QMessageBox::YesToAll && overwritePropertiesButton != QMessageBox::NoToAll) {
              overwritePropertiesButton = QMessageBox::question(this, tr("Property exist."), tr(
                  "A property with the name \"") + QString::fromStdString(propertyName) + tr(
                  "\" already exist. Overwrite?"), QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No
                  | QMessageBox::NoToAll, QMessageBox::Yes);
            }
            if (overwritePropertiesButton == QMessageBox::NoToAll || overwritePropertiesButton == QMessageBox::No) {
              continue;
            }
          }
          else {
            QMessageBox::critical(this, tr("Property exist."), tr("A property with the name \"")
                + QString::fromStdString(propertyName) + tr(
                "\" already exist with a different type. This property will be ignored."));
            continue;
          }
        }

        PropertyInterface *property = getProperty(getGraph(), propertyName, propertyType);
        if (property == NULL)
          continue;

        Iterator<node> *nodes = getGraph()->getNodes();
        for (unsigned int j = 0; j < data.size() && nodes->hasNext(); ++j) {
          node n = nodes->next();
          assert(n.isValid());
          property->setNodeStringValue(n, data[j]);
        }
      }
    }
    Observable::unholdObservers();
  }
  return true;
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
    if (it == dataRow.begin()) {
      dataType = guessDataType(*it, decimalChars);
    }
    else {
      if ((*it).empty()) {
        continue;
      }
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

