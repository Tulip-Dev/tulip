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

#include <tulip/SimpleTest.h>
#include <tulip/GraphTest.h>

class SimpleTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Simple", "Tulip team", "18/04/2012",
                    "Tests whether a graph is simple or not.<br/>A simple graph is an undirected "
                    "graph with no loops and no multiple edges.",
                    "1.0", "Topological Test")
  SimpleTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {}

  bool test() override {
    return tlp::SimpleTest::isSimple(graph);
  }
};
PLUGIN(SimpleTest)

class MakeSimple : public tlp::Algorithm {
public:
  PLUGININFORMATION("Make Simple", "Tulip team", "18/04/2012", "Makes a graph simple.<br/>A simple "
                                                               "graph is an undirected graph with "
                                                               "no loops and no multiple edges.",
                    "1.0", "Topology Update")
  MakeSimple(const tlp::PluginContext *context) : tlp::Algorithm(context) {}

  bool run() override {
    std::vector<tlp::edge> edges;
    tlp::SimpleTest::makeSimple(graph, edges);
    return true;
  }
};
PLUGIN(MakeSimple)
