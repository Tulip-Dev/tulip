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
#include "tulip/CSVGraphImport.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/PropertyTypes.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;
using namespace std;

static const char minusChar = '-';

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

AbstractCSVToGraphDataMapping::AbstractCSVToGraphDataMapping(Graph* graph,ElementType type, const vector<unsigned int>& colIds, const vector<string>& propertyNames):graph(graph),type(type),columnIds(colIds) {
  assert(graph!=NULL);

  for (unsigned int i = 0; i < propertyNames.size(); ++i) {
    assert(graph->existProperty(propertyNames[i]));
    keyProperties.push_back(graph->getProperty(propertyNames[i]));
  }
}

void AbstractCSVToGraphDataMapping::init(unsigned int) {
  //Clean old informations.
  valueToId.clear();

  //Fill map with graph values.
  if(type == NODE) {
    node n;
    forEach(n,graph->getNodes()) {
      string key;

      for (unsigned int i = 0; i < keyProperties.size(); ++i)
        key.append(keyProperties[i]->getNodeStringValue(n));

      valueToId[key]=n.id;
    }
  }
  else {
    edge e;
    forEach(e,graph->getEdges()) {
      string key;

      for (unsigned int i = 0; i < keyProperties.size(); ++i)
        key.append(keyProperties[i]->getEdgeStringValue(e));

      valueToId[key]=e.id;
    }
  }
}

pair<ElementType, vector<unsigned int> > AbstractCSVToGraphDataMapping::getElementsForRow(const vector<string>& tokens, const vector<PropertyInterface*>) {
  vector<unsigned int> results(1);

  bool idsOK = true;

  //Check if the ids are available for this line
  for (unsigned int i = 0; i < columnIds.size(); ++i) {
    if (columnIds[i] >= tokens.size()) {
      idsOK = false;
      break;
    }
  }

  if(idsOK) {
    string key;
    vector<string> keys;

    for (unsigned int i = 0; i < columnIds.size(); ++i) {
      string token = tokens[columnIds[i]];
      key.append(token);
      keys.push_back(token);
    }

    if (valueToId.find(key) == valueToId.end()) {
      //Try to generate the element
      unsigned int id =
        buildIndexForRow(0, keys);

      //If the token was correctly generated.
      if(id!=UINT_MAX) {
        //Store the id corresponding to the token.
        valueToId[key] = id;
      }

      results[0] = id;
    }
    else
      //Use the last found id.
      results[0] = valueToId[key];
  }
  else
    results[0] = UINT_MAX;

  return pair<ElementType, vector<unsigned int> >(type, results);
}

CSVToNewNodeIdMapping::CSVToNewNodeIdMapping(Graph* graph):graph(graph) {
}

void CSVToNewNodeIdMapping::init(unsigned int rowNumber) {
  graph->reserveNodes(rowNumber);
}

pair<ElementType, vector<unsigned int> > CSVToNewNodeIdMapping::getElementsForRow(const vector<string>&, const vector<PropertyInterface*>) {
  vector<unsigned int> result(1);
  result[0] = graph->addNode().id;
  return make_pair(NODE, result);
}

CSVToGraphNodeIdMapping::CSVToGraphNodeIdMapping(Graph* graph, const vector<unsigned int>& colIds, const vector<string>& propNames,bool createNode):AbstractCSVToGraphDataMapping(graph,NODE,colIds,propNames),createMissingNodes(createNode) {
}

void CSVToGraphNodeIdMapping::init(unsigned int rowNumber) {
  AbstractCSVToGraphDataMapping::init(rowNumber);

  if(createMissingNodes) {
    graph->reserveNodes(rowNumber);
  }
}

unsigned int CSVToGraphNodeIdMapping::buildIndexForRow(unsigned int, const vector<string>& keys) {
  if(createMissingNodes && keys.size() == keyProperties.size()) {
    node newNode = graph->addNode();

    for (unsigned int i = 0; i < keys.size(); ++i)
      keyProperties[i]->setNodeStringValue(newNode, keys[i]);

    return newNode.id;
  }
  else {
    return UINT_MAX;
  }
}

CSVToGraphEdgeIdMapping::CSVToGraphEdgeIdMapping(Graph* graph, const vector<unsigned int>& colIds,const vector<string>& propNames):AbstractCSVToGraphDataMapping(graph,EDGE,colIds,propNames) {
}

