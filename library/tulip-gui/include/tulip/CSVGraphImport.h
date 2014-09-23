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

#ifndef CSVGRAPHIMPORT_H
#define CSVGRAPHIMPORT_H

#include <tulip/CSVContentHandler.h>
#include <tulip/Graph.h>
#include <tulip/tulipconf.h>

#include <QMessageBox>

namespace tlp {
class PropertyInterface;

/**
  * @brief Store import parameters for a CSV file column.
  *
  * Contains all the parameters defined by user for a given CSV column (the name of the column, its data type and if user want to import it).
  **/
class TLP_QT_SCOPE CSVColumn {
public:
  CSVColumn(const std::string& columnName="",bool isUsed=false,const std::string& columnType=""):name(columnName),used(isUsed),type(columnType) {

  }

  /**
    * @brief Get the name of the column.
    **/
  std::string columnName()const {
    return name;
  }

  /**
    * @brief Tells if the property marked for import.
    **/
  bool isUsed()const {
    return used;
  }
  /**
    * @brief Return the property data type.
    **/
  std::string columnDataType()const {
    return type;
  }

private:
  std::string name;
  bool used;
  std::string type;
};
/**
  * @brief Store all the advanced import parameters for the CSV file.
  *
  * Store the information about columns and rows to import.
  * Use this object to configure the import process of a CSVImportGraph object.
  **/
class TLP_QT_SCOPE CSVImportParameters {
public:
  CSVImportParameters(unsigned int fromLine=0,unsigned int toLine=UINT_MAX,const std::vector<CSVColumn>& columns = std::vector<CSVColumn>());
  virtual ~CSVImportParameters();

  /**
    * @brief Return the number of column.
    **/
  unsigned int columnNumber()const;

  /**
    * @brief return true if the column is marked for import.
    **/
  bool importColumn(unsigned int column)const;
  /**
    * @brief Get the column name.
    **/
  std::string getColumnName(unsigned int column)const;
  /**
    * @brief Get the column data type.
    **/
  std::string getColumnDataType(unsigned int column)const;

  /**
    * @brief Return the index of the first line to import.
    **/
  unsigned int getFirstLineIndex()const;
  /**
    * @brief Return the index of the last line to import.
    **/
  unsigned int getLastLineIndex()const;
  /**
    * @brief Return true if the given row is between the first row to import and the last row to import.
    **/
  bool importRow(unsigned int row)const;
private:
  unsigned int fromLine;
  unsigned int toLine;
  std::vector<CSVColumn> columns;
};

/**
  * @brief Interface to map CSV rows to graph elements.
  *
  * To build the mapping user had to parse the CSV file.
  * @code
  * CSVParser *parser;
  * CSVToGraphDataMapping *mapping;
  * parser->parse(mapping);
  * //Now the mapping has been built.
  * //Get the element for the first row.
  * pair<tlp::ElementType,unsigned int> element = mapping->getElementForRow(0);
  * @endcode
  **/
class TLP_QT_SCOPE CSVToGraphDataMapping {
public:
  virtual ~CSVToGraphDataMapping() {}
  virtual std::pair<tlp::ElementType, std::vector<unsigned int> > getElementsForRow(const std::vector<std::string>& tokens, const std::vector<PropertyInterface*> props)=0;
  virtual void init(unsigned int rowNumber)=0;
};

/**
  * @brief Abstract class handling node or edge mapping between a CSV column and a graph property.
  *
  * Be sure there is a property with the given name in the graph or an error will occur.
  * Automatically handle CSV file parsing just implements the buildIndexForRow function to fill the rowToGraphId map with the right graph element.
  **/
class TLP_QT_SCOPE AbstractCSVToGraphDataMapping : public CSVToGraphDataMapping {
public:
  AbstractCSVToGraphDataMapping(tlp::Graph* graph,tlp::ElementType type,
                                const std::vector<unsigned int>& columnIds,
                                const std::vector<std::string>& propertyNames);
  virtual ~AbstractCSVToGraphDataMapping() {}

