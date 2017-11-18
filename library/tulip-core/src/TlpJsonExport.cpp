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

#ifdef _WIN32
#include <time.h>
#endif

#include <sstream>

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
 * @brief Exports a Tulip Graph to a JSON format.
 *
 * In order to maintain full capabilities of exporting to older format versions, the export of the
 *data is decomposed in two parts:
 * * The metadata
 * * The Graph Hierarchy
 *
 * The metadata is exported by saveMetaData() and the graph hierarchy saved (recursively) by
 *saveGraph().
 *
 * These functions are suffixed by the format version they export to (e.g. saveGraph_V4() as of
 *version 4.0 of the format).
 * Under no circumstances should these functions be modified for anything besides a simple bugfix.
 *
 * Any feature addition should be done by writing a new version of saveMetaData and saveGraph, and
 *switching on the version number in the main function.
 *
 **/
class TlpJsonExport : public ExportModule {
public:
  PLUGININFORMATION("JSON Export", "Charles Huet", "18/05/2011",
                    "<p>Supported extensions: json</p><p>Exports a graph in a file using the Tulip "
                    "JSON format.</p>",
                    "1.0", "File")

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
  TlpJsonExport(tlp::PluginContext *context) : ExportModule(context) {
    addInParameter<bool>("Beautify JSON string",
                         "If true, generate a JSON string with indentation and line breaks.",
                         "false");
  }

