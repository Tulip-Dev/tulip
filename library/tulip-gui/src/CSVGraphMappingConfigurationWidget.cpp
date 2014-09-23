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

#include "tulip/CSVGraphMappingConfigurationWidget.h"
#include "ui_CSVGraphMappingConfigurationWidget.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <tulip/CSVGraphImport.h>
#include <tulip/PropertyCreationDialog.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
#include <tulip/StringsListSelectionDialog.h>

using namespace tlp;
using namespace std;

CSVGraphMappingConfigurationWidget::CSVGraphMappingConfigurationWidget(QWidget *parent) :
  QWidget(parent),graph(NULL),
  ui(new Ui::CSVGraphMappingConfigurationWidget) {
  ui->setupUi(this);
  connect(ui->mappingConfigurationStackedWidget,SIGNAL(currentChanged(int)),this,SIGNAL(mappingChanged()));
  connect(ui->nodeColumnsButton, SIGNAL(pressed()), this, SLOT(selectNodeColumns()));
  connect(ui->nodePropertiesButton, SIGNAL(pressed()), this, SLOT(selectNodeProperties()));
  connect(ui->edgeColumnsButton, SIGNAL(pressed()), this, SLOT(selectEdgeColumns()));
  connect(ui->edgePropertiesButton, SIGNAL(pressed()), this, SLOT(selectEdgeProperties()));
  connect(ui->srcColumnsButton, SIGNAL(pressed()), this, SLOT(selectSrcColumns()));
  connect(ui->tgtColumnsButton, SIGNAL(pressed()), this, SLOT(selectTgtColumns()));
  connect(ui->srcPropertiesButton, SIGNAL(pressed()), this, SLOT(selectSrcProperties()));
  connect(ui->tgtPropertiesButton, SIGNAL(pressed()), this, SLOT(selectTgtProperties()));

  connect(ui->newPropertyOnNodesButton,SIGNAL(clicked(bool)),this,SLOT(createNewProperty()),Qt::QueuedConnection);
  connect(ui->newPropertyOnEdgesButton,SIGNAL(clicked(bool)),this,SLOT(createNewProperty()),Qt::QueuedConnection);
}

CSVGraphMappingConfigurationWidget::~CSVGraphMappingConfigurationWidget() {
  delete ui;
}

void CSVGraphMappingConfigurationWidget::updateWidget(tlp::Graph* graph,const CSVImportParameters& importParameters) {
  this->graph = graph;

  // initialize columns infos
  columns.clear();
  srcColumnIds.clear();
  tgtColumnIds.clear();
  nodeColumnIds.clear();
  edgeColumnIds.clear();
  int srcColumn = -1, tgtColumn = -1;

  for(unsigned int i = 0 ; i< importParameters.columnNumber(); ++i) {
    if (importParameters.importColumn(i)) {
      columns.push_back(importParameters.getColumnName(i));

      if (srcColumn == -1) {
        srcColumn = i;
        srcColumnIds.push_back(i);
        nodeColumnIds.push_back(i);
        edgeColumnIds.push_back(i);
      }
      else if (tgtColumn == -1) {
        tgtColumn = i;
        tgtColumnIds.push_back(i);
      }
    }
    else
      columns.push_back("");
  }

  //Init mapping widgets.
  //Update node mapping widget
  ui->nodeColumnsButton->setEnabled(false);
  //update edge from source and target mapping widget
  ui->srcColumnsButton->setEnabled(false);
  ui->tgtColumnsButton->setEnabled(false);
  //Update edge mapping widget
  ui->edgeColumnsButton->setEnabled(false);

  //Init default values
  if(importParameters.columnNumber() >0) {
    if (srcColumn != -1) {
      ui->nodeColumnsButton->setText(tlpStringToQString(importParameters.getColumnName(srcColumn)));
      ui->nodeColumnsButton->setEnabled(true);
      ui->edgeColumnsButton->setText(tlpStringToQString(importParameters.getColumnName(srcColumn)));
      ui->edgeColumnsButton->setEnabled(true);

      //Select default columns for relations import
      if (tgtColumn != -1) {
        //Choose the first column as source id column
        ui->srcColumnsButton->setEnabled(true);
        ui->srcColumnsButton->setText(tlpStringToQString(importParameters.getColumnName(srcColumn)));
        //Choose the second column as target id column
        ui->tgtColumnsButton->setEnabled(true);
        ui->tgtColumnsButton->setText(tlpStringToQString(importParameters.getColumnName(tgtColumn)));
      }
    }
  }

  // initialize properties infos
  nodeProperties.clear();
  edgeProperties.clear();
  srcProperties.clear();
  tgtProperties.clear();

  //Choose viewLabel as default property
  ui->nodePropertiesButton->setText("viewLabel");
  nodeProperties.push_back("viewLabel");
  ui->edgePropertiesButton->setText("viewLabel");
  edgeProperties.push_back("viewLabel");
  ui->srcPropertiesButton->setText("viewLabel");
  srcProperties.push_back("viewLabel");
  ui->tgtPropertiesButton->setText("viewLabel");
  tgtProperties.push_back("viewLabel");
}

