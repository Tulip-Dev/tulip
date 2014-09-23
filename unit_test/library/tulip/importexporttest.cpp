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
#include "importexporttest.h"

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>

#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/StringCollection.h>

using namespace tlp;
using namespace std;

ImportExportTest::ImportExportTest(const string &importAlgorithm, const string &exportAlgorithm) :
  importAlgorithm(importAlgorithm), exportAlgorithm(exportAlgorithm) {}

void ImportExportTest::setUp() {
  CppUnit::TestFixture::setUp();

  CPPUNIT_ASSERT_MESSAGE("Export plugin not loaded: " + exportAlgorithm, PluginLister::pluginExists(exportAlgorithm));
  CPPUNIT_ASSERT_MESSAGE("Import plugin not loaded: " + importAlgorithm, PluginLister::pluginExists(importAlgorithm));
}

void ImportExportTest::testgridImportExport() {
  Graph* original = createSimpleGraph();

  importExportGraph(original);
}

void ImportExportTest::testAttributes() {
  Graph* original = createSimpleGraph();

  const string originalName = "rootGraph";
  const bool b = true;
  const double d = 3.14;
  const float f = 0.2f;
  const int i = 33;
  const unsigned int ui = 256;
  const long l = 56845725;
  const Coord c(1,2,3);
  const Coord c2(10,20,30);
  const Color col = Color::Azure;
  const Size s(4,5,6);
  vector<string> vs;
  vs.push_back("foo");
  vs.push_back("bar");
  vector<double> vd;
  vd.push_back(12.56);
  vd.push_back(45.85);
  vector<int> vi;
  vi.push_back(45);
  vi.push_back(-120);
  vector<bool> vb;
  vb.push_back(true);
  vb.push_back(false);
  vector<Coord> vc;
  vc.push_back(c);
  vc.push_back(c2);
  set<edge> setEdge;
  vector<edge> vectorEdge;
  vector<node> vectorNode;
  node n;
  edge e;
  forEach(n, original->getNodes()) {
    vectorNode.push_back(n);
  }
  forEach(e, original->getEdges()) {
    vectorEdge.push_back(e);
    setEdge.insert(e);
  }
  n = original->getOneNode();
  e = original->getOneEdge();
  StringCollection sc;
  sc.push_back("foo");
  sc.push_back("bar");

  DataSet dataSet;
  dataSet.set("name", originalName);
  dataSet.set("boolean", b);
  dataSet.set("double", d);
  dataSet.set("float", f);
  dataSet.set("integer", i);
  dataSet.set("unsigned integer", ui);
  dataSet.set("long", l);
  dataSet.set("tlp::Coord", c);
  dataSet.set("tlp::Size", s);
  dataSet.set("tlp::Color", col);
  dataSet.set("tlp::node", n);
  dataSet.set("tlp::edge", e);
  dataSet.set("tlp::StringCollection", sc);
  dataSet.set("vector<string>", vs);
  dataSet.set("vector<double>", vd);
  dataSet.set("vector<int>", vi);
  dataSet.set("vector<bool>", vb);
  dataSet.set("vector<node>", vectorNode);
  dataSet.set("vector<edge>", vectorEdge);
  dataSet.set("vector<Coord>", vc);
  dataSet.set("set<edge>", setEdge);

  original->setAttribute("name", originalName);
  original->setAttribute("boolean", b);
  original->setAttribute("double", d);
  original->setAttribute("float", f);
  original->setAttribute("integer", i);
  original->setAttribute("unsigned integer", ui);
  original->setAttribute("long", l);
  original->setAttribute("tlp::Coord", c);
  original->setAttribute("tlp::Size", s);
  original->setAttribute("tlp::Color", col);
  original->setAttribute("tlp::node", n);
  original->setAttribute("tlp::edge", e);
  original->setAttribute("tlp::StringCollection", sc);
  original->setAttribute("vector<string>", vs);
  original->setAttribute("vector<double>", vd);
  original->setAttribute("vector<int>", vi);
  original->setAttribute("vector<bool>", vb);
  original->setAttribute("vector<node>", vectorNode);
  original->setAttribute("vector<edge>", vectorEdge);
  original->setAttribute("vector<Coord>", vc);
  original->setAttribute("set<edge>", setEdge);
  original->setAttribute("tlp::DataSet", dataSet);

  importExportGraph(original);
}

