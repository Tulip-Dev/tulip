/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <cerrno>
#include <sys/stat.h>
#include <sstream>
#include <stack>

#include <tulip/Graph.h>
#include <tulip/GraphAbstract.h>
#include <tulip/ImportModule.h>
#include <tulip/PropertyInterface.h>
#include <tulip/TlpTools.h>
#include <tulip/JsonTokens.h>
#include <tulip/YajlFacade.h>
#include <tulip/GraphProperty.h>

using namespace std;
using namespace tlp;

typedef std::map<unsigned, unsigned> TemporaryGraphValue;
typedef std::map<std::string, TemporaryGraphValue> TemporaryGraphProperty;

class TlpJsonGraphParser : public YajlParseFacade {
public:
  TlpJsonGraphParser(tlp::Graph *parentGraph, tlp::PluginProgress *progress)
      : YajlParseFacade(progress), _parsingEdges(false), _parsingNodes(false), _newEdge(false),
        _edgeSource(UINT_MAX), _parsingNodesIds(false), _parsingEdgesIds(false),
        _parsingEdgesNumber(false), _parsingInterval(false), _newInterval(false),
        _intervalSource(UINT_MAX), _graph(parentGraph),
        _dataSet(&const_cast<DataSet &>(parentGraph->getAttributes())), _parsingAttributes(false),
        _currentAttributeName(std::string()), _currentAttributeTypeName(std::string()),
        _parsingProperties(false), _currentProperty(nullptr), _propertyName(std::string()),
        _currentIdentifier(UINT_MAX), _parsingPropertyType(false),
        _parsingPropertyNodeValues(false), _parsingPropertyEdgeValues(false),
        _parsingPropertyDefaultEdgeValue(false), _parsingPropertyDefaultNodeValue(false),
        _parsingPathViewProperty(false), _waitingForGraphId(false) {}

  void setGraphPropertiesValues() {
    for (std::map<tlp::Graph *, TemporaryGraphProperty>::const_iterator graphIterator =
             _graphProperties.begin();
         graphIterator != _graphProperties.end(); ++graphIterator) {
      for (TemporaryGraphProperty::const_iterator propertyIterator = graphIterator->second.begin();
           propertyIterator != graphIterator->second.end(); ++propertyIterator) {
        tlp::GraphProperty *prop =
            graphIterator->first->getProperty<tlp::GraphProperty>(propertyIterator->first);

        for (TemporaryGraphValue::const_iterator valueIterator = propertyIterator->second.begin();
             valueIterator != propertyIterator->second.end(); ++valueIterator) {
          prop->setNodeValue(tlp::node(valueIterator->first), _clusterIndex[valueIterator->second]);
        }
      }
    }
  }

  void parseStartArray() override {

    if (!_parsingSubgraph.empty() && !_parsingNodesIds && !_parsingEdgesIds &&
        !_parsingAttributes) {
      ++_parsingSubgraph.top();
    }

    if (_parsingEdges) {
      _newEdge = true;
    }

    if (_parsingInterval) {
      _newInterval = true;
    }

    if (_parsingNodesIds || _parsingEdgesIds) {
      _parsingInterval = true;
    }
  }

  void parseEndArray() override {

    if (!_parsingSubgraph.empty() && !_parsingNodesIds && !_parsingEdgesIds &&
        !_parsingAttributes && !_parsingInterval) {
      --_parsingSubgraph.top();

      // finished parsing the subgraphs
      if (_parsingSubgraph.top() == 0) {
        setGraphPropertiesValues();
        _parsingSubgraph.pop();
        _graph = _graph->getSuperGraph();
      }
    }

    // if the current array was not an edge but was the array of edges, we are done parsing edges
    if (!_newEdge && _parsingEdges) {
      _parsingEdges = false;
    }

    if (_newEdge) {
      _newEdge = false;
    }

    if ((_parsingNodesIds || _parsingEdgesIds) && !_newInterval) {
      _parsingNodesIds = false;
      _parsingEdgesIds = false;
    }

    if (!_newInterval) {
      _parsingInterval = false;
    }

    if (_newInterval) {
      _newInterval = false;
    }
  }

