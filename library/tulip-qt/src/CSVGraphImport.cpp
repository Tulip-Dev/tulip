/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/CSVGraphImport.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
using namespace tlp;
using namespace std;

static const char minusChar = '-';

string PropertyTools::getPropertyTypeLabel(const string& typeName) {
  return QStringToTlpString(propertyTypeToPropertyTypeLabel(typeName));
}

string PropertyTools::getPropertyTypeFromPropertyTypeLabel(const string& typeNameLabel) {
  return propertyTypeLabelToPropertyType(tlpStringToQString(typeNameLabel));
}

QStringList PropertyTools::getPropertyTypeLabelsList() {
  QStringList labels;
  //Init properties list
  labels<<propertyTypeToPropertyTypeLabel("color");
  labels<<propertyTypeToPropertyTypeLabel("int");
  labels<<propertyTypeToPropertyTypeLabel("layout");
  labels<<propertyTypeToPropertyTypeLabel("double");
  labels<<propertyTypeToPropertyTypeLabel("bool");
  labels<<propertyTypeToPropertyTypeLabel("size");
  labels<<propertyTypeToPropertyTypeLabel("string");
  labels<<propertyTypeToPropertyTypeLabel("vector<bool>");
  labels<<propertyTypeToPropertyTypeLabel("vector<color>");
  labels<<propertyTypeToPropertyTypeLabel("vector<coord>");
  labels<<propertyTypeToPropertyTypeLabel("vector<double>");
  labels<<propertyTypeToPropertyTypeLabel("vector<int>");
  labels<<propertyTypeToPropertyTypeLabel("vector<size>");
  labels<<propertyTypeToPropertyTypeLabel("vector<string>");
  return labels;
}

string PropertyTools::guessDataType(const string& data, const string& decimalSeparator) {
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
    return "string";
  }
  else if (intValue) {
    return "int";
  }
  else if (doubleValue) {
    return "double";
  }
  else {
    return "";
  }
}

bool  PropertyTools::existingPropertyIsCompatibleWithType(Graph* graph, const string& propertyName,
    const string& propertyType) {
  if(!graph->existProperty(propertyName)) {
    return false;
  }

  return graph->getProperty(propertyName)->getTypename().compare(propertyType)==0;
}

PropertyInterface *PropertyTools::getProperty(Graph* graph, const string& propertyName,
    const string& propertyType) {

  if (propertyType.compare("double") == 0) {
    return graph->getProperty<DoubleProperty> (propertyName);
  }
  else if (propertyType.compare("layout")==0) {
    return graph->getProperty<LayoutProperty> (propertyName);
  }
  else if (propertyType.compare("string")==0) {
    return graph->getProperty<StringProperty> (propertyName);
  }
  else if (propertyType.compare("int")==0) {
    return graph->getProperty<IntegerProperty> (propertyName);
  }
  else if (propertyType.compare("color")==0) {
    return graph->getProperty<ColorProperty> (propertyName);
  }
  else if (propertyType.compare("size")==0) {
    return graph->getProperty<SizeProperty> (propertyName);
  }
  else if (propertyType.compare("bool")==0) {
    return graph->getProperty<BooleanProperty> (propertyName);
  }
  else if (propertyType.compare("vector<double>")==0) {
    return graph->getProperty<DoubleVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<string>")==0) {
    return graph->getProperty<StringVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<int>")==0) {
    return graph->getProperty<IntegerVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<coord>")==0) {
    return graph->getProperty<CoordVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<color>")==0) {
    return graph->getProperty<ColorVectorProperty> (propertyName);
  }
  else if (propertyType.compare("BooleanVector")==0) {
    return graph->getProperty<BooleanVectorProperty> (propertyName);
  }
  else if (propertyType.compare("SizeVector")==0) {
    return graph->getProperty<SizeVectorProperty> (propertyName);
  }
  else {
    return NULL;
  }
}

