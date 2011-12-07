#include <sstream>
#include <stack>

#include <tulip/Graph.h>
#include <tulip/ImportModule.h>
#include <tulip/PropertyInterface.h>
#include <tulip/ForEach.h>
#include <tulip/MethodFactory.h>
#include <tulip/JsonTokens.h>

#include <tulip/YajlFacade.h>

using namespace std;
using namespace tlp;

class TlpJsonGraphParser : public YajlParseFacade {
public:
  TlpJsonGraphParser(tlp::Graph* parentGraph) :
    _parsingEdges(false),
    _parsingNodes(false),
    _newEdge(false),
    _edgeSource(UINT_MAX),
    _parsingNodesIds(false),
    _parsingEdgesIds(false),
    _parsingInterval(false),
    _newInterval(false),
    _intervalSource(UINT_MAX),
    _graph(parentGraph),
    _dataSet(&const_cast<DataSet&>(parentGraph->getAttributes())),
    _parsingAttributes(false),
    _currentAttributeName(std::string()),
    _currentAttributeTypeName(std::string()),
    _parsingProperties(false),
    _currentProperty(NULL),
    _propertyName(std::string()),
    _currentIdentifier(UINT_MAX),
    _parsingPropertyType(false),
    _parsingPropertyNodeValues(false),
    _parsingPropertyEdgeValues(false),
    _parsingPropertyDefaultEdgeValue(false),
    _parsingPropertyDefaultNodeValue(false),
    _waitingForGraphId(false) {
  }

  virtual void parseStartArray() {
    if(_parsingEdges)  {
      _newEdge = true;
    }

    if(_parsingInterval) {
      _newInterval = true;
    }

    if(_parsingNodesIds || _parsingEdgesIds) {
      _parsingInterval = true;
    }
  }

  virtual void parseEndArray() {
    //if the current array was not an edge but was the array of edges, we are done parsing edges
    if(!_newEdge && _parsingEdges)  {
      _parsingEdges = false;
    }

    if(_newEdge) {
      _newEdge = false;
    }

    if((_parsingNodesIds || _parsingEdgesIds) && !_newInterval) {
      _parsingNodesIds = false;
      _parsingEdgesIds = false;
    }

    if(!_newInterval) {
      _parsingInterval = false;
    }

    if(_newInterval) {
      _newInterval = false;
    }
  }

  virtual void parseMapKey(const std::string& value) {
    if(_parsingProperties && !_parsingPropertyNodeValues && !_parsingPropertyEdgeValues && !_parsingPropertyDefaultEdgeValue && !_parsingPropertyDefaultNodeValue && _propertyName.empty()) {
      _propertyName = value;
//         std::cout << "now parsing property: " << propertyName << std::endl;
    }

    if(value == GraphIDToken) {
      _waitingForGraphId = true;
    }
    else if(value == NodesIDsToken) {
      _parsingNodesIds = true;
    }
    else if(value == EdgesIDsToken) {
      _parsingEdgesIds = true;
    }
    else if(!_currentProperty && value == EdgesToken)  {
      _parsingEdges = true;
    }
    else if(value == "attributes") {
      _parsingAttributes = true;
    }
    else if(_currentProperty && value == NodesValuesToken) {
      _parsingPropertyNodeValues = true;
    }
    else if(_currentProperty && value == EdgesValuesToken) {
      _parsingPropertyEdgeValues = true;
    }
    else if(value == EdgeDefaultToken) {
      _parsingPropertyDefaultEdgeValue = true;
    }
    else if (value == NodeDefaultToken) {
      _parsingPropertyDefaultNodeValue = true;
    }
    else if (value == PropertiesToken) {
      _parsingProperties = true;
    }
    else if(value == TypeToken) {
      _parsingPropertyType = true;
    }
    else if(value == NodesNumberToken) {
      _parsingNodes = true;
    }
    else if(_parsingPropertyNodeValues || _parsingPropertyEdgeValues) {
      _currentIdentifier = atoi(value.c_str());
    }
    else if(_parsingAttributes) {
      _currentAttributeName = value;
    }
    else if(value == SubgraphsToken) {
      _parsingSubgraph.push(0);
    }
  }

  virtual void parseStartMap() {
    if(!_parsingSubgraph.empty()) {
      ++_parsingSubgraph.top();
    }
  }