CSVToGraphDataMapping *CSVGraphMappingConfigurationWidget::buildMappingObject() const {
  if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importNewNodesPage) {
    return new CSVToNewNodeIdMapping(graph);
  }
  else if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importNodesPage) {
    if (nodeProperties.empty() || nodeColumnIds.empty()) {
      return NULL;
    }

    bool createMissingElement = ui->createMissingNodesCheckBox->isChecked();
    return new CSVToGraphNodeIdMapping(graph, nodeColumnIds, nodeProperties, createMissingElement);
  }
  else if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importEdgesPages) {
    if(edgeProperties.empty() || edgeColumnIds.empty()) {
      return NULL;
    }

    return new CSVToGraphEdgeIdMapping(graph, edgeColumnIds, edgeProperties);
  }
  else if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importEdgesFromNodesPage) {
    // src and tgt columns must be different
    bool sameColumns =  true;

    for(unsigned int i = 0; i < srcColumnIds.size(); ++i) {
      bool found = false;

      for (unsigned int j = 0; j < tgtColumnIds.size(); ++j) {
        if (srcColumnIds[i] == tgtColumnIds[j]) {
          sameColumns = true;
          break;
        }
      }

      if ((sameColumns = found) == false)
        break;
    }

    if (sameColumns) {
      return NULL;
    }

    bool createMissingElement = ui->addMissingEdgeAndNodeCheckBox->isChecked();
    return new CSVToGraphEdgeSrcTgtMapping(graph,srcColumnIds, tgtColumnIds, srcProperties, tgtProperties, createMissingElement);
  }
  else {
    return NULL;
  }
}

bool CSVGraphMappingConfigurationWidget::isValid() const {
  if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importNewNodesPage) {
    return true;
  }
  else if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importNodesPage) {
    return !nodeProperties.empty() && !nodeColumnIds.empty();
  }
  else if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importEdgesPages) {
    return !edgeProperties.empty() && !edgeColumnIds.empty();
  }
  else if(ui->mappingConfigurationStackedWidget->currentWidget()==ui->importEdgesFromNodesPage) {
    // src and tgt columns must be different
    bool sameColumns = true;

    for(unsigned int i = 0; i < srcColumnIds.size(); ++i) {
      bool found = false;

      for (unsigned int j = 0; j < tgtColumnIds.size(); ++j) {
        if (srcColumnIds[i] == tgtColumnIds[j]) {
          sameColumns = true;
          break;
        }
      }

      if ((sameColumns = found) == false)
        break;
    }

    if (sameColumns) {
      return false;
    }

    return true;
  }
  else {
    return false;
  }
}

void CSVGraphMappingConfigurationWidget::createNewProperty() {
  PropertyCreationDialog::createNewProperty(graph,this);
}

void CSVGraphMappingConfigurationWidget::selectProperties(const QString& title, std::vector<std::string>& selProperties, QPushButton* button) {
  vector<string> graphProperties;
  string propertyName;
  forEach(propertyName, graph->getProperties()) {
    graphProperties.push_back(propertyName);
  }

  if (StringsListSelectionDialog::choose(title, graphProperties, selProperties,
                                         this)) {
    if (selProperties.size() == 0) {
      selProperties.push_back("viewLabel");
      button->setText("viewLabel");
    }
    else {
      QString buttonName;

      for (unsigned int i = 0; i < selProperties.size(); ++i) {
        if (i)
          buttonName.append(", ");

        buttonName.append(tlpStringToQString(selProperties[i]));
      }

      button->setText(buttonName);
    }
  }
}

void CSVGraphMappingConfigurationWidget::selectSrcProperties() {
  selectProperties("Choose source node properties", srcProperties,
                   ui->srcPropertiesButton);
}

void CSVGraphMappingConfigurationWidget::selectTgtProperties() {
  selectProperties("Choose target node properties", tgtProperties,
                   ui->tgtPropertiesButton);
}

void CSVGraphMappingConfigurationWidget::selectNodeProperties() {
  selectProperties("Choose node identification properties", nodeProperties,
                   ui->nodePropertiesButton);
}

void CSVGraphMappingConfigurationWidget::selectEdgeProperties() {
  selectProperties("Choose edge identification properties", edgeProperties,
                   ui->edgePropertiesButton);
}

void CSVGraphMappingConfigurationWidget::selectColumns(const QString& title, std::vector<unsigned int>& columnIds, QPushButton* button) {
  vector<string> tmpColumns;
  vector<string> selColumns;

  for (unsigned int i = 0; i < columns.size(); ++i) {
    if (!columns[i].empty())
      tmpColumns.push_back(columns[i]);
  }

  for (unsigned int i= 0; i < columnIds.size(); ++i) {
    selColumns.push_back(columns[columnIds[i]]);
  }

  if (StringsListSelectionDialog::choose(title, tmpColumns, selColumns, this)) {
    if (selColumns.size() == 0) {
      columnIds.clear();

      for (unsigned int i = 0; i < columns.size(); ++i) {
        if (!columns[i].empty()) {
          columnIds.push_back(i);
          break;
        }
      }
    }
    else {
      columnIds.clear();
      QString buttonName;

      for (unsigned int i = 0; i < selColumns.size(); ++i) {
        if (i)
          buttonName.append(", ");

        buttonName.append(tlpStringToQString(selColumns[i]));

        for (unsigned int j = 0; j < columns.size(); ++j) {
          if (selColumns[i] == columns[j]) {
            columnIds.push_back(j);
            break;
          }
        }
      }

      button->setText(buttonName);
    }
  }
}

void CSVGraphMappingConfigurationWidget::selectNodeColumns() {
  selectColumns("Choose columns for node identifier", nodeColumnIds,
                ui->nodeColumnsButton);
}

void CSVGraphMappingConfigurationWidget::selectEdgeColumns() {
  selectColumns("Choose columns for edge identifier", edgeColumnIds,
                ui->edgeColumnsButton);
}

void CSVGraphMappingConfigurationWidget::selectSrcColumns() {
  selectColumns("Choose columns for source", srcColumnIds,
                ui->srcColumnsButton);
}

void CSVGraphMappingConfigurationWidget::selectTgtColumns() {
  selectColumns("Choose columns for target", tgtColumnIds,
                ui->tgtColumnsButton);
}
