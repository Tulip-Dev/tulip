/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

using namespace tlp;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ImportExportTest);

// ImportExportTest::ImportExportTest() :importAlgorithm("TLP Import"), exportAlgorithm("TLP Export") {
ImportExportTest::ImportExportTest() :importAlgorithm("JSON Import"), exportAlgorithm("JSON Export") {
}

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
  original->setAttribute("name", originalName);
  //TODO test attributes of type int, double, Coord, Size, ...

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
  string msg;
  original->computeProperty<DoubleProperty>("Id", id, msg);

  return original;
}

void ImportExportTest::importExportGraph(tlp::Graph* original) {
  const string tlpFileName = "subout.tlp";
  ofstream os(tlpFileName.c_str());
  DataSet set;
  tlp::exportGraph(original, os, exportAlgorithm, set);

  os.close();

  DataSet input;
  input.set<string>("file::filename", tlpFileName);
  Graph* imported = tlp::importGraph(importAlgorithm, input);

  testGraphsAreEqual(original, imported);
}

void ImportExportTest::testGraphsAreEqual(Graph* first, Graph* second) {
  CPPUNIT_ASSERT_MESSAGE("The import failed; the graph is null", second != NULL);

  testGraphsTopologiesAreEqual(first, second);
  testGraphAttributesAreEqual(first, second);

  CPPUNIT_ASSERT_EQUAL_MESSAGE("Graphs have different number of subgraphs", first->numberOfSubGraphs(), second->numberOfSubGraphs());

  Iterator<Graph*>* firstSubGraphs = first->getSubGraphs();
  Iterator<Graph*>* secondSubGraphs = second->getSubGraphs();

  while(firstSubGraphs->hasNext() && secondSubGraphs->hasNext()) {
    Graph* firstSub = firstSubGraphs->next();
    Graph* secondSub = secondSubGraphs->next();

    testGraphsTopologiesAreEqual(firstSub, secondSub);
    testGraphAttributesAreEqual(firstSub, secondSub);
  }
}

void ImportExportTest::testGraphAttributesAreEqual(tlp::Graph* first, tlp::Graph* second) {
  std::pair<std::string, tlp::DataType*> attribute;
  forEach(attribute, first->getAttributes().getValues()) {
    stringstream attributeNameMessage;
    attributeNameMessage << "attribute \"" << attribute.first << "\" does not exists on imported graph. Should be: " << *(std::string*)attribute.second->value;
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
  unsigned int firstLocalPropertiesCount = 0;
  unsigned int secondLocalPropertiesCount = 0;

  PropertyInterface* firstProperty;
  PropertyInterface* secondProperty;
  forEach(firstProperty, first->getObjectProperties()) {
    ++firstPropertiesCount;
  }
  forEach(firstProperty, first->getLocalObjectProperties()) {
    ++firstLocalPropertiesCount;
  }
  forEach(secondProperty, second->getObjectProperties()) {
    ++secondPropertiesCount;
  }
  forEach(secondProperty, second->getLocalObjectProperties()) {
    ++secondLocalPropertiesCount;
  }

  CPPUNIT_ASSERT_EQUAL_MESSAGE("Graphs have different number of properties", firstPropertiesCount, secondPropertiesCount);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("Graphs have different number of local properties", firstLocalPropertiesCount, secondLocalPropertiesCount);

  Iterator<string>* firstIt = first->getProperties();

  while(firstIt->hasNext()) {
    string firstPropertyName = firstIt->next();
    firstProperty = first->getProperty(firstPropertyName);

    stringstream message;
    message << "the property " << firstPropertyName << " does not exist in the second graph !";
    CPPUNIT_ASSERT_MESSAGE(message.str(), second->existProperty(firstPropertyName));
  }
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

  Iterator<node>* firstIt = first->getNodes();
  Iterator<node>* secondIt = second->getNodes();

  while(firstIt->hasNext() && secondIt->hasNext()) {
    node firstNode = firstIt->next();
    node secondNode = secondIt->next();

    CPPUNIT_ASSERT_MESSAGE("id control property does not exists on original graph, please add it before testing if graphs are equal.", first->existProperty("id"));
    CPPUNIT_ASSERT_MESSAGE("id control property does not exists on imported graph", second->existProperty("id"));

    DoubleProperty* firstIdProperty = first->getProperty<DoubleProperty>("id");
    DoubleProperty* secondIdProperty = second->getProperty<DoubleProperty>("id");

    double firstId = firstIdProperty->getNodeValue(firstNode);
    double secondId = secondIdProperty->getNodeValue(secondNode);

    CPPUNIT_ASSERT_MESSAGE("layout property does not exists on original graph", first->existProperty("viewLayout"));
    CPPUNIT_ASSERT_MESSAGE("layout property does not exists on imported graph", second->existProperty("viewLayout"));

    LayoutProperty* firstLayout = first->getProperty<LayoutProperty>("viewLayout");
    LayoutProperty* secondLayout = second->getProperty<LayoutProperty>("viewLayout");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nodes are not in the same order, this test is a failure by design. ITERATE !!", firstId, secondId);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nodes do not have same degree", first->deg(firstNode), second->deg(secondNode));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nodes are not at the same position", firstLayout->getNodeStringValue(firstNode), secondLayout->getNodeStringValue(secondNode));
  }

  CPPUNIT_ASSERT_MESSAGE("all nodes of the first graph have not been iterated upon", !firstIt->hasNext());
  CPPUNIT_ASSERT_MESSAGE("all nodes of the second graph have not been iterated upon", !secondIt->hasNext());

  delete firstIt;
  delete secondIt;
}