  virtual void parseEndMap() {
    if(!_currentProperty && _propertyName.empty()) {
      _parsingProperties = false;
    }

    if(!_parsingPropertyNodeValues && !_parsingPropertyEdgeValues && !_propertyName.empty()) {
//       cout << "finished parsing property '" << _propertyName << "'" << endl;
      _currentProperty = NULL;
      _propertyName = string();
    }

    if(_parsingPropertyNodeValues) {
      _parsingPropertyNodeValues = false;
    }

    if(_parsingPropertyEdgeValues) {
      _parsingPropertyEdgeValues = false;
    }

    if(_parsingAttributes) {
      _parsingAttributes = false;
    }

    if(_parsingEdgesIds) {
      _parsingEdgesIds = false;
    }

    if(_parsingNodesIds) {
      _parsingNodesIds = false;
    }

    if(_parsingEdges) {
      _parsingEdges = false;
    }

    if(!_parsingSubgraph.empty()) {
      --_parsingSubgraph.top();

      if(_parsingSubgraph.top() == 0) {
        _parsingSubgraph.pop();
        _graph = _graph->getSuperGraph();
      }
    }
  }

  virtual void parseInteger(long long integerVal) {
    if(_waitingForGraphId) {
      if(integerVal > 0) {
        _graph = _graph->addSubGraph(0, integerVal);
        _dataSet = &const_cast<DataSet&>(_graph->getAttributes());
      }

      _waitingForGraphId = false;
      return; //just ignore whatever is next, we've done what we came for.
    }

    if(_parsingNodes)  {
      for(int i = 0; i < integerVal; ++i) {
        _graph->addNode();
      }

      _parsingNodes = false;
      return; //just ignore whatever is next, we've done what we came for.
    }

    //if the int is the source or target of an edge
    if(_newEdge) {
      //if the source has not been set, this int is the source
      if(_edgeSource == UINT_MAX) {
        _edgeSource = integerVal;
      }
      //if the source has been set, this int is the target
      else {
        node source(_edgeSource);
        node target(integerVal);
        _graph->addEdge(source, target);
        _edgeSource = UINT_MAX;
      }

      return; //just ignore whatever is next, we've done what we came for.
    }

    if(_parsingInterval) {
      if(_newInterval) {
        if(_intervalSource == UINT_MAX) {
          _intervalSource = integerVal;
        }
        else {
          for(unsigned int id = _intervalSource; id <= integerVal; ++id) {
            if(_parsingEdgesIds) {
              edge e(id);
              _graph->addEdge(e);
            }

            if(_parsingNodesIds) {
              node n(id);
              _graph->addNode(n);
            }
          }

          _intervalSource = UINT_MAX;
        }
      }
      else {
        if(_parsingEdgesIds) {
          edge e(integerVal);
          _graph->addEdge(e);
        }

        if(_parsingNodesIds) {
          node n(integerVal);
          _graph->addNode(n);
        }
      }
    }
  }

