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

#include <tulip/Edge.h>
#include <tulip/GraphTest.h>
#include <tulip/SimpleTest.h>

class SimpleTest : public tlp::GraphTest {
public:
  PLUGININFORMATION(
      "Simple", "Tulip team", "18/04/2012",
      "Tests whether a graph is simple or not.<br/>An undirected graph "
      "is simple if it has no loops and no more than one "
      "edge between any unordered pair of vertices. "
      "A directed graph is simple if has no loops and no more than one "
      "edge between any ordered pair of vertices.",
      "1.1", "Topological Test")
  SimpleTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {
    addInParameter<bool>(
        "directed",
        "Indicates if the graph should be considered as directed or not.",
        "false");
  }

  bool test() override {
    bool directed = false;
    if (dataSet) {
      dataSet->get("directed", directed);
    }
    return tlp::SimpleTest::isSimple(graph, directed);
  }
};
PLUGIN(SimpleTest)

class MakeSimple : public tlp::Algorithm {
public:
  PLUGININFORMATION(
      "Make Simple", "Tulip team", "18/04/2012",
      "Makes a graph simple.<br/>An undirected graph "
      "is simple if it has no loops and no more than one "
      "edge between any unordered pair of vertices. "
      "A directed graph is simple if has no loops and no more than one "
      "edge between any ordered pair of vertices.",
      "1.1", "Topology Update")
  MakeSimple(const tlp::PluginContext *context) : tlp::Algorithm(context) {
    addInParameter<bool>(
        "directed",
        "Indicates if the graph should be considered as directed or not.",
        "false");
  }

  bool run() override {
    bool directed = false;
    if (dataSet)
      dataSet->get("directed", directed);
    std::vector<tlp::edge> edges;
    tlp::SimpleTest::makeSimple(graph, edges, directed);
    return true;
  }
};
PLUGIN(MakeSimple)
