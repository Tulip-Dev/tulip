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

#include <sstream>
#include <time.h>

#include <tulip/ForEach.h>
#include <tulip/ExportModule.h>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>
#include <tulip/MutableContainer.h>
#include <tulip/TlpTools.h>
#include <tulip/YajlFacade.h>
#include <tulip/JsonTokens.h>

using namespace std;
using namespace tlp;

/**
 * @brief Helper class to iterate over the new IDs of nodes and edges from the old IDs.
 *
 * This class takes an iterator over the nodes or edges of a graph, and the MutableContainer used to map from old ID to new ID.
 * It iterates over the nodes/edges and returns the new IDs.
 **/
template<typename TYPE>
class NewValueIterator : public tlp::Iterator<unsigned int> {
public:
  NewValueIterator(Iterator<TYPE>* iterator, const MutableContainer<unsigned int>& newValues) : _iterator(iterator), _newValues(&newValues) {
  }

  ~NewValueIterator() {
    delete _iterator;
  }

  virtual unsigned int next() {
    return _newValues->get(_iterator->next().id);
  }

  virtual bool hasNext() {
    return _iterator->hasNext();
  }

private:
  tlp::Iterator<TYPE>* _iterator;
  const MutableContainer<unsigned int>* _newValues;
};

/**
 * @brief Exports a Tulip Graph to a JSON format.
 *
 * In order to maintain full capabilities of exporting to older format versions, the export of the data is decomposed in two parts:
 * * The metadata
 * * The Graph Hierarchy
 *
 * The metadata is exported by saveMetaData() and the graph hierarchy saved (recursively) by saveGraph().
 *
 * These functions are suffixed by the format version they export to (e.g. saveGraph_V4() as of version 4.0 of the format).
 * Under no circumstances should these functions be modified for anything besides a simple bugfix.
 *
 * Any feature addition should be done by writing a new version of saveMetaData and saveGraph, and switching on the version number in the main function.
 *
 **/
class TlpJsonExport : public ExportModule {
public:
  PLUGININFORMATION("JSON Export","Charles Huet","18/05/2011","Tulip JSON format","1.0","File")

  std::string fileExtension() const {
    return "json";
  }

  std::string icon() const {
    return ":/tulip/gui/icons/json32x32.png";
  }

  /**
   * @brief Mandatory constructor to initialize the AlgorithmContext.
   *
   * @param context The context this export algorithm will be initialized with.
   **/
  TlpJsonExport(tlp::PluginContext* context) : ExportModule(context) {
    addInParameter<bool>("Beautify JSON string", "If true, generate a JSON string with indentation and line breaks.", "false");
  }

  virtual bool exportGraph(ostream& fileOut) {

    if (dataSet && dataSet->exist("Beautify JSON string")) {
      bool beautify = false;
      dataSet->get("Beautify JSON string", beautify);
      _writer.beautifyString(beautify);
    }

    //initialization of the maps from old ID to new ID here, before entering saveGraph (as it is recursive).
    node n;
    int i = 0;
    // the export only works for the root graph
    Graph *superGraph = graph->getSuperGraph();
    graph->setSuperGraph(graph);
    forEach(n, graph->getNodes()) {
      _newNodeId.set(n.id, i++);
    }

    _writer.writeMapOpen(); //top-level map

    _writer.writeString("version");
    _writer.writeString("4.0");

    saveMetaData_V4();

    _writer.writeString(GraphToken);
    _writer.writeMapOpen(); //graph hierarchy map
    saveGraph_V4(graph);
    _writer.writeMapClose(); // graph hierarchy map

    _writer.writeMapClose(); // top-level map

    fileOut << _writer.generatedString();

    graph->setSuperGraph(superGraph);

    return true;
  }

  /**
   * @brief Saves the metadata of the graph, such as date and comment.
   * This version does not save anything else.
   *
   * @return void
   **/
  void saveMetaData_V4() {
    time_t ostime = time(NULL);
    // get local time
    struct tm *currTime = localtime(&ostime);
    // format date
    char currDate[32];
    strftime(currDate, 32, "%Y-%m-%d", currTime);
    _writer.writeString("date");
    _writer.writeString(currDate);

    std::string comment;
    dataSet->get<string>("comment", comment);
    _writer.writeString("comment");
    _writer.writeString(comment);
  }

