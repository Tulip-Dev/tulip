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

/**
 *
 * tulip.js : port of the Tulip framework to JavaScript through emscripten
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/Graph.h>
#include <tulip/GraphAbstract.h>
#include <tulip/ForEach.h>
#include <tulip/ColorProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/YajlFacade.h>
#include <tulip/DataSet.h>
#include <tulip/PluginLister.h>
#include <tulip/StringCollection.h>
#include <tulip/SimplePluginProgress.h>
#include <tulip/ImportModule.h>
#include <tulip/PluginContext.h>
#include <tulip/ColorScale.h>
#include <tulip/TlpTools.h>
#include <tulip/ConnectedTest.h>

#include <emscripten/emscripten.h>
#include <emscripten/val.h>

#include <vector>

using namespace emscripten;

// ==================================================================================================================

// static std::string replaceString(std::string subject, const std::string& search,
//                                 const std::string& replace) {
//  size_t pos = 0;
//  while ((pos = subject.find(search, pos)) != std::string::npos) {
//    subject.replace(pos, search.length(), replace);
//    pos += replace.length();
//  }
//  return subject;
//}

static void replaceStringInPlace(std::string &subject, const std::string &search, const std::string &replace) {
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

// ==================================================================================================================

template <class StringContainer> static unsigned int getStringsNumberOfBytesFromContainer(const StringContainer &container, unsigned int *array) {
  typename StringContainer::const_iterator it = container.begin();
  unsigned int ret = 0;
  for (; it != container.end(); ++it) {
    *array++ = (it->length() + 1);
    ret += (it->length() + 1);
  }
  return ret;
}

template <class StringContainer> static void fillBytesArrayFromStringsContainer(const StringContainer &container, unsigned char *array) {
  typename StringContainer::const_iterator it = container.begin();
  unsigned char *arrayPtr = array;
  for (; it != container.end(); ++it) {
    memcpy(arrayPtr, it->c_str(), (it->length() + 1));
    arrayPtr += (it->length() + 1);
  }
}

static unsigned int getStringsNumberOfBytesFromStringIterator(tlp::Iterator<std::string> *itS, unsigned int *array) {
  unsigned int ret = 0;
  std::string s;
  forEach(s, itS) {
    *array++ = (s.length() + 1);
    ret += (s.length() + 1);
  }
  return ret;
}

static void fillBytesArrayFromStringIterator(tlp::Iterator<std::string> *itS, unsigned char *array) {
  unsigned char *arrayPtr = array;
  std::string s;
  forEach(s, itS) {
    memcpy(arrayPtr, s.c_str(), (s.length() + 1));
    arrayPtr += (s.length() + 1);
  }
}

static void fillStringVectorFromStringsBytes(std::vector<std::string> &vs, unsigned char *stringsBytes, unsigned int *stringsNbBytes,
                                             unsigned int nbStrings) {
  vs.resize(nbStrings);
  char *start = reinterpret_cast<char *>(stringsBytes);
  for (unsigned int i = 0; i < nbStrings; ++i) {
    vs[i] = std::string(start, stringsNbBytes[i] - 1);
    start += stringsNbBytes[i];
  }
}

// ==================================================================================================================

extern "C" {
bool workerMode();
}

static val tulipJS = val::undefined();

static val tulip() {
  if (tulipJS.isUndefined()) {
    tulipJS = val::global("tulip");
  }
  return tulipJS;
}

static val wrapPropertyToJs(tlp::PropertyInterface *prop) {
  tlp::BooleanProperty *bp = dynamic_cast<tlp::BooleanProperty *>(prop);
  tlp::ColorProperty *cp = dynamic_cast<tlp::ColorProperty *>(prop);
  tlp::DoubleProperty *dp = dynamic_cast<tlp::DoubleProperty *>(prop);
  tlp::IntegerProperty *ip = dynamic_cast<tlp::IntegerProperty *>(prop);
  tlp::LayoutProperty *lp = dynamic_cast<tlp::LayoutProperty *>(prop);
  tlp::SizeProperty *sp = dynamic_cast<tlp::SizeProperty *>(prop);
  tlp::StringProperty *strp = dynamic_cast<tlp::StringProperty *>(prop);

  val jsProp = val::null();

  if (bp) {
    jsProp = tulip()["BooleanProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else if (cp) {
    jsProp = tulip()["ColorProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else if (dp) {
    jsProp = tulip()["DoubleProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else if (ip) {
    jsProp = tulip()["IntegerProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else if (lp) {
    jsProp = tulip()["LayoutProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else if (sp) {
    jsProp = tulip()["SizeProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else if (strp) {
    jsProp = tulip()["StringProperty"].new_(reinterpret_cast<unsigned long>(prop));
  } else {
    jsProp = tulip()["PropertyInterface"].new_(reinterpret_cast<unsigned long>(prop));
  }

  return jsProp;
}

static std::set<unsigned long> deletedPointers;

class TulipObjectsObserver : public tlp::Observable {

public:
  void treatEvent(const tlp::Event &event) {

    if (!tulip().call<bool>("hasListener", reinterpret_cast<unsigned long>(event.sender()))) {
      return;
    }

    const tlp::GraphEvent *ge = dynamic_cast<const tlp::GraphEvent *>(&event);
    const tlp::PropertyEvent *pe = dynamic_cast<const tlp::PropertyEvent *>(&event);
    if (ge) {
      val jsGraph = tulip()["Graph"].new_(reinterpret_cast<unsigned long>(ge->getGraph()));
      val graphEvent = tulip()["GraphEvent"].new_(jsGraph, static_cast<unsigned long>(ge->getType()));
      if (ge->getType() == tlp::GraphEvent::TLP_ADD_NODE || ge->getType() == tlp::GraphEvent::TLP_DEL_NODE) {
        graphEvent.set("node", tulip()["Node"].new_(ge->getNode().id));
      } else if (ge->getType() == tlp::GraphEvent::TLP_ADD_EDGE || ge->getType() == tlp::GraphEvent::TLP_DEL_EDGE ||
                 ge->getType() == tlp::GraphEvent::TLP_REVERSE_EDGE || ge->getType() == tlp::GraphEvent::TLP_BEFORE_SET_ENDS ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_SET_ENDS) {
        graphEvent.set("edge", tulip()["Edge"].new_(ge->getEdge().id));
      } else if (ge->getType() == tlp::GraphEvent::TLP_ADD_NODES) {
        val nodesArray = val::array();
        const std::vector<tlp::node> &nodes = ge->getNodes();
        for (size_t i = 0; i < nodes.size(); ++i) {
          nodesArray.call<void>("push", tulip()["Node"].new_(nodes[i].id));
        }
        graphEvent.set("nodes", nodesArray);
      } else if (ge->getType() == tlp::GraphEvent::TLP_ADD_EDGES) {
        val edgesArray = val::array();
        const std::vector<tlp::edge> &edges = ge->getEdges();
        for (size_t i = 0; i < edges.size(); ++i) {
          edgesArray.call<void>("push", tulip()["Edge"].new_(edges[i].id));
        }
        graphEvent.set("edges", edgesArray);
      } else if (ge->getType() == tlp::GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH ||
                 ge->getType() == tlp::GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH || ge->getType() == tlp::GraphEvent::TLP_BEFORE_ADD_SUBGRAPH ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_ADD_SUBGRAPH || ge->getType() == tlp::GraphEvent::TLP_BEFORE_DEL_SUBGRAPH ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_DEL_SUBGRAPH) {
        graphEvent.set("subgraph", tulip()["Graph"].new_(reinterpret_cast<unsigned long>(ge->getSubGraph())));
      } else if (ge->getType() == tlp::GraphEvent::TLP_ADD_LOCAL_PROPERTY || ge->getType() == tlp::GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY ||
                 ge->getType() == tlp::GraphEvent::TLP_ADD_INHERITED_PROPERTY ||
                 ge->getType() == tlp::GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY) {
        graphEvent.set("name", ge->getPropertyName());
      } else if (ge->getType() == tlp::GraphEvent::TLP_BEFORE_ADD_LOCAL_PROPERTY ||
                 ge->getType() == tlp::GraphEvent::TLP_BEFORE_ADD_INHERITED_PROPERTY ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY ||
                 ge->getType() == tlp::GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
        graphEvent.set("name", ge->getPropertyName());
      } else if (ge->getType() == tlp::GraphEvent::TLP_BEFORE_SET_ATTRIBUTE || ge->getType() == tlp::GraphEvent::TLP_AFTER_SET_ATTRIBUTE ||
                 ge->getType() == tlp::GraphEvent::TLP_REMOVE_ATTRIBUTE) {
        graphEvent.set("name", ge->getAttributeName());
      }

      tulip().call<void>("sendEventToListeners", reinterpret_cast<unsigned long>(event.sender()), graphEvent);
    }

    if (pe) {
      val jsProp = wrapPropertyToJs(pe->getProperty());

      val propEvent = tulip()["PropertyEvent"].new_(jsProp, static_cast<unsigned long>(pe->getType()));
      if (pe->getType() == tlp::PropertyEvent::TLP_BEFORE_SET_NODE_VALUE || pe->getType() == tlp::PropertyEvent::TLP_AFTER_SET_NODE_VALUE) {
        propEvent.set("node", tulip()["Node"].new_(pe->getNode().id));
      } else if (pe->getType() == tlp::PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE || pe->getType() == tlp::PropertyEvent::TLP_AFTER_SET_EDGE_VALUE) {
        propEvent.set("edge", tulip()["Edge"].new_(pe->getEdge().id));
      }

      tulip().call<void>("sendEventToListeners", reinterpret_cast<unsigned long>(event.sender()), propEvent);
    }
  }

  void treatEvents(const std::vector<tlp::Event> &events) {
    val jsEvents = val::array();
    unsigned int nbEvents = 0;
    for (size_t i = 0; i < events.size(); ++i) {
      if (events[i].type() == tlp::Event::TLP_DELETE) {
        deletedPointers.insert(reinterpret_cast<unsigned long>(events[i].sender()));
      }
      if (!tulip().call<bool>("hasObserver", reinterpret_cast<unsigned long>(events[i].sender()))) {
        continue;
      }
      val jsEvent = val::null();
      tlp::Graph *graph = dynamic_cast<tlp::Graph *>(events[i].sender());
      tlp::PropertyInterface *prop = dynamic_cast<tlp::PropertyInterface *>(events[i].sender());
      if (graph) {
        val jsgraph = tulip()["Graph"].new_(reinterpret_cast<unsigned long>(graph));
        jsEvent = tulip()["Event"].new_(jsgraph, static_cast<unsigned long>(events[i].type()));
      } else if (prop) {
        val jsprop = wrapPropertyToJs(prop);
        jsEvent = tulip()["Event"].new_(jsprop, static_cast<unsigned long>(events[i].type()));
      } else {
        jsEvent = tulip()["Event"].new_(reinterpret_cast<unsigned long>(events[i].sender()), static_cast<unsigned long>(events[i].type()));
      }
      jsEvents.call<void>("push", jsEvent);
      ++nbEvents;
    }

    if (nbEvents > 0) {
      tulip().call<void>("sendEventsToObservers", jsEvents);
    }
  }
};

static TulipObjectsObserver tlpObjObs;

void observeObject(tlp::Observable *observable) {
  if (!observable || workerMode())
    return;
  // observable->addListener(&tlpObjObs);
  observable->addObserver(&tlpObjObs);
  // emscripten often reuses previous freed address so ensure to remove it from the deleted pointers set
  unsigned long pointerValue = reinterpret_cast<unsigned long>(observable);
  deletedPointers.erase(pointerValue);
}

void observeGraphHierarchy(tlp::Graph *root) {
  observeObject(root);
  for (tlp::Graph *sg : root->getSubGraphs()) {
    observeGraphHierarchy(sg);
  }
}

void observeGraph(tlp::Graph *g) {
  if (!g || workerMode()) {
    return;
  }
  g->addListener(&tlpObjObs);
  g->addObserver(&tlpObjObs);
  std::string propName;
  forEach(propName, g->getProperties()) {
    g->getProperty(propName)->addListener(&tlpObjObs);
    g->getProperty(propName)->addObserver(&tlpObjObs);
  }
}

void unobserveGraph(tlp::Graph *g) {
  if (!g || workerMode()) {
    return;
  }
  g->removeListener(&tlpObjObs);
  g->removeObserver(&tlpObjObs);
  std::string propName;
  forEach(propName, g->getProperties()) {
    g->getProperty(propName)->removeListener(&tlpObjObs);
    g->getProperty(propName)->removeObserver(&tlpObjObs);
  }
}

template <typename T> std::list<std::string> getTulipPluginsList() {
  if (typeid(T).name() == typeid(tlp::Algorithm).name()) {
    std::list<std::string> algoList = tlp::PluginLister::instance()->availablePlugins<tlp::Algorithm>();
    std::list<std::string> propAlgoList = tlp::PluginLister::instance()->availablePlugins<tlp::PropertyAlgorithm>();
    std::list<std::string> realAlgoList;
    for (std::list<std::string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
      if (std::find(propAlgoList.begin(), propAlgoList.end(), *it) == propAlgoList.end()) {
        realAlgoList.push_back(*it);
      }
    }
    return realAlgoList;
  } else {
    return tlp::PluginLister::instance()->availablePlugins<T>();
  }
}

template <typename T> bool pluginExists(const std::string &pluginName) {
  std::list<std::string> pluginsList = getTulipPluginsList<T>();
  return std::find(pluginsList.begin(), pluginsList.end(), pluginName) != pluginsList.end();
}

tlp::DataSet getDefaultAlgorithmParameters(const std::string &algoName, tlp::Graph *graph) {
  tlp::DataSet result;
  const tlp::ParameterDescriptionList &parameters = tlp::PluginLister::getPluginParameters(algoName);
  parameters.buildDefaultDataSet(result, graph);
  return result;
}

class AlgorithParametersJSONParser : public YajlParseFacade {

public:
  AlgorithParametersJSONParser(const std::string &algorithmName, tlp::Graph *graph = NULL) : _algoName(algorithmName) {
    _dataSet = getDefaultAlgorithmParameters(algorithmName, graph);
    _pointer = 0;
    _parsingObject = false;
  }

  tlp::DataSet *getParametersDataSet() {
    return &_dataSet;
  }

  void parseMapKey(const std::string &value) {
    if (!_parsingObject) {
      if (_dataSet.exist(value)) {
        _curParamName = value;
      } else {
        _curParamName = "";
      }
    } else {
      _curObjectKey = value;
    }
  }

  void parseStartMap() {
    if (_curParamName != "") {
      _parsingObject = true;
    }
  }

  void parseEndMap() {

    if (_parsingObject && _typename != "") {

      tlp::DataType *dt = _dataSet.getData(_curParamName);
      if ((_typename == "tlp::PropertyInterface" || _typename == "tlp::BooleanProperty" || _typename == "tlp::ColorProperty" ||
           _typename == "tlp::DoubleProperty" || _typename == "tlp::IntegerProperty" || _typename == "tlp::LayoutProperty" ||
           _typename == "tlp::SizeProperty" || _typename == "tlp::StringProperty") &&
          dt->getTypeName() == std::string(typeid(tlp::PropertyInterface *).name())) {
        _dataSet.set(_curParamName, reinterpret_cast<tlp::PropertyInterface *>(_pointer));
      }

      if ((_typename == "tlp::DoubleProperty" || _typename == "tlp::IntegerProperty") &&
          dt->getTypeName() == std::string(typeid(tlp::NumericProperty *).name())) {
        _dataSet.set(_curParamName, reinterpret_cast<tlp::NumericProperty *>(_pointer));
      }

      if (_typename == "tlp::ColorScale" && dt->getTypeName() == std::string(typeid(tlp::ColorScale).name())) {
        _dataSet.set(_curParamName, *reinterpret_cast<tlp::ColorScale *>(_pointer));
      }

      _parsingObject = false;
      _curObjectKey = "";
      _typename = "";
      _pointer = 0;
    }
  }

  void parseBoolean(bool boolVal) {
    if (_curParamName.empty())
      return;
    tlp::DataType *dt = _dataSet.getData(_curParamName);
    if (dt->getTypeName() == std::string(typeid(bool).name())) {
      _dataSet.set(_curParamName, boolVal);
    }
  }

  void parseInteger(long long integerVal) {
    if (_curParamName.empty())
      return;
    if (_parsingObject && _curObjectKey == "cppPointer") {
      _pointer = static_cast<unsigned long>(integerVal);
      return;
    }
    tlp::DataType *dt = _dataSet.getData(_curParamName);
    if (dt->getTypeName() == std::string(typeid(int).name())) {
      _dataSet.set(_curParamName, static_cast<int>(integerVal));
    } else if (dt->getTypeName() == std::string(typeid(unsigned int).name())) {
      _dataSet.set(_curParamName, static_cast<unsigned int>(integerVal));
    } else if (dt->getTypeName() == std::string(typeid(long).name())) {
      _dataSet.set(_curParamName, static_cast<long>(integerVal));
    } else if (dt->getTypeName() == std::string(typeid(unsigned long).name())) {
      _dataSet.set(_curParamName, static_cast<unsigned long>(integerVal));
    } else if (dt->getTypeName() == std::string(typeid(double).name())) {
      _dataSet.set(_curParamName, static_cast<double>(integerVal));
    } else if (dt->getTypeName() == std::string(typeid(float).name())) {
      _dataSet.set(_curParamName, static_cast<float>(integerVal));
    }
  }

  void parseDouble(double doubleVal) {
    if (_curParamName.empty())
      return;
    tlp::DataType *dt = _dataSet.getData(_curParamName);
    if (dt->getTypeName() == std::string(typeid(float).name())) {
      _dataSet.set(_curParamName, static_cast<float>(doubleVal));
    } else if (dt->getTypeName() == std::string(typeid(double).name())) {
      _dataSet.set(_curParamName, doubleVal);
    } else if (dt->getTypeName() == std::string(typeid(int).name())) {
      _dataSet.set(_curParamName, static_cast<int>(doubleVal));
    } else if (dt->getTypeName() == std::string(typeid(unsigned int).name())) {
      _dataSet.set(_curParamName, static_cast<unsigned int>(doubleVal));
    } else if (dt->getTypeName() == std::string(typeid(long).name())) {
      _dataSet.set(_curParamName, static_cast<long>(doubleVal));
    } else if (dt->getTypeName() == std::string(typeid(unsigned long).name())) {
      _dataSet.set(_curParamName, static_cast<unsigned long>(doubleVal));
    }
  }

  void parseString(const std::string &value) {
    if (_curParamName.empty())
      return;
    if (_parsingObject && _curObjectKey == "wrappedTypename") {
      _typename = value;
      return;
    }
    tlp::DataType *dt = _dataSet.getData(_curParamName);
    if (dt->getTypeName() == std::string(typeid(std::string).name())) {
      _dataSet.set(_curParamName, value);
    } else if (dt->getTypeName() == std::string(typeid(tlp::StringCollection).name())) {
      tlp::StringCollection sc;
      _dataSet.get(_curParamName, sc);
      std::vector<std::string> values = sc.getValues();
      if (std::find(values.begin(), values.end(), value) != values.end()) {
        sc.setCurrent(value);
      }
      _dataSet.set(_curParamName, sc);
    }
  }

private:
  std::string _algoName;
  tlp::DataSet _dataSet;
  std::string _curParamName;
  bool _parsingObject;
  std::string _curObjectKey;
  std::string _typename;
  unsigned long _pointer;
};

tlp::DataSet getAlgorithmParametersDataSet(const std::string &algoName, tlp::Graph *graph, const char *jsonParameters) {
  AlgorithParametersJSONParser apjp(algoName, graph);
  apjp.parse(reinterpret_cast<const unsigned char *>(jsonParameters), strlen(jsonParameters));
  return *apjp.getParametersDataSet();
}

std::string convertDataSetToJSON(const tlp::DataSet &dataSet) {
  static std::string ret;
  unsigned int i = 1;
  std::pair<std::string, tlp::DataType *> dataSetEntry;
  std::ostringstream oss;
  oss << "{";
  forEach(dataSetEntry, dataSet.getValues()) {
    std::string entryName = dataSetEntry.first;
    tlp::DataType *dt = dataSetEntry.second;
    oss << "\"" << entryName << "\" : ";
    tlp::DataTypeSerializer *serializer = tlp::DataSet::typenameToSerializer(dt->getTypeName());
    if (dt->getTypeName() == std::string(typeid(tlp::StringCollection).name())) {
      tlp::StringCollection sc;
      dataSet.get(entryName, sc);
      oss << "\"" << sc.getCurrentString() << "\"";
    } else if (dt->getTypeName() == std::string(typeid(tlp::BooleanProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::ColorProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::DoubleProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::IntegerProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::LayoutProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::SizeProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::StringProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::BooleanVectorProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::ColorVectorProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::DoubleProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::IntegerVectorProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::CoordVectorProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::SizeVectorProperty *).name()) ||
               dt->getTypeName() == std::string(typeid(tlp::StringVectorProperty *).name())) {
      tlp::PropertyInterface *prop = NULL;
      dataSet.get(entryName, prop);
      if (prop) {
        oss << "{\"type\" : \"property\", \"name\" : \"" << prop->getName() << "\", \"pointer\" : " << reinterpret_cast<unsigned long>(prop) << "}";
      } else {
        oss << "null";
      }
    } else if (serializer) {
      std::string s = serializer->toString(dt);
      if (dt->getTypeName() != std::string(typeid(std::string).name())) {
        replaceStringInPlace(s, "(", "[");
        replaceStringInPlace(s, ")", "]");
      }
      oss << s;
    } else {
      oss << "null";
    }

    if (i++ != dataSet.size()) {
      oss << ", ";
    }
  }

  oss << "}";
  ret = oss.str();
  return ret;
}

// ==================================================================================================================

static tlp::KnownTypeSerializer<tlp::UnsignedIntegerVectorType> uintSerializer("");

static std::map<tlp::node, unsigned int> metaNodeValue;
static std::map<tlp::edge, std::string> metaEdgeValue;

void createNode(tlp::Graph *graph, unsigned int nodeId) {
  if (graph && graph->numberOfNodes() == nodeId) {
    graph->addNode();
  }
}

void createEdge(tlp::Graph *graph, unsigned int edgeId, unsigned int srcNodeId, unsigned int tgtNodeId) {
  if (graph && graph->numberOfEdges() == edgeId) {
    tlp::node srcNode(srcNodeId);
    tlp::node tgtNode(tgtNodeId);
    if (graph && graph->isElement(srcNode) && graph->isElement(tgtNode)) {
      graph->addEdge(srcNode, tgtNode);
    }
  }
}

extern "C" {

const char *EMSCRIPTEN_KEEPALIVE getDefaultAlgorithmParametersJSON(const char *algoName, tlp::Graph *graph) {
  tlp::DataSet params = getDefaultAlgorithmParameters(algoName, graph);
  static std::string ret;
  ret = convertDataSetToJSON(params);
  return ret.c_str();
}

void EMSCRIPTEN_KEEPALIVE createGraphProperty(tlp::Graph *graph, const char *type, const char *name, const char *nodeDefaultStringValue,
                                              const char *edgeDefaultStringValue) {
  if (!graph)
    return;
  tlp::PropertyInterface *prop = NULL;
  std::string typeStr = type;
  if (typeStr == "bool") {
    prop = graph->getLocalProperty<tlp::BooleanProperty>(name);
  }
  if (typeStr == "color") {
    prop = graph->getLocalProperty<tlp::ColorProperty>(name);
  }
  if (typeStr == "double") {
    prop = graph->getLocalProperty<tlp::DoubleProperty>(name);
  }
  if (typeStr == "string") {
    prop = graph->getLocalProperty<tlp::StringProperty>(name);
  }
  if (typeStr == "int") {
    prop = graph->getLocalProperty<tlp::IntegerProperty>(name);
  }
  if (typeStr == "layout") {
    prop = graph->getLocalProperty<tlp::LayoutProperty>(name);
  }
  if (typeStr == "size") {
    prop = graph->getLocalProperty<tlp::SizeProperty>(name);
  }
  if (typeStr == "graph") {
    prop = graph->getLocalProperty<tlp::GraphProperty>(name);
  }
  if (typeStr == "vector<color>") {
    prop = graph->getLocalProperty<tlp::ColorVectorProperty>(name);
  }
  if (typeStr == "vector<double>") {
    prop = graph->getLocalProperty<tlp::DoubleVectorProperty>(name);
  }
  if (typeStr == "vector<string>") {
    prop = graph->getLocalProperty<tlp::StringVectorProperty>(name);
  }
  if (typeStr == "vector<int>") {
    prop = graph->getLocalProperty<tlp::IntegerVectorProperty>(name);
  }
  if (typeStr == "vector<coord>") {
    prop = graph->getLocalProperty<tlp::CoordVectorProperty>(name);
  }
  if (typeStr == "vector<bool>") {
    prop = graph->getLocalProperty<tlp::BooleanVectorProperty>(name);
  }
  if (typeStr == "vector<size>") {
    prop = graph->getLocalProperty<tlp::SizeVectorProperty>(name);
  }
  if (prop) {
    prop->setAllNodeStringValue(nodeDefaultStringValue);
    prop->setAllEdgeStringValue(edgeDefaultStringValue);
  }
}

void setNodePropertyStringValue(tlp::Graph *graph, unsigned int nodeId, const std::string &propertyName, const std::string &propertyStringValue) {
  tlp::node n(nodeId);
  if (graph && graph->isElement(n)) {
    tlp::PropertyInterface *prop = graph->getProperty(propertyName);
    if (prop) {
      // fix what looks like an emscripten bug, some strings ends up corrupt in the property storage
      // to check if it is still present in future emscripten releases
      if (prop->getTypename() == "string") {
        static_cast<tlp::StringProperty *>(prop)->setNodeValue(n, propertyStringValue);
      } else {
        prop->setNodeStringValue(n, propertyStringValue);
      }
    }
  }
}

void setEdgePropertyStringValue(tlp::Graph *graph, unsigned int edgeId, const std::string &propertyName, const std::string &propertyStringValue) {
  tlp::edge e(edgeId);
  if (graph && graph->isElement(e)) {
    tlp::PropertyInterface *prop = graph->getProperty(propertyName);
    if (prop) {
      // fix what looks like an emscripten bug, some strings ends up corrupt in the property storage
      // to check if it is still present in future emscripten releases
      if (prop->getTypename() == "string") {
        static_cast<tlp::StringProperty *>(prop)->setEdgeValue(e, propertyStringValue);
      } else {
        prop->setEdgeStringValue(e, propertyStringValue);
      }
    }
  }
}

class GraphAttributesJSONDataParser : public YajlParseFacade {

public:
  GraphAttributesJSONDataParser(tlp::Graph *graph) {
    _attributesDataset = &const_cast<tlp::DataSet &>(graph->getAttributes());
  }

  void parseMapKey(const std::string &value) {
    _attributeName = value;
  }

  void parseString(const std::string &value) {
    if (_attributeTypename.empty()) {
      _attributeTypename = value;
    } else {
      std::stringstream data(value);
      _attributesDataset->readData(data, _attributeName, _attributeTypename);
      _attributeTypename = "";
    }
  }

private:
  tlp::DataSet *_attributesDataset;
  std::string _attributeName;
  std::string _attributeTypename;
};

void EMSCRIPTEN_KEEPALIVE parseGraphAttributesJSONData(tlp::Graph *graph, const char *attributes) {
  GraphAttributesJSONDataParser gajdp(graph);
  gajdp.parse(reinterpret_cast<const unsigned char *>(attributes), strlen(attributes));
}

class SubGraphPropertiesJSONDataParser : public YajlParseFacade {

public:
  SubGraphPropertiesJSONDataParser(tlp::Graph *subgraph)
      : _subgraph(subgraph), _treeLevel(-1), _parsingNodesPropertyValues(false), _parsingEdgesPropertyValues(false) {
  }

  void parseStartMap() {
    ++_treeLevel;
  }

  void parseEndMap() {
    --_treeLevel;
    if (_treeLevel == 1 && _parsingNodesPropertyValues) {
      _parsingNodesPropertyValues = false;
    }
    if (_treeLevel == 1 && _parsingEdgesPropertyValues) {
      _parsingEdgesPropertyValues = false;
    }
  }

  void parseMapKey(const std::string &value) {
    _lastMapKey = value;
    if (_treeLevel == 0) {
      _currentPropertyName = value;
    }
    if (_treeLevel == 1 && value == "nodesValues") {
      _parsingNodesPropertyValues = true;
    }
    if (_treeLevel == 1 && value == "edgesValues") {
      _parsingEdgesPropertyValues = true;
    }
    if (_treeLevel == 2) {
      _elementId = strtoul(value.c_str(), NULL, 10);
    }
  }

  void parseString(const std::string &value) {
    if (_lastMapKey == "type" && _treeLevel == 1) {
      _currentPropertyType = value;
    }
    if (_lastMapKey == "nodeDefault" && _treeLevel == 1) {
      _currentPropertyNodeDefaultValue = value;
    }
    if (_lastMapKey == "edgeDefault" && _treeLevel == 1) {
      _currentPropertyNodeDefaultValue = value;
      createGraphProperty(_subgraph, _currentPropertyType.c_str(), _currentPropertyName.c_str(), _currentPropertyNodeDefaultValue.c_str(),
                          _currentPropertyEdgeDefaultValue.c_str());
    }
    if (_treeLevel == 2 && _parsingNodesPropertyValues) {
      setNodePropertyStringValue(_subgraph, _elementId, _currentPropertyName.c_str(), value.c_str());
    }
    if (_treeLevel == 2 && _parsingEdgesPropertyValues) {
      setEdgePropertyStringValue(_subgraph, _elementId, _currentPropertyName.c_str(), value.c_str());
    }
  }

private:
  tlp::Graph *_subgraph;
  int _treeLevel;
  std::string _lastMapKey;
  std::string _currentPropertyName;
  std::string _currentPropertyType;
  std::string _currentPropertyNodeDefaultValue;
  std::string _currentPropertyEdgeDefaultValue;
  bool _parsingNodesPropertyValues;
  bool _parsingEdgesPropertyValues;
  unsigned int _elementId;
};

tlp::Graph *EMSCRIPTEN_KEEPALIVE addSubGraph(tlp::Graph *graph, unsigned int parentGraphId, unsigned int subGraphId, const char *nodesIds,
                                             const char *edgesIds, const char *attributes, const char *properties) {

  tlp::Graph *parentGraph = graph;
  if (parentGraph->getId() != parentGraphId) {
    parentGraph = parentGraph->getDescendantGraph(parentGraphId);
  }

  tlp::Graph *sg = parentGraph->getSubGraph(subGraphId);
  if (!sg) {
    sg = static_cast<tlp::GraphAbstract *>(parentGraph)->addSubGraph(subGraphId);
    observeObject(sg);
  }

  std::vector<unsigned int> ids;
  std::istringstream iss;
  iss.str(nodesIds);
  uintSerializer.read(iss, ids);
  for (size_t i = 0; i < ids.size(); ++i) {
    sg->addNode(tlp::node(ids[i]));
  }

  iss.str(edgesIds);
  uintSerializer.read(iss, ids);
  for (size_t i = 0; i < ids.size(); ++i) {
    sg->addEdge(tlp::edge(ids[i]));
  }

  parseGraphAttributesJSONData(sg, attributes);

  SubGraphPropertiesJSONDataParser sgpjdp(sg);
  sgpjdp.parse(reinterpret_cast<const unsigned char *>(properties), strlen(properties));

  return sg;
}

class NodesJSONDataParser : public YajlParseFacade {

public:
  NodesJSONDataParser(tlp::Graph *graph) : _graph(graph), _parsingProperties(false) {
  }

  void parseInteger(long long integerVal) {
    if (_lastMapKey == "nodeId") {
      _nodeId = static_cast<unsigned int>(integerVal);
      createNode(_graph, _nodeId);
    }
  }
  void parseString(const std::string &value) {
    if (_parsingProperties) {
      if (_lastMapKey != "viewMetaGraph") {
        setNodePropertyStringValue(_graph, _nodeId, _lastMapKey, value);
      } else if (value != "") {
        metaNodeValue[tlp::node(_nodeId)] = strtoul(value.c_str(), NULL, 10);
      }
    }
  }
  void parseMapKey(const std::string &value) {
    _lastMapKey = value;
    if (value == "properties") {
      _parsingProperties = true;
    }
  }
  void parseEndMap() {
    if (_parsingProperties) {
      _parsingProperties = false;
    }
  }

private:
  tlp::Graph *_graph;
  unsigned int _nodeId;
  bool _parsingProperties;
  std::string _lastMapKey;
};

void EMSCRIPTEN_KEEPALIVE parseNodesJSONData(tlp::Graph *graph, const char *jsonStr) {
  NodesJSONDataParser njdp(graph);
  njdp.parse(reinterpret_cast<const unsigned char *>(jsonStr), strlen(jsonStr));
}

class EdgesJSONDataParser : public YajlParseFacade {

public:
  EdgesJSONDataParser(tlp::Graph *graph) : _graph(graph), _parsingProperties(false) {
  }

  void parseInteger(long long integerVal) {
    if (_lastMapKey == "edgeId") {
      _edgeId = static_cast<unsigned int>(integerVal);
    } else if (_lastMapKey == "srcNodeId") {
      _srcNodeId = static_cast<unsigned int>(integerVal);
    } else if (_lastMapKey == "tgtNodeId") {
      unsigned int tgtNodeId = static_cast<unsigned int>(integerVal);
      createEdge(_graph, _edgeId, _srcNodeId, tgtNodeId);
    }
  }
  void parseString(const std::string &value) {
    if (_parsingProperties) {
      if (_lastMapKey != "viewMetaGraph") {
        setEdgePropertyStringValue(_graph, _edgeId, _lastMapKey, value);
      } else if (value != "()") {
        metaEdgeValue[tlp::edge(_edgeId)] = value;
      }
    }
  }
  void parseMapKey(const std::string &value) {
    _lastMapKey = value;
    if (value == "properties") {
      _parsingProperties = true;
    }
  }
  void parseEndMap() {
    if (_parsingProperties) {
      _parsingProperties = false;
    }
  }

private:
  tlp::Graph *_graph;
  unsigned int _edgeId;
  unsigned int _srcNodeId;
  bool _parsingProperties;
  std::string _lastMapKey;
};

void EMSCRIPTEN_KEEPALIVE parseEdgesJSONData(tlp::Graph *graph, const char *jsonStr) {
  EdgesJSONDataParser ejdp(graph);
  ejdp.parse(reinterpret_cast<const unsigned char *>(jsonStr), strlen(jsonStr));
}

void EMSCRIPTEN_KEEPALIVE fillMetaGraphInfos(tlp::Graph *graph) {
  tlp::GraphProperty *viewMetaGraph = graph->getProperty<tlp::GraphProperty>("viewMetaGraph");
  for (auto it = metaNodeValue.begin(); it != metaNodeValue.end(); ++it) {
    viewMetaGraph->setNodeValue(it->first, graph->getDescendantGraph(it->second));
  }
  std::vector<unsigned int> ids;
  std::istringstream iss;
  for (auto it = metaEdgeValue.begin(); it != metaEdgeValue.end(); ++it) {
    iss.str(it->second);
    uintSerializer.read(iss, ids);
    std::set<tlp::edge> edges;
    for (size_t i = 0; i < ids.size(); ++i) {
      edges.insert(tlp::edge(ids[i]));
    }
    viewMetaGraph->setEdgeValue(it->first, edges);
    ids.clear();
  }
  metaNodeValue.clear();
  metaEdgeValue.clear();
}
}

// ==================================================================================================================

extern "C" {
void setProgressBarValueWorker(unsigned long graphId, int value);
void setProgressBarCommentWorker(unsigned long graphId, const char *comment);
}

class EmscriptenPluginProgress : public tlp::SimplePluginProgress {

public:
  void setGraphId(const unsigned long graphId) {
    _graphId = graphId;
  }

  void setComment(const std::string &comment) {
    if (workerMode()) {
      setProgressBarCommentWorker(_graphId, comment.c_str());
    }
  }

protected:
  void progress_handler(int step, int max_step) {
    double percent = (static_cast<double>(step) * 100.0) / static_cast<double>(max_step);
    if (workerMode()) {
      setProgressBarValueWorker(_graphId, static_cast<int>(percent));
    }
  }

  unsigned long _graphId;
};

static EmscriptenPluginProgress progress;
static tlp::SimplePluginProgress dummyProgress;

// ==================================================================================================================

extern "C" {

bool EMSCRIPTEN_KEEPALIVE isPointerDeleted(unsigned long pointer) {
  return deletedPointers.find(pointer) != deletedPointers.end();
}

void EMSCRIPTEN_KEEPALIVE setPluginProgressGraphId(unsigned long graphId) {
  progress.setGraphId(graphId);
}

const char *EMSCRIPTEN_KEEPALIVE getJSONGraph(tlp::Graph *graph) {
  static std::ostringstream oss;
  static std::string jsonGraph;
  oss.str("");
  tlp::DataSet dataSet;
  tlp::exportGraph(graph, oss, "JSON Export", dataSet);
  jsonGraph = oss.str();
  return jsonGraph.c_str();
}

// ==================================================================================================================

tlp::Graph *EMSCRIPTEN_KEEPALIVE loadGraph(const char *filename, bool notifyProgress = false) {
  tlp::PluginProgress *pluginProgress = notifyProgress ? &progress : &dummyProgress;
  tlp::Graph *g = tlp::loadGraph(filename, pluginProgress);
  if (!g) {
    std::cerr << "Error : the import of the graph file " << filename << " failed." << std::endl;
    if (!pluginProgress->getError().empty()) {
      std::cerr << pluginProgress->getError() << std::endl;
    }
  }
  observeGraphHierarchy(g);
  return g;
}

bool EMSCRIPTEN_KEEPALIVE saveGraph(tlp::Graph *graph, const char *filename, bool notifyProgress = false) {
  tlp::PluginProgress *pluginProgress = notifyProgress ? &progress : &dummyProgress;
  bool ret = tlp::saveGraph(graph, filename, pluginProgress);
  if (!ret) {
    std::cerr << "Error : the export to the graph file " << filename << " failed." << std::endl;
    if (!pluginProgress->getError().empty()) {
      std::cerr << pluginProgress->getError() << std::endl;
    }
  }
  return ret;
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_newGraph() {
  tlp::Graph *g = tlp::newGraph();
  // observeGraph(g);
  observeObject(g);
  return g;
}

void EMSCRIPTEN_KEEPALIVE Graph_delete(tlp::Graph *graph) {
  delete graph;
}

bool EMSCRIPTEN_KEEPALIVE Graph_applyAlgorithm(tlp::Graph *graph, const char *algorithm, const char *jsonParameters, bool notifyProgress = false) {
  std::string errMsg;
  tlp::Observable::holdObservers();
  tlp::DataSet parameters = getAlgorithmParametersDataSet(algorithm, graph, jsonParameters);
  bool ret = graph->applyAlgorithm(algorithm, errMsg, &parameters, notifyProgress ? &progress : NULL);
  tlp::Observable::unholdObservers();
  if (!ret && !errMsg.empty()) {
    std::cerr << "Error when executing Tulip algorithm named '" << algorithm << "' : " << errMsg << std::endl;
  }
  return ret;
}

void EMSCRIPTEN_KEEPALIVE Graph_clear(tlp::Graph *graph) {
  tlp::Observable::holdObservers();
  graph->clear();
  tlp::Observable::unholdObservers();
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_addSubGraph1(tlp::Graph *graph, tlp::BooleanProperty *selection, const char *name) {
  tlp::Graph *sg = graph->addSubGraph(selection, name);
  observeObject(sg);
  return sg;
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_addSubGraph2(tlp::Graph *graph, const char *name) {
  tlp::Graph *sg = graph->addSubGraph(name);
  observeObject(sg);
  return sg;
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_addCloneSubGraph(tlp::Graph *graph, const char *name, bool addSibling) {
  tlp::Graph *sg = graph->addCloneSubGraph(name, addSibling);
  observeObject(sg);
  return sg;
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_inducedSubGraph(tlp::Graph *graph, unsigned int *nodesIds, tlp::Graph *parentSubGraph = NULL) {
  std::set<tlp::node> nodeSet;
  while (*nodesIds != UINT_MAX) {
    nodeSet.insert(tlp::node(*nodesIds++));
  }
  tlp::Graph *sg = graph->inducedSubGraph(nodeSet, parentSubGraph);
  observeObject(sg);
  return sg;
}

void EMSCRIPTEN_KEEPALIVE Graph_delSubGraph(tlp::Graph *graph, tlp::Graph *sg) {
  graph->delSubGraph(sg);
}

void EMSCRIPTEN_KEEPALIVE Graph_delAllSubGraphs(tlp::Graph *graph, tlp::Graph *sg) {
  graph->delAllSubGraphs(sg);
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getSuperGraph(tlp::Graph *graph) {
  return graph->getSuperGraph();
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getRoot(tlp::Graph *graph) {
  return graph->getRoot();
}

void EMSCRIPTEN_KEEPALIVE Graph_getSubGraphs(tlp::Graph *graph, unsigned int *sgPointers) {
  tlp::Graph *sg = NULL;
  forEach(sg, graph->getSubGraphs()) {
    *sgPointers++ = reinterpret_cast<unsigned int>(sg);
  }
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getNthSubGraph(tlp::Graph *graph, unsigned int n) {
  return graph->getNthSubGraph(n);
}

double EMSCRIPTEN_KEEPALIVE Graph_numberOfSubGraphs(tlp::Graph *graph) {
  return graph->numberOfSubGraphs();
}

double EMSCRIPTEN_KEEPALIVE Graph_numberOfDescendantGraphs(tlp::Graph *graph) {
  return graph->numberOfDescendantGraphs();
}

bool EMSCRIPTEN_KEEPALIVE Graph_isSubGraph(tlp::Graph *graph, const tlp::Graph *subGraph) {
  return graph->isSubGraph(subGraph);
}

bool EMSCRIPTEN_KEEPALIVE Graph_isDescendantGraph(tlp::Graph *graph, const tlp::Graph *subGraph) {
  return graph->isDescendantGraph(subGraph);
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getSubGraph1(tlp::Graph *graph, unsigned int id) {
  return graph->getSubGraph(id);
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getSubGraph2(tlp::Graph *graph, const char *name) {
  return graph->getSubGraph(name);
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getDescendantGraph1(tlp::Graph *graph, unsigned int id) {
  return graph->getDescendantGraph(id);
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE Graph_getDescendantGraph2(tlp::Graph *graph, const char *name) {
  return graph->getDescendantGraph(name);
}

void EMSCRIPTEN_KEEPALIVE Graph_getDescendantGraphs(tlp::Graph *graph, unsigned int *sgPointers) {
  tlp::Graph *sg = NULL;
  forEach(sg, graph->getDescendantGraphs()) {
    *sgPointers++ = reinterpret_cast<unsigned int>(sg);
  }
}

double EMSCRIPTEN_KEEPALIVE Graph_addNode1(tlp::Graph *graph) {
  return graph->addNode().id;
}

void EMSCRIPTEN_KEEPALIVE Graph_addNode2(tlp::Graph *graph, unsigned int nodeId) {
  graph->addNode(tlp::node(nodeId));
}

void EMSCRIPTEN_KEEPALIVE Graph_addNodes1(tlp::Graph *graph, unsigned int nbNodes, unsigned int *nodesIds) {
  std::vector<tlp::node> addedNodes;
  graph->addNodes(nbNodes, addedNodes);
  for (size_t i = 0; i < addedNodes.size(); ++i) {
    *nodesIds++ = addedNodes[i].id;
  }
}

void EMSCRIPTEN_KEEPALIVE Graph_addNodes2(tlp::Graph *graph, unsigned int nbNodes, unsigned int *nodesIds) {
  std::vector<tlp::node> nodesToAdd;
  for (size_t i = 0; i < nbNodes; ++i) {
    nodesToAdd.push_back(tlp::node(nodesIds[i]));
  }
  graph->addNodes(nodesToAdd);
}

void EMSCRIPTEN_KEEPALIVE Graph_delNode(tlp::Graph *graph, unsigned int nodeId, bool deleteInAllGraphs) {
  graph->delNode(tlp::node(nodeId), deleteInAllGraphs);
}

void EMSCRIPTEN_KEEPALIVE Graph_delNodes(tlp::Graph *graph, unsigned int nbNodes, unsigned int *nodesIds, bool deleteInAllGraphs) {
  std::vector<tlp::node> nodesToRemove;
  for (size_t i = 0; i < nbNodes; ++i) {
    nodesToRemove.push_back(tlp::node(nodesIds[i]));
  }
  graph->delNodes(nodesToRemove, deleteInAllGraphs);
}

double EMSCRIPTEN_KEEPALIVE Graph_addEdge1(tlp::Graph *graph, unsigned int srcId, unsigned int tgtId) {
  return graph->addEdge(tlp::node(srcId), tlp::node(tgtId)).id;
}

void EMSCRIPTEN_KEEPALIVE Graph_addEdge2(tlp::Graph *graph, unsigned int edgeId) {
  graph->addEdge(tlp::edge(edgeId));
}

void EMSCRIPTEN_KEEPALIVE Graph_addEdges1(tlp::Graph *graph, unsigned int nbEdges, unsigned int *nodesIds, unsigned int *edgesIds) {
  std::vector<std::pair<tlp::node, tlp::node>> edgesToAdd;
  for (size_t i = 0; i < nbEdges; ++i) {
    edgesToAdd.push_back(std::make_pair(tlp::node(nodesIds[2 * i]), tlp::node(nodesIds[2 * i + 1])));
  }
  std::vector<tlp::edge> addedEdges;
  graph->addEdges(edgesToAdd, addedEdges);
  for (size_t i = 0; i < addedEdges.size(); ++i) {
    *edgesIds++ = addedEdges[i].id;
  }
}

void EMSCRIPTEN_KEEPALIVE Graph_addEdges2(tlp::Graph *graph, unsigned int nbEdges, unsigned int *edgesIds) {
  std::vector<tlp::edge> edgesToAdd;
  for (size_t i = 0; i < nbEdges; ++i) {
    edgesToAdd.push_back(tlp::edge(edgesIds[i]));
  }
  graph->addEdges(edgesToAdd);
}

void EMSCRIPTEN_KEEPALIVE Graph_delEdge(tlp::Graph *graph, unsigned int edgeId, bool deleteInAllGraphs) {
  graph->delEdge(tlp::edge(edgeId), deleteInAllGraphs);
}

void EMSCRIPTEN_KEEPALIVE Graph_delEdges(tlp::Graph *graph, unsigned int nbEdges, unsigned int *edgesIds, bool deleteInAllGraphs) {
  std::vector<tlp::edge> edgesToRemove;
  for (size_t i = 0; i < nbEdges; ++i) {
    edgesToRemove.push_back(tlp::edge(edgesIds[i]));
  }
  graph->delEdges(edgesToRemove, deleteInAllGraphs);
}

void EMSCRIPTEN_KEEPALIVE Graph_setEdgeOrder(tlp::Graph *graph, unsigned int nodeId, unsigned int nbEdges, unsigned int *edgesIds) {
  std::vector<tlp::edge> edges;
  for (size_t i = 0; i < nbEdges; ++i) {
    edges.push_back(tlp::edge(edgesIds[i]));
  }
  graph->setEdgeOrder(tlp::node(nodeId), edges);
}

void EMSCRIPTEN_KEEPALIVE Graph_swapEdgeOrder(tlp::Graph *graph, unsigned int nodeId, unsigned int edgeId1, unsigned int edgeId2) {
  graph->swapEdgeOrder(tlp::node(nodeId), tlp::edge(edgeId1), tlp::edge(edgeId2));
}

void EMSCRIPTEN_KEEPALIVE Graph_setSource(tlp::Graph *graph, unsigned int edgeId, unsigned int sourceNodeId) {
  graph->setSource(tlp::edge(edgeId), tlp::node(sourceNodeId));
}

void EMSCRIPTEN_KEEPALIVE Graph_setTarget(tlp::Graph *graph, unsigned int edgeId, unsigned int targetNodeId) {
  graph->setTarget(tlp::edge(edgeId), tlp::node(targetNodeId));
}

void EMSCRIPTEN_KEEPALIVE Graph_setEnds(tlp::Graph *graph, unsigned int edgeId, unsigned int sourceNodeId, unsigned int targetNodeId) {
  graph->setEnds(tlp::edge(edgeId), tlp::node(sourceNodeId), tlp::node(targetNodeId));
}

void EMSCRIPTEN_KEEPALIVE Graph_reverse(tlp::Graph *graph, unsigned int edgeId) {
  graph->reverse(tlp::edge(edgeId));
}

double EMSCRIPTEN_KEEPALIVE Graph_getSource(tlp::Graph *graph) {
  return graph->getSource().id;
}

void fillNodesIds(tlp::Iterator<tlp::node> *itNodes, unsigned int *nodesIds) {
  tlp::node n;
  forEach(n, itNodes) {
    *nodesIds++ = n.id;
  }
}

void EMSCRIPTEN_KEEPALIVE Graph_getNodes(tlp::Graph *graph, unsigned int *nodesIds) {
  fillNodesIds(graph->getNodes(), nodesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_getInNodes(tlp::Graph *graph, unsigned int nodeId, unsigned int *nodesIds) {
  fillNodesIds(graph->getInNodes(tlp::node(nodeId)), nodesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_getOutNodes(tlp::Graph *graph, unsigned int nodeId, unsigned int *nodesIds) {
  fillNodesIds(graph->getOutNodes(tlp::node(nodeId)), nodesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_getInOutNodes(tlp::Graph *graph, unsigned int nodeId, unsigned int *nodesIds) {
  fillNodesIds(graph->getInOutNodes(tlp::node(nodeId)), nodesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_bfs(tlp::Graph *graph, unsigned int rootNodeId, unsigned int *nodesIds) {
  unsigned int nodeCount = 0;
  tlp::node n;
  forEach(n, graph->bfs(tlp::node(rootNodeId))) {
    *nodesIds++ = n.id;
    ++nodeCount;
  }
  if (nodeCount < graph->numberOfNodes()) {
    *nodesIds++ = UINT_MAX;
  }
}

void EMSCRIPTEN_KEEPALIVE Graph_dfs(tlp::Graph *graph, unsigned int rootNodeId, unsigned int *nodesIds) {
  unsigned int nodeCount = 0;
  tlp::node n;
  forEach(n, graph->dfs(tlp::node(rootNodeId))) {
    *nodesIds++ = n.id;
    ++nodeCount;
  }
  if (nodeCount < graph->numberOfNodes()) {
    *nodesIds++ = UINT_MAX;
  }
}

void fillEdgesIds(tlp::Iterator<tlp::edge> *itEdges, unsigned int *edgesIds) {
  tlp::edge e;
  forEach(e, itEdges) {
    *edgesIds++ = e.id;
  }
}

void EMSCRIPTEN_KEEPALIVE Graph_getEdges(tlp::Graph *graph, unsigned int *edgesIds) {
  fillEdgesIds(graph->getEdges(), edgesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_getOutEdges(tlp::Graph *graph, unsigned int nodeId, unsigned int *edgesIds) {
  fillEdgesIds(graph->getOutEdges(tlp::node(nodeId)), edgesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_getInOutEdges(tlp::Graph *graph, unsigned int nodeId, unsigned int *edgesIds) {
  fillEdgesIds(graph->getInOutEdges(tlp::node(nodeId)), edgesIds);
}

void EMSCRIPTEN_KEEPALIVE Graph_getInEdges(tlp::Graph *graph, unsigned int nodeId, unsigned int *edgesIds) {
  fillEdgesIds(graph->getInEdges(tlp::node(nodeId)), edgesIds);
}

double EMSCRIPTEN_KEEPALIVE Graph_getId(tlp::Graph *graph) {
  return graph->getId();
}

double EMSCRIPTEN_KEEPALIVE Graph_numberOfNodes(tlp::Graph *graph) {
  return graph->numberOfNodes();
}

double EMSCRIPTEN_KEEPALIVE Graph_numberOfEdges(tlp::Graph *graph) {
  return graph->numberOfEdges();
}

double EMSCRIPTEN_KEEPALIVE Graph_deg(tlp::Graph *graph, unsigned int nodeId) {
  return graph->deg(tlp::node(nodeId));
}

double EMSCRIPTEN_KEEPALIVE Graph_indeg(tlp::Graph *graph, unsigned int nodeId) {
  return graph->indeg(tlp::node(nodeId));
}

double EMSCRIPTEN_KEEPALIVE Graph_outdeg(tlp::Graph *graph, unsigned int nodeId) {
  return graph->outdeg(tlp::node(nodeId));
}

double EMSCRIPTEN_KEEPALIVE Graph_source(tlp::Graph *graph, unsigned int edgeId) {
  return graph->source(tlp::edge(edgeId)).id;
}

double EMSCRIPTEN_KEEPALIVE Graph_target(tlp::Graph *graph, unsigned int edgeId) {
  return graph->target(tlp::edge(edgeId)).id;
}

void EMSCRIPTEN_KEEPALIVE Graph_ends(tlp::Graph *graph, unsigned int edgeId, unsigned int *array) {
  std::pair<tlp::node, tlp::node> ends = graph->ends(tlp::edge(edgeId));
  array[0] = ends.first.id;
  array[1] = ends.second.id;
}

double EMSCRIPTEN_KEEPALIVE Graph_opposite(tlp::Graph *graph, unsigned int edgeId, unsigned int nodeId) {
  return graph->opposite(tlp::edge(edgeId), tlp::node(nodeId)).id;
}

bool EMSCRIPTEN_KEEPALIVE Graph_isNodeElement(tlp::Graph *graph, unsigned int nodeId) {
  return graph->isElement(tlp::node(nodeId));
}

bool EMSCRIPTEN_KEEPALIVE Graph_isEdgeElement(tlp::Graph *graph, unsigned int edgeId) {
  return graph->isElement(tlp::edge(edgeId));
}

bool EMSCRIPTEN_KEEPALIVE Graph_hasEdge(tlp::Graph *graph, unsigned int sourceNodeId, unsigned int targetNodeId, bool directed) {
  return graph->hasEdge(tlp::node(sourceNodeId), tlp::node(targetNodeId), directed);
}

double EMSCRIPTEN_KEEPALIVE Graph_existEdge(tlp::Graph *graph, unsigned int sourceNodeId, unsigned int targetNodeId, bool directed) {
  return graph->existEdge(tlp::node(sourceNodeId), tlp::node(targetNodeId), directed).id;
}

void EMSCRIPTEN_KEEPALIVE Graph_getEdges2(tlp::Graph *graph, unsigned int sourceNodeId, unsigned int targetNodeId, bool directed,
                                          unsigned int *array) {
  std::vector<tlp::edge> edges = graph->getEdges(tlp::node(sourceNodeId), tlp::node(targetNodeId), directed);
  for (size_t i = 0; i < edges.size(); ++i) {
    *array++ = edges[i].id;
  }
  if (edges.size() < graph->deg(tlp::node(sourceNodeId))) {
    *array = UINT_MAX;
  }
}

void EMSCRIPTEN_KEEPALIVE Graph_setName(tlp::Graph *graph, const char *name) {
  graph->setName(name);
}

const char *EMSCRIPTEN_KEEPALIVE Graph_getName(tlp::Graph *graph) {
  static std::string s;
  s = graph->getName();
  return s.c_str();
}

double EMSCRIPTEN_KEEPALIVE Graph_numberOfProperties(tlp::Graph *graph) {
  unsigned int ret = 0;
  std::string propName;
  forEach(propName, graph->getProperties()) {
    ++ret;
  }
  return ret;
}

double EMSCRIPTEN_KEEPALIVE Graph_propertiesNamesLengths(tlp::Graph *graph, unsigned int *array) {
  return getStringsNumberOfBytesFromStringIterator(graph->getProperties(), array);
}

void EMSCRIPTEN_KEEPALIVE Graph_getProperties(tlp::Graph *graph, unsigned char *array) {
  fillBytesArrayFromStringIterator(graph->getProperties(), array);
}

tlp::PropertyInterface *EMSCRIPTEN_KEEPALIVE Graph_getProperty(tlp::Graph *graph, const char *name) {
  tlp::PropertyInterface *prop = graph->getProperty(name);
  observeObject(prop);
  return prop;
}

tlp::BooleanProperty *EMSCRIPTEN_KEEPALIVE Graph_getBooleanProperty(tlp::Graph *graph, const char *name) {
  tlp::BooleanProperty *prop = graph->getProperty<tlp::BooleanProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::BooleanProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalBooleanProperty(tlp::Graph *graph, const char *name) {
  tlp::BooleanProperty *prop = graph->getLocalProperty<tlp::BooleanProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE Graph_getColorProperty(tlp::Graph *graph, const char *name) {
  tlp::ColorProperty *prop = graph->getProperty<tlp::ColorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalColorProperty(tlp::Graph *graph, const char *name) {
  tlp::ColorProperty *prop = graph->getLocalProperty<tlp::ColorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::DoubleProperty *EMSCRIPTEN_KEEPALIVE Graph_getDoubleProperty(tlp::Graph *graph, const char *name) {
  tlp::DoubleProperty *prop = graph->getProperty<tlp::DoubleProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::DoubleProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalDoubleProperty(tlp::Graph *graph, const char *name) {
  tlp::DoubleProperty *prop = graph->getLocalProperty<tlp::DoubleProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE Graph_getIntegerProperty(tlp::Graph *graph, const char *name) {
  tlp::IntegerProperty *prop = graph->getProperty<tlp::IntegerProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalIntegerProperty(tlp::Graph *graph, const char *name) {
  tlp::IntegerProperty *prop = graph->getLocalProperty<tlp::IntegerProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::LayoutProperty *EMSCRIPTEN_KEEPALIVE Graph_getLayoutProperty(tlp::Graph *graph, const char *name) {
  tlp::LayoutProperty *prop = graph->getProperty<tlp::LayoutProperty>(name);
  if (!workerMode())
    observeObject(prop);
  return prop;
}

tlp::LayoutProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalLayoutProperty(tlp::Graph *graph, const char *name) {
  tlp::LayoutProperty *prop = graph->getLocalProperty<tlp::LayoutProperty>(name);
  if (!workerMode())
    observeObject(prop);
  return prop;
}

tlp::SizeProperty *EMSCRIPTEN_KEEPALIVE Graph_getSizeProperty(tlp::Graph *graph, const char *name) {
  tlp::SizeProperty *prop = graph->getProperty<tlp::SizeProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::SizeProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalSizeProperty(tlp::Graph *graph, const char *name) {
  tlp::SizeProperty *prop = graph->getLocalProperty<tlp::SizeProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE Graph_getStringProperty(tlp::Graph *graph, const char *name) {
  tlp::StringProperty *prop = graph->getProperty<tlp::StringProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalStringProperty(tlp::Graph *graph, const char *name) {
  tlp::StringProperty *prop = graph->getLocalProperty<tlp::StringProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::BooleanVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getBooleanVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::BooleanVectorProperty *prop = graph->getProperty<tlp::BooleanVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::BooleanVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalBooleanVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::BooleanVectorProperty *prop = graph->getLocalProperty<tlp::BooleanVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::ColorVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getColorVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::ColorVectorProperty *prop = graph->getProperty<tlp::ColorVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::ColorVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalColorVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::ColorVectorProperty *prop = graph->getLocalProperty<tlp::ColorVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::DoubleVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getDoubleVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::DoubleVectorProperty *prop = graph->getProperty<tlp::DoubleVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::DoubleVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalDoubleVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::DoubleVectorProperty *prop = graph->getLocalProperty<tlp::DoubleVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::IntegerVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getIntegerVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::IntegerVectorProperty *prop = graph->getProperty<tlp::IntegerVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::IntegerVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalIntegerVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::IntegerVectorProperty *prop = graph->getLocalProperty<tlp::IntegerVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::CoordVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getCoordVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::CoordVectorProperty *prop = graph->getProperty<tlp::CoordVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::CoordVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalCoordVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::CoordVectorProperty *prop = graph->getLocalProperty<tlp::CoordVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::SizeVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getSizeVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::SizeVectorProperty *prop = graph->getProperty<tlp::SizeVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::SizeVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalSizeVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::SizeVectorProperty *prop = graph->getLocalProperty<tlp::SizeVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::StringVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getStringVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::StringVectorProperty *prop = graph->getProperty<tlp::StringVectorProperty>(name);
  observeObject(prop);
  return prop;
}

tlp::StringVectorProperty *EMSCRIPTEN_KEEPALIVE Graph_getLocalStringVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::StringVectorProperty *prop = graph->getProperty<tlp::StringVectorProperty>(name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE Graph_delLocalProperty(tlp::Graph *graph, const char *name) {
  graph->delLocalProperty(name);
}

bool EMSCRIPTEN_KEEPALIVE Graph_applyPropertyAlgorithm(tlp::Graph *graph, const char *algorithm, tlp::PropertyInterface *result,
                                                       const char *jsonParameters, bool notifyProgress) {
  std::string errMsg;
  tlp::DataSet parameters = getAlgorithmParametersDataSet(algorithm, graph, jsonParameters);
  tlp::Observable::holdObservers();
  bool ret = graph->applyPropertyAlgorithm(algorithm, result, errMsg, notifyProgress ? &progress : NULL, &parameters);
  tlp::Observable::unholdObservers();
  if (!ret && !errMsg.empty()) {
    std::cerr << "Error when executing Tulip " << result->getTypename() << " algorithm named '" << algorithm << "' : " << errMsg << std::endl;
  }
  return ret;
}

void EMSCRIPTEN_KEEPALIVE Graph_push(tlp::Graph *graph) {
  graph->push();
}

void EMSCRIPTEN_KEEPALIVE Graph_pop(tlp::Graph *graph) {
  graph->pop();
}

void EMSCRIPTEN_KEEPALIVE Graph_setEventsActivated(tlp::Graph *graph, bool eventsActivated) {
  if (eventsActivated) {
    observeGraph(graph);
  } else {
    unobserveGraph(graph);
  }
}

const char *EMSCRIPTEN_KEEPALIVE Graph_getAttributesJSON(tlp::Graph *graph) {
  static std::string ret;
  ret = convertDataSetToJSON(graph->getAttributes());
  return ret.c_str();
}

bool EMSCRIPTEN_KEEPALIVE Graph_isMetaNode(tlp::Graph *graph, unsigned int nodeId) {
  return graph->isMetaNode(tlp::node(nodeId));
}

void EMSCRIPTEN_KEEPALIVE Graph_openMetaNode(tlp::Graph *graph, unsigned int nodeId) {
  graph->openMetaNode(tlp::node(nodeId));
}

bool EMSCRIPTEN_KEEPALIVE Graph_isConnected(tlp::Graph *graph) {
  return tlp::ConnectedTest::isConnected(graph);
}

double EMSCRIPTEN_KEEPALIVE Graph_getRandomNode(tlp::Graph *graph) {
  return graph->getRandomNode().id;
}

double EMSCRIPTEN_KEEPALIVE Graph_getRandomEdge(tlp::Graph *graph) {
  return graph->getRandomEdge().id;
}

// ==================================================================================================================

bool EMSCRIPTEN_KEEPALIVE algorithmExists(const char *algoName) {
  return pluginExists<tlp::Algorithm>(algoName);
}

double EMSCRIPTEN_KEEPALIVE numberOfPlugins() {
  return tlp::PluginLister::instance()->availablePlugins().size();
}

double EMSCRIPTEN_KEEPALIVE pluginsNamesLengths(unsigned int *array) {
  return getStringsNumberOfBytesFromContainer(tlp::PluginLister::instance()->availablePlugins(), array);
}

void EMSCRIPTEN_KEEPALIVE pluginsList(unsigned char *array) {
  fillBytesArrayFromStringsContainer(tlp::PluginLister::instance()->availablePlugins(), array);
}

double EMSCRIPTEN_KEEPALIVE numberOfAlgorithms() {
  return getTulipPluginsList<tlp::Algorithm>().size();
}

double EMSCRIPTEN_KEEPALIVE algorithmsNamesLengths(unsigned int *array) {
  return getStringsNumberOfBytesFromContainer(getTulipPluginsList<tlp::Algorithm>(), array);
}

void EMSCRIPTEN_KEEPALIVE algorithmsList(unsigned char *array) {
  fillBytesArrayFromStringsContainer(getTulipPluginsList<tlp::Algorithm>(), array);
}

bool EMSCRIPTEN_KEEPALIVE propertyAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::PropertyAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE booleanAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::BooleanAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE colorAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::ColorAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE doubleAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::DoubleAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE integerAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::IntegerAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE layoutAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::LayoutAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE sizeAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::SizeAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE stringAlgorithmExists(const char *algoName) {
  return pluginExists<tlp::StringAlgorithm>(algoName);
}

bool EMSCRIPTEN_KEEPALIVE importPluginExists(const char *algoName) {
  return pluginExists<tlp::ImportModule>(algoName);
}

// ==================================================================================================================

void EMSCRIPTEN_KEEPALIVE holdObservers() {
  tlp::Observable::holdObservers();
}

void EMSCRIPTEN_KEEPALIVE unholdObservers() {
  tlp::Observable::unholdObservers();
}

// ==================================================================================================================

void EMSCRIPTEN_KEEPALIVE PropertyInterface_delete(tlp::PropertyInterface *property) {
  delete property;
}

const char *EMSCRIPTEN_KEEPALIVE PropertyInterface_getName(tlp::PropertyInterface *property) {
  return property->getName().c_str();
}

const char *EMSCRIPTEN_KEEPALIVE PropertyInterface_getTypename(tlp::PropertyInterface *property) {
  return property->getTypename().c_str();
}

tlp::Graph *EMSCRIPTEN_KEEPALIVE PropertyInterface_getGraph(tlp::PropertyInterface *property) {
  return property->getGraph();
}

const char *EMSCRIPTEN_KEEPALIVE PropertyInterface_getNodeStringValue(tlp::PropertyInterface *property, unsigned int nodeId) {
  static std::string s;
  s = property->getNodeStringValue(tlp::node(nodeId));
  return s.c_str();
}

const char *EMSCRIPTEN_KEEPALIVE PropertyInterface_getEdgeStringValue(tlp::PropertyInterface *property, unsigned int edgeId) {
  static std::string s;
  s = property->getEdgeStringValue(tlp::edge(edgeId));
  return s.c_str();
}

void EMSCRIPTEN_KEEPALIVE PropertyInterface_copy(tlp::PropertyInterface *property, tlp::PropertyInterface *propertyToCopy) {
  property->copy(propertyToCopy);
}

// ==================================================================================================================

tlp::BooleanProperty *EMSCRIPTEN_KEEPALIVE createBooleanProperty(tlp::Graph *graph, const char *name) {
  tlp::BooleanProperty *prop = new tlp::BooleanProperty(graph, name);
  observeObject(prop);
  return prop;
}

bool EMSCRIPTEN_KEEPALIVE BooleanProperty_getNodeDefaultValue(tlp::BooleanProperty *booleanProperty) {
  return booleanProperty->getNodeDefaultValue();
}

bool EMSCRIPTEN_KEEPALIVE BooleanProperty_getNodeValue(tlp::BooleanProperty *booleanProperty, unsigned int n) {
  return booleanProperty->getNodeValue(tlp::node(n));
}

void EMSCRIPTEN_KEEPALIVE BooleanProperty_setNodeValue(tlp::BooleanProperty *booleanProperty, unsigned int n, bool value) {
  booleanProperty->setNodeValue(tlp::node(n), value);
}

bool EMSCRIPTEN_KEEPALIVE BooleanProperty_getEdgeDefaultValue(tlp::BooleanProperty *booleanProperty) {
  return booleanProperty->getEdgeDefaultValue();
}

bool EMSCRIPTEN_KEEPALIVE BooleanProperty_getEdgeValue(tlp::BooleanProperty *booleanProperty, unsigned int e) {
  return booleanProperty->getEdgeValue(tlp::edge(e));
}

void EMSCRIPTEN_KEEPALIVE BooleanProperty_setEdgeValue(tlp::BooleanProperty *booleanProperty, unsigned int e, bool value) {
  booleanProperty->setEdgeValue(tlp::edge(e), value);
}

void EMSCRIPTEN_KEEPALIVE BooleanProperty_setAllNodeValue(tlp::BooleanProperty *booleanProperty, bool value) {
  booleanProperty->setAllNodeValue(value);
}

void EMSCRIPTEN_KEEPALIVE BooleanProperty_setAllEdgeValue(tlp::BooleanProperty *booleanProperty, bool value) {
  booleanProperty->setAllEdgeValue(value);
}

void EMSCRIPTEN_KEEPALIVE BooleanProperty_getNodesEqualTo(tlp::BooleanProperty *booleanProperty, bool value, tlp::Graph *g, unsigned int *nodesIds) {
  unsigned int nodeCount = 0;
  tlp::node n;
  forEach(n, booleanProperty->getNodesEqualTo(value, g)) {
    *nodesIds++ = n.id;
    ++nodeCount;
  }
  if (nodeCount < booleanProperty->getGraph()->numberOfNodes()) {
    *nodesIds = UINT_MAX;
  }
}

void EMSCRIPTEN_KEEPALIVE BooleanProperty_getEdgesEqualTo(tlp::BooleanProperty *booleanProperty, bool value, tlp::Graph *g, unsigned int *edgesIds) {
  unsigned int edgeCount = 0;
  tlp::edge e;
  forEach(e, booleanProperty->getEdgesEqualTo(value, g)) {
    *edgesIds++ = e.id;
    ++edgeCount;
  }
  if (edgeCount < booleanProperty->getGraph()->numberOfEdges()) {
    *edgesIds = UINT_MAX;
  }
}

// ==================================================================================================================

tlp::BooleanVectorProperty *EMSCRIPTEN_KEEPALIVE createBooleanVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::BooleanVectorProperty *prop = new tlp::BooleanVectorProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_setAllNodeValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned char *array,
                                                                unsigned int arraySize) {
  std::vector<bool> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i] > 0;
  }
  boolVectorProperty->setAllNodeValue(v);
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_setNodeValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned int n, unsigned char *array,
                                                             unsigned int arraySize) {
  std::vector<bool> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i] > 0;
  }
  boolVectorProperty->setNodeValue(tlp::node(n), v);
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_setAllEdgeValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned char *array,
                                                                unsigned int arraySize) {
  std::vector<bool> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i] > 0;
  }
  boolVectorProperty->setAllEdgeValue(v);
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_setEdgeValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned int e, unsigned char *array,
                                                             unsigned int arraySize) {
  std::vector<bool> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i] > 0;
  }
  boolVectorProperty->setEdgeValue(tlp::edge(e), v);
}

double EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getNodeDefaultVectorSize(tlp::BooleanVectorProperty *boolVectorProperty) {
  const std::vector<bool> &value = boolVectorProperty->getNodeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getNodeDefaultValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned char *array) {
  const std::vector<bool> &value = boolVectorProperty->getNodeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

double EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getNodeVectorSize(tlp::BooleanVectorProperty *boolVectorProperty, unsigned int n) {
  const std::vector<bool> &value = boolVectorProperty->getNodeValue(tlp::node(n));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getNodeValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned int n, unsigned char *array) {
  const std::vector<bool> &value = boolVectorProperty->getNodeValue(tlp::node(n));
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

unsigned EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getEdgeDefaultVectorSize(tlp::BooleanVectorProperty *boolVectorProperty) {
  const std::vector<bool> &value = boolVectorProperty->getEdgeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getEdgeDefaultValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned char *array) {
  const std::vector<bool> &value = boolVectorProperty->getEdgeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

double EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getEdgeVectorSize(tlp::BooleanVectorProperty *boolVectorProperty, unsigned int e) {
  const std::vector<bool> &value = boolVectorProperty->getEdgeValue(tlp::edge(e));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE BooleanVectorProperty_getEdgeValue(tlp::BooleanVectorProperty *boolVectorProperty, unsigned int e, unsigned char *array) {
  const std::vector<bool> &value = boolVectorProperty->getEdgeValue(tlp::edge(e));
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

// ==================================================================================================================

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE createColorProperty(tlp::Graph *graph, const char *name) {
  tlp::ColorProperty *prop = new tlp::ColorProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_getNodeDefaultValue(tlp::ColorProperty *colorProperty, unsigned char *array) {
  const tlp::Color &c = colorProperty->getNodeDefaultValue();
  for (unsigned int i = 0; i < 4; ++i) {
    array[i] = c[i];
  }
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_getNodeValue(tlp::ColorProperty *colorProperty, unsigned int n, unsigned char *array) {
  const tlp::Color &c = colorProperty->getNodeValue(tlp::node(n));
  for (unsigned int i = 0; i < 4; ++i) {
    array[i] = c[i];
  }
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_setNodeValue(tlp::ColorProperty *colorProperty, unsigned int n, unsigned char r, unsigned char g,
                                                     unsigned char b, unsigned char a) {
  colorProperty->setNodeValue(tlp::node(n), tlp::Color(r, g, b, a));
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_getEdgeDefaultValue(tlp::ColorProperty *colorProperty, unsigned char *array) {
  const tlp::Color &c = colorProperty->getEdgeDefaultValue();
  for (unsigned int i = 0; i < 4; ++i) {
    array[i] = c[i];
  }
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_getEdgeValue(tlp::ColorProperty *colorProperty, unsigned int e, unsigned char *array) {
  const tlp::Color &c = colorProperty->getEdgeValue(tlp::edge(e));
  for (unsigned int i = 0; i < 4; ++i) {
    array[i] = c[i];
  }
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_setEdgeValue(tlp::ColorProperty *colorProperty, unsigned int e, unsigned char r, unsigned char g,
                                                     unsigned char b, unsigned char a) {
  colorProperty->setEdgeValue(tlp::edge(e), tlp::Color(r, g, b, a));
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_setAllNodeValue(tlp::ColorProperty *colorProperty, unsigned char r, unsigned char g, unsigned char b,
                                                        unsigned char a) {
  colorProperty->setAllNodeValue(tlp::Color(r, g, b, a));
}

void EMSCRIPTEN_KEEPALIVE ColorProperty_setAllEdgeValue(tlp::ColorProperty *colorProperty, unsigned char r, unsigned char g, unsigned char b,
                                                        unsigned char a) {
  colorProperty->setAllEdgeValue(tlp::Color(r, g, b, a));
}

// ==================================================================================================================

tlp::ColorVectorProperty *EMSCRIPTEN_KEEPALIVE createColorVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::ColorVectorProperty *prop = new tlp::ColorVectorProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_setAllNodeValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned char *array,
                                                              unsigned int arraySize) {
  std::vector<tlp::Color> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = tlp::Color(array[4 * i], array[4 * i + 1], array[4 * i + 2], array[4 * i + 3]);
  }
  colorVectorProperty->setAllNodeValue(v);
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_setNodeValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned int n, unsigned char *array,
                                                           unsigned int arraySize) {
  std::vector<tlp::Color> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = tlp::Color(array[4 * i], array[4 * i + 1], array[4 * i + 2], array[4 * i + 3]);
  }
  colorVectorProperty->setNodeValue(tlp::node(n), v);
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_setAllEdgeValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned char *array,
                                                              unsigned int arraySize) {
  std::vector<tlp::Color> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = tlp::Color(array[4 * i], array[4 * i + 1], array[4 * i + 2], array[4 * i + 3]);
  }
  colorVectorProperty->setAllEdgeValue(v);
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_setEdgeValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned int e, unsigned char *array,
                                                           unsigned int arraySize) {
  std::vector<tlp::Color> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = tlp::Color(array[4 * i], array[4 * i + 1], array[4 * i + 2], array[4 * i + 3]);
  }
  colorVectorProperty->setEdgeValue(tlp::edge(e), v);
}

double EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getNodeDefaultVectorSize(tlp::ColorVectorProperty *colorVectorProperty) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getNodeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getNodeDefaultValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned char *array) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getNodeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    for (size_t j = 0; j < 4; ++j)
      *array++ = value[i][j];
  }
}

double EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getNodeVectorSize(tlp::ColorVectorProperty *colorVectorProperty, unsigned int n) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getNodeValue(tlp::node(n));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getNodeValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned int n, unsigned char *array) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getNodeValue(tlp::node(n));
  for (size_t i = 0; i < value.size(); ++i) {
    for (size_t j = 0; j < 4; ++j)
      *array++ = value[i][j];
  }
}

unsigned EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getEdgeDefaultVectorSize(tlp::ColorVectorProperty *colorVectorProperty) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getEdgeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getEdgeDefaultValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned char *array) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getEdgeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    for (size_t j = 0; j < 4; ++j)
      *array++ = value[i][j];
  }
}

double EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getEdgeVectorSize(tlp::ColorVectorProperty *colorVectorProperty, unsigned int e) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getEdgeValue(tlp::edge(e));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE ColorVectorProperty_getEdgeValue(tlp::ColorVectorProperty *colorVectorProperty, unsigned int e, unsigned char *array) {
  const std::vector<tlp::Color> &value = colorVectorProperty->getEdgeValue(tlp::edge(e));
  for (size_t i = 0; i < value.size(); ++i) {
    for (size_t j = 0; j < 4; ++j)
      *array++ = value[i][j];
  }
}

// ==================================================================================================================

tlp::LayoutProperty *EMSCRIPTEN_KEEPALIVE createLayoutProperty(tlp::Graph *graph, const char *name) {
  tlp::LayoutProperty *prop = new tlp::LayoutProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_getNodeDefaultValue(tlp::LayoutProperty *layoutProperty, float *array) {
  const tlp::Coord &c = layoutProperty->getNodeDefaultValue();
  for (unsigned int i = 0; i < 3; ++i) {
    array[i] = c[i];
  }
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_getNodeValue(tlp::LayoutProperty *layoutProperty, unsigned int n, float *array) {
  const tlp::Coord &c = layoutProperty->getNodeValue(tlp::node(n));
  for (unsigned int i = 0; i < 3; ++i) {
    array[i] = c[i];
  }
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_setNodeValue(tlp::LayoutProperty *layoutProperty, unsigned int n, float x, float y, float z) {
  layoutProperty->setNodeValue(tlp::node(n), tlp::Coord(x, y, z));
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_setAllNodeValue(tlp::LayoutProperty *layoutProperty, float x, float y, float z) {
  layoutProperty->setAllNodeValue(tlp::Coord(x, y, z));
}

double EMSCRIPTEN_KEEPALIVE LayoutProperty_getEdgeDefaultNumberOfBends(tlp::LayoutProperty *layoutProperty) {
  const std::vector<tlp::Coord> &vc = layoutProperty->getEdgeDefaultValue();
  return vc.size();
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_getEdgeDefaultValue(tlp::LayoutProperty *layoutProperty, float *array) {
  const std::vector<tlp::Coord> &vc = layoutProperty->getEdgeDefaultValue();
  for (size_t i = 0; i < vc.size(); ++i) {
    const tlp::Coord &c = vc[i];
    for (unsigned int j = 0; j < 3; ++j) {
      array[3 * i + j] = c[j];
    }
  }
}

double EMSCRIPTEN_KEEPALIVE LayoutProperty_getEdgeNumberOfBends(tlp::LayoutProperty *layoutProperty, unsigned int edgeId) {
  const std::vector<tlp::Coord> &vc = layoutProperty->getEdgeValue(tlp::edge(edgeId));
  return vc.size();
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_getEdgeValue(tlp::LayoutProperty *layoutProperty, unsigned int e, float *array) {
  const std::vector<tlp::Coord> &vc = layoutProperty->getEdgeValue(tlp::edge(e));
  for (size_t i = 0; i < vc.size(); ++i) {
    const tlp::Coord &c = vc[i];
    for (unsigned int j = 0; j < 3; ++j) {
      array[3 * i + j] = c[j];
    }
  }
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_setAllEdgeValue(tlp::LayoutProperty *layoutProperty, float *array, unsigned int nbCoords) {
  std::vector<tlp::Coord> vc(nbCoords);
  for (unsigned i = 0; i < nbCoords; ++i) {
    vc[i] = tlp::Coord(array[3 * i], array[3 * i + 1], array[3 * i + 2]);
  }
  layoutProperty->setAllEdgeValue(vc);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_setEdgeValue(tlp::LayoutProperty *layoutProperty, unsigned int e, float *array, unsigned int nbCoords) {
  std::vector<tlp::Coord> vc(nbCoords);
  for (unsigned i = 0; i < nbCoords; ++i) {
    vc[i] = tlp::Coord(array[3 * i], array[3 * i + 1], array[3 * i + 2]);
  }
  layoutProperty->setEdgeValue(tlp::edge(e), vc);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_getMin(tlp::LayoutProperty *layoutProperty, tlp::Graph *sg, float *min) {
  tlp::Coord minPos = layoutProperty->getMin(sg);
  for (unsigned int i = 0; i < 3; ++i)
    min[i] = minPos[i];
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_getMax(tlp::LayoutProperty *layoutProperty, tlp::Graph *sg, float *max) {
  tlp::Coord maxPos = layoutProperty->getMax(sg);
  for (unsigned int i = 0; i < 3; ++i)
    max[i] = maxPos[i];
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_translate(tlp::LayoutProperty *layoutProperty, float dx, float dy, float dz, tlp::Graph *sg) {
  layoutProperty->translate(tlp::Vec3f(dx, dy, dz), sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_scale(tlp::LayoutProperty *layoutProperty, float sx, float sy, float sz, tlp::Graph *sg) {
  layoutProperty->scale(tlp::Vec3f(sx, sy, sz), sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_rotateX(tlp::LayoutProperty *layoutProperty, double alpha, tlp::Graph *sg) {
  layoutProperty->rotateX(alpha, sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_rotateY(tlp::LayoutProperty *layoutProperty, double alpha, tlp::Graph *sg) {
  layoutProperty->rotateY(alpha, sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_rotateZ(tlp::LayoutProperty *layoutProperty, double alpha, tlp::Graph *sg) {
  layoutProperty->rotateZ(alpha, sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_center(tlp::LayoutProperty *layoutProperty, tlp::Graph *sg) {
  layoutProperty->center(sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_center2(tlp::LayoutProperty *layoutProperty, float cx, float cy, float cz, tlp::Graph *sg) {
  layoutProperty->center(tlp::Vec3f(cx, cy, cz), sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_normalize(tlp::LayoutProperty *layoutProperty, tlp::Graph *sg) {
  layoutProperty->normalize(sg);
}

void EMSCRIPTEN_KEEPALIVE LayoutProperty_perfectAspectRatio(tlp::LayoutProperty *layoutProperty) {
  layoutProperty->perfectAspectRatio();
}

// ==================================================================================================================

tlp::SizeProperty *EMSCRIPTEN_KEEPALIVE createSizeProperty(tlp::Graph *graph, const char *name) {
  tlp::SizeProperty *prop = new tlp::SizeProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_getNodeDefaultValue(tlp::SizeProperty *sizeProperty, float *array) {
  const tlp::Size &s = sizeProperty->getNodeDefaultValue();
  for (unsigned int i = 0; i < 3; ++i) {
    array[i] = s[i];
  }
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_getNodeValue(tlp::SizeProperty *sizeProperty, unsigned int n, float *array) {
  const tlp::Size &s = sizeProperty->getNodeValue(tlp::node(n));
  for (unsigned int i = 0; i < 3; ++i) {
    array[i] = s[i];
  }
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_setAllNodeValue(tlp::SizeProperty *sizeProperty, float w, float h, float d) {
  sizeProperty->setAllNodeValue(tlp::Size(w, h, d));
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_setNodeValue(tlp::SizeProperty *sizeProperty, unsigned int n, float w, float h, float d) {
  sizeProperty->setNodeValue(tlp::node(n), tlp::Size(w, h, d));
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_getEdgeDefaultValue(tlp::SizeProperty *sizeProperty, float *array) {
  const tlp::Size &s = sizeProperty->getEdgeDefaultValue();
  for (unsigned int i = 0; i < 3; ++i) {
    array[i] = s[i];
  }
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_getEdgeValue(tlp::SizeProperty *sizeProperty, unsigned int e, float *array) {
  const tlp::Size &s = sizeProperty->getEdgeValue(tlp::edge(e));
  for (unsigned int i = 0; i < 3; ++i) {
    array[i] = s[i];
  }
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_setAllEdgeValue(tlp::SizeProperty *sizeProperty, float w, float h, float d) {
  sizeProperty->setAllEdgeValue(tlp::Size(w, h, d));
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_setEdgeValue(tlp::SizeProperty *sizeProperty, unsigned int e, float w, float h, float d) {
  sizeProperty->setEdgeValue(tlp::edge(e), tlp::Size(w, h, d));
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_scale(tlp::SizeProperty *sizeProperty, float wf, float hf, float df, tlp::Graph *sg) {
  sizeProperty->scale(tlp::Vec3f(wf, hf, df), sg);
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_getMin(tlp::SizeProperty *sizeProperty, tlp::Graph *sg, float *min) {
  tlp::Size minSize = sizeProperty->getMin(sg);
  for (unsigned int i = 0; i < 3; ++i)
    min[i] = minSize[i];
}

void EMSCRIPTEN_KEEPALIVE SizeProperty_getMax(tlp::SizeProperty *sizeProperty, tlp::Graph *sg, float *max) {
  tlp::Size maxSize = sizeProperty->getMax(sg);
  for (unsigned int i = 0; i < 3; ++i)
    max[i] = maxSize[i];
}

// ==================================================================================================================

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE createIntegerProperty(tlp::Graph *graph, const char *name) {
  tlp::IntegerProperty *prop = new tlp::IntegerProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE IntegerProperty_setAllNodeValue(tlp::IntegerProperty *integerProperty, int val) {
  integerProperty->setAllNodeValue(val);
}

void EMSCRIPTEN_KEEPALIVE IntegerProperty_setNodeValue(tlp::IntegerProperty *integerProperty, unsigned int n, int val) {
  integerProperty->setNodeValue(tlp::node(n), val);
}

void EMSCRIPTEN_KEEPALIVE IntegerProperty_setAllEdgeValue(tlp::IntegerProperty *integerProperty, int val) {
  integerProperty->setAllEdgeValue(val);
}

void EMSCRIPTEN_KEEPALIVE IntegerProperty_setEdgeValue(tlp::IntegerProperty *integerProperty, unsigned int e, int val) {
  integerProperty->setEdgeValue(tlp::edge(e), val);
}

int EMSCRIPTEN_KEEPALIVE IntegerProperty_getNodeDefaultValue(tlp::IntegerProperty *integerProperty) {
  return integerProperty->getNodeDefaultValue();
}

int EMSCRIPTEN_KEEPALIVE IntegerProperty_getNodeValue(tlp::IntegerProperty *integerProperty, unsigned int n) {
  return integerProperty->getNodeValue(tlp::node(n));
}

int EMSCRIPTEN_KEEPALIVE IntegerProperty_getEdgeDefaultValue(tlp::IntegerProperty *integerProperty) {
  return integerProperty->getEdgeDefaultValue();
}

int EMSCRIPTEN_KEEPALIVE IntegerProperty_getEdgeValue(tlp::IntegerProperty *integerProperty, unsigned int e) {
  return integerProperty->getEdgeValue(tlp::edge(e));
}

// ==================================================================================================================

tlp::DoubleProperty *EMSCRIPTEN_KEEPALIVE createDoubleProperty(tlp::Graph *graph, const char *name) {
  tlp::DoubleProperty *prop = new tlp::DoubleProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE DoubleProperty_setAllNodeValue(tlp::DoubleProperty *doubleProperty, double val) {
  doubleProperty->setAllNodeValue(val);
}

void EMSCRIPTEN_KEEPALIVE DoubleProperty_setNodeValue(tlp::DoubleProperty *doubleProperty, unsigned int n, double val) {
  doubleProperty->setNodeValue(tlp::node(n), val);
}

void EMSCRIPTEN_KEEPALIVE DoubleProperty_setAllEdgeValue(tlp::DoubleProperty *doubleProperty, double val) {
  doubleProperty->setAllEdgeValue(val);
}

void EMSCRIPTEN_KEEPALIVE DoubleProperty_setEdgeValue(tlp::DoubleProperty *doubleProperty, unsigned int e, double val) {
  doubleProperty->setEdgeValue(tlp::edge(e), val);
}

double EMSCRIPTEN_KEEPALIVE DoubleProperty_getNodeDefaultValue(tlp::DoubleProperty *doubleProperty) {
  return doubleProperty->getNodeDefaultValue();
}

double EMSCRIPTEN_KEEPALIVE DoubleProperty_getNodeValue(tlp::DoubleProperty *doubleProperty, unsigned int n) {
  return doubleProperty->getNodeValue(tlp::node(n));
}

double EMSCRIPTEN_KEEPALIVE DoubleProperty_getEdgeDefaultValue(tlp::DoubleProperty *doubleProperty) {
  return doubleProperty->getEdgeDefaultValue();
}

double EMSCRIPTEN_KEEPALIVE DoubleProperty_getEdgeValue(tlp::DoubleProperty *doubleProperty, unsigned int e) {
  return doubleProperty->getEdgeValue(tlp::edge(e));
}

void EMSCRIPTEN_KEEPALIVE DoubleProperty_getSortedEdges(tlp::DoubleProperty *doubleProperty, tlp::Graph *sg, bool ascendingOrder,
                                                        unsigned int *edgesIds) {
  fillEdgesIds(doubleProperty->getSortedEdges(sg, ascendingOrder), edgesIds);
}

// ==================================================================================================================

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE createStringProperty(tlp::Graph *graph, const char *name) {
  tlp::StringProperty *prop = new tlp::StringProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE StringProperty_setAllNodeValue(tlp::StringProperty *stringProperty, const char *val) {
  stringProperty->setAllNodeValue(val);
}

void EMSCRIPTEN_KEEPALIVE StringProperty_setNodeValue(tlp::StringProperty *stringProperty, unsigned int n, const char *val) {
  stringProperty->setNodeValue(tlp::node(n), val);
}

void EMSCRIPTEN_KEEPALIVE StringProperty_setAllEdgeValue(tlp::StringProperty *stringProperty, const char *val) {
  stringProperty->setAllEdgeValue(val);
}

void EMSCRIPTEN_KEEPALIVE StringProperty_setEdgeValue(tlp::StringProperty *stringProperty, unsigned int e, const char *val) {
  stringProperty->setEdgeValue(tlp::edge(e), val);
}

const char *EMSCRIPTEN_KEEPALIVE StringProperty_getNodeDefaultValue(tlp::StringProperty *stringProperty) {
  static std::string s;
  s = stringProperty->getNodeDefaultValue();
  return s.c_str();
}

const char *EMSCRIPTEN_KEEPALIVE StringProperty_getNodeValue(tlp::StringProperty *stringProperty, unsigned int n) {
  static std::string s;
  s = stringProperty->getNodeValue(tlp::node(n));
  return s.c_str();
}

const char *EMSCRIPTEN_KEEPALIVE StringProperty_getEdgeDefaultValue(tlp::StringProperty *stringProperty) {
  static std::string s;
  s = stringProperty->getEdgeDefaultValue();
  return s.c_str();
}

const char *EMSCRIPTEN_KEEPALIVE StringProperty_getEdgeValue(tlp::StringProperty *stringProperty, unsigned int e) {
  static std::string s;
  s = stringProperty->getEdgeValue(tlp::edge(e));
  return s.c_str();
}

// ==================================================================================================================

tlp::StringVectorProperty *EMSCRIPTEN_KEEPALIVE createStringVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::StringVectorProperty *prop = new tlp::StringVectorProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_setAllNodeValue(tlp::StringVectorProperty *stringVectorProperty, unsigned char *stringsBytes,
                                                               unsigned int *stringsNbBytes, unsigned int nbStrings) {
  std::vector<std::string> vs;
  fillStringVectorFromStringsBytes(vs, stringsBytes, stringsNbBytes, nbStrings);
  stringVectorProperty->setAllNodeValue(vs);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_setNodeValue(tlp::StringVectorProperty *stringVectorProperty, unsigned int n,
                                                            unsigned char *stringsBytes, unsigned int *stringsNbBytes, unsigned int nbStrings) {
  std::vector<std::string> vs;
  fillStringVectorFromStringsBytes(vs, stringsBytes, stringsNbBytes, nbStrings);
  stringVectorProperty->setNodeValue(tlp::node(n), vs);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_setAllEdgeValue(tlp::StringVectorProperty *stringVectorProperty, unsigned char *stringsBytes,
                                                               unsigned int *stringsNbBytes, unsigned int nbStrings) {
  std::vector<std::string> vs;
  fillStringVectorFromStringsBytes(vs, stringsBytes, stringsNbBytes, nbStrings);
  stringVectorProperty->setAllEdgeValue(vs);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_setEdgeValue(tlp::StringVectorProperty *stringVectorProperty, unsigned int e,
                                                            unsigned char *stringsBytes, unsigned int *stringsNbBytes, unsigned int nbStrings) {
  std::vector<std::string> vs;
  fillStringVectorFromStringsBytes(vs, stringsBytes, stringsNbBytes, nbStrings);
  stringVectorProperty->setEdgeValue(tlp::edge(e), vs);
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getNodeDefaultVectorSize(tlp::StringVectorProperty *stringVectorProperty) {
  return stringVectorProperty->getNodeDefaultValue().size();
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getNodeDefaultStringsLengths(tlp::StringVectorProperty *stringVectorProperty, unsigned int *array) {
  return getStringsNumberOfBytesFromContainer(stringVectorProperty->getNodeDefaultValue(), array);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_getNodeDefaultValue(tlp::StringVectorProperty *stringVectorProperty, unsigned char *array) {
  fillBytesArrayFromStringsContainer(stringVectorProperty->getNodeDefaultValue(), array);
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getNodeVectorSize(tlp::StringVectorProperty *stringVectorProperty, unsigned int n) {
  return stringVectorProperty->getNodeValue(tlp::node(n)).size();
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getNodeStringsLengths(tlp::StringVectorProperty *stringVectorProperty, unsigned int n,
                                                                       unsigned int *array) {
  return getStringsNumberOfBytesFromContainer(stringVectorProperty->getNodeValue(tlp::node(n)), array);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_getNodeValue(tlp::StringVectorProperty *stringVectorProperty, unsigned int n, unsigned char *array) {
  fillBytesArrayFromStringsContainer(stringVectorProperty->getNodeValue(tlp::node(n)), array);
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getEdgeDefaultVectorSize(tlp::StringVectorProperty *stringVectorProperty) {
  return stringVectorProperty->getEdgeDefaultValue().size();
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getEdgeDefaultStringsLengths(tlp::StringVectorProperty *stringVectorProperty, unsigned int *array) {
  return getStringsNumberOfBytesFromContainer(stringVectorProperty->getEdgeDefaultValue(), array);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_getEdgeDefaultValue(tlp::StringVectorProperty *stringVectorProperty, unsigned char *array) {
  fillBytesArrayFromStringsContainer(stringVectorProperty->getEdgeDefaultValue(), array);
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getEdgeVectorSize(tlp::StringVectorProperty *stringVectorProperty, unsigned int e) {
  return stringVectorProperty->getEdgeValue(tlp::edge(e)).size();
}

double EMSCRIPTEN_KEEPALIVE StringVectorProperty_getEdgeStringsLengths(tlp::StringVectorProperty *stringVectorProperty, unsigned int e,
                                                                       unsigned int *array) {
  return getStringsNumberOfBytesFromContainer(stringVectorProperty->getEdgeValue(tlp::edge(e)), array);
}

void EMSCRIPTEN_KEEPALIVE StringVectorProperty_getEdgeValue(tlp::StringVectorProperty *stringVectorProperty, unsigned int e, unsigned char *array) {
  fillBytesArrayFromStringsContainer(stringVectorProperty->getEdgeValue(tlp::edge(e)), array);
}

// ==================================================================================================================

tlp::DoubleVectorProperty *EMSCRIPTEN_KEEPALIVE createDoubleVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::DoubleVectorProperty *prop = new tlp::DoubleVectorProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_setAllNodeValue(tlp::DoubleVectorProperty *doubleVectorProperty, double *array,
                                                               unsigned int arraySize) {
  std::vector<double> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  doubleVectorProperty->setAllNodeValue(v);
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_setNodeValue(tlp::DoubleVectorProperty *doubleVectorProperty, unsigned int n, double *array,
                                                            unsigned int arraySize) {
  std::vector<double> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  doubleVectorProperty->setNodeValue(tlp::node(n), v);
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_setAllEdgeValue(tlp::DoubleVectorProperty *doubleVectorProperty, double *array,
                                                               unsigned int arraySize) {
  std::vector<double> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  doubleVectorProperty->setAllEdgeValue(v);
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_setEdgeValue(tlp::DoubleVectorProperty *doubleVectorProperty, unsigned int e, double *array,
                                                            unsigned int arraySize) {
  std::vector<double> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  doubleVectorProperty->setEdgeValue(tlp::edge(e), v);
}

double EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getNodeDefaultVectorSize(tlp::DoubleVectorProperty *doubleVectorProperty) {
  const std::vector<double> &value = doubleVectorProperty->getNodeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getNodeDefaultValue(tlp::DoubleVectorProperty *doubleVectorProperty, double *array) {
  const std::vector<double> &value = doubleVectorProperty->getNodeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

double EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getNodeVectorSize(tlp::DoubleVectorProperty *doubleVectorProperty, unsigned int n) {
  const std::vector<double> &value = doubleVectorProperty->getNodeValue(tlp::node(n));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getNodeValue(tlp::DoubleVectorProperty *doubleVectorProperty, unsigned int n, double *array) {
  const std::vector<double> &value = doubleVectorProperty->getNodeValue(tlp::node(n));
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

unsigned EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getEdgeDefaultVectorSize(tlp::DoubleVectorProperty *doubleVectorProperty) {
  const std::vector<double> &value = doubleVectorProperty->getEdgeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getEdgeDefaultValue(tlp::DoubleVectorProperty *doubleVectorProperty, double *array) {
  const std::vector<double> &value = doubleVectorProperty->getEdgeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

double EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getEdgeVectorSize(tlp::DoubleVectorProperty *doubleVectorProperty, unsigned int e) {
  const std::vector<double> &value = doubleVectorProperty->getEdgeValue(tlp::edge(e));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE DoubleVectorProperty_getEdgeValue(tlp::DoubleVectorProperty *doubleVectorProperty, unsigned int e, double *array) {
  const std::vector<double> &value = doubleVectorProperty->getEdgeValue(tlp::edge(e));
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

// ==================================================================================================================

tlp::IntegerVectorProperty *EMSCRIPTEN_KEEPALIVE createIntegerVectorProperty(tlp::Graph *graph, const char *name) {
  tlp::IntegerVectorProperty *prop = new tlp::IntegerVectorProperty(graph, name);
  observeObject(prop);
  return prop;
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_setAllNodeValue(tlp::IntegerVectorProperty *intVectorProperty, int *array, unsigned int arraySize) {
  std::vector<int> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  intVectorProperty->setAllNodeValue(v);
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_setNodeValue(tlp::IntegerVectorProperty *intVectorProperty, unsigned int n, int *array,
                                                             unsigned int arraySize) {
  std::vector<int> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }

  intVectorProperty->setNodeValue(tlp::node(n), v);
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_setAllEdgeValue(tlp::IntegerVectorProperty *intVectorProperty, int *array, unsigned int arraySize) {
  std::vector<int> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  intVectorProperty->setAllEdgeValue(v);
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_setEdgeValue(tlp::IntegerVectorProperty *intVectorProperty, unsigned int e, int *array,
                                                             unsigned int arraySize) {
  std::vector<int> v(arraySize);
  for (unsigned int i = 0; i < arraySize; ++i) {
    v[i] = array[i];
  }
  intVectorProperty->setEdgeValue(tlp::edge(e), v);
}

double EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getNodeDefaultVectorSize(tlp::IntegerVectorProperty *intVectorProperty) {
  const std::vector<int> &value = intVectorProperty->getNodeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getNodeDefaultValue(tlp::IntegerVectorProperty *intVectorProperty, int *array) {
  const std::vector<int> &value = intVectorProperty->getNodeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

double EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getNodeVectorSize(tlp::IntegerVectorProperty *intVectorProperty, unsigned int n) {
  const std::vector<int> &value = intVectorProperty->getNodeValue(tlp::node(n));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getNodeValue(tlp::IntegerVectorProperty *intVectorProperty, unsigned int n, int *array) {
  const std::vector<int> &value = intVectorProperty->getNodeValue(tlp::node(n));
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

unsigned EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getEdgeDefaultVectorSize(tlp::IntegerVectorProperty *intVectorProperty) {
  const std::vector<int> &value = intVectorProperty->getEdgeDefaultValue();
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getEdgeDefaultValue(tlp::IntegerVectorProperty *intVectorProperty, int *array) {
  const std::vector<int> &value = intVectorProperty->getEdgeDefaultValue();
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

double EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getEdgeVectorSize(tlp::IntegerVectorProperty *intVectorProperty, unsigned int e) {
  const std::vector<int> &value = intVectorProperty->getEdgeValue(tlp::edge(e));
  return value.size();
}

void EMSCRIPTEN_KEEPALIVE IntegerVectorProperty_getEdgeValue(tlp::IntegerVectorProperty *intVectorProperty, unsigned int e, int *array) {
  const std::vector<int> &value = intVectorProperty->getEdgeValue(tlp::edge(e));
  for (size_t i = 0; i < value.size(); ++i) {
    *array++ = value[i];
  }
}

// ========================================================================================================================================================

tlp::ColorScale *EMSCRIPTEN_KEEPALIVE ColorScale_newColorScale() {
  return new tlp::ColorScale();
}

void EMSCRIPTEN_KEEPALIVE ColorScale_setColorAtPos(tlp::ColorScale *colorScale, float pos, unsigned char r, unsigned char g, unsigned char b,
                                                   unsigned char a) {
  colorScale->setColorAtPos(pos, tlp::Color(r, g, b, a));
}

void EMSCRIPTEN_KEEPALIVE ColorScale_setColorScale(tlp::ColorScale *colorScale, unsigned char *array, unsigned int nbColors) {
  std::vector<tlp::Color> colors(nbColors);
  for (unsigned int i = 0; i < nbColors; ++i) {
    colors[i] = tlp::Color(array[4 * i], array[4 * i + 1], array[4 * i + 2], array[4 * i + 3]);
  }
  colorScale->setColorScale(colors);
}

void EMSCRIPTEN_KEEPALIVE ColorScale_getColorAtPos(tlp::ColorScale *colorScale, float pos, unsigned char *array) {
  tlp::Color color = colorScale->getColorAtPos(pos);
  for (unsigned int i = 0; i < 4; ++i) {
    array[i] = color[i];
  }
}

double EMSCRIPTEN_KEEPALIVE ColorScale_numberOfColors(tlp::ColorScale *colorScale) {
  auto colorMap = colorScale->getColorMap();
  return colorMap.size();
}

void EMSCRIPTEN_KEEPALIVE ColorScale_getOffsets(tlp::ColorScale *colorScale, float *array) {
  auto colorMap = colorScale->getColorMap();
  for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {
    *array++ = it->first;
  }
}

void EMSCRIPTEN_KEEPALIVE ColorScale_getColors(tlp::ColorScale *colorScale, unsigned char *array) {
  auto colorMap = colorScale->getColorMap();
  unsigned int i = 0;
  for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {
    for (unsigned int j = 0; j < 4; ++j) {
      array[4 * i + j] = it->second[j];
    }
    ++i;
  }
}
}