  virtual bool exportGraph(ostream &fileOut) {

    if (dataSet && dataSet->exist("Beautify JSON string")) {
      bool beautify = false;
      dataSet->get("Beautify JSON string", beautify);
      _writer.beautifyString(beautify);
    }

    // the export only works for the root graph
    Graph *superGraph = graph->getSuperGraph();
    graph->setSuperGraph(graph);

    _writer.writeMapOpen(); // top-level map

    _writer.writeString("version");
    _writer.writeString("4.0");

    saveMetaData_V4();

    _writer.writeString(GraphToken);
    _writer.writeMapOpen(); // graph hierarchy map
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
  void saveGraph_V4(Graph *g) {
    node n;
    edge e;

    _writer.writeString(GraphIDToken);

    if (g->getSuperGraph() == g) {
      _writer.writeInteger(0);
    } else {
      _writer.writeInteger(g->getId());
    }

    // we need to save all nodes and edges on the root graph
    if (g->getSuperGraph() == g) {
      // saving nodes only requires knowing how many of them there are
      _writer.writeString(NodesNumberToken);
      _writer.writeInteger(g->numberOfNodes());
      // saving the number of edges will speed up the import phase
      // because the space needed to store the edges will be
      // allocated in one call
      _writer.writeString(EdgesNumberToken);
      const std::vector<edge> &edges = g->edges();
      unsigned int nbEdges = edges.size();
      _writer.writeInteger(nbEdges);
      // saving edges requires writing source and target for every edge
      _writer.writeString(EdgesToken);
      _writer.writeArrayOpen();

      for (unsigned int i = 0; i < nbEdges; ++i) {
        std::pair<node, node> ends = g->ends(edges[i]);
        _writer.writeArrayOpen();
        _writer.writeInteger(graph->nodePos(ends.first));
        _writer.writeInteger(graph->nodePos(ends.second));
        _writer.writeArrayClose();
      }

      _writer.writeArrayClose();
    } else {
      // only saving relevant nodes and edges
      const std::vector<node> &nodes = g->nodes();
      unsigned int nbElts = nodes.size();
      std::vector<unsigned int> pos(nbElts);

      for (unsigned int i = 0; i < nbElts; ++i)
        pos[i] = graph->nodePos(nodes[i]);

      std::sort(pos.begin(), pos.end());
      writeInterval(NodesIDsToken, pos);

      const std::vector<edge> &edges = g->edges();
      pos.resize(nbElts = edges.size());

      for (unsigned int i = 0; i < nbElts; ++i)
        pos[i] = graph->edgePos(edges[i]);

      std::sort(pos.begin(), pos.end());
      writeInterval(EdgesIDsToken, pos);
    }

    _writer.writeString(PropertiesToken);
    _writer.writeMapOpen();
    // saving properties
    Iterator<PropertyInterface *> *itP = NULL;

    if (g->getSuperGraph() == g) {
      itP = g->getObjectProperties();
    } else {
      itP = g->getLocalObjectProperties();
    }

    PropertyInterface *property;
    forEach(property, itP) {
      _writer.writeString(property->getName());
      _writer.writeMapOpen();

      _writer.writeString(TypeToken);
      _writer.writeString(property->getTypename());

      _writer.writeString(NodeDefaultToken);
      bool writingPathViewProperty = (property->getName() == string("viewFont") ||
                                      property->getName() == string("viewTexture"));

      string dsValue = property->getNodeDefaultStringValue();

      if (writingPathViewProperty && !TulipBitmapDir.empty()) {
        size_t pos = dsValue.find(TulipBitmapDir);

        if (pos != string::npos)
          dsValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
      }

      _writer.writeString(dsValue);

      _writer.writeString(EdgeDefaultToken);
      dsValue = property->getEdgeDefaultStringValue();

      if (writingPathViewProperty && !TulipBitmapDir.empty()) {
        size_t pos = dsValue.find(TulipBitmapDir);

        if (pos != string::npos)
          dsValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
      }

      _writer.writeString(dsValue);

      if (property->numberOfNonDefaultValuatedNodes() > 0) {
        _writer.writeString(NodesValuesToken);
        _writer.writeMapOpen();
        forEach(n, property->getNonDefaultValuatedNodes(g)) {
          stringstream temp;
          temp << graph->nodePos(n);
          _writer.writeString(temp.str());
          string sValue = property->getNodeStringValue(n);

          if (writingPathViewProperty && !TulipBitmapDir.empty()) {
            size_t pos = sValue.find(TulipBitmapDir);

            if (pos != string::npos)
              sValue.replace(pos, TulipBitmapDir.size(), "TulipBitmapDir/");
          }

          _writer.writeString(sValue);
        }
        _writer.writeMapClose();
      }

      if (property->numberOfNonDefaultValuatedEdges() > 0) {
        _writer.writeString(EdgesValuesToken);
        _writer.writeMapOpen();
        forEach(e, property->getNonDefaultValuatedEdges(g)) {
          stringstream temp;
          temp << graph->edgePos(e);
          _writer.writeString(temp.str());
          string sValue = property->getEdgeStringValue(e);

          if (writingPathViewProperty && !TulipBitmapDir.empty()) {
            size_t pos = sValue.find(TulipBitmapDir);

            if (pos != string::npos)
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
    // saving attributes
    DataSet attributes = g->getAttributes();
    pair<string, DataType *> attribute;
    forEach(attribute, attributes.getValues()) {
      // If nodes and edges are stored as graph attributes
      // we need to update their id before serializing them
      // as nodes and edges have been reindexed
      if (attribute.second->getTypeName() == string(typeid(node).name())) {
        node *n = static_cast<node *>(attribute.second->value);
        n->id = graph->nodePos(*n);
      } else if (attribute.second->getTypeName() == string(typeid(edge).name())) {
        edge *e = static_cast<edge *>(attribute.second->value);
        e->id = g->edgePos(*e);
      } else if (attribute.second->getTypeName() == string(typeid(vector<node>).name())) {
        vector<node> *vn = static_cast<vector<node> *>(attribute.second->value);

        for (size_t i = 0; i < vn->size(); ++i) {
          (*vn)[i].id = graph->nodePos((*vn)[i]);
        }
      } else if (attribute.second->getTypeName() == string(typeid(vector<edge>).name())) {
        vector<edge> *ve = static_cast<vector<edge> *>(attribute.second->value);

        for (size_t i = 0; i < ve->size(); ++i) {
          (*ve)[i].id = graph->edgePos((*ve)[i]);
        }
      }

      DataTypeSerializer *serializer =
          DataSet::typenameToSerializer(attribute.second->getTypeName());
      _writer.writeString(attribute.first);
      _writer.writeArrayOpen();
      _writer.writeString(serializer->outputTypeName);

      stringstream temp;
      serializer->writeData(temp, attribute.second);
      _writer.writeString(temp.str());
      _writer.writeArrayClose();
    }
    _writer.writeMapClose();

    // saving subgraphs
    _writer.writeString(SubgraphsToken);
    _writer.writeArrayOpen();
    Graph *sub;
    forEach(sub, g->getSubGraphs()) {
      _writer.writeMapOpen();
      saveGraph_V4(sub);
      _writer.writeMapClose();
    }
    _writer.writeArrayClose();
  }

  /**
   * @brief Writes a set of identifiers as contiguous intervals (defined by arrays containing lower
   *and higher bounds).
   * e.g. the set {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 15, 17} will be saved as the array:
   * [ [0, 7], [9, 11], 15, 17]
   *
   * @param intervalName The name of the interval.
   * @param iterator An iterator over the values to save.
   * @return void
   **/
  void writeInterval(const std::string &intervalName, std::vector<unsigned int> &pos) {
    _writer.writeString(intervalName);
    _writer.writeArrayOpen();
    unsigned int intervalBegin = UINT_MAX;
    unsigned int intervalEnd = UINT_MAX;
    unsigned int previousId = UINT_MAX;
    unsigned int currentId = UINT_MAX;
    unsigned int nbElts = pos.size();

    for (unsigned int i = 0; i < nbElts; ++i) {
      currentId = pos[i];

      // we don't need/want to do all this on the first time we loop
      if (previousId != UINT_MAX) {

        // if the ID are continuous, define an interval, otherwise write the IDs (either intervals
        // or single)
        if (currentId == previousId + 1) {
          // if we have no interval being defined, set the lower bound to the previous edge ID
          // if an interval is being defined, set its end to the current edge ID
          if (intervalBegin == UINT_MAX) {
            intervalBegin = previousId;
          }

          intervalEnd = currentId;
        } else {
          // if an interval is defined, write it
          if (intervalBegin != UINT_MAX) {
            _writer.writeArrayOpen();
            _writer.writeInteger(intervalBegin);
            _writer.writeInteger(intervalEnd);
            _writer.writeArrayClose();
            intervalBegin = UINT_MAX;
            intervalEnd = UINT_MAX;
          } else {
            _writer.writeInteger(previousId);
          }
        }

        if (i == (nbElts - 1)) {
          if (intervalBegin != UINT_MAX) {
            _writer.writeArrayOpen();
            _writer.writeInteger(intervalBegin);
            _writer.writeInteger(intervalEnd);
            _writer.writeArrayClose();
          } else {
            _writer.writeInteger(currentId);
          }
        }
      }

      previousId = currentId;
    }

    // handle the case where there is only one id to write
    if (nbElts == 1) {
      _writer.writeInteger(currentId);
    }

    _writer.writeArrayClose();
  }

protected:
  YajlWriteFacade _writer;
};

PLUGIN(TlpJsonExport)