  virtual void parseString(const std::string& value) {
    if(_parsingProperties) {
      if(_parsingPropertyType && !_propertyName.empty()) {
        _parsingPropertyType = false;
        //       std::cout << "getting new property: " << propertyName << "(" << value << ")" << std::sendl;
        //       std::cout << "of type: " << value << std::endl;
        _currentProperty = _graph->getProperty(_propertyName, value);

        if(!_currentProperty) {
          cout << "[error] The property '" << _propertyName << "' of type: '" << value << "' could not be created" << endl;
        }

      }

      if(_currentProperty) {
        if(_parsingPropertyDefaultNodeValue) {
          _currentProperty->setAllNodeStringValue(value);
          _parsingPropertyDefaultNodeValue = false;
          //       std::cout << "of default node value: " << value << std::endl;
        }

        if(_parsingPropertyDefaultEdgeValue) {
          _currentProperty->setAllEdgeStringValue(value);
          _parsingPropertyDefaultEdgeValue = false;
          //       std::cout << "of default edge value: " << value << std::endl;
        }

        if(_parsingPropertyNodeValues) {
          assert(_currentIdentifier != UINT_MAX);
          assert(_currentProperty);
          node n(_currentIdentifier);
          _currentProperty->setNodeStringValue(n, value);
          //       cout << "setting node value: " << n.id << " " << value << "; "<< currentProperty->getNodeStringValue(n) << endl;;
        }

        if(_parsingPropertyEdgeValues) {
          assert(_currentIdentifier != UINT_MAX);
          assert(_currentProperty);
          edge e(_currentIdentifier);
          _currentProperty->setEdgeStringValue(e, value);
        }
      }
      else {
        std::cout << "[error] The property '" << _propertyName << "'was null when trying to fill it" << std::endl;
      }
    }

    if(_parsingAttributes) {
      if(_currentAttributeTypeName.empty()) {
        _currentAttributeTypeName = value;
      }
      else {
        stringstream data;

        if(_currentAttributeTypeName == "string") {
          data << "\"" << value << "\"";
        }
        else {
          data << value;
        }

        if(_dataSet->exist(_currentAttributeName)) {
          std::cout << "[error] attribute '" << _currentAttributeName << "' already exists" << std::endl;
        }
        else {
          bool result = _dataSet->readData(data, _currentAttributeName, _currentAttributeTypeName);
          _currentAttributeTypeName = string();

          if(!result) {
            std::cout << "[error] while reading attribute: " << _currentAttributeName << " of type '" << _currentAttributeTypeName << "' and value: " << data.str() << std::endl;
          }
        }
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
  bool _parsingInterval;
  bool _newInterval;
  unsigned int _intervalSource;

  tlp::Graph* _graph;

  tlp::DataSet* _dataSet;

  bool _parsingAttributes;
  std::string _currentAttributeName;
  std::string _currentAttributeTypeName;

  bool _parsingProperties;
  PropertyInterface* _currentProperty;
  std::string _propertyName;
  unsigned int _currentIdentifier;
  bool _parsingPropertyType;
  bool _parsingPropertyNodeValues;
  bool _parsingPropertyEdgeValues;
  bool _parsingPropertyDefaultEdgeValue;
  bool _parsingPropertyDefaultNodeValue;

  bool _waitingForGraphId;
};

/**
 * @brief A simple proxy class for the YajlParseFacade.
 **/
class YajlProxy : public YajlParseFacade {
public:
  virtual void parseBoolean(bool boolVal) {
    _proxy->parseBoolean(boolVal);
  }
  virtual void parseDouble(double doubleVal) {
    _proxy->parseDouble(doubleVal);
  }
  virtual void parseEndArray() {
    _proxy->parseEndArray();
  }
  virtual void parseEndMap() {
    _proxy->parseEndMap();
  }
  virtual void parseInteger(long long integerVal) {
    _proxy->parseInteger(integerVal);
  }
  virtual void parseMapKey(const std::string& value) {
    _proxy->parseMapKey(value);
  }
  virtual void parseNull() {
    _proxy->parseNull();
  }
  virtual void parseNumber(const char* numberVal, size_t numberLen) {
    _proxy->parseNumber(numberVal, numberLen);
  }
  virtual void parseStartArray() {
    _proxy->parseStartArray();
  }
  virtual void parseStartMap() {
    _proxy->parseStartMap();
  }
  virtual void parseString(const std::string& value) {
    _proxy->parseString(value);
  }
protected:
  YajlParseFacade* _proxy;
};

class TlpJsonImport : public ImportModule, YajlProxy {
public:
  TlpJsonImport(AlgorithmContext context) :ImportModule(context) {
    addParameter<std::string>("file::filename", "");
  }

  virtual bool import() {
    std::string filename;
    std::string data;

    if(dataSet->exist("file::filename")) {
      dataSet->get<string>("file::filename", filename);

      _proxy = new YajlParseFacade();
//       QTime t = QTime::currentTime();
      parse(filename);
//       cout << "msecs: " << t.msecsTo(QTime::currentTime()) << endl;
    }

    pluginProgress->setError(_proxy->errorMessage());
    return _proxy->parsingSucceeded();
  }

  virtual void parseMapKey(const std::string& value) {
    if(value == GraphToken) {
      delete _proxy;
      _proxy = new TlpJsonGraphParser(graph);
    }

    YajlProxy::parseMapKey(value);
  }
};

IMPORTPLUGINOFGROUP(TlpJsonImport ,"TlpJsonImport", "Charles Huet", "18/05/2011", "Tulip JSon format", "1.0", "File")