PropertyInterface *PropertyTools::getLocalProperty(Graph* graph, const string& propertyName,
    const string& propertyType) {

  if (propertyType.compare("double") == 0) {
    return graph->getLocalProperty<DoubleProperty> (propertyName);
  }
  else if (propertyType.compare("layout")==0) {
    return graph->getLocalProperty<LayoutProperty> (propertyName);
  }
  else if (propertyType.compare("string")==0) {
    return graph->getLocalProperty<StringProperty> (propertyName);
  }
  else if (propertyType.compare("int")==0) {
    return graph->getLocalProperty<IntegerProperty> (propertyName);
  }
  else if (propertyType.compare("color")==0) {
    return graph->getLocalProperty<ColorProperty> (propertyName);
  }
  else if (propertyType.compare("size")==0) {
    return graph->getLocalProperty<SizeProperty> (propertyName);
  }
  else if (propertyType.compare("bool")==0) {
    return graph->getLocalProperty<BooleanProperty> (propertyName);
  }
  else if (propertyType.compare("vector<double>")==0) {
    return graph->getLocalProperty<DoubleVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<string>")==0) {
    return graph->getLocalProperty<StringVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<int>")==0) {
    return graph->getLocalProperty<IntegerVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<coord>")==0) {
    return graph->getLocalProperty<CoordVectorProperty> (propertyName);
  }
  else if (propertyType.compare("vector<color>")==0) {
    return graph->getLocalProperty<ColorVectorProperty> (propertyName);
  }
  else if (propertyType.compare("BooleanVector")==0) {
    return graph->getLocalProperty<BooleanVectorProperty> (propertyName);
  }
  else if (propertyType.compare("SizeVector")==0) {
    return graph->getLocalProperty<SizeVectorProperty> (propertyName);
  }
  else {
    return NULL;
  }
}


CSVImportParameters::CSVImportParameters(unsigned int fromLine,unsigned int toLine,const vector<CSVColumn>& columns):fromLine(fromLine),toLine(toLine),columns(columns) {
}

CSVImportParameters::~CSVImportParameters() {
}

unsigned int CSVImportParameters::columnNumber()const {
  return columns.size();
}
bool CSVImportParameters::importColumn(unsigned int column)const {
  if(column < columns.size()) {
    return columns[column].isUsed();
  }
  else {
    return false;
  }
}
string CSVImportParameters::getColumnName(unsigned int column)const {
  if(column < columns.size()) {
    return columns[column].columnName();
  }
  else {
    return string();
  }
}
string CSVImportParameters::getColumnDataType(unsigned int column)const {
  if(column < columns.size()) {
    return columns[column].columnDataType();
  }
  else {
    return string();
  }
}
bool CSVImportParameters::importRow(unsigned int row)const {
  return row >= fromLine && row <= toLine;
}

unsigned int CSVImportParameters::getFirstLineIndex()const {
  return fromLine;
}
unsigned int CSVImportParameters::getLastLineIndex()const {
  return toLine;
}

AbstractCSVToGraphDataMapping::AbstractCSVToGraphDataMapping(Graph* graph,ElementType type,unsigned int columnIndex,const string& propertyName):graph(graph),type(type),columnIndex(columnIndex),keyProperty(NULL) {
  assert(graph!=NULL);
  assert(graph->existProperty(propertyName));
  keyProperty = graph->getProperty(propertyName);
}

void AbstractCSVToGraphDataMapping::init(unsigned int) {
  //Clean old informations.
  valueToId.clear();

  //Fill map with graph values.
  if(type == NODE) {
    node n;
    forEach(n,graph->getNodes()) {
      valueToId[keyProperty->getNodeStringValue(n)]=n.id;
    }
  }
  else {
    edge e;
    forEach(e,graph->getEdges()) {
      valueToId[keyProperty->getEdgeStringValue(e)]=e.id;
    }
  }
}


pair<ElementType,unsigned int> AbstractCSVToGraphDataMapping::getElementForRow(const vector<string>& tokens) {
  if(tokens.size() > columnIndex) {
    if(valueToId.find(tokens[columnIndex]) == valueToId.end()) {
      //Try to generate the element
      unsigned int id = buildIndexForRow(0,tokens[columnIndex],graph,keyProperty);

      //If the token was corectly generated.
      if(id!=UINT_MAX) {
        //Store the id corresponding to the token.
        valueToId[tokens[columnIndex]] = id;
      }

      return pair<ElementType,unsigned int>(type,id);
    }
    else {
      //Use the last found id.
      return pair<ElementType,unsigned int>(type,valueToId[tokens[columnIndex]]);
    }
  }
  else {
    return pair<ElementType,unsigned int>(type,UINT_MAX);
  }
}

CSVToNewNodeIdMapping::CSVToNewNodeIdMapping(Graph* graph):graph(graph) {
}

