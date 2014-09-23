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

#ifndef CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H
#define CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H

#include <QComboBox>
#include <QPushButton>
#include <QString>

#include <tulip/tulipconf.h>

namespace Ui {
class CSVGraphMappingConfigurationWidget;
}

namespace tlp {

class Graph;
class CSVGraphImport;
class CSVImportParameters;
class CSVToGraphDataMapping;

/**
  * @brief Widget generating the CSVToGraphDataMapping object.
  *
  * This widget allow user to configure a CSVToGraphDataMapping object. This object is used during the CSV import process to map CSV columns to graph elements like nodes or edges.
  **/
class TLP_QT_SCOPE CSVGraphMappingConfigurationWidget : public QWidget {
  Q_OBJECT
public:
  CSVGraphMappingConfigurationWidget(QWidget *parent = NULL);
  ~CSVGraphMappingConfigurationWidget();

  /**
    * @brief Configure the widget with the CSV import parameters.
    **/
  void updateWidget(tlp::Graph* graph,const CSVImportParameters& importParameters);
  /**
    * @brief Build the mapping object between the CSV columns and the graph elements.
    **/
  CSVToGraphDataMapping *buildMappingObject() const;

  /**
    * @brief Check if the values entered by user are valid.
    *
    * If return true you are sure that buildMappingObject never return a NULL object.
    **/
  bool isValid()const;

protected:
  tlp::Graph* graph;
  std::vector<std::string> columns;
  std::vector<unsigned int> nodeColumnIds;
  std::vector<std::string> nodeProperties;
  std::vector<unsigned int> edgeColumnIds;
  std::vector<std::string> edgeProperties;
  std::vector<unsigned int> srcColumnIds;
  std::vector<unsigned int> tgtColumnIds;
  std::vector<std::string> srcProperties;
  std::vector<std::string> tgtProperties;

private:
  Ui::CSVGraphMappingConfigurationWidget *ui;
  void selectColumns(const QString& title,
                     std::vector<unsigned int>& columnIds,
                     QPushButton* button);
  void selectProperties(const QString& title,
                        std::vector<std::string>& selProperties,
                        QPushButton* button);

private slots:
  void createNewProperty();
  void selectNodeColumns();
  void selectEdgeColumns();
  void selectSrcColumns();
  void selectTgtColumns();
  void selectSrcProperties();
  void selectTgtProperties();
  void selectNodeProperties();
  void selectEdgeProperties();

signals:
  void mappingChanged();
};
}
#endif // CSVGRAPHMAPPINGCONFIGURATIONWIDGET_H
///@endcond