  void parseMapKey(const std::string &value) override {
    if (_parsingProperties && !_parsingPropertyNodeValues && !_parsingPropertyEdgeValues &&
        !_parsingPropertyDefaultEdgeValue && !_parsingPropertyDefaultNodeValue &&
        _propertyName.empty()) {
      _propertyName = value;
    }

    if (_currentProperty && value == NodesValuesToken) {
      _parsingPropertyNodeValues = true;
    } else if (_currentProperty && value == EdgesValuesToken) {
      _parsingPropertyEdgeValues = true;
    } else if (value == EdgeDefaultToken) {
      _parsingPropertyDefaultEdgeValue = true;
    } else if (value == NodeDefaultToken) {
      _parsingPropertyDefaultNodeValue = true;
    } else if (value == GraphIDToken) {
      _waitingForGraphId = true;
    } else if (value == NodesIDsToken) {
      _parsingNodesIds = true;
    } else if (value == EdgesIDsToken) {
      _parsingEdgesIds = true;
    } else if (!_currentProperty && value == EdgesToken) {
      _parsingEdges = true;
    } else if (value == AttributesToken) {
      _parsingAttributes = true;
    } else if (value == PropertiesToken) {
      _parsingProperties = true;
    } else if (value == TypeToken) {
      _parsingPropertyType = true;
    } else if (value == NodesNumberToken) {
      _parsingNodes = true;
    } else if (value == EdgesNumberToken) {
      _parsingEdgesNumber = true;
    } else if (_parsingPropertyNodeValues || _parsingPropertyEdgeValues) {
      _currentIdentifier = atoi(value.c_str());
    } else if (_parsingAttributes) {
      _currentAttributeName = value;
    } else if (value == SubgraphsToken) {
      _parsingSubgraph.push(0);
    }
  }

  void parseStartMap() override {}

  void parseEndMap() override {

    if (!_currentProperty && _propertyName.empty()) {
      _parsingProperties = false;
    }

    if (!_parsingPropertyNodeValues && !_parsingPropertyEdgeValues && !_propertyName.empty()) {
      _currentProperty = nullptr;
      _propertyName = string();
    }

    if (_parsingPropertyNodeValues) {
      _parsingPropertyNodeValues = false;
    }

    if (_parsingPropertyEdgeValues) {
      _parsingPropertyEdgeValues = false;
    }

    if (_parsingAttributes) {
      _parsingAttributes = false;
    }

    if (_parsingEdgesIds) {
      _parsingEdgesIds = false;
    }

    if (_parsingNodesIds) {
      _parsingNodesIds = false;
    }

    if (_parsingEdges) {
      _parsingEdges = false;
    }
  }

  void parseInteger(long long integerVal) override {
    if (_waitingForGraphId) {
      if (integerVal > 0) {
        _graph = static_cast<GraphAbstract *>(_graph)->addSubGraph(integerVal);
        _dataSet = &const_cast<DataSet &>(_graph->getAttributes());
        _clusterIndex[integerVal] = _graph;
      }

      _waitingForGraphId = false;
      return; // just ignore whatever is next, we've done what we came for.
    }

    if (_parsingNodes) {
      // allocate space needed for nodes in one call
      _graph->reserveNodes(integerVal);

      // then add nodes
      for (int i = 0; i < integerVal; ++i) {
        _graph->addNode();
      }

      _parsingNodes = false;
      return; // just ignore whatever is next, we've done what we came for.
    }

    if (_parsingEdgesNumber) {
      // allocate space needed for edges in one call
      _graph->reserveEdges(integerVal);

      _parsingEdgesNumber = false;
      return; // just ignore whatever is next, we've done what we came for.
    }

    // if the int is the source or target of an edge
    if (_newEdge) {
      // if the source has not been set, this int is the source
      if (_edgeSource == UINT_MAX) {
        _edgeSource = integerVal;
      }
      // if the source has been set, this int is the target
      else {
        node source(_edgeSource);
        node target(integerVal);
        _graph->addEdge(source, target);
        _edgeSource = UINT_MAX;
      }

      return; // just ignore whatever is next, we've done what we came for.
    }

    if (_parsingInterval) {
      if (_newInterval) {
        if (_intervalSource == UINT_MAX) {
          _intervalSource = integerVal;
        } else {
          for (unsigned int id = _intervalSource; id <= integerVal; ++id) {
            if (_parsingEdgesIds) {
              edge e(id);
              _graph->addEdge(e);
            }

            if (_parsingNodesIds) {
              node n(id);
              _graph->addNode(n);
            }
          }

          _intervalSource = UINT_MAX;
        }
      } else {
        if (_parsingEdgesIds) {
          edge e(integerVal);
          _graph->addEdge(e);
        }

        if (_parsingNodesIds) {
          node n(integerVal);
          _graph->addNode(n);
        }
      }
    }
  }

