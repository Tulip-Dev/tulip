/*
 * ImportCSVDataWidget.h
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

#ifndef IMPORTCSVDATAWIDGET_H_
#define IMPORTCSVDATAWIDGET_H_

#include <QtGui/QWidget>
#include <tulip/ImportCSVDataConfigurationWidget.h>
#include <tulip/CSVContentHandler.h>
#include <tulip/Iterator.h>
#include <tulip/Node.h>
#include <QtGui/QMessageBox>
class QTableWidget;
class QSplitter;
namespace tlp {
class Graph;
class PropertyInterface;
class CSVParser;
class PluginProgress;
/*
 *  @brief Import csv data in the given graph.
 *  Import csv data in the given graph. This widget parse a CSV file and give to user a representation of the 5 first lines in a table widget.
 *  This class only create new properties it cannot create new nodes or edges.
 */
class TLP_QT_SCOPE ImportCSVDataWidget: public QWidget, public CSVContentHandler {
  Q_OBJECT
public:
  ImportCSVDataWidget(QWidget *parent = NULL);
  virtual ~ImportCSVDataWidget();

  QWidget* buildWidget(QWidget *parent);
  bool loadDataIntoGraph();

  void setGraph(tlp::Graph* graph);
  tlp::Graph *getGraph() const {
    return graph;
  }
  void begin();
  void token(unsigned int row, unsigned int column, const std::string& token);
  void end(unsigned int rowNumber,unsigned int columnNumber);

protected:
  /**
   * Function used to check if all the selected options are valid.
   */
  virtual bool checkIfDataAreValid();
  void updateWidgetState();
  /**
   * Function called to set the graph.
   */
  void graphChanged(tlp::Graph* newGraph);
  virtual ImportCSVDataConfigurationWidget *buildConfigurationWidget(QWidget* parent);

  /**
   * Create and return the CSV parser object.
   */
  virtual CSVParser *buildCSVParser();

  virtual void updatePreviews();
  /**
   * Parse and load data in the graph.
   */
  virtual void loadCSVData();

  /**
   * Try to find the type of a vector of string.
   */
  ImportCSVDataConfigurationWidget::PropertyType guessVectorDataType(const std::vector<std::string> &dataRow);
  /**
   * Try to find the type from a string.
   */
  ImportCSVDataConfigurationWidget::PropertyType guessDataType(const std::string& data,
      const std::string& decimalSeparator);
  /**
   * Test if a property is compatible with the given type.
   */
  bool existingPropertyIsCompatibleWithType(tlp::Graph* graph,const std::string& propertyName,
      ImportCSVDataConfigurationWidget::PropertyType propertyType) const;
  /**
   * Find or create a property in the graph.
   */
  tlp::PropertyInterface *getProperty(tlp::Graph* graph, const std::string& propertyName,
      ImportCSVDataConfigurationWidget::PropertyType propertyType);

  /**
   * Get the next node in the graph if it exists or an invalid node.
   */
  tlp::node getNextNode();

  /**
   * Stop the current data loading.
   */
  void stopDataLoading();

  /**
   * Get the property interface for the given row or column or create it if needed.
   */
  tlp::PropertyInterface *getPropertyInterface(unsigned int row,unsigned int column,const std::string& token);

  /**
   * Compute the node number in function of the given row and column.
   */
  tlp::node getNode(unsigned int row,unsigned int column);

  tlp::Graph *graph;
  QWidget *rootWidget;
  ImportCSVDataConfigurationWidget* propertiesWidget;
  CSVParser *parser;

  std::map<unsigned int,tlp::node> nodesBuffer;
  tlp::Iterator<tlp::node>* nodesIterator;
  QMessageBox::StandardButton overwritePropertiesButton;
  std::map<unsigned int ,tlp::PropertyInterface*> propertiesBuffer;
  tlp::node currentNode;
  tlp::PluginProgress *pluginProgress;

protected slots:
  virtual void CSVFileChanged(QString file);
  virtual void separatorChanged(QString newSeparator);
  virtual void useFirstRowAsPropertyNameChanged(bool useFirstRow);
  virtual void propertyOrientationChanged(ImportCSVDataConfigurationWidget::PropertyOrientation oritentation);

  signals:
  void validStateChange(bool valid);
};
}
#endif /* IMPORTCSVDATAWIDGET_H_ */