void CSVToNewNodeIdMapping::init(unsigned int rowNumber) {
  graph->reserveNodes(rowNumber);
}

pair<ElementType,unsigned int> CSVToNewNodeIdMapping::getElementForRow(const vector<string>&) {
  return make_pair(NODE,graph->addNode().id);
}

CSVToGraphNodeIdMapping::CSVToGraphNodeIdMapping(Graph* graph,unsigned int columnIndex,const string& propertyName,bool createNode):AbstractCSVToGraphDataMapping(graph,NODE,columnIndex,propertyName),createMissingNodes(createNode) {
}

void CSVToGraphNodeIdMapping::init(unsigned int rowNumber) {
  AbstractCSVToGraphDataMapping::init(rowNumber);

  if(createMissingNodes) {
    graph->reserveNodes(rowNumber);
  }
}

unsigned int CSVToGraphNodeIdMapping::buildIndexForRow(unsigned int,const string& indexKey,Graph* graph,PropertyInterface* keyProperty) {
  if(createMissingNodes) {
    node newNode = graph->addNode();
    keyProperty->setNodeStringValue(newNode,indexKey);
    return newNode.id;
  }
  else {
    return UINT_MAX;
  }
}

CSVToGraphEdgeIdMapping::CSVToGraphEdgeIdMapping(Graph* graph,unsigned int columnIndex,const string& propertyName):AbstractCSVToGraphDataMapping(graph,EDGE,columnIndex,propertyName) {
}

unsigned int CSVToGraphEdgeIdMapping::buildIndexForRow(unsigned int ,const string& ,Graph* ,PropertyInterface* ) {
  return UINT_MAX;
}

CSVToGraphEdgeSrcTgtMapping::CSVToGraphEdgeSrcTgtMapping(Graph* graph,
    unsigned int srcColumnIndex,unsigned int tgtColumnIndex,const string& propertyName,
    bool createMissinNodes):
  graph(graph),srcColumnIndex(srcColumnIndex),tgtColumnIndex(tgtColumnIndex),keyProperty(graph->getProperty(propertyName)),
  buildMissingElements(createMissinNodes) {
}

void CSVToGraphEdgeSrcTgtMapping::init(unsigned int rowNumber) {
  valueToId.clear();
  node n;
  forEach(n,graph->getNodes()) {
    valueToId[keyProperty->getNodeStringValue(n)]=n.id;
  }
  //Reserve memory
  graph->reserveEdges(rowNumber);

  if(buildMissingElements) {
    //Need to reserve for source and target nodes.
    graph->reserveNodes(2*rowNumber);
  }
}
pair<ElementType,unsigned int> CSVToGraphEdgeSrcTgtMapping::getElementForRow(const vector<string>& lineTokens) {
  //Check if the index is avalable for this line
  node src;
  node tgt;

  if( lineTokens.size() > srcColumnIndex ) {
    TLP_HASH_MAP<string,unsigned int>::iterator it = valueToId.find(lineTokens[srcColumnIndex]);

    //token exists in the map
    if(it != valueToId.end()) {
      src = node(it->second);
    }
    else if(buildMissingElements) {
      src = graph->addNode();
      keyProperty->setNodeStringValue(src,lineTokens[srcColumnIndex]);
      valueToId[lineTokens[srcColumnIndex]] = src.id;
    }
  }

  //Check if the index is avalable for this line
  if(lineTokens.size() > tgtColumnIndex) {
    TLP_HASH_MAP<string,unsigned int>::iterator it = valueToId.find(lineTokens[tgtColumnIndex]);

    //token exists in the map
    if(it != valueToId.end()) {
      tgt =node(it->second);
    }
    else if(buildMissingElements) {
      tgt = graph->addNode();
      keyProperty->setNodeStringValue(tgt,lineTokens[tgtColumnIndex]);
      valueToId[lineTokens[tgtColumnIndex]] = tgt.id;
    }
  }

  //If nodes are found create a new edge.
  edge e;

  if( src.isValid() && tgt.isValid()) {
    e = graph->addEdge(src,tgt);
  }

  return make_pair(EDGE,e.id);
}

CSVImportColumnToGraphPropertyMappingProxy::CSVImportColumnToGraphPropertyMappingProxy(Graph* graph,const CSVImportParameters& importParameters,QWidget* parent):graph(graph),importParameters(importParameters),parent(parent) {

}

