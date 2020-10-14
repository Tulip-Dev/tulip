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
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

#include <tulip/ImportModule.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpQtTools.h>
#include <tulip/StableIterator.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>

#include <QXmlStreamReader>
#include <QFile>

using namespace tlp;
using namespace std;

/** \file
 *  \brief - Import GEXF format graph file.
 * This plugin imports a graph from a file in GEXF format,<br/>
 * as it is described in the XML Schema 1.2draft (http://gexf.net/format/schema.html). Dynamic mode
 * is not yet supported.
 *  <b>HISTORY</b>
 *
 *  - 04/06/2012 Version 1.0: Initial release
 *
 *  \author Antoine Lambert & Patrick Mary of Tulip Team http://tulip.labri.fr/
 *
 *
 */

static const char *paramHelp[] = {
    // filename
    "This parameter defines the pathname of the GEXF file to import.",

    // curved edges
    "Indicates if Bézier curves should be used to draw the edges."};

class GEXFImport : public ImportModule {

public:
  PLUGININFORMATION("GEXF", "Antoine LAMBERT", "05/05/2010",
                    "<p>Supported extensions: gexf</p><p>Imports a new graph from a file in the "
                    "GEXF input format<br/>as it is described in the XML Schema 1.2 draft<br/>"
                    "(<a "
                    "href=\"http://gexf.net/format/schema.html\">http://gexf.net/format/"
                    "schema.html</a>).</p><p>Warning: dynamic mode is not supported.</p>",
                    "1.0", "File")
  GEXFImport(const PluginContext *context)
      : ImportModule(context), viewLayout(nullptr), viewSize(nullptr), viewColor(nullptr),
        viewLabel(nullptr), viewShape(nullptr), nodesHaveCoordinates(false) {
    // add a file parameter for the plugin
    addInParameter<string>("file::filename", paramHelp[0], "");
    addInParameter<bool>("Curved edges", paramHelp[1], "false");
  }

  ~GEXFImport() override {}

  std::string icon() const override {
    return ":/tulip/graphperspective/icons/32/import_gephi.png";
  }

  std::list<std::string> fileExtensions() const override {
    std::list<std::string> l;
    l.push_back("gexf");
    return l;
  }

  // import plugins must implement bool importGraph()
  bool importGraph() override {
    string filename;
    bool curvedEdges = false;
    // get the filename chosen by the user
    dataSet->get<string>("file::filename", filename);
    dataSet->get<bool>("Curved edges", curvedEdges);

    QString qfilename = tlpStringToQString(filename);

    // if wrong extension, abort
    if (!qfilename.endsWith(".gexf")) {
      return false;
    }

    // get Tulip visual attributes properties associated to the empty graph we want to fill
    viewLayout = graph->getProperty<LayoutProperty>("viewLayout");
    viewLabel = graph->getProperty<StringProperty>("viewLabel");
    viewSize = graph->getProperty<SizeProperty>("viewSize");
    viewColor = graph->getProperty<ColorProperty>("viewColor");
    viewShape = graph->getProperty<IntegerProperty>("viewShape");

    nodesHaveCoordinates = false;

    // Open the GEXF file choosed by the user
    QFile xmlFile(qfilename);

    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      // get error
      pluginProgress->setError(QStringToTlpString(xmlFile.errorString()));
      return false;
    }

    // Instantiate a QXmlStreamReader to parse the file (GEXF is xml)
    QXmlStreamReader xmlReader(&xmlFile);

    // Parse each line of the file
    while (!xmlReader.atEnd()) {
      if (xmlReader.readNextStartElement()) {
        // only static graph are supported
        if (xmlReader.name() == "graph") {
          string mode = QStringToTlpString(xmlReader.attributes().value("mode").toString());

          if (mode == "dynamic") {
            pluginProgress->setError("dynamic graph is not yet supported");
            return false;
          }
        }
        // create Tulip Properties from Gephi attributes
        else if (xmlReader.name() == "attributes") {
          createPropertiesFromAttributes(xmlReader);
        }
        // parse graph node data
        else if (xmlReader.name() == "nodes") {
          createNodes(xmlReader, graph);
        }
        // parse graph edge data
        else if (xmlReader.name() == "edges") {
          createEdges(xmlReader);
        }
      }
    }

    xmlFile.close();

    // Special case : some GEXF files declare edges before nodes
    // so we have to add edges once nodes have been parsed
    for (size_t i = 0; i < edgesTmp.size(); ++i) {
      graph->addEdge(nodesMap[edgesTmp[i].first], nodesMap[edgesTmp[i].second]);
    }

    // nodes shape will be circle
    viewShape->setAllNodeValue(NodeShape::Circle);