  void init(unsigned int rowNumber);
  std::pair<tlp::ElementType, std::vector<unsigned int> > getElementsForRow(const std::vector<std::string>& tokens, const std::vector<PropertyInterface*> props);
protected:
  /**
    * @brief Create a new element if no elements for the given row was found.
    * @return Return the graph element id or UINT_MAX if no new element is created.
    **/
  virtual unsigned int buildIndexForRow(unsigned int row,
                                        const std::vector<std::string>& keys)=0;

protected:
  TLP_HASH_MAP<std::string,unsigned int> valueToId;
  tlp::Graph* graph;
  tlp::ElementType type;
  std::vector<unsigned int> columnIds;
  std::vector<tlp::PropertyInterface*> keyProperties;
};
/**
  * @brief Map each row of the CSV file on a new node.
  **/
class TLP_QT_SCOPE CSVToNewNodeIdMapping: public CSVToGraphDataMapping {
public:
  CSVToNewNodeIdMapping(tlp::Graph* graph);
  void init(unsigned int rowNumber);
  std::pair<tlp::ElementType, std::vector<unsigned int> > getElementsForRow(const std::vector<std::string>& tokens, const std::vector<PropertyInterface*> props);
private:
  tlp::Graph* graph;
};

/**
  * @brief Try to map CSV file rows to nodes according to value between a CSV column and a graph property.
  *
  * Be sure there is a property with the given name in the graph before using it.
  **/
class TLP_QT_SCOPE CSVToGraphNodeIdMapping: public AbstractCSVToGraphDataMapping {
public:
  /**
    * @param graph The graph where the nodes will be searched.
    * @param columnIndex The index of the column with the ids in the CSV file.
    * @param propertyName The name of the property to search ids.
    * @param firstRow The first row to search ids.
    * @param lastRow The last row to search ids.
    * @param createNode If set to true if there is no node for an id in the CSV file a new node will be created for this id.
    **/
  CSVToGraphNodeIdMapping(tlp::Graph* graph,
                          const std::vector<unsigned int>& columnIds,
                          const std::vector<std::string>& propertyNames,
                          bool createNode=false);
  void init(unsigned int rowNumber);
protected:
  unsigned int buildIndexForRow(unsigned int row,
                                const std::vector<std::string>& keys);
private:
  bool createMissingNodes;
};
/**
  * @brief Try to map CSV file rows to edges according to value between a CSV column and a graph property.
  *
  * Be sure there is a property with the given name in the graph before using it.
  **/
class TLP_QT_SCOPE CSVToGraphEdgeIdMapping: public AbstractCSVToGraphDataMapping {
public:
  /**
  * @param graph The graph where the edges will be searched.
  * @param columnIndex The index of the column with the ids in the CSV file.
  * @param propertyName The name of the property to search ids.
  * @param firstRow The first row to search ids.
  * @param lastRow The last row to search ids.
  **/
  CSVToGraphEdgeIdMapping(tlp::Graph* graph,
                          const std::vector<unsigned int>& columnIds,
                          const std::vector<std::string>& propertyNames);
protected:
  unsigned int buildIndexForRow(unsigned int row,
                                const std::vector<std::string>& keys);
};

/**
  * @brief Try to map CSV file rows to edges according to edge source and destination.
  *
  * For each row in the CSV file create an edge in the graph between source and destination nodes. Find source node by comparing id in the source CSV column and destination node by comparing id in the destination CSV column.
  **/
class TLP_QT_SCOPE CSVToGraphEdgeSrcTgtMapping: public CSVToGraphDataMapping {
public:
  /**
  * @param graph The graph where the edges will be searched.
  * @param srcColumnIndex The index of the column with the source node id in the CSV file.
  * @param tgtColumnIndex The index of the column with the taret node id in the CSV file.
  * @param srcPropertyName The name of the property to search source node id.
  * @param tgtPropertyName The name of the property to search target node id.
  * @param firstRow The first row to search ids.
  * @param lastRow The last row to search ids.
  * @param createMissinElements If true create source node, destination node if one of them is not found in the graph.
  **/
  CSVToGraphEdgeSrcTgtMapping(tlp::Graph* graph,
                              const std::vector<unsigned int>& srcColumnIds,
                              const std::vector<unsigned int>& tgtColumnIds,
                              const std::vector<std::string>& srcPropNames,
                              const std::vector<std::string>& tgtPropNames,
                              bool createMissinElements=false);
  std::pair<tlp::ElementType,unsigned int> getElementForRow(unsigned int row);
  void init(unsigned int lineNumbers);
  std::pair<tlp::ElementType, std::vector<unsigned int> > getElementsForRow(const std::vector<std::string>& tokens, const std::vector<PropertyInterface*> props);
private:
  tlp::Graph* graph;
  TLP_HASH_MAP<std::string,unsigned int> srcValueToId;
  TLP_HASH_MAP<std::string,unsigned int> tgtValueToId;
  std::vector<unsigned int> srcColumnIds;
  std::vector<unsigned int> tgtColumnIds;
  std::vector<tlp::PropertyInterface*> srcProperties;
  std::vector<tlp::PropertyInterface*> tgtProperties;
  bool sameSrcTgtProperties;
  bool buildMissingElements;
};

/**
  * @brief Interface to perform mapping between CSV columns and graph properties during the CSV import process.
  *
  **/
class TLP_QT_SCOPE CSVImportColumnToGraphPropertyMapping {
public:
  virtual ~CSVImportColumnToGraphPropertyMapping() {}
  /**
    * @brief Return the property corresponding to the column index.
    * @param column The index of the column.
    * @param token The current token. May be needed to determine column data type.
    *
    * The token parameter is used to guess property type if needed.
    **/
  virtual tlp::PropertyInterface* getPropertyInterface(unsigned int column,const std::string& token)=0;
};

/**
  * @brief Proxy to handle all the properties operations like access, creation, data type detection during the CSV parsing process.
  *
  * Try to guess the type of the property in function of the first token if user don't tell which type the property is.
  **/
class TLP_QT_SCOPE CSVImportColumnToGraphPropertyMappingProxy : public CSVImportColumnToGraphPropertyMapping {
public:
  CSVImportColumnToGraphPropertyMappingProxy(tlp::Graph* graph,const CSVImportParameters& importParameters,QWidget* parent=NULL);
  virtual ~CSVImportColumnToGraphPropertyMappingProxy() {}
  tlp::PropertyInterface* getPropertyInterface(unsigned int column,const std::string& token);

private:

  tlp::Graph* graph;
  CSVImportParameters importParameters;
  TLP_HASH_MAP<unsigned int,tlp::PropertyInterface*>propertiesBuffer;
  QMessageBox::StandardButton overwritePropertiesButton;
  QWidget* parent;
};

/**
  * @brief Manage all the CSV import process. Use the mapping object to find the graph element in function of the row and the propertiesManager to find the property corresponding to the column.
  * The import parameters are used to filter the rows and the columns to import.
  **/
class TLP_QT_SCOPE CSVGraphImport : public tlp::CSVContentHandler {
public:
  CSVGraphImport(CSVToGraphDataMapping* mapping,CSVImportColumnToGraphPropertyMapping* propertiesManager,const CSVImportParameters& importParameters);
  virtual ~CSVGraphImport();
  void begin();
  void line(unsigned int row,const std::vector<std::string>& lineTokens);
  void end(unsigned int rowNumber, unsigned int columnNumber);
protected:

  CSVToGraphDataMapping* mapping;
  CSVImportColumnToGraphPropertyMapping* propertiesManager;
  CSVImportParameters importParameters;
};

}
#endif // CSVGRAPHIMPORT_H
///@endcond