unsigned int CSVToGraphEdgeIdMapping::buildIndexForRow(unsigned int, const vector<string>&) {
  return UINT_MAX;
}

CSVToGraphEdgeSrcTgtMapping::CSVToGraphEdgeSrcTgtMapping(Graph* graph,
    const vector<unsigned int>& srcColIds, const vector<unsigned int>& tgtColIds,const vector<string>& srcPropNames, const vector<string>& tgtPropNames,
    bool createMissinNodes):
  graph(graph),srcColumnIds(srcColIds),tgtColumnIds(tgtColIds),sameSrcTgtProperties(srcPropNames.size() == tgtPropNames.size()),buildMissingElements(createMissinNodes) {
  assert(graph != NULL);


  for (unsigned int i = 0; i < srcPropNames.size(); ++i) {
    assert(graph->existProperty(srcPropNames[i]));
    srcProperties.push_back(graph->getProperty(srcPropNames[i]));
  }

  for (unsigned int i = 0; i < tgtPropNames.size(); ++i) {
    assert(graph->existProperty(tgtPropNames[i]));
    tgtProperties.push_back(graph->getProperty(tgtPropNames[i]));
    sameSrcTgtProperties = sameSrcTgtProperties &&
                           (tgtPropNames[i] == srcPropNames[i]);
  }
}

void CSVToGraphEdgeSrcTgtMapping::init(unsigned int rowNumber) {
  srcValueToId.clear();
  node n;
  forEach(n,graph->getNodes()) {
    string key;

    for (unsigned int i = 0; i < srcProperties.size(); ++i)
      key.append(srcProperties[i]->getNodeStringValue(n));

    srcValueToId[key]=n.id;

    if (!sameSrcTgtProperties) {
      key.clear();

      for (unsigned int i = 0; i < tgtProperties.size(); ++i)
        key.append(tgtProperties[i]->getNodeStringValue(n));

      tgtValueToId[key]=n.id;
    }
  }

  //Reserve memory
  graph->reserveEdges(rowNumber);

  if(buildMissingElements) {
    //Need to reserve for source and target nodes.
    graph->reserveNodes(2*rowNumber);
  }
}

// split a columns contents into string tokens
static bool splitIntoTokens(const string& token, vector<string>& tokens,
                            char sepChar) {
  tokens.clear();
  char c = ' ';
  char strDelimChar = '\0';

  istringstream is(token);

  // go to first non space char
  while((is >> c) && isspace(c)) {}

  if (c == '"')
    strDelimChar = '"';

  if (c == '\'')
    strDelimChar = '\'';

  is.unget();

  is.unsetf(ios_base::skipws);
  bool firstVal = true;
  bool sepFound = false;

  for( ;; ) {
    if ( !(is >> c) )
      return (!sepFound);

    if (isspace(c))
      continue;

    if (c == sepChar) {
      if (sepFound)
        return false;

      sepFound = true;
    }
    else {
      if ((firstVal || sepFound) && (!strDelimChar || c == strDelimChar)) {
        string str;
        is.unget();

        if (!StringType::read(is, str, strDelimChar, sepChar))
          return false;

        tokens.push_back(str);

        if (!strDelimChar)
          // last read char was sepChar
          is.unget();

        firstVal = false;
        sepFound = false;
      }
      else
        return false;
    }
  }
}