  void parseString(const std::string &value) override {
    if (_parsingProperties) {
      if (_parsingPropertyType && !_propertyName.empty()) {
        _parsingPropertyType = false;

        if (_progress) {
          _progress->setComment("parsing property: '" + _propertyName + "'");
        }

        _currentProperty = _graph->getLocalProperty(_propertyName, value);
        _parsingPathViewProperty = (_propertyName == std::string("viewFont") ||
                                    _propertyName == std::string("viewTexture"));

        if (value == "graph") {
          _graphProperties[_graph] = TemporaryGraphProperty();
          _graphProperties[_graph][_propertyName] = TemporaryGraphValue();
        }

        if (!_currentProperty) {
          tlp::error() << "The property '" << _propertyName << "' of type: '" << value
                       << "' could not be created" << endl;
        }
      }

      if (_currentProperty) {
        if (_parsingPropertyDefaultNodeValue) {
          if (_parsingPathViewProperty) {
            // if needed replace symbolic path by real path
            size_t pos = value.find("TulipBitmapDir/");

            if (pos != std::string::npos) {
              string dValue(value);
              dValue.replace(pos, 15, TulipBitmapDir);
              _currentProperty->setAllNodeStringValue(dValue);
            } else
              _currentProperty->setAllNodeStringValue(value);
          } else
            _currentProperty->setAllNodeStringValue(value);

          _parsingPropertyDefaultNodeValue = false;
        }

        if (_parsingPropertyDefaultEdgeValue) {
          if (_parsingPathViewProperty) {
            // if needed replace symbolic path by real path
            size_t pos = value.find("TulipBitmapDir/");

            if (pos != std::string::npos) {
              string dValue(value);
              dValue.replace(pos, 15, TulipBitmapDir);
              _currentProperty->setAllEdgeStringValue(dValue);
            } else
              _currentProperty->setAllEdgeStringValue(value);
          } else
            _currentProperty->setAllEdgeStringValue(value);

          _parsingPropertyDefaultEdgeValue = false;
        }

        if (_parsingPropertyNodeValues) {
          // if parsing a graphproperty, add it in the temporary buffer so the values are correctly
          // set afterwards
          if (_graphProperties[_graph].find(_currentProperty->getName()) !=
              _graphProperties[_graph].end()) {
            _graphProperties[_graph][_currentProperty->getName()].insert(
                std::pair<unsigned, unsigned>(_currentIdentifier, atoi(value.c_str())));
          }

          assert(_currentIdentifier != UINT_MAX);
          assert(_currentProperty);
          node n(_currentIdentifier);

          if (_parsingPathViewProperty) {
            // if needed replace symbolic path by real path
            size_t pos = value.find("TulipBitmapDir/");

            if (pos != std::string::npos) {
              string nValue(value);
              nValue.replace(pos, 15, TulipBitmapDir);
              _currentProperty->setNodeStringValue(n, nValue);
            } else
              _currentProperty->setNodeStringValue(n, value);
          } else
            _currentProperty->setNodeStringValue(n, value);
        }

        if (_parsingPropertyEdgeValues) {
          assert(_currentIdentifier != UINT_MAX);
          assert(_currentProperty);
          edge e(_currentIdentifier);

          if (_parsingPathViewProperty) {
            // if needed replace symbolic path by real path
            string eValue(value);
            size_t pos = eValue.find("TulipBitmapDir/");
            if (pos != std::string::npos) {
              eValue.replace(pos, 15, TulipBitmapDir);
            }
            _currentProperty->setEdgeStringValue(e, eValue);
          } else {
            // setEdgeStringValue is a no-op with GraphProperty
            if (_currentProperty->getTypename() == GraphProperty::propertyTypename) {
              set<edge> edges;
              EdgeSetType::fromString(edges, value);
              static_cast<GraphProperty*>(_currentProperty)->setEdgeValue(e, edges);
            } else {
              _currentProperty->setEdgeStringValue(e, value);
            }
          }
        }
      } else {
        tlp::error() << "The property '" << _propertyName << "'was null when trying to fill it"
                     << std::endl;
      }
    }

    if (_parsingAttributes) {

      if (_currentAttributeTypeName.empty()) {
        _currentAttributeTypeName = value;
      } else {
        stringstream data(value);
        bool result = _dataSet->readData(data, _currentAttributeName, _currentAttributeTypeName);

        if (!result) {
          tlp::error() << "error reading attribute: " << _currentAttributeName << " of type '"
                       << _currentAttributeTypeName << "' and value: " << data.str() << std::endl;
        }

        _currentAttributeTypeName = string();
      }
    }
  }

private:
  std::stack<unsigned int> _parsingSubgraph;
  bool _parsingEdges;
  bool _parsingNodes;
  /**
   * @brief indicates whether the parser is currently parsing an array representing an edge
   **/
  bool _newEdge;
  unsigned int _edgeSource;

