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

#include <tulip/PlanarityTest.h>
#include <tulip/GraphTest.h>

class PlanarityTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Planar", "Tulip team", "18/04/2012", "Tests whether a graph is planar or not.",
                    "1.0", "Topological Test")
  PlanarityTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {}

  bool test() override {
    return tlp::PlanarityTest::isPlanar(graph);
  }
};
PLUGIN(PlanarityTest)

class PlanarEmbeddingTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Planar Embedding", "Tulip team", "18/04/2012",
                    "Tests whether a graph is a planar embedding or not.", "1.0",
                    "Topological Test")
  PlanarEmbeddingTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {}

  bool test() override {
    return tlp::PlanarityTest::isPlanarEmbedding(graph);
  }
};
PLUGIN(PlanarEmbeddingTest)

class MakePlanarEmbedding : public tlp::Algorithm {
public:
  PLUGININFORMATION("Make Planar Embedding", "Tulip team", "18/04/2012",
                    "Makes the graph a planar embedding if it is planar.", "1.0", "Topology Update")
  MakePlanarEmbedding(const tlp::PluginContext *context) : tlp::Algorithm(context) {}

  bool run() override {
    return tlp::PlanarityTest::planarEmbedding(graph);
  }

  bool check(std::string &errorMsg) override {
    bool result = tlp::PlanarityTest::isPlanar(graph);
    if (!result)
      errorMsg = "The graph must be planar";
    return result;
  }
};
PLUGIN(MakePlanarEmbedding)