void ImportExportTest::testSubGraphsImportExport() {
  Graph* original = createSimpleGraph();
  const string originalName = "rootGraph";
  original->setAttribute("name", originalName);

  std::set<node> sub1nodes;
  int sub1lowerBound = 5;
  int sub1higherBound = 25;
  std::set<node> sub2nodes;
  int sub2lowerBound = 50;
  int sub2higherBound = 150;
  std::set<node> subsubnodes;
  int subsublowerBound = 75;
  int subsubhigherBound = 125;
  int i = 0;
  node n;
  forEach(n, original->getNodes()) {
    if(i >= sub1lowerBound && i <= sub1higherBound) {
      sub1nodes.insert(n);
    }

    if(i >= sub2lowerBound && i <= sub2higherBound) {
      sub2nodes.insert(n);

      if(i >= subsublowerBound && i <= subsubhigherBound) {
        subsubnodes.insert(n);
      }
    }

    ++i;
  }
  const string sub1name = "sub1";
  const string sub2name = "sub2";
  const string subsubname = "subsub";
  Graph* sub1 = original->inducedSubGraph(sub1nodes);
  Graph* sub2 = original->inducedSubGraph(sub2nodes);
  Graph* subsub = sub2->inducedSubGraph(subsubnodes);
  sub1->setAttribute("name", sub1name);
  sub2->setAttribute("name", sub2name);
  subsub->setAttribute("name", subsubname);

  i = 0;
  DoubleProperty* sub1id = sub1->getLocalProperty<DoubleProperty>("sub1id");
  forEach(n, sub1->getNodes()) {
    sub1id->setNodeValue(n, i++);
  }

  i = 0;
  DoubleProperty* sub2id = sub2->getLocalProperty<DoubleProperty>("sub2id");
  forEach(n, sub2->getNodes()) {
    sub2id->setNodeValue(n, i++);
  }

  i = 0;
  DoubleProperty* subsubid = subsub->getLocalProperty<DoubleProperty>("subsubid");
  forEach(n, subsub->getNodes()) {
    subsubid->setNodeValue(n, i++);
  }

  importExportGraph(original);
  importExportGraph(sub1);
  importExportGraph(sub2);
  importExportGraph(subsub);
}

Graph* ImportExportTest::createSimpleGraph() const {
  Graph* original = tlp::newGraph();
  LayoutProperty* layout = original->getProperty<LayoutProperty>("viewLayout");

  for(unsigned int i = 0; i < 100; ++i) {
    node n = original->addNode();
    layout->setNodeValue(n, Coord(i%11, i/10));
  }

  for(int x = 0; x < 10; ++x) {
    for(int y = 0; y < 10; ++y) {
      node origin(x+10*y);

      for(int xDelta = -1; xDelta <= 1; ++xDelta) {
        for(int yDelta = -1; yDelta <= 1; ++yDelta) {
          if(x+xDelta >= 0 && x+xDelta <= 9 && y+yDelta >= 0 && y+yDelta <= 9) {
            node destination(x+xDelta + 10*(y+yDelta));
            original->addEdge(origin, destination);
          }
        }
      }
    }
  }

  DoubleProperty* id = original->getProperty<DoubleProperty>("id");
  node n;
  forEach(n, original->getNodes()) {
    id->setNodeValue(n, n.id);
  }
  edge e;
  forEach(e, original->getEdges()) {
    id->setEdgeValue(e, e.id);
  }

  return original;
}

void ImportExportTest::importExportGraph(tlp::Graph* original) {
  const string exportFilename = "graph_export";
  ofstream os(exportFilename.c_str());
  DataSet set;
  tlp::exportGraph(original, os, exportAlgorithm, set);

  os.close();

  DataSet input;
  input.set<string>("file::filename", exportFilename);
  Graph* imported = tlp::importGraph(importAlgorithm, input);

  testGraphsAreEqual(original, imported);
}

void ImportExportTest::testGraphsAreEqual(Graph* first, Graph* second) {
  CPPUNIT_ASSERT_MESSAGE("The import failed; the graph is null", second != NULL);

  testGraphsTopologiesAreEqual(first, second);
  testGraphAttributesAreEqual(first, second);
  testGraphPropertiesAreEqual(first, second);

  CPPUNIT_ASSERT_EQUAL_MESSAGE("Graphs have different number of subgraphs", first->numberOfSubGraphs(), second->numberOfSubGraphs());

  Iterator<Graph*>* firstSubGraphs = first->getSubGraphs();
  Iterator<Graph*>* secondSubGraphs = second->getSubGraphs();

  while(firstSubGraphs->hasNext() && secondSubGraphs->hasNext()) {
    Graph* firstSub = firstSubGraphs->next();
    Graph* secondSub = secondSubGraphs->next();

    testGraphsAreEqual(firstSub, secondSub);
  }
  delete firstSubGraphs;
  delete secondSubGraphs;
}

