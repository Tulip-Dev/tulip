/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/GraphPropertiesTableWidget.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/ForEach.h>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>
#include <algorithm>
#include <string>
using namespace tlp;
using namespace std;

// same value as in TulipTableWidget.cpp
#define ROW_HEIGHT 18

GraphPropertiesTableWidget::GraphPropertiesTableWidget(QWidget *parent) :
  QTableWidget(parent), graph(NULL), typeFilter(All), nameFilter(".*") {
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  // set sorting order to avoid different default value
  // depending on implementation (needed by gui tests)
  horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
}

GraphPropertiesTableWidget::~GraphPropertiesTableWidget() {
}

void GraphPropertiesTableWidget::setGraph(Graph* newGraph) {
  graph = newGraph;
  updateTable();
}

void GraphPropertiesTableWidget::updateTable() {
  clear();
  if (graph != NULL) {
    //Get the header list.
    QStringList headersList = getHeaderList();
    //Update the column number and labels.
    setColumnCount(headersList.size());
    setHorizontalHeaderLabels(getHeaderList());

    vector<string> propertiesName;
    string propertyName;
    //Get the properties list to display
    forEach(propertyName,graph->getProperties()) {
      if (checkPropertyFilter(propertyName) && checkPropertyName(propertyName)) {
        propertiesName.push_back(propertyName);
      }
    }
    //Update the row number.
    setRowCount(propertiesName.size());
    //Fill the table.
    for (int i = 0; i < propertiesName.size(); ++i) {
      for (int j = 0; j < columnCount(); ++j) {
        setItem(i, j, createPropertyItem(graph->getProperty(propertiesName[i]), j));
      }
      setRowHeight(i, ROW_HEIGHT);
    }
  }
}

QStringList GraphPropertiesTableWidget::getHeaderList() const {
  QStringList headersLabels;
  headersLabels << tr("Name") << tr("Type") << tr("Range");
  return headersLabels;
}
QTableWidgetItem* GraphPropertiesTableWidget::createPropertyItem(PropertyInterface* property, int column) {
  if (column == 0) {
    return new QTableWidgetItem(QString::fromStdString(property->getName()));
  }
  else if (column == 1) {
    return new QTableWidgetItem(QString::fromStdString(property->getTypename()));
  }
  else if (column == 2) {
    if (graph->existLocalProperty(property->getName())) {
      return new QTableWidgetItem(tr("Local"));
    }
    else {
      return new QTableWidgetItem(tr("Inherited from graph : ") + QString::number(property->getGraph()->getId()));
    }
  }
  else {
    return NULL;
  }
}

vector<string> GraphPropertiesTableWidget::getSelectedPropertiesNames() const {
  vector<string> properties;
  //Use the range difference to compute the number of row selected.
  QList<QTableWidgetSelectionRange> range = selectedRanges();
  for (QList<QTableWidgetSelectionRange>::iterator it = range.begin(); it != range.end(); ++it) {
    for (int i = 0; i < (*it).rowCount(); ++i) {
      if (((*it).bottomRow() + i) < rowCount()) {
        properties.push_back(getPropertyNameForRow((*it).bottomRow() + i));
      }
    }
  }
  return properties;
}

vector<string> GraphPropertiesTableWidget::getDisplayedPropertiesNames() const {
  vector<string> properties;
  for (int i = 0; i < rowCount(); ++i) {
    properties.push_back(getPropertyNameForRow(i));
  }
  return properties;
}

string GraphPropertiesTableWidget::getPropertyNameForRow(int row) const {
  assert(rowCount()>row);
  return item(row, 0)->text().toStdString();
}

void GraphPropertiesTableWidget::setPropertyTypeFilter(PropertyType typeFilter) {
  this->typeFilter = typeFilter;
  updateTable();
}

bool GraphPropertiesTableWidget::checkPropertyFilter(const string& propertyName) {
  if (typeFilter == All) {
    return true;
  }
  bool isViewPrefix = propertyName.substr(0, 4).compare("view") == 0;
  if (typeFilter == View) {
    return isViewPrefix;
  }
  else if (typeFilter == User) {
    return !isViewPrefix;
  }
}

bool GraphPropertiesTableWidget::checkPropertyName(const string& propertyName) {

  //Don't display viewMetaGraphProperty
  if (propertyName.compare("viewMetaGraph") == 0) {
    return false;
  }

  if (nameFilter.isValid()) {
    //Find if the reg exp match in the string.
    return nameFilter.indexIn(QString::fromStdString(propertyName)) != -1;
  }
  else {
    return true;
  }
}

void GraphPropertiesTableWidget::setPropertyNameFilter(const QRegExp& nameRegExp) {
  nameFilter = nameRegExp;
  updateTable();
}