  /**
   * @brief Saves the graph recursively.
   *
   * @param graph The graph to save.
   * @return void
   **/
  void saveGraph_V4(Graph* graph) {
    node n;
    edge e;

    _writer.writeString(GraphIDToken);

    if (graph->getSuperGraph() == graph) {
      _writer.writeInteger(0);
    }
    else {
      _writer.writeInteger(graph->getId());
    }

    //we need to save all nodes and edges on the root graph
    if(graph->getSuperGraph() == graph) {
      //saving nodes only requires knowing how many of them there are
      _writer.writeString(NodesNumberToken);
      _writer.writeInteger(graph->numberOfNodes());
      //saving the number of edges will speed up the import phase
      //because the space needed to store the edges will be
      //allocated in one call
      _writer.writeString(EdgesNumberToken);
      _writer.writeInteger(graph->numberOfEdges());
      //saving edges requires writing source and target for every edge
      _writer.writeString(EdgesToken);
      _writer.writeArrayOpen();
      unsigned int i = 0;
      forEach(e, graph->getEdges()) {
        _newEdgeId.set(e.id, i++);

        unsigned int source = _newNodeId.get(graph->source(e).id);
        unsigned int target = _newNodeId.get(graph->target(e).id);

        _writer.writeArrayOpen();
        _writer.writeInteger(source);
        _writer.writeInteger(target);
        _writer.writeArrayClose();
      }
      _writer.writeArrayClose();
    }
    else {
      //only saving relevant nodes and edges
      writeInterval(NodesIDsToken, new NewValueIterator<tlp::node>(graph->getNodes(), _newNodeId));
      writeInterval(EdgesIDsToken, new NewValueIterator<tlp::edge>(graph->getEdges(), _newEdgeId));
    }

    _writer.writeString(PropertiesToken);
    _writer.writeMapOpen();
    //saving properties
    Iterator<PropertyInterface*> *itP = NULL;

    if (graph->getSuperGraph() == graph) {
      itP = graph->getObjectProperties();
    }
    else {
      itP = graph->getLocalObjectProperties();
    }

    PropertyInterface* property;
    forEach(property, itP) {
      _writer.writeString(property->getName());
      _writer.writeMapOpen();

      _writer.writeString(TypeToken);
      _writer.writeString(property->getTypename());

      _writer.writeString(NodeDefaultToken);
      bool writingPathViewProperty =
        (property->getName() == string("viewFont") ||
         property->getName() == string("viewTexture"));

      string dsValue = property->getNodeDefaultStringValue();

      if (writingPathViewProperty && !TulipBitmapDir.empty()) {
        size_t pos = dsValue.find(TulipBitmapDir);

        if(pos != string::npos)
          dsValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
      }

      _writer.writeString(dsValue);

      _writer.writeString(EdgeDefaultToken);
      dsValue = property->getEdgeDefaultStringValue();

      if (writingPathViewProperty && !TulipBitmapDir.empty()) {
        size_t pos = dsValue.find(TulipBitmapDir);

        if(pos != string::npos)
          dsValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
      }

      _writer.writeString(dsValue);

      if(property->numberOfNonDefaultValuatedNodes() > 0) {
        _writer.writeString(NodesValuesToken);
        _writer.writeMapOpen();
        forEach(n, property->getNonDefaultValuatedNodes(graph)) {
          stringstream temp;
          temp << _newNodeId.get(n.id);
          _writer.writeString(temp.str());
          string sValue = property->getNodeStringValue(n);

          if (writingPathViewProperty && !TulipBitmapDir.empty()) {
            size_t pos = sValue.find(TulipBitmapDir);

            if(pos != string::npos)
              sValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
          }

          _writer.writeString(sValue);
        }
        _writer.writeMapClose();
      }

      if(property->numberOfNonDefaultValuatedEdges() > 0) {
        _writer.writeString(EdgesValuesToken);
        _writer.writeMapOpen();
        forEach(e, property->getNonDefaultValuatedEdges(graph)) {
          stringstream temp;
          temp << _newEdgeId.get(e.id);
          _writer.writeString(temp.str());
          string sValue = property->getEdgeStringValue(e);

          if (writingPathViewProperty && !TulipBitmapDir.empty()) {
            size_t pos = sValue.find(TulipBitmapDir);

            if(pos != string::npos)
              sValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
          }

          _writer.writeString(sValue);
        }
        _writer.writeMapClose();
      }

      _writer.writeMapClose();
    }
    _writer.writeMapClose();

    _writer.writeString(AttributesToken);
    _writer.writeMapOpen();
    //saving attributes
    DataSet attributes = graph->getAttributes();
    pair<string, DataType*> attribute;
    forEach(attribute, attributes.getValues()) {
      // If nodes and edges are stored as graph attributes
      // we need to update their id before serializing them
      // as nodes and edges have been reindexed
      if (attribute.second->getTypeName() == string(typeid(node).name())) {
        node *n = reinterpret_cast<node*>(attribute.second->value);
        n->id = _newNodeId.get(n->id);
      }
      else if (attribute.second->getTypeName() == string(typeid(edge).name())) {
        edge *e = reinterpret_cast<edge*>(attribute.second->value);
        e->id = _newEdgeId.get(e->id);
      }
      else if (attribute.second->getTypeName() == string(typeid(vector<node>).name())) {
        vector<node> *vn = reinterpret_cast<vector<node>*>(attribute.second->value);

        for (size_t i = 0 ; i < vn->size() ; ++i) {
          (*vn)[i].id = _newNodeId.get((*vn)[i].id);
        }
      }
      else if (attribute.second->getTypeName() == string(typeid(vector<edge>).name())) {
        vector<edge> *ve = reinterpret_cast<vector<edge>*>(attribute.second->value);

        for (size_t i = 0 ; i < ve->size() ; ++i) {
          (*ve)[i].id = _newEdgeId.get((*ve)[i].id);
        }
      }

      DataTypeSerializer* serializer = DataSet::typenameToSerializer(attribute.second->getTypeName());
      _writer.writeString(attribute.first);
      _writer.writeArrayOpen();
      _writer.writeString(serializer->outputTypeName);

      stringstream temp;
      serializer->writeData(temp, attribute.second);
      _writer.writeString(temp.str());
      _writer.writeArrayClose();
    }
    _writer.writeMapClose();

    //saving subgraphs
    _writer.writeString(SubgraphsToken);
    _writer.writeArrayOpen();
    Graph* sub;
    forEach(sub, graph->getSubGraphs()) {
      _writer.writeMapOpen();
      saveGraph_V4(sub);
      _writer.writeMapClose();
    }
    _writer.writeArrayClose();
  }