pair<ElementType, vector<unsigned int> > CSVToGraphEdgeSrcTgtMapping::getElementsForRow(const vector<string>& lineTokens, const vector<PropertyInterface*> props) {

  vector<unsigned int> results;

  vector<node> srcs;
  vector<node> tgts;

  bool idsOK = true;

  //Check if the src ids are available for this line
  for (unsigned int i = 0; i < srcColumnIds.size(); ++i) {
    if (srcColumnIds[i] >= lineTokens.size()) {
      idsOK = false;
      break;
    }
  }

  if (idsOK) {
    vector<vector<string> > tokens;

    for (unsigned int i = 0; i < srcColumnIds.size();  ++i) {
      const string& token = lineTokens[srcColumnIds[i]];
      vector<string> currentTokens;
      PropertyInterface* srcProp = props[srcColumnIds[i]];

      // if srcProp is of type vector, we may find multiple tokens
      if (srcProp && srcProp->getTypename().find("vector") == 0) {
        if (!splitIntoTokens(token, currentTokens, ',')) {
          results.push_back(UINT_MAX);
          return make_pair(EDGE, results);
        }
      }
      else
        currentTokens.push_back(token);

      // merge current tokens with previous ones
      if (tokens.empty()) {
        tokens.resize(currentTokens.size());

        for (unsigned int j = 0; j < currentTokens.size(); ++j)
          tokens[j].push_back(currentTokens[j]);
      }
      else {
        vector<vector<string> > previousTokens(tokens);
        tokens.clear();
        tokens.resize(previousTokens.size() * currentTokens.size());

        for(unsigned int j = 0; j < previousTokens.size(); ++j) {
          for (unsigned int k = 0; k < currentTokens.size(); ++k) {
            tokens[j * currentTokens.size() + k] = previousTokens[j];
            tokens[j * currentTokens.size() + k].push_back(currentTokens[k]);
          }
        }
      }
    }

    for(unsigned int i = 0; i < tokens.size(); ++i) {
      // because column values may be of type vector
      // we can have several source entities
      string key;

      for (unsigned int j = 0; j < tokens[i].size(); ++j)
        key.append(tokens[i][j]);

      TLP_HASH_MAP<string,unsigned int>::iterator it =
        srcValueToId.find(key);

      //token exists in the map
      if (it != srcValueToId.end()) {
        srcs.push_back(node(it->second));
      }
      else if (buildMissingElements &&
               srcProperties.size() == tokens[i].size()) {
        node src = graph->addNode();
        srcs.push_back(src);

        for (unsigned int j = 0;  j < tokens[i].size(); ++j)
          srcProperties[j]->setNodeStringValue(src, tokens[i][j]);

        srcValueToId[key] = src.id;
      }
    }
  }

  //Check if the target ids are available for this line
  for (unsigned int i = 0; i < tgtColumnIds.size(); ++i) {
    if (tgtColumnIds[i] >= lineTokens.size()) {
      idsOK = false;
      break;
    }
  }

  if (idsOK) {
    vector<vector<string> > tokens;

    for (unsigned int i = 0; i < tgtColumnIds.size();  ++i) {
      const string& token = lineTokens[tgtColumnIds[i]];
      vector<string> currentTokens;
      PropertyInterface* tgtProp = props[tgtColumnIds[i]];

      // if tgtProp is of type vector, we may find multiple tokens
      if (tgtProp && tgtProp->getTypename().find("vector") == 0) {
        if (!splitIntoTokens(token, currentTokens, ',')) {
          results.push_back(UINT_MAX);
          return make_pair(EDGE, results);
        }
      }
      else
        currentTokens.push_back(token);

      // merge current tokens with previous ones
      if (tokens.empty()) {
        tokens.resize(currentTokens.size());

        for (unsigned int j = 0; j < currentTokens.size(); ++j)
          tokens[j].push_back(currentTokens[j]);
      }
      else {
        vector<vector<string> > previousTokens(tokens);
        tokens.clear();
        tokens.resize(previousTokens.size() * currentTokens.size());

        for(unsigned int j = 0; j < previousTokens.size(); ++j) {
          for (unsigned int k = 0; k < currentTokens.size(); ++k) {
            tokens[j * currentTokens.size() + k] = previousTokens[j];
            tokens[j * currentTokens.size() + k].push_back(currentTokens[k]);
          }
        }
      }
    }

    TLP_HASH_MAP<string,unsigned int>& valueToId =
      sameSrcTgtProperties ? srcValueToId : tgtValueToId;

    for(unsigned int i = 0; i < tokens.size(); ++i) {
      // because column values may be of type vector
      // we can have several target entities
      string key;

      for (unsigned int j = 0; j < tokens[i].size(); ++j)
        key.append(tokens[i][j]);

      TLP_HASH_MAP<string,unsigned int>::iterator it = valueToId.find(key);

      //token exists in the map
      if (it != valueToId.end()) {
        tgts.push_back(node(it->second));
      }
      else if (buildMissingElements &&
               tgtProperties.size() == tokens[i].size()) {
        node tgt = graph->addNode();
        tgts.push_back(tgt);

        for (unsigned int j = 0;  j < tokens[i].size(); ++j)
          tgtProperties[j]->setNodeStringValue(tgt, tokens[i][j]);

        valueToId[key] = tgt.id;
      }
    }
  }

  // we create as much edges as we can build
  // of valid source-target entities couple
  for (unsigned int i = 0; i < srcs.size(); ++i) {
    for (unsigned int j = 0; j < tgts.size(); ++j) {
      edge e;

      if(srcs[i].isValid() && tgts[j].isValid()) {
        results.push_back(graph->addEdge(srcs[i],tgts[j]).id);
      }
    }
  }

  return make_pair(EDGE, results);
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
      qWarning()<<__PRETTY_FUNCTION__<<" No type for the column "<<propertyName<<" set to string";
      propertyType = "string";
    }

    PropertyInterface *interf=NULL;

    //The property already exists. Need to check if existing property is compatible with the new one.
    if (graph->existProperty(propertyName)) {
      PropertyInterface *existingProperty = graph->getProperty(propertyName);

      //If the properties are compatible query if we had to override existing.
      if (existingProperty->getTypename().compare(propertyType)==0) {
        if (overwritePropertiesButton != QMessageBox::YesToAll && overwritePropertiesButton != QMessageBox::NoToAll) {
          overwritePropertiesButton = QMessageBox::question(parent, parent->tr("Property already exists."),
                                      parent->tr("A property with the name \"") + tlpStringToQString(propertyName) + parent->tr(
                                        "\" already exists. Overwrite?"), QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No
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
        QMessageBox::critical(parent, parent->tr("Property already existing."), parent->tr("A property with the name \"") + tlpStringToQString(
                                propertyName) + parent->tr("\" already exists with a different type. This property will be ignored."));
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

void CSVGraphImport::line(unsigned int row,const vector<string>& lineTokens) {
  //Check if user wants to import the line.
  if(!importParameters.importRow(row) ) {
    return;
  }

  vector<PropertyInterface *> props(lineTokens.size(), NULL);

  for(size_t column = 0 ; column < lineTokens.size() ; ++column) {
    if (importParameters.importColumn(column))
      props[column] =
        propertiesManager->getPropertyInterface(column, lineTokens[column]);
  }

  //Compute the element id associated to the line
  pair<ElementType, vector<unsigned int> > elements =
    mapping->getElementsForRow(lineTokens, props);

  for(size_t column = 0 ; column < lineTokens.size() ; ++column) {
    //If user wants to import the column
    if (importParameters.importColumn(column)) {
      const string& token = lineTokens[column];
      PropertyInterface *property = props[column];

      //If the property don't exists or
      //if the token is empty no need to import the value
      if (property != NULL && !token.empty()) {
        bool isVectorProperty = (property->getTypename().find("vector") == 0);
        bool openParen = false;

        if (isVectorProperty) {
          size_t first = token.find_last_not_of(" \t\f\v");

          if (first != string::npos)
            openParen = (token[first] == '(');
        }

        if(elements.first == NODE) {
          for (size_t i = 0; i < elements.second.size(); ++i) {
            if (elements.second[i] == UINT_MAX )
              continue;

            if (!(isVectorProperty ?
                  ((VectorPropertyInterface *)property)->setNodeStringValueAsVector(node(elements.second[i]), token, openParen ? '(' : '\0', ',', openParen ? ')' : '\0') :
                    property->setNodeStringValue(node(elements.second[i]), token))) {
              //We add one to the row number as in the configuration widget we start from row 1 not row 0
              qWarning()<<__PRETTY_FUNCTION__<<":"<<__LINE__<<" error when importing token \""<< token <<"\" in property \""<<property->getName()<<"\" of type \""<<property->getTypename()<<"\" at line "<<row+1;
            }
          }
        }
        else {
          for (size_t i = 0; i < elements.second.size(); ++i) {
            if(!(isVectorProperty ?
                 ((VectorPropertyInterface *)property)->setEdgeStringValueAsVector(edge(elements.second[i]), token, openParen ? '(' : '\0', ',', openParen ? ')' : '\0') :
                   property->setEdgeStringValue(edge(elements.second[i]), token))) {
              //We add one to the row number as in the configuration widget we start from row 1 not row 0
              qWarning()<<__PRETTY_FUNCTION__<<":"<<__LINE__<<" error when importing token \""<<token<<"\" in property \""<<property->getName()<<"\" of type \""<<property->getTypename()<<"\" at line "<<row+1;
            }
          }
        }
      }
    }
  }
}

void CSVGraphImport::end(unsigned int , unsigned int ) {
}

