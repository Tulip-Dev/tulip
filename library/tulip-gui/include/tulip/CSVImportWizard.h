/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/CSVContentHandler.h>

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
class CSVParsingConfigurationQWizardPage : public QWizardPage, public CSVContentHandler {
  Q_OBJECT
public:
  CSVParsingConfigurationQWizardPage(QWidget *parent = nullptr);
  bool begin() override;
  bool line(unsigned int row, const std::vector<CSVToken> &lineTokens) override;
  bool end(unsigned int rowNumber, unsigned int columnNumber) override;
  bool isComplete() const override;
  bool validatePage() override;
  CSVParser *buildParser(int firstLine = 0) const;
  int getFirstLineIndex() const;

private:
  void updatePreview();
  CSVParserConfigurationWidget *parserConfigurationWidget;
  CSVTableWidget *previewTableWidget;
  unsigned int previewLineNumber, columnCount;
  bool validColumnCount;

private slots:
  void parserChanged();
};

class CSVImportConfigurationQWizardPage : public QWizardPage {
  Q_OBJECT
public:
  CSVImportConfigurationQWizardPage(QWidget *parent = nullptr);
  void initializePage() override;
  CSVImportParameters getImportParameters() const;

private:
  CSVImportConfigurationWidget *importConfigurationWidget;
};

class CSVGraphMappingConfigurationQWizardPage : public QWizardPage {
  Q_OBJECT
public:
  CSVGraphMappingConfigurationQWizardPage(QWidget *parent = nullptr);
  void initializePage() override;
  bool isComplete() const override;
  CSVToGraphDataMapping *buildMappingObject() const;

private:
  CSVGraphMappingConfigurationWidget *graphMappingConfigurationWidget;
};

class TLP_QT_SCOPE CSVImportWizard : public QWizard {
  Q_OBJECT

public:
  explicit CSVImportWizard(QWidget *parent = nullptr);
  ~CSVImportWizard() override;

  CSVParsingConfigurationQWizardPage *getParsingConfigurationPage() const;
  CSVImportConfigurationQWizardPage *getImportConfigurationPage() const;
  CSVGraphMappingConfigurationQWizardPage *getMappingConfigurationPage() const;

  static void setGraph(Graph *g) {
    graph = g;
  }

  static Graph *getGraph() {
    return graph;
  }

public slots:
  void accept() override;

private:
  Ui::CSVImportWizard *ui;
  static tlp::Graph *graph;
};
} // namespace tlp
#endif // CSVIMPORTWIZARD_H
///@endcond
