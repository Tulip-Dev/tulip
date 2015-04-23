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
#include "ui_CSVImportWizard.h"

#include <QVBoxLayout>
#include <QLabel>

#include <tulip/CSVGraphImport.h>
#include <tulip/CSVParserConfigurationWidget.h>
#include <tulip/CSVImportWizard.h>
#include <tulip/CSVImportConfigurationWidget.h>
#include <tulip/CSVGraphMappingConfigurationWidget.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/CSVParser.h>

using namespace tlp;

CSVParsingConfigurationQWizardPage::CSVParsingConfigurationQWizardPage ( QWidget * parent):QWizardPage(parent),parserConfigurationWidget(new CSVParserConfigurationWidget(this)),previewTableWidget(new CSVTableWidget(this)),previewLineNumber(5) {
  QVBoxLayout* vbLayout = new QVBoxLayout();
  vbLayout->setContentsMargins(0,0,0,0);
  vbLayout->setSpacing(0);
  setLayout(vbLayout);
  layout()->addWidget(parserConfigurationWidget);
  layout()->addWidget(previewTableWidget);
  previewTableWidget->setMaxPreviewLineNumber(previewLineNumber);
  previewTableWidget->horizontalHeader()->setVisible(false);
  previewTableWidget->verticalHeader()->setVisible(false);
  connect(parserConfigurationWidget,SIGNAL(parserChanged()),this,SLOT(parserChanged()));
  QLabel* noteWidget=new QLabel(this);
  noteWidget->setWordWrap(true);
  noteWidget->setText(" <em>Note: several (node and/or edge) import operations using the same source file may be required to get all data to be imported and inserted into a same graph.</em>");
  layout()->addWidget(noteWidget);

  // init with last opened file if possible
  parserConfigurationWidget->initWithLastOpenedFile();
}

bool CSVParsingConfigurationQWizardPage::isComplete() const {
  return parserConfigurationWidget->isValid();
}

void CSVParsingConfigurationQWizardPage::parserChanged() {
  //Fill the preview widget
  CSVParser* parser = parserConfigurationWidget->buildParser(0,previewLineNumber);
  //Force widget to clear content.
  previewTableWidget->begin();

  if(parser!=NULL) {
    previewTableWidget->setEnabled(true);
    SimplePluginProgressDialog progress(this);
    progress.showPreview(false);
    progress.setWindowTitle(tr("Parsing file"));
    parser->parse(previewTableWidget,&progress);
  }
  else {
    previewTableWidget->setEnabled(false);
  }

  delete parser;
  emit completeChanged();
}

//CSVToGraphDataMapping* CSVGraphMappingConfigurationQWizardPage::buildMappingObject()const {
//  return graphMappingConfigurationWidget->buildMappingObject();
//}

CSVToGraphDataMapping* CSVGraphMappingConfigurationQWizardPage::buildMappingObject() const {
  return graphMappingConfigurationWidget->buildMappingObject();
}

void CSVParsingConfigurationQWizardPage::updatePreview() {
  previewTableWidget->setRowCount(0);
  previewTableWidget->setColumnCount(0);
}

CSVParser* CSVParsingConfigurationQWizardPage::buildParser() const {
  return parserConfigurationWidget->buildParser();
}

CSVImportConfigurationQWizardPage::CSVImportConfigurationQWizardPage ( QWidget * parent):QWizardPage(parent),importConfigurationWidget(new CSVImportConfigurationWidget(this)) {
  setLayout(new QVBoxLayout());
  layout()->addWidget(importConfigurationWidget);
}
void CSVImportConfigurationQWizardPage::initializePage() {
  CSVImportWizard* csvWizard= qobject_cast<CSVImportWizard*>(wizard());
  assert(csvWizard!=NULL);
  importConfigurationWidget->setNewParser(csvWizard->getParsingConfigurationPage()->buildParser());
}

CSVGraphMappingConfigurationQWizardPage::CSVGraphMappingConfigurationQWizardPage ( QWidget * parent ):QWizardPage(parent),graphMappingConfigurationWidget(new CSVGraphMappingConfigurationWidget()) {
  setLayout(new QVBoxLayout());
  layout()->addWidget(graphMappingConfigurationWidget);
  connect(graphMappingConfigurationWidget,SIGNAL(mappingChanged()),this,SIGNAL(completeChanged()));
}

bool CSVGraphMappingConfigurationQWizardPage::isComplete() const {
  return graphMappingConfigurationWidget->isValid();
}

CSVImportParameters CSVImportConfigurationQWizardPage::getImportParameters()const {
  return importConfigurationWidget->getImportParameters();
}

void CSVGraphMappingConfigurationQWizardPage::initializePage() {
  CSVImportWizard* csvWizard= qobject_cast<CSVImportWizard*>(wizard());
  assert(csvWizard!=NULL);
  graphMappingConfigurationWidget->updateWidget(csvWizard->getGraph(),csvWizard->getImportConfigurationPage()->getImportParameters());
}

CSVImportWizard::CSVImportWizard(QWidget *parent) :
  QWizard(parent),
  ui(new Ui::CSVImportWizard) {
  // ensure there is a Cancel button (may be hidden on Mac)
  setOptions(options() & ~QWizard::NoCancelButton);
  ui->setupUi(this);
}

CSVImportWizard::~CSVImportWizard() {
  delete ui;
}

CSVParsingConfigurationQWizardPage* CSVImportWizard::getParsingConfigurationPage()const {
  return qobject_cast<CSVParsingConfigurationQWizardPage*>(page(0));
}
CSVImportConfigurationQWizardPage* CSVImportWizard::getImportConfigurationPage()const {
  return qobject_cast<CSVImportConfigurationQWizardPage*>(page(1));
}
CSVGraphMappingConfigurationQWizardPage* CSVImportWizard::getMappingConfigurationPage()const {
  return qobject_cast<CSVGraphMappingConfigurationQWizardPage*>(page(2));
}

void CSVImportWizard::accept() {
  bool processIsValid=false;

  if(graph != NULL) {
    CSVParser* parser = getParsingConfigurationPage()->buildParser();

    if(parser != NULL) {
      processIsValid = true;
      CSVImportParameters importParam = getImportConfigurationPage()->getImportParameters();
      //Get row to graph element mapping
      CSVToGraphDataMapping *rowMapping = getMappingConfigurationPage()->buildMappingObject();
      //Get column to graph properties mapping
      CSVImportColumnToGraphPropertyMapping *columnMapping = new CSVImportColumnToGraphPropertyMappingProxy(graph,importParam,this);

      //Invalid mapping objects
      if(rowMapping== NULL || columnMapping==NULL) {
        processIsValid = false;
      }

      if(processIsValid) {
        //Launch the import process
        SimplePluginProgressDialog progress(this);
        progress.showPreview(false);
        progress.show();
        //Build import object
        CSVGraphImport csvToGraph(rowMapping,columnMapping,importParam);
        progress.setWindowTitle("Importing data");
        processIsValid = parser->parse(&csvToGraph,&progress);
      }

      //Release objects
      delete rowMapping;
      delete columnMapping;
      delete parser;
    }
  }

  if(processIsValid) {
    //Call QDialog accept
    QWizard::accept();
  }
  else {
    QWizard::reject();
  }
}