void ImportExportTest::testGraphAttributesAreEqual(tlp::Graph* first, tlp::Graph* second) {
  std::pair<std::string, tlp::DataType*> attribute;
  forEach(attribute, first->getAttributes().getValues()) {
    stringstream attributeNameMessage;

    attributeNameMessage << "attribute \"" << attribute.first << "\" does not exists on imported graph.";
    CPPUNIT_ASSERT_MESSAGE(attributeNameMessage.str(), second->attributeExist(attribute.first));

    stringstream attributeTypeMessage;
    attributeTypeMessage << "attribute \"" << attribute.first << "\" has different type on imported graph";
    CPPUNIT_ASSERT_EQUAL_MESSAGE(attributeTypeMessage.str(), attribute.second->getTypeName(), second->getAttribute(attribute.first)->getTypeName());

    stringstream attributeValueMessage;
    attributeValueMessage << "attribute \"" << attribute.first << "\" has different value on imported graph";
    tlp::DataTypeSerializer* serializer = DataSet::typenameToSerializer(attribute.second->getTypeName());
    stringstream firstValue;
    stringstream secondValue;
    serializer->writeData(firstValue, attribute.second);
    serializer->writeData(secondValue, second->getAttribute(attribute.first));

    CPPUNIT_ASSERT_EQUAL_MESSAGE(attributeValueMessage.str(), firstValue.str(), secondValue.str());
  }
}

void ImportExportTest::testGraphPropertiesAreEqual(Graph* first, Graph* second) {
  unsigned int firstPropertiesCount = 0;
  unsigned int secondPropertiesCount = 0;

  PropertyInterface* firstProperty;
  PropertyInterface* secondProperty;
  forEach(firstProperty, first->getObjectProperties()) {
    ++firstPropertiesCount;
  }
  forEach(secondProperty, second->getObjectProperties()) {
    ++secondPropertiesCount;
  }

  CPPUNIT_ASSERT_EQUAL_MESSAGE("Graphs have different number of properties", firstPropertiesCount, secondPropertiesCount);

  Iterator<string>* firstPropIt = first->getProperties();

  while(firstPropIt->hasNext()) {
    string firstPropertyName = firstPropIt->next();
    firstProperty = first->getProperty(firstPropertyName);

    stringstream message;
    message << "the property " << firstPropertyName << " does not exist in the second graph !";
    CPPUNIT_ASSERT_MESSAGE(message.str(), second->existProperty(firstPropertyName));

    secondProperty = second->getProperty(firstPropertyName);

    message.str("");
    message << "a node value for property " << firstPropertyName << " in the first graph is not equal to the one in the second graph";

    node n1, n2;
    Iterator<node>* firstNodeIt = first->getNodes();
    Iterator<node>* secondNodeIt = second->getNodes();
    while (firstNodeIt->hasNext()) {
      n1 = firstNodeIt->next();
      n2 = secondNodeIt->next();
      CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), firstProperty->getNodeStringValue(n1), secondProperty->getNodeStringValue(n2));
    }
    delete firstNodeIt;
    delete secondNodeIt;

    message.str("");
    message << "an edge value for property " << firstPropertyName << " in the first graph is not equal to the one in the second graph";

    edge e1, e2;
    Iterator<edge>* firstEdgeIt = first->getEdges();
    Iterator<edge>* secondEdgeIt = second->getEdges();
    while (firstEdgeIt->hasNext()) {
      e1 = firstEdgeIt->next();
      e2 = secondEdgeIt->next();
      CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), firstProperty->getEdgeStringValue(e1), secondProperty->getEdgeStringValue(e2));
    }
    delete firstEdgeIt;
    delete secondEdgeIt;
  }
  delete firstPropIt;
}