    // add subgraph nodes, edges and meta nodes if needed
    Graph *quotientGraph = addSubGraphsNodes();

    if (quotientGraph) {
      addSubGraphsEdges();
      computeMetaNodes(quotientGraph);
    }

    // Set edges to be rendered as Cubic Bézier curves and
    // compute curve control points for each edge
    if (curvedEdges && nodesHaveCoordinates) {
      viewShape->setAllEdgeValue(EdgeShape::BezierCurve);
      curveGraphEdges();
    }

    return true;
  }

  // Create a set of Tulip Properties from the attributes declared in the GEXF file
  // according to data types
  void createPropertiesFromAttributes(QXmlStreamReader &xmlReader) {
    bool nodeProperties = xmlReader.attributes().value("class") == "node";
    unordered_map<string, PropertyInterface *> &propertiesMap =
        nodeProperties ? nodePropertiesMap : edgePropertiesMap;

    while (!(xmlReader.isEndElement() && xmlReader.name() == "attributes")) {
      xmlReader.readNext();

      // create a Tulip property and store mapping between attribute id and property
      if (xmlReader.isStartElement() && xmlReader.name() == "attribute") {
        string attributeId = QStringToTlpString(xmlReader.attributes().value("id").toString());
        string attributeName = QStringToTlpString(xmlReader.attributes().value("title").toString());
        string attributeType = QStringToTlpString(xmlReader.attributes().value("type").toString());

        if (attributeType == "string") {
          propertiesMap[attributeId] = graph->getProperty<StringProperty>(attributeName);
        } else if (attributeType == "float" || attributeType == "double") {
          propertiesMap[attributeId] = graph->getProperty<DoubleProperty>(attributeName);
        } else if (attributeType == "integer") {
          propertiesMap[attributeId] = graph->getProperty<IntegerProperty>(attributeName);
        } else if (attributeType == "boolean") {
          propertiesMap[attributeId] = graph->getProperty<BooleanProperty>(attributeName);
        }
      }
    }
  }

  // create nodes
  void createNodes(QXmlStreamReader &xmlReader, Graph *g) {
    while (!(xmlReader.isEndElement() && xmlReader.name() == "nodes")) {
      xmlReader.readNext();

      // must be a node
      if (xmlReader.isStartElement() && xmlReader.name() == "node")
        parseNode(xmlReader, g);
    }
  }

  // create edges
  void createEdges(QXmlStreamReader &xmlReader) {
    while (!(xmlReader.isEndElement() && xmlReader.name() == "edges")) {
      xmlReader.readNext();

      // must be an edge
      if (xmlReader.isStartElement() && xmlReader.name() == "edge")
        parseEdge(xmlReader);
    }
  }

  Graph *addInParent(node n, const string &pid) {
    // get the parent node
    // to find the corresponding sub graph
    // in which the current node must be added
    node pn;

    if (nodesMap.find(pid) == nodesMap.end()) {
      // create a fake meta node
      pn = graph->addNode();
      nodesMap[pid] = pn;
    } else
      pn = nodesMap[pid];

    Graph *sg = nodeToSubgraph.get(pn.id);

    if (sg == nullptr) {
      // add a subgraph for the fake meta node
      sg = graph->addSubGraph();
      // record pn as its fake meta node
      sg->setAttribute<node>("meta-node", pn);
      // and vice-versa
      nodeToSubgraph.set(pn.id, sg);
    }

    // add n in the subgraph found
    sg->addNode(n);

    return sg;
  }

  // Parse node data
  void parseNode(QXmlStreamReader &xmlReader, Graph *g) {
    node n;
    string nodeId = QStringToTlpString(xmlReader.attributes().value("id").toString());

    if (nodesMap.find(nodeId) == nodesMap.end()) {
      // if needed, add a node in the graph we are building
      n = g->addNode();
      // save mapping between gexf node id and created Tulip node
      nodesMap[nodeId] = n;
    } else
      n = nodesMap[nodeId];

    // parse node label
    if (xmlReader.attributes().hasAttribute("label")) {
      string nodeLabel = QStringToTlpString(xmlReader.attributes().value("label").toString());
      viewLabel->setNodeValue(n, nodeLabel);
    }

    // parse node pid
    if (xmlReader.attributes().hasAttribute("pid")) {
      string pid = QStringToTlpString(xmlReader.attributes().value("pid").toString());

      if (g == graph)
        g = addInParent(n, pid);
      else
        cerr << "multiple parents are not supported: " << pid.c_str()
             << " will be not added as parent of " << nodeId.c_str() << endl;
    }

    xmlReader.readNext();

    while (!(xmlReader.isEndElement() && xmlReader.name() == "node")) {
      // parse node color
      if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "viz:color") {
        unsigned int r = xmlReader.attributes().value("r").toString().toUInt();
        unsigned int g = xmlReader.attributes().value("g").toString().toUInt();
        unsigned int b = xmlReader.attributes().value("b").toString().toUInt();
        float a = 1.0f;

        if (xmlReader.attributes().hasAttribute("a")) {
          a = xmlReader.attributes().value("a").toString().toFloat();
        }

        viewColor->setNodeValue(n, Color(uchar(r), uchar(g), uchar(b), uchar(a * 255)));
      }
      // parse node coordinates
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "viz:position") {
        nodesHaveCoordinates = true;
        float x = xmlReader.attributes().value("x").toString().toFloat();
        float y = xmlReader.attributes().value("y").toString().toFloat();
        float z = xmlReader.attributes().value("z").toString().toFloat();
        viewLayout->setNodeValue(n, Coord(x, y, z));
      }
      // parse node size
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "viz:size") {
        float size = xmlReader.attributes().value("value").toString().toFloat();
        viewSize->setNodeValue(n, Size(size, size, size));
      }
      // parse node attributes
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "attvalue") {
        string attributeId = "";

        if (xmlReader.attributes().hasAttribute("id")) {
          attributeId = QStringToTlpString(xmlReader.attributes().value("id").toString());
        } else if (xmlReader.attributes().hasAttribute("for")) {
          attributeId = QStringToTlpString(xmlReader.attributes().value("for").toString());
        }

        string attributeStrValue =
            QStringToTlpString(xmlReader.attributes().value("value").toString());

        if (nodePropertiesMap.find(attributeId) != nodePropertiesMap.end()) {
          nodePropertiesMap[attributeId]->setNodeStringValue(n, attributeStrValue);
        }
      }
      // check for subgraph
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "nodes") {
        Graph *sg = nodeToSubgraph.get(n.id);

        if (sg == nullptr) {
          // add subgraph
          sg = graph->addSubGraph();
          // record the current node as its fake meta node
          sg->setAttribute<node>("meta-node", n);
          // and vice-versa
          nodeToSubgraph.set(n.id, sg);
        }

        // create its nodes
        createNodes(xmlReader, sg);
      } else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "edges") {
        // create its edges
        createEdges(xmlReader);
      } else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "parents") {
        while (!(xmlReader.isEndElement() && xmlReader.name() == "parents")) {
          xmlReader.readNext();

          // must be a parent
          if (xmlReader.isStartElement() && xmlReader.name() == "parent") {
            string pid = QStringToTlpString(xmlReader.attributes().value("for").toString());

            if (g == graph) {
              g = addInParent(n, pid);
            } else
              cerr << "multiple parents are not supported: " << pid.c_str()
                   << " will be not added as parent of " << nodeId.c_str() << endl;
          }
        }
      }

      xmlReader.readNext();
    }
  }

  // Parse edge data
  void parseEdge(QXmlStreamReader &xmlReader) {
    // parse the source node id and target node id
    string srcId = QStringToTlpString(xmlReader.attributes().value("source").toString());
    string tgtId = QStringToTlpString(xmlReader.attributes().value("target").toString());

    // Check if nodes have been parsed
    if (!nodesMap.empty()) {
      // add an edge in the graph we are building
      edge e = graph->addEdge(nodesMap[srcId], nodesMap[tgtId]);

      if (xmlReader.attributes().hasAttribute("label")) {
        string edgeLabel = QStringToTlpString(xmlReader.attributes().value("label").toString());
        viewLabel->setEdgeValue(e, edgeLabel);
      }

      xmlReader.readNext();

      while (!(xmlReader.isEndElement() && xmlReader.name() == "edge")) {
        // parse edge attribute
        if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "attvalue") {
          string attributeId = "";

          if (xmlReader.attributes().hasAttribute("id")) {
            attributeId = QStringToTlpString(xmlReader.attributes().value("id").toString());
          } else if (xmlReader.attributes().hasAttribute("for")) {
            attributeId = QStringToTlpString(xmlReader.attributes().value("for").toString());
          }

          string attributeStrValue =
              QStringToTlpString(xmlReader.attributes().value("value").toString());

          if (edgePropertiesMap.find(attributeId) != edgePropertiesMap.end()) {
            edgePropertiesMap[attributeId]->setEdgeStringValue(e, attributeStrValue);
          }
        }

        xmlReader.readNext();
      }

      // Store edge extremities information to add them to the graph once nodes will be parsed
    } else {
      edgesTmp.emplace_back(srcId, tgtId);
    }
  }

  Graph *addSubGraphsNodes() {
    // quotientGraph will be created
    // as soon as there is a subgraph
    Graph *quotientGraph = nullptr;
    // iterate on each subgraph of graph
    // and add missing nodes
    for (Graph *sg : stableIterator(graph->getSubGraphs())) {
      if (!quotientGraph)
        quotientGraph = graph->addCloneSubGraph("quotient graph");

      // iterate on nodes
      for (auto n : stableIterator(sg->getNodes())) {
        Graph *msg = nodeToSubgraph.get(n.id);

        if (msg) {
          // if the current node is a fake meta node
          // add the nodes of the pointed subgraph
          for (auto msn : msg->nodes()) {
            sg->addNode(msn);
            // the nodes in a pointed subgraph have to be removed
            // from quotientGraph
            quotientGraph->delNode(msn);
          }
        }
      }
    }

    return quotientGraph;
  }

  void addSubGraphsEdges() {
    // iterate on each subgraph of graph
    // and add needed edges
    for (Graph *sg : graph->subGraphs()) {
      // iterate on nodes
      for (auto n : sg->nodes()) {
        // add its out edges
        for (auto e : graph->getOutEdges(n)) {
          if (sg->isElement(graph->target(e)))
            sg->addEdge(e);
        }
      }
    }
  }

  void computeMetaNodes(Graph *quotientGraph) {
    // iterate on each subgraph of g
    // and add needed meta nodes

    for (Graph *sg : graph->subGraphs()) {
      // iterate on nodes
      vector<node> sgNodes(sg->nodes());
      for (auto n : sgNodes) {
        Graph *msg = nodeToSubgraph.get(n.id);

        if (msg != nullptr) {
          // n is a fake meta node
          // create the real meta node
          // pointing to msg
          node mn = sg->createMetaNode(msg);
          // set its label as the name
          // of the pointed subgraph
          string label = viewLabel->getNodeValue(n);

          if (!label.empty())
            msg->setName(label);

          // set meta node properties values to the ones
          // of the fake meta node
          for (PropertyInterface *prop : graph->getObjectProperties()) {
            prop->copy(mn, n, prop, true);
          }

          // add it to quotient graph if needed
          if (sg != quotientGraph)
            quotientGraph->addNode(mn);

          // replace n by mn
          for (auto e : graph->getInOutEdges(n)) {
            const pair<node, node> &eEnds = graph->ends(e);

            if (eEnds.first == n) {
              graph->setEnds(e, mn, eEnds.second);

              if ((sg != quotientGraph) && quotientGraph->isElement(eEnds.second))
                quotientGraph->addEdge(e);
            } else {
              graph->setEnds(e, eEnds.first, mn);

              if ((sg != quotientGraph) && quotientGraph->isElement(eEnds.first))
                quotientGraph->addEdge(e);
            }
          }
          // remove fake meta node
          graph->delNode(n);
          msg->removeAttribute("meta-node");
          nodeToSubgraph.set(n.id, nullptr);
        }
      }
    }
  }

  // Methods which compute Cubic Bézier control points for each edge
  void curveGraphEdges() {
    for (auto e : graph->edges()) {
      auto eEnds = graph->ends(e);
      const Coord &srcCoord = viewLayout->getNodeValue(eEnds.first);
      const Coord &tgtCoord = viewLayout->getNodeValue(eEnds.second);
      Coord &&dir = tgtCoord - srcCoord;
      dir /= dir.norm();
      float length = srcCoord.dist(tgtCoord);
      float factor = 0.2f * length;
      Coord &&normal = Coord(dir[1], -dir[0]);
      normal *= factor;

      Coord p1 = dir;
      p1 *= factor;
      p1 += srcCoord;
      p1 += normal;

      Coord p2 = std::move(dir);
      p2 *= -factor;
      p2 += tgtCoord;
      p2 += normal;

      // Set the second and third Cubic Bézier curve control points as edge bends
      viewLayout->setEdgeValue(e, vector<Coord>{{p1, p2}});
    }
  }

private:
  // maps associating attribute id to Tulip property
  unordered_map<string, PropertyInterface *> nodePropertiesMap;
  unordered_map<string, PropertyInterface *> edgePropertiesMap;

  // map associating GEXF node id to Tulip node
  unordered_map<string, node> nodesMap;

  // vector to store edge information in case edges are declared before nodes in GEXF file
  vector<pair<string, string>> edgesTmp;

  // Visual attributes properties pointers to the graph we are building
  LayoutProperty *viewLayout;
  SizeProperty *viewSize;
  ColorProperty *viewColor;
  StringProperty *viewLabel;
  IntegerProperty *viewShape;
  // to register the subgraph corresponding
  // to a fake meta node
  MutableContainer<Graph *> nodeToSubgraph;

  bool nodesHaveCoordinates;
};

// Macro for declaring import plugin in Tulip, it will then be accessible since the File -> Import
// -> File menu entry in Tulip
PLUGIN(GEXFImport)
