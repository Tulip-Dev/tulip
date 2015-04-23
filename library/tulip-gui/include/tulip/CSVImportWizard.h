/*
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
///@cond DOXYGEN_HIDDEN

#ifndef CSVIMPORTWIZARD_H
#define CSVIMPORTWIZARD_H

#include <QWizard>
#include <QWizardPage>

#include <tulip/tulipconf.h>

namespace Ui {
class CSVImportWizard;
}
namespace tlp {

class Graph;
class CSVParser;
class CSVParserConfigurationWidget;
class CSVTableWidget;
class CSVImportConfigurationWidget;
class CSVToGraphDataMapping;
class CSVGraphMappingConfigurationWidget;
class CSVImportParameters;

/**
  * @brief QWIzardPage encapsulating a CSVParserConfigurationWidget and a preview.
  **/
class CSVParsingConfigurationQWizardPage : public QWizardPage {
  Q_OBJECT
public:
  CSVParsingConfigurationQWizardPage ( QWidget * parent = NULL );
  bool isComplete() const;
  CSVParser* buildParser()const;

private:
  void updatePreview();
  CSVParserConfigurationWidget* parserConfigurationWidget;
  CSVTableWidget* previewTableWidget;
  unsigned int previewLineNumber;

private slots:
  void parserChanged();
};

class CSVImportConfigurationQWizardPage : public QWizardPage {
  Q_OBJECT
public:
  CSVImportConfigurationQWizardPage ( QWidget * parent = NULL );
  void initializePage();
  CSVImportParameters getImportParameters()const;

private:
  CSVImportConfigurationWidget* importConfigurationWidget;
};

class CSVGraphMappingConfigurationQWizardPage : public QWizardPage {
  Q_OBJECT
public:
  CSVGraphMappingConfigurationQWizardPage ( QWidget * parent = NULL );
  void initializePage();
  bool isComplete() const;
  CSVToGraphDataMapping* buildMappingObject()const;

private:
  CSVGraphMappingConfigurationWidget* graphMappingConfigurationWidget;
};

class TLP_QT_SCOPE CSVImportWizard : public QWizard {
  Q_OBJECT

public:
  explicit CSVImportWizard(QWidget *parent = NULL);
  ~CSVImportWizard();

  CSVParsingConfigurationQWizardPage* getParsingConfigurationPage()const;
  CSVImportConfigurationQWizardPage* getImportConfigurationPage()const;
  CSVGraphMappingConfigurationQWizardPage* getMappingConfigurationPage()const;

  inline  void setGraph(Graph* graph) {
    this->graph = graph;
  }

  inline  Graph* getGraph()const {
    return graph;
  }

public slots:
  void accept();
private:
  Ui::CSVImportWizard *ui;
  tlp::Graph* graph;
};
}
#endif // CSVIMPORTWIZARD_H
///@endcond
