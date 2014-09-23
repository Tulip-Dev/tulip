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
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <tulip/ImportModule.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/ForEach.h>
#include <tulip/TulipViewSettings.h>

#include <QXmlStreamReader>
#include <QFile>


using namespace tlp;
using namespace std;


/** \file
 *  \brief - Import GEXF format graph file.
 * This plugin imports a graph from a file in GEXF format,<br/>
 * as it is described in the XML Schema 1.2draft (http://gexf.net/format/schema.html). Dynamic mode is not yet supported.
 *  <b>HISTORY</b>
 *
 *  - 04/06/2012 Version 1.0: Initial release
 *
 *  \author Antoine Lambert & Patrick Mary of Tulip Team http://tulip-software.org/
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

static const char * paramHelp[] = {
  // filename
  HTML_HELP_OPEN()            \
  HTML_HELP_DEF( "type", "pathname" )       \
  HTML_HELP_BODY()                  \
  "This parameter defines the file pathname to import."       \
  HTML_HELP_CLOSE(),
  // curved edges
  HTML_HELP_OPEN()            \
  HTML_HELP_DEF( "type", "bool" )       \
  HTML_HELP_DEF( "values", "true, false" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY()                  \
  "Indicates if Bezier curves will be used to draw the edges."       \
  HTML_HELP_CLOSE(),
};

class GEXFImport : public ImportModule {

public :

  PLUGININFORMATION("GEXF","Antoine LAMBERT","05/05/2010","Imports a new graph from a file in the GEXF input format.","1.0","File")
  GEXFImport(const PluginContext*  context):ImportModule(context),
    viewLayout(NULL), viewSize(NULL), viewColor(NULL), viewLabel(NULL),
    viewShape(NULL), nodesHaveCoordinates(false) {
    // add a file parameter for the plugin
    addInParameter<string>("file::filename", paramHelp[0],"");
    addInParameter<bool>("Curved edges",paramHelp[1], "false");
  }

  ~GEXFImport() {}

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/import_gephi.png";
  }

  std::list<std::string> fileExtensions() const {
    std::list<std::string> l;
    l.push_back("gexf");
    return l;
  }

  // import plugins must implement bool importGraph()
  bool importGraph() {
    string filename;
    bool curvedEdges = false;
    // get the filename choosed by the user
    dataSet->get<string>("file::filename", filename);
    dataSet->get<bool>("Curved edges", curvedEdges);

    QString qfilename = QString::fromUtf8(filename.c_str());

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
    QFile *xmlFile = new QFile(qfilename);

    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
      // get error
      pluginProgress->setError(xmlFile->errorString().toUtf8().data());
      return false;
    }

    // Instantiate a QXmlStreamReader to parse the file (GEXF is xml)
    QXmlStreamReader xmlReader(xmlFile);

    // Parse each line of the file
    while (!xmlReader.atEnd()) {
      if (xmlReader.readNextStartElement()) {
        // only static graph are supported
        if (xmlReader.name() == "graph") {
          string mode =
            xmlReader.attributes().value("mode").toString().toStdString();

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

    delete xmlFile;

    // Special case : some GEXF files declare edges before nodes
    // so we have to add edges once nodes have been parsed
    for (size_t i = 0 ; i < edgesTmp.size() ; ++i) {
      graph->addEdge(nodesMap[edgesTmp[i].first], nodesMap[edgesTmp[i].second]);
    }

    // nodes shape will be circle
    viewShape->setAllNodeValue(NodeShape::Circle);

    // add subgraph nodes, edges and meta nodes if needed
    Graph*quotientGraph = addSubGraphsNodes();

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
    map<string, PropertyInterface *> *propertiesMap = NULL;

    if (nodeProperties) {
      propertiesMap = &nodePropertiesMap;
    }
    else {
      propertiesMap = &edgePropertiesMap;
    }

    while (!(xmlReader.isEndElement() && xmlReader.name() == "attributes")) {
      xmlReader.readNext();

      // create a Tulip property and store mapping between attribute id and property
      if (xmlReader.isStartElement() && xmlReader.name() == "attribute") {
        string attributeId = xmlReader.attributes().value("id").toString().toStdString();
        string attributeName = xmlReader.attributes().value("title").toString().toUtf8().data();
        string attributeType = xmlReader.attributes().value("type").toString().toStdString();

        if (attributeType == "string") {
          (*propertiesMap)[attributeId] = graph->getProperty<StringProperty>(attributeName);
        }
        else if (attributeType == "float" || attributeType == "double") {
          (*propertiesMap)[attributeId] = graph->getProperty<DoubleProperty>(attributeName);
        }
        else if (attributeType == "integer") {
          (*propertiesMap)[attributeId] = graph->getProperty<IntegerProperty>(attributeName);
        }
        else if (attributeType == "boolean") {
          (*propertiesMap)[attributeId] = graph->getProperty<BooleanProperty>(attributeName);
        }
      }
    }
  }

  // create nodes
  void createNodes(QXmlStreamReader &xmlReader, Graph* g) {
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

  Graph* addInParent(node n, const string& pid) {
    // get the parent node
    // to find the corresponding sub graph
    // in which the current node must be added
    node pn;

    if (nodesMap.find(pid) == nodesMap.end()) {
      // create a fake meta node
      pn = graph->addNode();
      nodesMap[pid] = pn;
    }
    else
      pn = nodesMap[pid];

    Graph* sg = (Graph*) nodeToSubgraph.get(pn.id);

    if (sg == NULL) {
      // add a subgraph for the fake meta node
      sg = graph->addSubGraph();
      // record pn as its fake meta node
      sg->setAttribute<node>("meta-node", pn);
      // and vice-versa
      nodeToSubgraph.set(pn.id, (size_t) sg);
    }

    // add n in the subgraph found
    sg->addNode(n);

    return sg;
  }

  // Parse node data
  void parseNode(QXmlStreamReader &xmlReader, Graph* g) {
    node n;
    string nodeId = xmlReader.attributes().value("id").toString().toStdString();

    if (nodesMap.find(nodeId) == nodesMap.end()) {
      // if needed, add a node in the graph we are building
      n = g->addNode();
      // save mapping between gexf node id and created Tulip node
      nodesMap[nodeId] = n;
    }
    else
      n = nodesMap[nodeId];

    // parse node label
    if (xmlReader.attributes().hasAttribute("label")) {
      string nodeLabel =
        xmlReader.attributes().value("label").toString().toUtf8().data();
      viewLabel->setNodeValue(n, nodeLabel);
    }

    // parse node pid
    if (xmlReader.attributes().hasAttribute("pid")) {
      string pid =
        xmlReader.attributes().value("pid").toString().toStdString();

      if (g == graph)
        g = addInParent(n, pid);
      else
        cerr << "multiple parents are not supported: " << pid.c_str() << " will be not added as parent of " << nodeId.c_str() << endl;
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

        viewColor->setNodeValue(n, Color((unsigned char) r, (unsigned char) g,
                                         (unsigned char) b,
                                         (unsigned char) (a * 255)));
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
          attributeId = xmlReader.attributes().value("id").toString().toStdString();
        }
        else if (xmlReader.attributes().hasAttribute("for")) {
          attributeId = xmlReader.attributes().value("for").toString().toStdString();
        }

        string attributeStrValue = xmlReader.attributes().value("value").toString().toUtf8().data();

        if (nodePropertiesMap.find(attributeId) != nodePropertiesMap.end()) {
          nodePropertiesMap[attributeId]->setNodeStringValue(n, attributeStrValue);
        }
      }
      // check for subgraph
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "nodes") {
        Graph* sg = (Graph *) nodeToSubgraph.get(n.id);

        if (sg == NULL) {
          // add subgraph
          sg = graph->addSubGraph();
          // record the current node as its fake meta node
          sg->setAttribute<node>("meta-node", n);
          // and vice-versa
          nodeToSubgraph.set(n.id, (size_t) sg);
        }

        // create its nodes
        createNodes(xmlReader, sg);
      }
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "edges") {
        // create its edges
        createEdges(xmlReader);
      }
      else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "parents") {
        while (!(xmlReader.isEndElement() && xmlReader.name() == "parents")) {
          xmlReader.readNext();

          // must be a parent
          if (xmlReader.isStartElement() && xmlReader.name() == "parent") {
            string pid =
              xmlReader.attributes().value("for").toString().toStdString();

            if (g == graph) {
              g = addInParent(n, pid);
            }
            else
              cerr << "multiple parents are not supported: " << pid.c_str() << " will be not added as parent of " << nodeId.c_str() << endl;
          }
        }
      }

      xmlReader.readNext();
    }
  }

  // Parse edge data
  void parseEdge(QXmlStreamReader &xmlReader) {
    // parse the source node id and target node id
    string srcId = xmlReader.attributes().value("source").toString().toStdString();
    string tgtId = xmlReader.attributes().value("target").toString().toStdString();

    // Check if nodes have been parsed
    if (!nodesMap.empty()) {
      // add an edge in the graph we are building
      edge e = graph->addEdge(nodesMap[srcId], nodesMap[tgtId]);

      if (xmlReader.attributes().hasAttribute("label")) {
        string edgeLabel = xmlReader.attributes().value("label").toString().toUtf8().data();
        viewLabel->setEdgeValue(e, edgeLabel);
      }

      xmlReader.readNext();

      while (!(xmlReader.isEndElement() && xmlReader.name() == "edge")) {
        // parse edge attribute
        if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "attvalue") {
          string attributeId = "";

          if (xmlReader.attributes().hasAttribute("id")) {
            attributeId = xmlReader.attributes().value("id").toString().toStdString();
          }
          else if (xmlReader.attributes().hasAttribute("for")) {
            attributeId = xmlReader.attributes().value("for").toString().toStdString();
          }

          string attributeStrValue = xmlReader.attributes().value("value").toString().toUtf8().data();

          if (edgePropertiesMap.find(attributeId) != edgePropertiesMap.end()) {
            edgePropertiesMap[attributeId]->setEdgeStringValue(e, attributeStrValue);
          }
        }

        xmlReader.readNext();
      }

      // Store edge extremities information to add them to the graph once nodes will be parsed
    }
    else {
      edgesTmp.push_back(make_pair(srcId, tgtId));
    }
  }

  Graph* addSubGraphsNodes() {
    // quotientGraph will be created
    // as soon as there is a subgraph
    Graph* quotientGraph = NULL;
    // iterate on each subgraph of graph
    // and add missing nodes
    StableIterator<Graph*> itg(graph->getSubGraphs());

    while(itg.hasNext()) {
      if (!quotientGraph)
        quotientGraph = graph->addCloneSubGraph("quotient graph");

      Graph* sg = itg.next();
      // iterate on nodes
      StableIterator<node> itn(sg->getNodes());

      while(itn.hasNext()) {
        node n = itn.next();
        Graph* msg = (Graph*) nodeToSubgraph.get(n.id);

        if (msg) {
          // if the current node is a fake meta node
          // add the nodes of the pointed subgraph
          node msn;
          forEach(msn, msg->getNodes()) {
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
    Iterator<Graph*>* itg = graph->getSubGraphs();

    while(itg->hasNext()) {
      Graph* sg = itg->next();
      node n;
      // iterate on nodes
      forEach(n, sg->getNodes()) {
        // add its out edges
        edge e;
        forEach(e, graph->getOutEdges(n)) {
          if (sg->isElement(graph->target(e)))
            sg->addEdge(e);
        }
      }
    }
  }

  void computeMetaNodes(Graph* quotientGraph) {
    // iterate on each subgraph of g
    // and add needed meta nodes
    Iterator<Graph*>* itg = graph->getSubGraphs();

    while(itg->hasNext()) {
      Graph* sg = itg->next();
      // iterate on nodes
      StableIterator<node> itn(sg->getNodes());

      while(itn.hasNext()) {
        node n = itn.next();
        Graph* msg = (Graph*) nodeToSubgraph.get(n.id);

        if (msg != NULL) {
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
          PropertyInterface* prop;
          forEach(prop, graph->getObjectProperties()) {
            prop->copy(mn, n, prop, true);
          }

          // add it to quotient graph if needed
          if (sg != quotientGraph)
            quotientGraph->addNode(mn);

          // replace n by mn
          edge e;
          forEach(e, graph->getInOutEdges(n)) {
            pair<node, node> eEnds = graph->ends(e);

            if (eEnds.first == n) {
              graph->setEnds(e, mn, eEnds.second);

              if ((sg != quotientGraph) &&
                  quotientGraph->isElement(eEnds.second))
                quotientGraph->addEdge(e);
            }
            else {
              graph->setEnds(e, eEnds.first, mn);

              if ((sg != quotientGraph) &&
                  quotientGraph->isElement(eEnds.first))
                quotientGraph->addEdge(e);
            }
          }
          // remove fake meta node
          graph->delNode(n);
          msg->removeAttribute("meta-node");
          nodeToSubgraph.set(n.id, 0);
        }
      }
    }
  }

  // Methods which compute Cubic Bézier control points for each edge
  void curveGraphEdges() {
    edge e;
    forEach(e, graph->getEdges()) {
      node src = graph->source(e);
      node tgt = graph->target(e);
      Coord srcCoord = viewLayout->getNodeValue(src);
      Coord tgtCoord = viewLayout->getNodeValue(tgt);
      Coord dir = tgtCoord - srcCoord;
      dir /= dir.norm();
      float length = srcCoord.dist(tgtCoord);
      float factor = 0.2f * length;
      Coord normal = Coord(dir[1], -dir[0]);
      normal *= factor;

      Coord p1 = dir;
      p1 *= factor;
      p1 += srcCoord;
      p1 += normal;

      Coord p2 = dir;
      p2 *= -factor;
      p2 += tgtCoord;
      p2 += normal;

      // Set the second and third Cubic Bézier curve control points as edge bends
      vector<Coord> bends;
      bends.push_back(p1);
      bends.push_back(p2);

      viewLayout->setEdgeValue(e, bends);
    }
  }

private :

  // maps associating attribute id to Tulip property
  map<string, PropertyInterface *> nodePropertiesMap;
  map<string, PropertyInterface *> edgePropertiesMap;

  // map associating GEXF node id to Tulip node
  map<string, node> nodesMap;

  // vector to store edge information in case edges are declared before nodes in GEXF file
  vector<pair<string, string> > edgesTmp;

  // Visual attributes properties pointers to the graph we are building
  LayoutProperty *viewLayout;
  SizeProperty *viewSize;
  ColorProperty *viewColor;
  StringProperty *viewLabel;
  IntegerProperty *viewShape;
  // to register the subgraph corresponding
  // to a fake meta node
  MutableContainer<size_t> nodeToSubgraph;

  bool nodesHaveCoordinates;

};

// Macro for declaring import plugin in Tulip, it will then be accessible since the File -> Import -> File menu entry in Tulip
PLUGIN(GEXFImport)