PropertyInterface *CSVImportColumnToGraphPropertyMappingProxy::getPropertyInterface(unsigned int column,const string&) {
  TLP_HASH_MAP<unsigned int,PropertyInterface*>::iterator it = propertiesBuffer.find(column);

  //No properties
  if(it==propertiesBuffer.end()) {
    string propertyType = importParameters.getColumnDataType(column);
    string propertyName = importParameters.getColumnName(column);


    //If auto detection fail set to default type : string.
    if (propertyType.empty()) {
      std::cerr<<__PRETTY_FUNCTION__<<" No type for the column "<<propertyName<<" set to string"<<std::endl;
      propertyType = "string";
    }

    PropertyInterface *interf=NULL;

    //The property already exist need to check if existing is compatible with new one.
    if (graph->existProperty(propertyName)) {
      PropertyInterface *existingProperty = graph->getProperty(propertyName);

      //If the properties are compatible query if we had to override existing.
      if (existingProperty->getTypename().compare(propertyType)==0) {
        if (overwritePropertiesButton != QMessageBox::YesToAll && overwritePropertiesButton != QMessageBox::NoToAll) {
          overwritePropertiesButton = QMessageBox::question(parent, parent->tr("Property exist."),
                                      parent->tr("A property with the name \"") + tlpStringToQString(propertyName) + parent->tr(
                                        "\" already exist. Overwrite?"), QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No
                                      | QMessageBox::NoToAll, QMessageBox::Yes);
        }

        if (overwritePropertiesButton == QMessageBox::NoToAll || overwritePropertiesButton == QMessageBox::No) {
          interf = NULL;
        }
        else {
          interf = graph->getProperty(propertyName);
        }
      }
      else {
        //If the properties are not compatible skip.
        QMessageBox::critical(parent, parent->tr("Property exist."), parent->tr("A property with the name \"") + tlpStringToQString(
                                propertyName) + parent->tr("\" already exist with a different type. This property will be ignored."));
        interf = NULL;
      }
    }
    else {
      interf=graph->getProperty(propertyName, propertyType);
    }

    propertiesBuffer[column] = interf;
    return interf;
  }
  else {
    return it->second;
  }
}

CSVGraphImport::CSVGraphImport(CSVToGraphDataMapping* mapping,CSVImportColumnToGraphPropertyMapping* properties,const CSVImportParameters& importParameters):mapping(mapping),propertiesManager(properties),importParameters(importParameters) {
}
CSVGraphImport::~CSVGraphImport() {}
void CSVGraphImport::begin() {
  mapping->init(importParameters.getLastLineIndex()-importParameters.getFirstLineIndex()+1);
}

void CSVGraphImport::line(unsigned int row,const std::vector<std::string>& lineTokens) {
  //Check if user wants to import the line.
  if(!importParameters.importRow(row) ) {
    return;
  }

  //Compute the element id associated to the line
  pair<ElementType,unsigned int> element = mapping->getElementForRow(lineTokens);

  //Error when getting element
  if(element.second == UINT_MAX ) {
    return;
  }

  for(size_t column = 0 ; column < lineTokens.size() ; ++column) {
    //If user want to import the column
    if(importParameters.importColumn(column)) {
      PropertyInterface *property = propertiesManager->getPropertyInterface(column,lineTokens[column]);

      //If the property don't exists or if the token is empty do not import the value
      if( property != NULL && !lineTokens[column].empty()) {
        if(element.first == NODE) {
          if(!property->setNodeStringValue(node(element.second),lineTokens[column])) {
            //We add one to the row number as in the configuration widget we start from row 1 not row 0
            std::cerr<<__PRETTY_FUNCTION__<<":"<<__LINE__<<" error when importing token \""<<lineTokens[column]<<"\" in property \""<<property->getName()<<"\" of type \""<<property->getTypename()<<"\" at line "<<row+1<<std::endl;
          }
        }
        else {
          if(!property->setEdgeStringValue(edge(element.second),lineTokens[column])) {
            //We add one to the row number as in the configuration widget we start from row 1 not row 0
            std::cerr<<__PRETTY_FUNCTION__<<":"<<__LINE__<<" error when importing token \""<<lineTokens[column]<<"\" in property \""<<property->getName()<<"\" of type \""<<property->getTypename()<<"\" at line "<<row+1<<std::endl;
          }
        }
      }

    }
  }
}

void CSVGraphImport::end(unsigned int , unsigned int ) {
}