  bool _parsingNodesIds;
  bool _parsingEdgesIds;
  bool _parsingEdgesNumber;
  bool _parsingInterval;
  bool _newInterval;
  unsigned int _intervalSource;

  tlp::Graph *_graph;

  tlp::DataSet *_dataSet;

  bool _parsingAttributes;
  std::string _currentAttributeName;
  std::string _currentAttributeTypeName;

  bool _parsingProperties;
  PropertyInterface *_currentProperty;
  std::string _propertyName;
  unsigned int _currentIdentifier;
  bool _parsingPropertyType;
  bool _parsingPropertyNodeValues;
  bool _parsingPropertyEdgeValues;
  bool _parsingPropertyDefaultEdgeValue;
  bool _parsingPropertyDefaultNodeValue;
  bool _parsingPathViewProperty;

  bool _waitingForGraphId;

  // ugly workaround for GraphProperties as they do not support set*StringValue
  std::map<tlp::Graph *, TemporaryGraphProperty> _graphProperties;
  std::map<int, tlp::Graph *> _clusterIndex;
};

/**
 * @brief A simple proxy class for the YajlParseFacade.
 **/
class YajlProxy : public YajlParseFacade {
public:
  YajlProxy(tlp::PluginProgress *progress = nullptr) : YajlParseFacade(progress), _proxy(nullptr) {}
  ~YajlProxy() override {
    delete _proxy;
  }
  void parseBoolean(bool boolVal) override {
    _proxy->parseBoolean(boolVal);
  }
  void parseDouble(double doubleVal) override {
    _proxy->parseDouble(doubleVal);
  }
  void parseEndArray() override {
    _proxy->parseEndArray();
  }
  void parseEndMap() override {
    _proxy->parseEndMap();
  }
  void parseInteger(long long integerVal) override {
    _proxy->parseInteger(integerVal);
  }
  void parseMapKey(const std::string &value) override {
    _proxy->parseMapKey(value);
  }
  void parseNull() override {
    _proxy->parseNull();
  }
  void parseNumber(const char *numberVal, size_t numberLen) override {
    _proxy->parseNumber(numberVal, numberLen);
  }
  void parseStartArray() override {
    _proxy->parseStartArray();
  }
  void parseStartMap() override {
    _proxy->parseStartMap();
  }
  void parseString(const std::string &value) override {
    _proxy->parseString(value);
  }

protected:
  YajlParseFacade *_proxy;
};

class TlpJsonImport : public ImportModule, YajlProxy {
public:
  PLUGININFORMATION("JSON Import", "Charles Huet", "18/05/2011",
                    "<p>Supported extensions: json</p><p>Imports a graph recorded in a file using "
                    "the Tulip JSON format.</p>",
                    "1.0", "File")

  std::list<std::string> fileExtensions() const override {
    std::list<std::string> l;
    l.push_back("json");
    return l;
  }

  TlpJsonImport(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<std::string>("file::filename", "The pathname of the TLP JSON file to import.",
                                "");
  }

  std::string icon() const override {
    return ":/tulip/gui/icons/json32x32.png";
  }

  bool importGraph() override {
    Observable::holdObservers();
    std::string filename;

    if (_progress) {
      _progress->progress(0, 0);
    }

    if (dataSet->exists("file::filename")) {
      dataSet->get<string>("file::filename", filename);

      _proxy = new YajlParseFacade(_progress);
      parse(filename);
    }

    Observable::unholdObservers();

    if (!_proxy->parsingSucceeded()) {
      _parsingSucceeded = false;
      _errorMessage = _proxy->errorMessage();
    }

    pluginProgress->setError(_errorMessage);
    return _parsingSucceeded;
  }

  void parseMapKey(const std::string &value) override {
    if (value == GraphToken) {
      delete _proxy;
      _proxy = new TlpJsonGraphParser(graph, _progress);
    }

    YajlProxy::parseMapKey(value);
  }
};

PLUGIN(TlpJsonImport)
