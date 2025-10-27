/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/SimpleTest.h>
#include <tulip/GraphTest.h>
#include <tulip/Edge.h>

class SimpleTest : public tlp::GraphTest {
public:
  PLUGININFORMATION(
      "Simple", "Tulip team", "18/04/2012",
      "Tests whether a graph is simple or not."
      "<br/>An directed/undirected graph "
      "is simple if it has no self loops (no edges with the same node as source and target node) and no multiple edges"
      " (no more than one edge between any ordered pair of nodes).",
      "1.2", "Topological Test")
  SimpleTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {
    addInParameter<bool>(
        "directed", "Indicates if the graph should be considered as directed or not.", "false");
    addInParameter<bool>(
        "check loops", "Indicates if the existence of self loops has to be tested or not.", "true");
    addInParameter<bool>("check multiple edges",
                         "Indicates if the existence of multiple edges has to be tested or not.",
                         "true");
    addOutParameter<unsigned int>("#self loops", "The number of self loops found", "0");
    addOutParameter<unsigned int>("#multiple edges", "The number of multiple edges found", "0");
  }

  bool check(std::string &errorMsg) override {
    bool checkLoops = true;
    bool checkMultiple = true;
    if (dataSet) {
      dataSet->get("check loops", checkLoops);
      dataSet->get("check multiple edges", checkMultiple);
    }
    if (!checkLoops && !checkMultiple) {
      errorMsg.append(
          "The 'check loops' and 'check multiple edges' parameters cannot be both set to 'false'");
      return false;
    }
    return true;
  }

  bool test() override {
    bool directed = false;
    bool checkLoops = true;
    bool checkMultiple = true;
    if (dataSet) {
      dataSet->get("directed", directed);
      dataSet->get("check loops", checkLoops);
      dataSet->get("check multiple edges", checkMultiple);
    }
    std::vector<tlp::edge> multipleEdges;
    std::vector<tlp::edge> loops;
    bool result = tlp::SimpleTest::simpleTest(graph, &multipleEdges, &loops, directed);
    if (!result)
      result = (!checkLoops || loops.empty()) && (!checkMultiple || multipleEdges.empty());
    if (dataSet) {
      unsigned int nb = checkLoops ? loops.size() : 0;
      dataSet->set("#self loops", nb);
      nb = checkMultiple ? multipleEdges.size() : 0;
      dataSet->set("#multiple edges", nb);
    }
    return result;
  }
};
PLUGIN(SimpleTest)

class MakeSimple : public tlp::Algorithm {
public:
  PLUGININFORMATION("Make Simple", "Tulip team", "18/04/2012",
                    "Makes a graph simple.<br/>An undirected graph "
                    "is simple if it has no loops and no more than one "
                    "edge between any unordered pair of vertices. "
                    "A directed graph is simple if has no loops and no more than one "
                    "edge between any ordered pair of vertices.",
                    "1.1", "Topology Update")
  MakeSimple(const tlp::PluginContext *context) : tlp::Algorithm(context) {
    addInParameter<bool>(
        "directed", "Indicates if the graph should be considered as directed or not.", "false");
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