void ImportExportTest::testGraphsTopologiesAreEqual(tlp::Graph* first, tlp::Graph* second) {
  stringstream nodesMessage;
  string name1, name2;
  first->getAttribute("name", name1);
  second->getAttribute("name", name2);
  nodesMessage << "Graphs '" << name1 << "' and '" << name2 << "' have different number of nodes";
  stringstream edgesMessage;
  edgesMessage << "Graphs " << name1 << " and " << name2 << " have different number of edges";
  CPPUNIT_ASSERT_EQUAL_MESSAGE(nodesMessage.str(), first->numberOfNodes(), second->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL_MESSAGE(edgesMessage.str(), first->numberOfEdges(), second->numberOfEdges());

  CPPUNIT_ASSERT_MESSAGE("id control property does not exists on original graph, please add it before testing if graphs are equal.", first->existProperty("id"));
  CPPUNIT_ASSERT_MESSAGE("id control property does not exists on imported graph", second->existProperty("id"));

  DoubleProperty* firstIdProperty = first->getProperty<DoubleProperty>("id");
  DoubleProperty* secondIdProperty = second->getProperty<DoubleProperty>("id");

  CPPUNIT_ASSERT_MESSAGE("layout property does not exists on original graph", first->existProperty("viewLayout"));
  CPPUNIT_ASSERT_MESSAGE("layout property does not exists on imported graph", second->existProperty("viewLayout"));

  LayoutProperty* firstLayout = first->getProperty<LayoutProperty>("viewLayout");
  LayoutProperty* secondLayout = second->getProperty<LayoutProperty>("viewLayout");

  Iterator<node>* firstNodeIt = first->getNodes();
  Iterator<node>* secondNodeIt = second->getNodes();

  while(firstNodeIt->hasNext() && secondNodeIt->hasNext()) {
    node firstNode = firstNodeIt->next();
    node secondNode = secondNodeIt->next();

    double firstNodeId = firstIdProperty->getNodeValue(firstNode);
    double secondNodeId = secondIdProperty->getNodeValue(secondNode);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nodes are not in the same order", firstNodeId, secondNodeId);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nodes do not have same degree", first->deg(firstNode), second->deg(secondNode));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nodes are not at the same position", firstLayout->getNodeStringValue(firstNode), secondLayout->getNodeStringValue(secondNode));

  }

  CPPUNIT_ASSERT_MESSAGE("all nodes of the first graph have not been iterated upon", !firstNodeIt->hasNext());
  CPPUNIT_ASSERT_MESSAGE("all nodes of the second graph have not been iterated upon", !secondNodeIt->hasNext());

  delete firstNodeIt;
  delete secondNodeIt;

  Iterator<edge>* firstEdgeIt = first->getEdges();
  Iterator<edge>* secondEdgeIt = second->getEdges();

  while(firstEdgeIt->hasNext() && secondEdgeIt->hasNext()) {
    edge firstEdge = firstEdgeIt->next();
    edge secondEdge = secondEdgeIt->next();

    double firstEdgeId = firstIdProperty->getEdgeValue(firstEdge);
    double secondEdgeId = secondIdProperty->getEdgeValue(secondEdge);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("edges are not in the same order", firstEdgeId, secondEdgeId);

    node firstEdgeSrc = first->source(firstEdge);
    node firstEdgeTgt = first->target(firstEdge);
    node secondEdgeSrc = second->source(secondEdge);
    node secondEdgeTgt = second->target(secondEdge);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("an edge from the second graph does not have the same source node as in the first graph", firstIdProperty->getNodeValue(firstEdgeSrc), secondIdProperty->getNodeValue(secondEdgeSrc));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("an edge from the second graph does not have the same target node as in the first graph", firstIdProperty->getNodeValue(firstEdgeTgt), secondIdProperty->getNodeValue(secondEdgeTgt));
  }

  CPPUNIT_ASSERT_MESSAGE("all edges of the first graph have not been iterated upon", !firstEdgeIt->hasNext());
  CPPUNIT_ASSERT_MESSAGE("all edges of the second graph have not been iterated upon", !secondEdgeIt->hasNext());

  delete firstEdgeIt;
  delete secondEdgeIt;
}

CPPUNIT_TEST_SUITE_REGISTRATION(TlpImportExportTest);

TlpImportExportTest::TlpImportExportTest() : ImportExportTest("TLP Import", "TLP Export") {}

CPPUNIT_TEST_SUITE_REGISTRATION(JsonImportExportTest);

JsonImportExportTest::JsonImportExportTest() : ImportExportTest("JSON Import", "JSON Export") {}