  /**
   * @brief Writes a set of identifiers as contiguous intervals (defined by arrays containing lower and higher bounds).
   * e.g. the set {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 15, 17} will be saved as the array:
   * [ [0, 7], [9, 11], 15, 17]
   *
   * @param intervalName The name of the interval.
   * @param iterator An iterator over the values to save.
   * @return void
   **/
  void writeInterval(const std::string& intervalName, Iterator<unsigned int>* iterator) {
    _writer.writeString(intervalName);
    _writer.writeArrayOpen();
    unsigned int intervalBegin = UINT_MAX;
    unsigned int intervalEnd = UINT_MAX;
    unsigned int previousId = UINT_MAX;
    unsigned int currentId = UINT_MAX;
    unsigned int nbIdsIterated = 0;
    forEach(currentId, iterator) {
      //we don't need/want to do all this on the first time we loop
      if(previousId != UINT_MAX) {

        //if the ID are continuous, define an interval, otherwise write the IDs (either intervals or single)
        if(currentId == previousId + 1) {
          //if we have no interval being defined, set the lower bound to the previous edge ID
          //if an interval is being defined, set its end to the current edge ID
          if(intervalBegin == UINT_MAX) {
            intervalBegin = previousId;
          }

          intervalEnd = currentId;
        }
        else {
          //if an interval is defined, write it
          if(intervalBegin != UINT_MAX) {
            _writer.writeArrayOpen();
            _writer.writeInteger(intervalBegin);
            _writer.writeInteger(intervalEnd);
            _writer.writeArrayClose();
            intervalBegin = UINT_MAX;
            intervalEnd = UINT_MAX;
          }
          else {
            _writer.writeInteger(previousId);
          }
        }

        if(!_it_foreach._it->hasNext()) {
          if(intervalBegin != UINT_MAX) {
            _writer.writeArrayOpen();
            _writer.writeInteger(intervalBegin);
            _writer.writeInteger(intervalEnd);
            _writer.writeArrayClose();
          }
          else {
            _writer.writeInteger(currentId);
          }
        }
      }

      previousId = currentId;
      ++nbIdsIterated;
    }

    // handle the case where there is only one id to write
    if (nbIdsIterated == 1) {
      _writer.writeInteger(currentId);
    }

    _writer.writeArrayClose();
  }

protected:
  YajlWriteFacade _writer;
  MutableContainer<unsigned int> _newNodeId;
  MutableContainer<unsigned int> _newEdgeId;
};

PLUGIN(TlpJsonExport)
