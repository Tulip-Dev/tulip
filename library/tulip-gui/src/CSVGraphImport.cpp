/**
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
#include <iomanip>

#include "tulip/CSVGraphImport.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;
using namespace std;

CSVImportParameters::CSVImportParameters(unsigned int fromLine, unsigned int toLine,
                                         const vector<CSVColumn *> &columns)
    : fromLine(fromLine), toLine(toLine), columns(columns) {}

CSVImportParameters::~CSVImportParameters() {}

unsigned int CSVImportParameters::columnNumber() const {
  return columns.size();
}

bool CSVImportParameters::importColumn(unsigned int column) const {
  if (column < columns.size()) {
    return columns[column]->isUsed();
  } else {
    return false;
  }
}

string CSVImportParameters::getColumnName(unsigned int column) const {
  if (column < columns.size()) {
    return columns[column]->name();
  } else {
    return string();
  }
}

string CSVImportParameters::getColumnDataType(unsigned int column) const {
  if (column < columns.size()) {
    return columns[column]->dataType();
  } else {
    return string();
  }
}

char CSVImportParameters::getColumnMultiValueSeparator(unsigned int column) const {
  if (column < columns.size())
    return columns[column]->getMultiValueSeparator();
  return 0;
}

CSVColumn::Action CSVImportParameters::getColumnActionForToken(unsigned int column,
                                                               const std::string &token) const {
  if (column < columns.size())
    return columns[column]->getActionForToken(token);
  return CSVColumn::Action::SKIP_ROW;
}

bool CSVImportParameters::importRow(unsigned int row) const {
  return row >= fromLine && row <= toLine;
}

unsigned int CSVImportParameters::getFirstLineIndex() const {
  return fromLine;
}
unsigned int CSVImportParameters::getLastLineIndex() const {
  return toLine;
}

AbstractCSVToGraphDataMapping::AbstractCSVToGraphDataMapping(Graph *graph, ElementType type,
                                                             const vector<unsigned int> &colIds,
                                                             const vector<string> &propertyNames)
    : graph(graph), type(type), columnIds(colIds) {
  assert(graph != nullptr);

  keyProperties.reserve(propertyNames.size());
  for (const string &pName : propertyNames) {
    assert(graph->existProperty(pName));
    keyProperties.push_back(graph->getProperty(pName));
  }
}

void AbstractCSVToGraphDataMapping::init(unsigned int) {
  // Clean old information.
  valueToId.clear();

  // Fill map with graph values.
  if (type == NODE) {
    for (auto n : graph->nodes()) {
      string key;

      for (auto prop : keyProperties)
        key.append(prop->getNodeStringValue(n));

      valueToId.emplace(std::move(key), n.id);
    }
  } else {
    for (auto e : graph->edges()) {
      string key;

      for (auto prop : keyProperties)
        key.append(prop->getEdgeStringValue(e));

      valueToId.emplace(std::move(key), e.id);
    }
  }
}

pair<ElementType, vector<unsigned int>>
AbstractCSVToGraphDataMapping::getElementsForRow(const vector<vector<string>> &tokens) {
  unsigned int result;

  bool idsOK = true;

  // Check if the ids are available for this line
  for (unsigned int i = 0; i < columnIds.size(); ++i) {
    if (columnIds[i] >= tokens.size()) {
      idsOK = false;
      break;
    }
  }

  if (idsOK) {
    string key;
    vector<string> keys;

    for (unsigned int i = 0; i < columnIds.size(); ++i) {
      for (const string &token : tokens[columnIds[i]]) {
        key.append(token);
        keys.push_back(token);
      }
    }

    if (valueToId.find(key) == valueToId.end()) {
      // Try to generate the element
      unsigned int id = buildIndexForRow(0, keys);

      // If the token was correctly generated.
      if (id != UINT_MAX) {
        // Store the id corresponding to the token.
        valueToId.emplace(std::move(key), id);
      }

      result = id;
    } else
      // Use the last found id.
      result = valueToId[key];
  } else
    result = UINT_MAX;

  return pair<ElementType, vector<unsigned int>>(type, {result});
}

CSVToNewNodeIdMapping::CSVToNewNodeIdMapping(Graph *graph) : graph(graph) {}

void CSVToNewNodeIdMapping::init(unsigned int rowNumber) {
  Graph *root = graph->getRoot();
  root->reserveNodes(root->numberOfNodes() + rowNumber);
}

pair<ElementType, vector<unsigned int>>
CSVToNewNodeIdMapping::getElementsForRow(const vector<vector<string>> &) {
  return pair<ElementType, vector<unsigned int>>(NODE, {graph->addNode().id});
}

CSVToGraphNodeIdMapping::CSVToGraphNodeIdMapping(Graph *graph, const vector<unsigned int> &colIds,
                                                 const vector<string> &propNames, bool createNode)
    : AbstractCSVToGraphDataMapping(graph, NODE, colIds, propNames),
      createMissingNodes(createNode) {}

void CSVToGraphNodeIdMapping::init(unsigned int rowNumber) {
  AbstractCSVToGraphDataMapping::init(rowNumber);

  if (createMissingNodes) {
    Graph *root = graph->getRoot();
    root->reserveNodes(root->numberOfNodes() + rowNumber);
  }
}

unsigned int CSVToGraphNodeIdMapping::buildIndexForRow(unsigned int, const vector<string> &keys) {
  if (createMissingNodes && keys.size() == keyProperties.size()) {
    node newNode = graph->addNode();

    for (unsigned int i = 0; i < keys.size(); ++i)
      keyProperties[i]->setNodeStringValue(newNode, keys[i]);

    return newNode.id;
  } else {
    return UINT_MAX;
  }
}

CSVToGraphEdgeIdMapping::CSVToGraphEdgeIdMapping(Graph *graph, const vector<unsigned int> &colIds,
                                                 const vector<string> &propNames)
    : AbstractCSVToGraphDataMapping(graph, EDGE, colIds, propNames) {}

unsigned int CSVToGraphEdgeIdMapping::buildIndexForRow(unsigned int, const vector<string> &) {
  return UINT_MAX;
}

CSVToGraphEdgeSrcTgtMapping::CSVToGraphEdgeSrcTgtMapping(
    Graph *graph, const vector<unsigned int> &srcColIds, const vector<unsigned int> &tgtColIds,
    const vector<string> &srcPropNames, const vector<string> &tgtPropNames, bool createMissinNodes)
    : graph(graph), srcColumnIds(srcColIds), tgtColumnIds(tgtColIds),
      sameSrcTgtProperties(srcPropNames.size() == tgtPropNames.size()),
      buildMissingElements(createMissinNodes) {
  assert(graph != nullptr);

  for (const string &pName : srcPropNames) {
    assert(graph->existProperty(pName));
    srcProperties.push_back(graph->getProperty(pName));
  }

  for (unsigned int i = 0; i < tgtPropNames.size(); ++i) {
    assert(graph->existProperty(tgtPropNames[i]));
    tgtProperties.push_back(graph->getProperty(tgtPropNames[i]));
    sameSrcTgtProperties = sameSrcTgtProperties && (tgtPropNames[i] == srcPropNames[i]);
  }
}

void CSVToGraphEdgeSrcTgtMapping::init(unsigned int rowNumber) {
  srcValueToId.clear();
  tgtValueToId.clear();
  for (auto n : graph->nodes()) {
    string key;

    for (unsigned int i = 0; i < srcProperties.size(); ++i)
      key.append(srcProperties[i]->getNodeStringValue(n));

    if (sameSrcTgtProperties)
      srcValueToId.emplace(std::move(key), n.id);
    else {
      srcValueToId.emplace(key, n.id);
      key.clear();

      for (unsigned int i = 0; i < tgtProperties.size(); ++i)
        key.append(tgtProperties[i]->getNodeStringValue(n));

      tgtValueToId.emplace(std::move(key), n.id);
    }
  }

  // Reserve elements
  Graph *root = graph->getRoot();
  root->reserveEdges(root->numberOfEdges() + rowNumber);

  if (buildMissingElements) {
    // Need to reserve for source and target nodes.
    root->reserveNodes(2 * rowNumber + root->numberOfNodes());
  }
}

pair<ElementType, vector<unsigned int>>
CSVToGraphEdgeSrcTgtMapping::getElementsForRow(const vector<vector<string>> &tokens) {
  vector<node> srcs;
  vector<node> tgts;

  bool idsOK = true;

  // Check if the src ids are available for this line
  for (unsigned int i = 0; i < srcColumnIds.size(); ++i) {
    if (srcColumnIds[i] >= tokens.size()) {
      idsOK = false;
      break;
    }
  }

  if (idsOK) {
    vector<vector<string>> keyTokens;

    for (unsigned int i = 0; i < srcColumnIds.size(); ++i) {
      const vector<string> &currentTokens = tokens[srcColumnIds[i]];

      // merge current tokens with previous ones
      if (keyTokens.empty()) {
        keyTokens.resize(currentTokens.size());

        for (unsigned int j = 0; j < currentTokens.size(); ++j)
          keyTokens[j].push_back(currentTokens[j]);
      } else {
        vector<vector<string>> previousTokens(keyTokens);
        keyTokens.clear();
        keyTokens.resize(previousTokens.size() * currentTokens.size());

        for (unsigned int j = 0; j < previousTokens.size(); ++j) {
          for (unsigned int k = 0; k < currentTokens.size(); ++k) {
            keyTokens[j * currentTokens.size() + k] = previousTokens[j];
            keyTokens[j * currentTokens.size() + k].push_back(currentTokens[k]);
          }
        }
      }
    }

    for (unsigned int i = 0; i < keyTokens.size(); ++i) {
      // because column values may be of type vector
      // we can have several source entities
      string key;

      for (unsigned int j = 0; j < keyTokens[i].size(); ++j)
        key.append(keyTokens[i][j]);

      tlp_hash_map<string, unsigned int>::iterator it = srcValueToId.find(key);

      // token exists in the map
      if (it != srcValueToId.end()) {
        srcs.emplace_back(it->second);
      } else if (buildMissingElements && srcProperties.size() == keyTokens[i].size()) {
        node src = graph->addNode();
        srcs.push_back(src);

        for (unsigned int j = 0; j < keyTokens[i].size(); ++j)
          srcProperties[j]->setNodeStringValue(src, keyTokens[i][j]);

        srcValueToId.emplace(std::move(key), src.id);
      }
    }
  }

  // Check if the target ids are available for this line
  for (unsigned int i = 0; i < tgtColumnIds.size(); ++i) {
    if (tgtColumnIds[i] >= tokens.size()) {
      idsOK = false;
      break;
    }
  }

  if (idsOK) {
    vector<vector<string>> keyTokens;

    for (unsigned int i = 0; i < tgtColumnIds.size(); ++i) {
      const vector<string> &currentTokens = tokens[tgtColumnIds[i]];

      // merge current tokens with previous ones
      if (keyTokens.empty()) {
        keyTokens.resize(currentTokens.size());

        for (unsigned int j = 0; j < currentTokens.size(); ++j)
          keyTokens[j].push_back(currentTokens[j]);
      } else {
        vector<vector<string>> previousTokens(keyTokens);
        keyTokens.clear();
        keyTokens.resize(previousTokens.size() * currentTokens.size());

        for (unsigned int j = 0; j < previousTokens.size(); ++j) {
          for (unsigned int k = 0; k < currentTokens.size(); ++k) {
            keyTokens[j * currentTokens.size() + k] = previousTokens[j];
            keyTokens[j * currentTokens.size() + k].push_back(currentTokens[k]);
          }
        }
      }
    }

    tlp_hash_map<string, unsigned int> &valueToId =
        sameSrcTgtProperties ? srcValueToId : tgtValueToId;

    for (unsigned int i = 0; i < keyTokens.size(); ++i) {
      // because column values may be of type vector
      // we can have several target entities
      string key;

      for (unsigned int j = 0; j < keyTokens[i].size(); ++j)
        key.append(keyTokens[i][j]);

      tlp_hash_map<string, unsigned int>::iterator it = valueToId.find(key);

      // token exists in the map
      if (it != valueToId.end()) {
        tgts.emplace_back(it->second);
      } else if (buildMissingElements && tgtProperties.size() == keyTokens[i].size()) {
        node tgt = graph->addNode();
        tgts.push_back(tgt);

        for (unsigned int j = 0; j < keyTokens[i].size(); ++j)
          tgtProperties[j]->setNodeStringValue(tgt, keyTokens[i][j]);

        valueToId.emplace(std::move(key), tgt.id);
      }
    }
  }

  // we create as much edges as we can build
  // of valid source-target entities couple
  vector<unsigned int> results;
  results.reserve(srcs.size() * tgts.size());
  for (auto src : srcs) {
    for (auto tgt : tgts) {
      if (src.isValid() && tgt.isValid()) {
        results.push_back(graph->addEdge(src, tgt).id);
      }
    }
  }

  return pair<ElementType, vector<unsigned int>>(EDGE, std::move(results));
}

CSVImportColumnToGraphPropertyMappingProxy::CSVImportColumnToGraphPropertyMappingProxy(
    Graph *graph, const CSVImportParameters &importParameters, QWidget *parent)
    : graph(graph), importParameters(importParameters), parent(parent) {}

PropertyInterface *
CSVImportColumnToGraphPropertyMappingProxy::generateApproximateProperty(const std::string &name,
                                                                        const std::string &type) {
  // loop to generate a non existing approximate name
  std::ostringstream nameBuf;
  unsigned int nb = 1;
  while (true) {
    nameBuf << name << '_' << setfill('0') << setw(2) << nb;
    if (!graph->existProperty(nameBuf.str()))
      return graph->getProperty(nameBuf.str(), type);
    nameBuf.seekp(0);
    ++nb;
  }
  return nullptr;
}

PropertyInterface *
CSVImportColumnToGraphPropertyMappingProxy::getPropertyInterface(unsigned int column,
                                                                 const string &) {
  tlp_hash_map<unsigned int, PropertyInterface *>::iterator it = propertiesBuffer.find(column);

  // No properties
  if (it == propertiesBuffer.end()) {
    string propertyType = importParameters.getColumnDataType(column);
    string propertyName = importParameters.getColumnName(column);

    // If auto detection fails, set to default type : string.
    if (propertyType.empty()) {
      qWarning() << __PRETTY_FUNCTION__ << " No type for the column \"" << propertyName
                 << "\", set it to string";
      propertyType = "string";
    }

    PropertyInterface *interf = nullptr;

    // If the property already exists.
    // we need to check the compatibility
    if (graph->existProperty(propertyName)) {
      PropertyInterface *existingProperty = graph->getProperty(propertyName);

      // If the properties are compatible, query if we have to use existing.
      if (existingProperty->getTypename().compare(propertyType) == 0) {
        if (overwritePropertiesButton != QMessageBox::YesToAll &&
            overwritePropertiesButton != QMessageBox::NoToAll) {
          overwritePropertiesButton = QMessageBox::question(
              parent, parent->tr("Property already exists"),
              parent->tr("A property named \"") + tlpStringToQString(propertyName) +
                  parent->tr("\" already exists.\nDo you want to use it ?\nIf not, a property with "
                             "an approximate name will be generated."),
              QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll,
              QMessageBox::Yes);
        }

        if (overwritePropertiesButton == QMessageBox::NoToAll ||
            overwritePropertiesButton == QMessageBox::No) {
          interf = generateApproximateProperty(propertyName, propertyType);
        } else {
          interf = graph->getProperty(propertyName);
        }
      } else {
        // If the properties are not compatible,
        // query if we have to use the existing one.
        if (QMessageBox::question(
                parent, "Property already exists",
                QString(
                    "A property named \"%0\" of type '%1' already exists.\nDo you want to use it ?\nIf not, a property with "
                    "an approximate name will be generated.")
                    .arg(propertyName.c_str())
                    .arg(existingProperty->getTypename().c_str()),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
          interf = existingProperty;
        else
          interf = generateApproximateProperty(propertyName, propertyType);
      }
    } else {
      interf = graph->getProperty(propertyName, propertyType);
    }

    propertiesBuffer[column] = interf;
    return interf;
  } else {
    return it->second;
  }
}

CSVGraphImport::CSVGraphImport(CSVToGraphDataMapping *mapping,
                               CSVImportColumnToGraphPropertyMapping *properties,
                               const CSVImportParameters &importParameters)
    : mapping(mapping), propertiesManager(properties), importParameters(importParameters) {}
CSVGraphImport::~CSVGraphImport() {}
bool CSVGraphImport::begin() {
  mapping->init(importParameters.getLastLineIndex() - importParameters.getFirstLineIndex() + 1);
  return true;
}

bool CSVGraphImport::line(unsigned int row, const vector<CSVToken> &lineTokens) {
  // Check if user wants to import the line.
  if (!importParameters.importRow(row)) {
    return true;
  }

  // build vector of property interface and vector of input tokens
  vector<PropertyInterface *> props(lineTokens.size(), nullptr);
  vector<std::vector<std::string>> tokens(lineTokens.size());

  for (size_t column = 0; column < lineTokens.size(); ++column) {
    if (importParameters.importColumn(column)) {
      PropertyInterface *property = props[column] =
          propertiesManager->getPropertyInterface(column, lineTokens[column].value);
      const string &token = lineTokens[column].value;

      // If the property does not exists or
      // if the token is empty no need to import the value
      if (property != nullptr && !token.empty()) {
        CSVColumn::Action action = CSVColumn::Action::ASSIGN_VALUE;
        bool isVectorProperty = (property->getTypename().find("vector") == 0);

        if (isVectorProperty) {
          string tokenCopy = token;
          // check if the list of values is enclosed
          // between an openChar and a closeChar
          size_t first = token.find_first_not_of(" \t\f\v");

          if (first != string::npos) {
            char closeChar = '\0';
            // get the openChar and
            // the possible closeChar
            switch (token[first]) {
            case '(':
              closeChar = ')';
              break;

            case '[':
              closeChar = ']';
              break;

            case '{':
              closeChar = '}';
              break;

            case '<':
              closeChar = '>';
              break;

            default:
              break;
            }

            if (closeChar) {
              // check if we find the closeChar at the end of token
              size_t last = token.find_last_not_of(" \t\f\v");

              if (token[last] == closeChar) {
                // remove closeChar
                tokenCopy.resize(last);
                // and openChar
                tokenCopy.erase(0, first + 1);
              }
            }
          }
          static_cast<VectorPropertyInterface *>(property)->tokenize(
              tokenCopy, tokens[column], '\0',
              importParameters.getColumnMultiValueSeparator(column), '\0');
          // check tokens actions
          for (const std::string &tok : tokens[column]) {
            auto tokAction = importParameters.getColumnActionForToken(column, tok);
            if (tokAction == CSVColumn::Action::SKIP_ROW) {
              action = tokAction;
              break;
            } else if (tokAction != CSVColumn::Action::ASSIGN_VALUE)
              action = tokAction;
          }
        } else {
          action = importParameters.getColumnActionForToken(column, token);
          tokens[column].push_back(token);
        }
        if (action == CSVColumn::Action::SKIP_ROW)
          return true;
        if (action == CSVColumn::Action::ASSIGN_NO_VALUE)
          tokens[column].clear();
      }
    }
  }

  // Compute the element id associated to the line
  pair<ElementType, vector<unsigned int>> &&elements = mapping->getElementsForRow(tokens);

  for (size_t column = 0; column < lineTokens.size(); ++column) {
    PropertyInterface *property = props[column];

    // If the property does not exists or
    // if the token is empty no need to import the value
    if (property != nullptr && !tokens[column].empty()) {
      bool isVectorProperty = (property->getTypename().find("vector") == 0);
      if (elements.first == NODE) {
        for (size_t i = 0; i < elements.second.size(); ++i) {
          if (elements.second[i] == UINT_MAX)
            continue;

          if (!(isVectorProperty
                    ? static_cast<VectorPropertyInterface *>(property)->setNodeStringValueAsVector(
                          node(elements.second[i]), tokens[column])
                    : property->setNodeStringValue(node(elements.second[i]), tokens[column][0]))) {
            // We add one to the row number as in the configuration widget we start from row 1 not
            // row 0
            qWarning() << __PRETTY_FUNCTION__ << ":" << __LINE__ << " error when importing token \""
                       << lineTokens[column].value << "\" in property \"" << property->getName()
                       << "\" of type \"" << property->getTypename() << "\" at line " << row + 1;
          }
        }
      } else {
        for (size_t i = 0; i < elements.second.size(); ++i) {
          if (!(isVectorProperty
                    ? static_cast<VectorPropertyInterface *>(property)->setEdgeStringValueAsVector(
                          edge(elements.second[i]), tokens[column])
                    : property->setEdgeStringValue(edge(elements.second[i]), tokens[column][0]))) {
            // We add one to the row number as in the configuration widget we start from row 1 not
            // row 0
            qWarning() << __PRETTY_FUNCTION__ << ":" << __LINE__ << " error when importing token \""
                       << lineTokens[column].value << "\" in property \"" << property->getName()
                       << "\" of type \"" << property->getTypename() << "\" at line " << row + 1;
          }
        }
      }
    }
  }

  return true;
}

bool CSVGraphImport::end(unsigned int, unsigned int) {
  return true;
}
